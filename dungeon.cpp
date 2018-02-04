//=============================================================================
//
// ダンジョン処理 [dungeon.cpp]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#include "dungeon.h"
#include "player.h"
#include "enemy.h"
#include "font.h"
#include "treasure.h"
#include "camera.h"
#include "minimap.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_GAME_DUNGEON00		_T("data/TEXTURE/maptip001_alter_large.png")	// サンプル用画像

#define TEXTURE_PATTERN_DUNGEON_DIVIDE_X	(6)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DUNGEON_DIVIDE_Y	(4)	// アニメパターンのテクスチャ内分割数（Y)

#define DUNGEON00_DRAWABLE_TILES_X	(SCREEN_WIDTH/TEXTURE_DUNGEON_SIZE_X)
#define DUNGEON00_DRAWABLE_TILES_Y	(SCREEN_HEIGHT/TEXTURE_DUNGEON_SIZE_Y)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexDungeon(void);
void SetTextureDungeon(void);
void SetTextureExit(void);
void SetVertexDungeon(void);
void SetDungeon(void);
void SplitX(SPACE *t_room, SPACE *splited_room1, SPACE *splited_room2);
void SplitY(SPACE *t_room, SPACE *splited_room1, SPACE *splited_room2);
void Split(SPACE *t_room);
void MakeRoom(void);
void MakeTrail(void);
void DeleteTrail(void);
void SetStart(int no);
void SetExit(int no);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
int						g_map[MAP_WIDTH][MAP_HEIGHT];							// マップ配列
SPACE					g_space[MAX_ROOM_COUNT];								// 部屋決めの範囲
ROOM					boundary[MAX_ROOM_COUNT - 1];							// 箱割り方のための境界
int						g_Rcount;
int						g_Scount;

int						startX;
int						startY;
int						exitX;
int						exitY;

