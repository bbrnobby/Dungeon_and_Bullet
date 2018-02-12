//=============================================================================
//
// ミニマップ処理 [minimap.cpp]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#include "minimap.h"
#include "input.h"
#include "player.h"
#include "camera.h"
#include "dungeon.h"
#include "particle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexMiniMap(void);
void SetVertexMiniMap(void);
void SetDiffuseMiniMap(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
VERTEX_2D		g_vertexWkMiniMap[MAP_WIDTH * MAP_HEIGHT][NUM_VERTEX];	// 頂点情報格納ワーク
D3DXVECTOR3		g_posMiniMap;											// ミニマップの位置
float			g_scaleMiniMap;											// パネルサイズ
float			g_scaleMiniMapTo;										// パネルサイズの遷移
bool			g_bMiniMap[MAP_WIDTH][MAP_HEIGHT];						// ミニマップ描画判定

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMiniMap(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posMiniMap = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_scaleMiniMap = MINIMAP_SIZE_SMALL;
	g_scaleMiniMapTo = MINIMAP_SIZE_SMALL;

	for (int i = 0; i < MAP_WIDTH; i++)
	{
		for (int j = 0; j < MAP_HEIGHT; j++)
		{
			g_bMiniMap[i][j] = false;
		}
	}

	// 頂点情報の作成
	MakeVertexMiniMap();

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitMiniMap(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMiniMap(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F2))
	{
		SetMiniMap(0, 0, MAP_WIDTH - 1, MAP_HEIGHT - 1);
	}
#endif

	if (GetKeyboardTrigger(DIK_M) || IsButtonTriggered(0, BUTTON_L))
	{
		if (g_scaleMiniMap == MINIMAP_SIZE_SMALL)
		{
			g_scaleMiniMapTo = MINIMAP_SIZE_LARGE;
		}
		else if (g_scaleMiniMap == MINIMAP_SIZE_LARGE)
		{
			g_scaleMiniMapTo = MINIMAP_SIZE_SMALL;
		}
	}

	if (g_scaleMiniMap != g_scaleMiniMapTo)
	{
		if (fabs(g_scaleMiniMap - g_scaleMiniMapTo) < 0.1)
		{
			g_scaleMiniMap = g_scaleMiniMapTo;
		}
		else
		{
			g_scaleMiniMap = (g_scaleMiniMap * 3 + g_scaleMiniMapTo) * 0.25;
		}
	}

	g_posMiniMap = D3DXVECTOR3(MINIMAP_POS_X, MINIMAP_POS_Y - g_scaleMiniMap * MAP_HEIGHT, 0.0f);
	SetVertexMiniMap();
	SetDiffuseMiniMap();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMiniMap(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ライティングを無効に
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 加算合成に設定
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// マップチップ
	for (int i = 0; i < MAP_WIDTH; i++)
	{
		for (int j = 0; j < MAP_HEIGHT; j++)
		{
			if (g_bMiniMap[i][j])
			{
				// ポリゴンの描画
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkMiniMap[MAP_HEIGHT * i + j], sizeof(VERTEX_2D));
			}
		}
	}

	// ライティングを有効に
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 通常ブレンドに戻す
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexMiniMap(void)
{
	// マップ全体を処理する
	for (int i = 0; i < MAP_WIDTH; i++)
	{
		for (int j = 0; j < MAP_HEIGHT; j++)
		{
			// rhwの設定
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][0].rhw =
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][1].rhw =
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][2].rhw =
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][3].rhw = 1.0f;

			// 反射光の設定
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
	}

	return S_OK;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexMiniMap(void)
{
	for (int i = 0; i < MAP_WIDTH; i++)
	{
		for (int j = 0; j < MAP_HEIGHT; j++)
		{
			// 頂点座標の設定
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][0].vtx.x = g_scaleMiniMap * i;
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][0].vtx.y = g_scaleMiniMap * j;
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][0].vtx.z = 0.0f;
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][0].vtx += g_posMiniMap;

			g_vertexWkMiniMap[MAP_HEIGHT * i + j][1].vtx.x = g_scaleMiniMap * (i + 1);
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][1].vtx.y = g_scaleMiniMap * j;
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][1].vtx.z = 0.0f;
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][1].vtx += g_posMiniMap;

			g_vertexWkMiniMap[MAP_HEIGHT * i + j][2].vtx.x = g_scaleMiniMap * i;
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][2].vtx.y = g_scaleMiniMap * (j + 1);
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][2].vtx.z = 0.0f;
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][2].vtx += g_posMiniMap;

			g_vertexWkMiniMap[MAP_HEIGHT * i + j][3].vtx.x = g_scaleMiniMap * (i + 1);
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][3].vtx.y = g_scaleMiniMap * (j + 1);
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][3].vtx.z = 0.0f;
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][3].vtx += g_posMiniMap;
		}
	}
}