LPDIRECT3DTEXTURE9		g_pD3DTextureDungeon = NULL;							// テクスチャへのポリゴン
VERTEX_2D				g_vertexWkDungeon[MAP_WIDTH * MAP_HEIGHT][NUM_VERTEX];	// 頂点情報格納ワーク


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitDungeon(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_DUNGEON00,				// ファイルの名前
			&g_pD3DTextureDungeon);				// 読み込むメモリのポインタ
	}

	// ダンジョンの生成
	SetDungeon();

	// 頂点情報の作成
	MakeVertexDungeon();

	// テクスチャの設定
	SetTextureDungeon();

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitDungeon(void)
{
	if (g_pD3DTextureDungeon != NULL)
	{	// テクスチャの開放
		g_pD3DTextureDungeon->Release();
		g_pD3DTextureDungeon = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateDungeon(void)
{
	SetVertexDungeon();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawDungeon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureDungeon);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// マップチップ
	for (int i = 0; i < MAP_WIDTH; i++)
	{
		for (int j = 0; j < MAP_HEIGHT; j++)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkDungeon[MAP_HEIGHT * i + j], sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexDungeon(void)
{	
	// マップ全体を処理する
	for (int i = 0; i < MAP_WIDTH; i++)
	{
		for (int j = 0; j < MAP_HEIGHT; j++)
		{
			// rhwの設定
			g_vertexWkDungeon[MAP_HEIGHT * i + j][0].rhw =
			g_vertexWkDungeon[MAP_HEIGHT * i + j][1].rhw =
			g_vertexWkDungeon[MAP_HEIGHT * i + j][2].rhw =
			g_vertexWkDungeon[MAP_HEIGHT * i + j][3].rhw = 1.0f;

			// 反射光の設定
			if (g_map[i][j] == MAP_PATH || g_map[i][j] == MAP_TRAIL)
			{
				g_vertexWkDungeon[MAP_HEIGHT * i + j][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 128);
				g_vertexWkDungeon[MAP_HEIGHT * i + j][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 128);
				g_vertexWkDungeon[MAP_HEIGHT * i + j][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 128);
				g_vertexWkDungeon[MAP_HEIGHT * i + j][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 128);
			}
			else
			{
				g_vertexWkDungeon[MAP_HEIGHT * i + j][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				g_vertexWkDungeon[MAP_HEIGHT * i + j][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				g_vertexWkDungeon[MAP_HEIGHT * i + j][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				g_vertexWkDungeon[MAP_HEIGHT * i + j][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			}
		}
	}

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureDungeon(void)
{
	int tx, ty;
	for (int i = 0; i < MAP_WIDTH; i++)
	{
		for (int j = 0; j < MAP_HEIGHT; j++)
		{
			switch (g_map[i][j])
			{
			case MAP_BOUNDARY:
				tx = 3;
				ty = 2;
				break;
			case MAP_WALL_UP:
				tx = 1;
				ty = 0;
				break;
			case MAP_WALL_DOWN:
				tx = 1;
				ty = 2;
				break;
			case MAP_WALL_RIGHT:
				tx = 2;
				ty = 1;
				break;
			case MAP_WALL_LEFT:
				tx = 0;
				ty = 1;
				break;
			case MAP_WALL_UPRIGHT:
				tx = 2;
				ty = 0;
				break;
			case MAP_WALL_UPLEFT:
				tx = 0;
				ty = 0;
				break;
			case MAP_WALL_DOWNRIGHT:
				tx = 2;
				ty = 2;
				break;
			case MAP_WALL_DOWNLEFT:
				tx = 0;
				ty = 2;
				break;
			case MAP_ROOM:
				tx = 1;
				ty = 1;
				break;
			case MAP_TRAIL:
				tx = 1;
				ty = 1;
				break;
			case MAP_NOT_OBJECT:
				tx = 3;
				ty = 3;
				break;
			case MAP_PATH:
				tx = 1;
				ty = 1;
				break;
			case MAP_START:
				tx = 1;
				ty = 1;
				break;
			case MAP_EXIT:
				tx = 4;
				ty = 1;
				break;
			}
			g_vertexWkDungeon[MAP_HEIGHT * i + j][0].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DUNGEON_DIVIDE_X * tx, 1.0f / TEXTURE_PATTERN_DUNGEON_DIVIDE_Y * ty);
			g_vertexWkDungeon[MAP_HEIGHT * i + j][1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DUNGEON_DIVIDE_X * (tx + 1), 1.0f / TEXTURE_PATTERN_DUNGEON_DIVIDE_Y * ty);
			g_vertexWkDungeon[MAP_HEIGHT * i + j][2].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DUNGEON_DIVIDE_X * tx, 1.0f / TEXTURE_PATTERN_DUNGEON_DIVIDE_Y * (ty + 1));
			g_vertexWkDungeon[MAP_HEIGHT * i + j][3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DUNGEON_DIVIDE_X * (tx + 1), 1.0f / TEXTURE_PATTERN_DUNGEON_DIVIDE_Y * (ty + 1));

		}
	}
}

//=============================================================================
// テクスチャ座標の設定(階段)
//=============================================================================
void SetTextureExit(void)
{
	const int tx = 4, ty = 1;
	g_vertexWkDungeon[MAP_HEIGHT * exitX + exitY][0].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DUNGEON_DIVIDE_X * tx, 1.0f / TEXTURE_PATTERN_DUNGEON_DIVIDE_Y * ty);
	g_vertexWkDungeon[MAP_HEIGHT * exitX + exitY][1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DUNGEON_DIVIDE_X * (tx + 1), 1.0f / TEXTURE_PATTERN_DUNGEON_DIVIDE_Y * ty);
	g_vertexWkDungeon[MAP_HEIGHT * exitX + exitY][2].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DUNGEON_DIVIDE_X * tx, 1.0f / TEXTURE_PATTERN_DUNGEON_DIVIDE_Y * (ty + 1));
	g_vertexWkDungeon[MAP_HEIGHT * exitX + exitY][3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DUNGEON_DIVIDE_X * (tx + 1), 1.0f / TEXTURE_PATTERN_DUNGEON_DIVIDE_Y * (ty + 1));
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexDungeon(void)
{
	PLAYER *player = GetPlayer();
	D3DXVECTOR3 *posCamera = GetCameraPos();

	for (int i = 0; i < MAP_WIDTH; i++)
	{
		for (int j = 0; j < MAP_HEIGHT; j++)
		{
			// 頂点座標の設定
			g_vertexWkDungeon[MAP_HEIGHT * i + j][0].vtx.x = TEXTURE_DUNGEON_SIZE_X * i;
			g_vertexWkDungeon[MAP_HEIGHT * i + j][0].vtx.y = TEXTURE_DUNGEON_SIZE_Y * j + TEXTURE_PLAYER_SIZE_Y;
			g_vertexWkDungeon[MAP_HEIGHT * i + j][0].vtx.z = 0.0f;
			g_vertexWkDungeon[MAP_HEIGHT * i + j][0].vtx += *posCamera;

			g_vertexWkDungeon[MAP_HEIGHT * i + j][1].vtx.x = TEXTURE_DUNGEON_SIZE_X * (i + 1);
			g_vertexWkDungeon[MAP_HEIGHT * i + j][1].vtx.y = TEXTURE_DUNGEON_SIZE_Y * j + TEXTURE_PLAYER_SIZE_Y;
			g_vertexWkDungeon[MAP_HEIGHT * i + j][1].vtx.z = 0.0f;
			g_vertexWkDungeon[MAP_HEIGHT * i + j][1].vtx += *posCamera;

			g_vertexWkDungeon[MAP_HEIGHT * i + j][2].vtx.x = TEXTURE_DUNGEON_SIZE_X * i;
			g_vertexWkDungeon[MAP_HEIGHT * i + j][2].vtx.y = TEXTURE_DUNGEON_SIZE_Y * (j + 1) + TEXTURE_PLAYER_SIZE_Y;
			g_vertexWkDungeon[MAP_HEIGHT * i + j][2].vtx.z = 0.0f;
			g_vertexWkDungeon[MAP_HEIGHT * i + j][2].vtx += *posCamera;

			g_vertexWkDungeon[MAP_HEIGHT * i + j][3].vtx.x = TEXTURE_DUNGEON_SIZE_X * (i + 1);
			g_vertexWkDungeon[MAP_HEIGHT * i + j][3].vtx.y = TEXTURE_DUNGEON_SIZE_Y * (j + 1) + TEXTURE_PLAYER_SIZE_Y;
			g_vertexWkDungeon[MAP_HEIGHT * i + j][3].vtx.z = 0.0f;
			g_vertexWkDungeon[MAP_HEIGHT * i + j][3].vtx += *posCamera;
		}
	}
}


//=============================================================================
// ダンジョンの生成
//=============================================================================
void SetDungeon(void)
{
	g_Rcount = 0;
	g_Scount = 0;
	for (int i = 0; i < MAP_WIDTH; i++)
	{
		for (int j = 0; j < MAP_HEIGHT; j++)
		{
			g_map[i][j] = MAP_NOT_OBJECT;
		}
	}
	g_space[0].baseX = 0;
	g_space[0].baseY = 0;
	g_space[0].width = MAP_WIDTH;
	g_space[0].height = MAP_HEIGHT;
	Split(g_space);
	MakeRoom();
	MakeTrail();
	DeleteTrail();
	SetStart(rand() % (g_Scount + 1));
	SetExit(rand() % (g_Scount + 1));
}

void SplitX(SPACE *t_room, SPACE *splited_room1, SPACE *splited_room2)
{
	int num = t_room->baseX + (rand() % (t_room->width - MIN_ROOM_WIDTH * 2) + MIN_ROOM_WIDTH);

	if (g_Scount < MAX_ROOM_COUNT - 1)
	{
		for (int i = t_room->baseY; i < t_room->baseY + t_room->height; i++)
		{
			g_map[num][i] = MAP_BOUNDARY;
		}
		boundary[g_Scount].baseX = num;
		boundary[g_Scount].baseY = t_room->baseY;
		boundary[g_Scount].width = 0;
		boundary[g_Scount].height = t_room->height;
		g_Scount++;
	}
	splited_room1->baseX = t_room->baseX;
	splited_room1->baseY = t_room->baseY;
	splited_room1->width = num - t_room->baseX;
	splited_room1->height = t_room->height;

	splited_room2->baseX = num + 1;
	splited_room2->baseY = t_room->baseY;
	splited_room2->width = t_room->width + t_room->baseX - (num + 1);
	splited_room2->height = t_room->height;
}

void SplitY(SPACE *t_room, SPACE *splited_room1, SPACE *splited_room2)
{
	int num = t_room->baseY + (rand() % (t_room->height - MIN_ROOM_HEIGHT * 2) + MIN_ROOM_HEIGHT);

	if (g_Scount < MAX_ROOM_COUNT - 1)
	{
		for (int i = t_room->baseX; i < t_room->baseX + t_room->width; i++)
		{
			g_map[i][num] = MAP_BOUNDARY;
		}
		boundary[g_Scount].baseX = t_room->baseX;
		boundary[g_Scount].baseY = num;
		boundary[g_Scount].width = t_room->width;
		boundary[g_Scount].height = 0;
		g_Scount++;
	}
	splited_room1->baseX = t_room->baseX;
	splited_room1->baseY = t_room->baseY;
	splited_room1->width = t_room->width;
	splited_room1->height = num - t_room->baseY;

	splited_room2->baseX = t_room->baseX;
	splited_room2->baseY = num + 1;
	splited_room2->width = t_room->width;
	splited_room2->height = t_room->height + t_room->baseY - (num + 1);
}

void Split(SPACE *t_room)
{
	SPACE splited_room1, splited_room2;
	bool flag = false;

	if (g_Rcount == MAX_ROOM_COUNT) return;

	if (rand() % (t_room->width + t_room->height + 1) < t_room->width) {
		if (t_room->width > MIN_ROOM_WIDTH * 2) {
			SplitX(t_room, &splited_room1, &splited_room2);
			flag = true;
		}
	}
	else {
		if (t_room->height > MIN_ROOM_HEIGHT * 2) {
			SplitY(t_room, &splited_room1, &splited_room2);
			flag = true;
		}
	}

	if (flag) {
		Split(&splited_room1);
		Split(&splited_room2);
	}
	else {
		g_space[g_Rcount] = *t_room;
		g_Rcount++;
	}
}

void MakeRoom(void)
{
	for (int i = 0; i < g_Rcount; i++)
	{
		g_space[i].room.width = rand() % ((g_space[i].width > MAX_ROOM_WIDTH) ? (MAX_ROOM_WIDTH - MIN_ROOM_WIDTH + 1) : (g_space[i].width - MIN_ROOM_WIDTH + 1)) + MIN_ROOM_WIDTH;
		g_space[i].room.height = rand() % ((g_space[i].height > MAX_ROOM_HEIGHT) ? (MAX_ROOM_HEIGHT - MIN_ROOM_HEIGHT + 1) : (g_space[i].height - MIN_ROOM_HEIGHT + 1)) + MIN_ROOM_HEIGHT;
		g_space[i].room.baseX = g_space[i].baseX + (rand() % (g_space[i].width - g_space[i].room.width + 1));
		g_space[i].room.baseY = g_space[i].baseY + (rand() % (g_space[i].height - g_space[i].room.height + 1));
		g_space[i].room.cntEnemy = 0;
		g_space[i].room.spawnEnemy = 0;
		g_space[i].room.clear = false;

		for (int x = g_space[i].room.baseX; x < g_space[i].room.baseX + g_space[i].room.width; x++)
		{
			for (int y = g_space[i].room.baseY; y < g_space[i].room.baseY + g_space[i].room.height; y++)
			{
				if (y == g_space[i].room.baseY && x == g_space[i].room.baseX)
				{
					g_map[x][y] = MAP_WALL_UPLEFT;
				}
				else if (y == g_space[i].room.baseY && x == g_space[i].room.baseX + g_space[i].room.width - 1)
				{
					g_map[x][y] = MAP_WALL_UPRIGHT;
				}
				else if (y == g_space[i].room.baseY + g_space[i].room.height - 1 && x == g_space[i].room.baseX)
				{
					g_map[x][y] = MAP_WALL_DOWNLEFT;
				}
				else if (y == g_space[i].room.baseY + g_space[i].room.height - 1 && x == g_space[i].room.baseX + g_space[i].room.width - 1)
				{
					g_map[x][y] = MAP_WALL_DOWNRIGHT;
				}
				else if (y == g_space[i].room.baseY)
				{
					g_map[x][y] = MAP_WALL_UP;
				}
				else if (y == g_space[i].room.baseY + g_space[i].room.height - 1)
				{
					g_map[x][y] = MAP_WALL_DOWN;
				}
				else if (x == g_space[i].room.baseX)
				{
					g_map[x][y] = MAP_WALL_LEFT;
				}
				else if (x == g_space[i].room.baseX + g_space[i].room.width - 1)
				{
					g_map[x][y] = MAP_WALL_RIGHT;
				}
				else {
					g_map[x][y] = MAP_ROOM;
				}
			}
		}
	}
}

void MakeTrail(void)
{
	for (int i = 0; i < g_Rcount; i++)
	{
		int trailX, trailY;
		//if (g_space[i].room.height < MAP_HEIGHT / 2)
		{
			if (g_space[i].baseX > 0)
			{
				trailY = g_space[i].room.baseY + (rand() % (g_space[i].room.height - 2) + 1);
				for (int x = g_space[i].room.baseX; x >= g_space[i].baseX - 1; x--)
				{
					g_map[x][trailY] = MAP_TRAIL;
				}
			}
			if (g_space[i].baseX + g_space[i].width < MAP_WIDTH)
			{
				trailX = g_space[i].room.baseX + g_space[i].room.width - 1;//g_space[i].room.baseX + (rand() % (g_space[i].room.width - 2) + 1);
				trailY = g_space[i].room.baseY + (rand() % (g_space[i].room.height - 2) + 1);
				for (int x = trailX; x <= g_space[i].baseX + g_space[i].width; x++)
				{
					g_map[x][trailY] = MAP_TRAIL;
				}
			}
		}
		//if (g_space[i].room.width < MAP_WIDTH / 2)
		{
			if (g_space[i].baseY > 0)
			{
				trailX = g_space[i].room.baseX + (rand() % (g_space[i].room.width - 2) + 1);
				for (int y = g_space[i].room.baseY; y >= g_space[i].baseY - 1; y--)
				{
					g_map[trailX][y] = MAP_TRAIL;
				}
			}
			if (g_space[i].baseY + g_space[i].height < MAP_HEIGHT)
			{
				trailX = g_space[i].room.baseX + (rand() % (g_space[i].room.width - 2) + 1);
				trailY = g_space[i].room.baseY + g_space[i].room.height - 1;
				for (int y = trailY; y <= g_space[i].baseY + g_space[i].height; y++)
				{
					g_map[trailX][y] = MAP_TRAIL;
				}
			}
		}
	}
}

void DeleteTrail(void)
{
	bool flag;
	for (int i = 0; i < g_Scount; i++)
	{
		if (boundary[i].height == 0)
		{
			flag = false;
			for (int x = boundary[i].baseX; x < boundary[i].baseX + boundary[i].width; x++)
			{
				if (!flag && g_map[x][boundary[i].baseY] == MAP_BOUNDARY)
				{
					g_map[x][boundary[i].baseY] = MAP_NOT_OBJECT;
				}
				if (flag && g_map[x][boundary[i].baseY] != MAP_TRAIL)
				{
					g_map[x][boundary[i].baseY] = MAP_PATH;
				}
				if (g_map[x][boundary[i].baseY] == MAP_TRAIL)
				{
					flag = !flag;
				}
			}
			flag = false;
			for (int x = boundary[i].baseX + boundary[i].width - 1; x>= boundary[i].baseX; x--)
			{
				if (!flag && (g_map[x][boundary[i].baseY] == MAP_BOUNDARY || g_map[x][boundary[i].baseY] == MAP_PATH))
				{
					g_map[x][boundary[i].baseY] = MAP_NOT_OBJECT;
				}
				if (flag && g_map[x][boundary[i].baseY] != MAP_TRAIL)
				{
					g_map[x][boundary[i].baseY] = MAP_PATH;
				}
				if (g_map[x][boundary[i].baseY] == MAP_TRAIL)
				{
					if (flag)
					{
						flag = false;
						break;
					}
					flag = true;
				}
			}
			if (flag)
			{
				for (int x = boundary[i].baseX; g_map[x][boundary[i].baseY] == MAP_PATH; x++)
				{
					g_map[x][boundary[i].baseY] = MAP_NOT_OBJECT;
				}
			}
		}
		if (boundary[i].width == 0)
		{
			flag = false;
			for (int y = boundary[i].baseY; y < boundary[i].baseY + boundary[i].height; y++)
			{
				if (!flag && g_map[boundary[i].baseX][y] == MAP_BOUNDARY)
				{
					g_map[boundary[i].baseX][y] = MAP_NOT_OBJECT;
				}
				if (flag && g_map[boundary[i].baseX][y] != MAP_TRAIL)
				{
					g_map[boundary[i].baseX][y] = MAP_PATH;
				}
				if (g_map[boundary[i].baseX][y] == MAP_TRAIL)
				{
					flag = !flag;
				}
			}
			flag = false;
			for (int y = boundary[i].baseY + boundary[i].height - 1; y >= boundary[i].baseY; y--)
			{
				if (!flag && (g_map[boundary[i].baseX][y] == MAP_BOUNDARY || g_map[boundary[i].baseX][y] == MAP_PATH))
				{
					g_map[boundary[i].baseX][y] = MAP_NOT_OBJECT;
				}
				if (flag && g_map[boundary[i].baseX][y] != MAP_TRAIL)
				{
					g_map[boundary[i].baseX][y] = MAP_PATH;
				}
				if (g_map[boundary[i].baseX][y] == MAP_TRAIL)
				{
					if (flag)
					{
						flag = false;
						break;
					}
					flag = true;
				}
			}
			if (flag)
			{
				for (int y = boundary[i].baseY; g_map[boundary[i].baseX][y] == MAP_PATH; y++)
				{
					g_map[boundary[i].baseX][y] = MAP_NOT_OBJECT;
				}
			}
		}
	}
}

//=============================================================================
// 入り口を設定
//=============================================================================
void SetStart(int no)
{
	startX = rand() % (g_space[no].room.width - 3) + g_space[no].room.baseX + 2;
	startY = rand() % (g_space[no].room.height - 3) + g_space[no].room.baseY + 2;

	g_map[startX][startY] = MAP_START;

	SetPlayer((startX + 0.5f)*TEXTURE_DUNGEON_SIZE_X, (startY + 0.5f)*TEXTURE_DUNGEON_SIZE_Y);
	//SetCamera((startX + 0.5f)*TEXTURE_DUNGEON_SIZE_X, (startY + 0.5f)*TEXTURE_DUNGEON_SIZE_Y);
}

//=============================================================================
// 出口を設定
//=============================================================================
void SetExit(int no)
{
	do
	{
		exitX = rand() % (g_space[no].room.width - 3) + g_space[no].room.baseX + 2;
		exitY = rand() % (g_space[no].room.height - 3) + g_space[no].room.baseY + 2;

		if (g_map[exitX][exitY] != MAP_START) g_map[exitX][exitY] = MAP_EXIT;
	} while (g_map[exitX][exitY] != MAP_EXIT);
	// ダンジョン生成時は出口を隠す
	g_map[exitX][exitY] = MAP_ROOM;
}

//=============================================================================
// マップ情報を取得
//=============================================================================
int GetMap(int x, int y)
{
	return g_map[x][y];
}

//=============================================================================
// 位置情報からマップ情報を取得
//=============================================================================
int GetMapByPos(float x, float y)
{
	return g_map[(int)(x / TEXTURE_DUNGEON_SIZE_X)][(int)(y / TEXTURE_DUNGEON_SIZE_Y)];
}

//=============================================================================
// 部屋情報を取得
//=============================================================================
ROOM *GetRoom(int id)
{
	if (id < 0) return NULL;
	else return(&g_space[id].room);
}

ROOM *GetRoom2(int x, int y)
{
	return(GetRoom(GetRoomID(x, y)));
}

//=============================================================================
// 部屋IDを取得
//=============================================================================
int GetRoomID(int x, int y)
{
	if (g_map[x][y] == MAP_ROOM || g_map[x][y] == MAP_EXIT)
	{
		for (int i = 0; i < g_Rcount; i++)
		{
			if (g_space[i].room.baseX < x && g_space[i].room.baseY < y && x < g_space[i].room.baseX + g_space[i].room.width - 1 && y < g_space[i].room.baseY + g_space[i].room.height - 1)
			{
				return i;
			}
		}
	}

	return -1;
}

//=============================================================================
// 位置情報から部屋IDを取得
//=============================================================================
int GetRoomIDByPos(float x, float y)
{
	return GetRoomID((int)(x / TEXTURE_DUNGEON_SIZE_X), (int)(y / TEXTURE_DUNGEON_SIZE_Y));
}

//=============================================================================
// 出口のある部屋の取得
//=============================================================================
int GetExitRoomID(void)
{
	return GetRoomID(exitX, exitY);
}

//=============================================================================
// 部屋に入った時の処理
//=============================================================================
void EnterRoom(int ID)
{
	ROOM *room = &g_space[ID].room;

	// スタート地点を通常の部屋判定に
	if (g_map[startX][startY] == MAP_START)
	{
		g_map[startX][startY] = MAP_ROOM;
	}

	// 未クリア時の処理
	if (!room->clear)
	{
		RefreshFont();
		SetString("-しょうもくひょう-", SCREEN_CENTER_X, SCREEN_CENTER_Y + TEXTURE_FONT_SIZE, TEXTURE_FONT_SIZE, 120);
		SetString("へやのてきを　せんめつせよ", SCREEN_CENTER_X, SCREEN_CENTER_Y + TEXTURE_FONT_SIZE * 2, TEXTURE_FONT_SIZE, 120);

		// 部屋範囲でミニマップの可視化
		SetMiniMap(room->baseX, room->baseY, room->baseX + room->width, room->baseY + room->height);
	}

	// エネミーの配置がまだされていないなら配置
	if (room->cntEnemy == 0 && !room->clear)
	{
		room->cntEnemy = room->width * room->height / ENEMY_INDEX;

		for (int i = 0; i < room->cntEnemy; i++)
		{
			SetEnemy(ID);
		}
	}

	// エネミーのスポーン
	int spawnNum = room->cntEnemy / 2;
	if (spawnNum < ENEMY_ROOM_MIN_NUM) spawnNum = ENEMY_ROOM_MIN_NUM;
	for (int i = 0; i < spawnNum; i++)
	{
		float Ex, Ey;
		float Px, Py;

		Px = GetPlayer()->pos.x;
		Py = GetPlayer()->pos.y;
		do
		{
			Ex = (room->baseX + 2 + (room->width - 4) * (rand() % 100 / 100.0f)) * TEXTURE_DUNGEON_SIZE_X;
			Ey = (room->baseY + 2 + (room->height - 3) * (rand() % 100 / 100.0f)) * TEXTURE_DUNGEON_SIZE_Y;
		} while ((Ex - Px)*(Ex - Px) + (Ey - Py)*(Ey - Py) < SPAWN_DISTANCE * SPAWN_DISTANCE);
		if (SpawnEnemy(Ex, Ey, ID))
		{
			room->spawnEnemy++;
		}
		else
		{
			break;
		}
	}
}

//=============================================================================
// 部屋を離れた時の処理
//=============================================================================
void LeaveRoom(int ID)
{
	ROOM *room = &g_space[ID].room;

	while (room->spawnEnemy > 0)
	{
		DeSpawnEnemy(ID);
		room->spawnEnemy--;
	}
}

//=============================================================================
// 部屋のクリア
//=============================================================================
void ClearRoom(int ID)
{
	ROOM *room = &g_space[ID].room;

	room->clear = true;


	if (GetRoomID(exitX, exitY) == ID)
	{
		SetString("ミッションクリア", SCREEN_CENTER_X, SCREEN_CENTER_Y + TEXTURE_FONT_SIZE, TEXTURE_FONT_SIZE * 2, 300);
		SetString("かいだんを　はっけんした", SCREEN_CENTER_X, SCREEN_CENTER_Y + TEXTURE_FONT_SIZE * 3, TEXTURE_FONT_SIZE, 300);

		g_map[exitX][exitY] = MAP_EXIT;
		SetTextureExit();
	}
	else
	{
		SetString("へやをクリア", SCREEN_CENTER_X, SCREEN_CENTER_Y + TEXTURE_FONT_SIZE, TEXTURE_FONT_SIZE, 300);
		SetString("たからばこが　しゅつげん", SCREEN_CENTER_X, SCREEN_CENTER_Y + TEXTURE_FONT_SIZE * 2, TEXTURE_FONT_SIZE, 300);

		float Tx = (room->baseX + 2 + (room->width - 4) / 2.0f) * TEXTURE_DUNGEON_SIZE_X;
		float Ty = (room->baseY + 2 + (room->height - 4) / 2.0f) * TEXTURE_DUNGEON_SIZE_Y;
		
		int size = room->width * room->height;
		if (size < 150)
		{
			size = TEXTURE_TREASURE_SIZE / 4 * 3;
		}
		else if (size < 250)
		{
			size = TEXTURE_TREASURE_SIZE;
		}
		else
		{
			size = TEXTURE_TREASURE_SIZE / 2 * 3;
		}

		SetTreasure(Tx, Ty, rand() % TREASURE_BOX_MAX, size);
	}
}