//=============================================================================
// 反射光の設定
//=============================================================================
void SetDiffuseMiniMap(void)
{
	PLAYER *player = GetPlayer();
	D3DXCOLOR col = D3DCOLOR_RGBA(255, 255, 255, 255);
	int i, j;
	for (i = 0; i < MAP_WIDTH; i++)
	{
		for (j = 0; j < MAP_HEIGHT; j++)
		{
			// 反射光の設定
			switch (GetMap(i,j))
			{
			case MAP_NOT_OBJECT:
			case MAP_BOUNDARY:
			case MAP_WALL_UP:
			case MAP_WALL_DOWN:
			case MAP_WALL_RIGHT:
			case MAP_WALL_LEFT:
			case MAP_WALL_UPRIGHT:
			case MAP_WALL_UPLEFT:
			case MAP_WALL_DOWNLEFT:
			case MAP_WALL_DOWNRIGHT:
				col = D3DCOLOR_RGBA(0, 0, 0, 0);
				break;
			case MAP_ROOM:
			case MAP_START:
				if (GetRoom2(i, j) != NULL)
				{
					if (GetRoom2(i, j)->clear)
					{
						if (GetRoomID(i, j) == GetExitRoomID())
						{
							col = D3DCOLOR_RGBA(128, 255, 0, 128);
						}
						else
						{
							col = D3DCOLOR_RGBA(0, 255, 255, 128);
						}
					}
					else
					{
						col = D3DCOLOR_RGBA(0, 128, 255, 128);
					}
				}
				else

				{
					col = D3DCOLOR_RGBA(0, 0, 0, 0);

				}
				break;
			case MAP_TRAIL:
			case MAP_PATH:
				col = D3DCOLOR_RGBA(128, 0, 255, 128);
				break;
			case MAP_EXIT:
				col = D3DCOLOR_RGBA(255, 255, 255, 128);
				break;
			default:
				break;
			}

			g_vertexWkMiniMap[MAP_HEIGHT * i + j][0].diffuse =
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][1].diffuse =
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][2].diffuse =
			g_vertexWkMiniMap[MAP_HEIGHT * i + j][3].diffuse = col;
		}
	}

	i = (int)(player->pos.x / TEXTURE_DUNGEON_SIZE_X);
	j = (int)(player->pos.y / TEXTURE_DUNGEON_SIZE_Y);
	col = D3DCOLOR_RGBA(255, 255, 0, 128);

	g_vertexWkMiniMap[MAP_HEIGHT * i + j][0].diffuse =
	g_vertexWkMiniMap[MAP_HEIGHT * i + j][1].diffuse =
	g_vertexWkMiniMap[MAP_HEIGHT * i + j][2].diffuse =
	g_vertexWkMiniMap[MAP_HEIGHT * i + j][3].diffuse = col;
}

//=============================================================================
// ミニマップの可視化
//=============================================================================
void SetMiniMap(int startX, int startY, int endX, int endY)
{
	if (startX < 0) startX = 0;
	if (startY < 0) startY = 0;
	if (endX > MAP_WIDTH) endX = MAP_WIDTH;
	if (endY > MAP_HEIGHT) endY = MAP_HEIGHT;

	for (int x = startX; x < endX; x++)
	{
		for (int y = startY; y < endY; y++)
		{
			if (!g_bMiniMap[x][y])
			{
				g_bMiniMap[x][y] = true;
			}
		}
	}
}