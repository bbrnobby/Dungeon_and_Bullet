//=============================================================================
//
// ミニマップ処理 [minimap.cpp]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#include "minimap.h"
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
D3DXVECTOR3		g_scaleMiniMap;											// パネルの大きさ
bool			g_bMiniMap[MAP_WIDTH][MAP_HEIGHT];						// ミニマップ描画判定

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMiniMap(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posMiniMap = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_scaleMiniMap = D3DXVECTOR3(TEXTURE_MINIMAP_SIZE_X, TEXTURE_MINIMAP_SIZE_Y, 0.0f);

	for (int i = 0; i < MAP_WIDTH; i++)
	{

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
	// スクロール処理
	//g_posMiniMap = -GetPlayer(0)->pos + *GetCameraPos();
	g_posMiniMap = D3DXVECTOR3(30, SCREEN_HEIGHT - g_scaleMiniMap.y * MAP_HEIGHT - 30, 0.0f);
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
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkMiniMap[MAP_WIDTH * i + j], sizeof(VERTEX_2D));
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
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			// rhwの設定
			g_vertexWkMiniMap[MAP_WIDTH * i + j][0].rhw =
			g_vertexWkMiniMap[MAP_WIDTH * i + j][1].rhw =
			g_vertexWkMiniMap[MAP_WIDTH * i + j][2].rhw =
			g_vertexWkMiniMap[MAP_WIDTH * i + j][3].rhw = 1.0f;

			// 反射光の設定
			g_vertexWkMiniMap[MAP_WIDTH * i + j][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			g_vertexWkMiniMap[MAP_WIDTH * i + j][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			g_vertexWkMiniMap[MAP_WIDTH * i + j][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			g_vertexWkMiniMap[MAP_WIDTH * i + j][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
	}

	return S_OK;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexMiniMap(void)
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			// 頂点座標の設定
			g_vertexWkMiniMap[MAP_WIDTH * i + j][0].vtx.x = g_scaleMiniMap.x * j;
			g_vertexWkMiniMap[MAP_WIDTH * i + j][0].vtx.y = g_scaleMiniMap.y * i;
			g_vertexWkMiniMap[MAP_WIDTH * i + j][0].vtx.z = 0.0f;
			g_vertexWkMiniMap[MAP_WIDTH * i + j][0].vtx += g_posMiniMap;

			g_vertexWkMiniMap[MAP_WIDTH * i + j][1].vtx.x = g_scaleMiniMap.x * (j + 1);
			g_vertexWkMiniMap[MAP_WIDTH * i + j][1].vtx.y = g_scaleMiniMap.y * i;
			g_vertexWkMiniMap[MAP_WIDTH * i + j][1].vtx.z = 0.0f;
			g_vertexWkMiniMap[MAP_WIDTH * i + j][1].vtx += g_posMiniMap;

			g_vertexWkMiniMap[MAP_WIDTH * i + j][2].vtx.x = g_scaleMiniMap.x * j;
			g_vertexWkMiniMap[MAP_WIDTH * i + j][2].vtx.y = g_scaleMiniMap.y * (i + 1);
			g_vertexWkMiniMap[MAP_WIDTH * i + j][2].vtx.z = 0.0f;
			g_vertexWkMiniMap[MAP_WIDTH * i + j][2].vtx += g_posMiniMap;

			g_vertexWkMiniMap[MAP_WIDTH * i + j][3].vtx.x = g_scaleMiniMap.x * (j + 1);
			g_vertexWkMiniMap[MAP_WIDTH * i + j][3].vtx.y = g_scaleMiniMap.y * (i + 1);
			g_vertexWkMiniMap[MAP_WIDTH * i + j][3].vtx.z = 0.0f;
			g_vertexWkMiniMap[MAP_WIDTH * i + j][3].vtx += g_posMiniMap;
		}
	}
}

//=============================================================================
// 反射光の設定
//=============================================================================
void SetDiffuseMiniMap(void)
{
	D3DXCOLOR col = D3DCOLOR_RGBA(255, 255, 255, 255);
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
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
							col = D3DCOLOR_RGBA(255, 255, 0, 128);
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
					col = D3DCOLOR_RGBA(0, 128, 255, 128);

				}
				break;
			case MAP_TRAIL:
			case MAP_PATH:
				col = D3DCOLOR_RGBA(128, 0, 255, 128);
				break;
			case MAP_EXIT:
				col = D3DCOLOR_RGBA(255, 128, 0, 128);
				break;
			default:
				break;
			}

			g_vertexWkMiniMap[MAP_WIDTH * i + j][0].diffuse =
			g_vertexWkMiniMap[MAP_WIDTH * i + j][1].diffuse =
			g_vertexWkMiniMap[MAP_WIDTH * i + j][2].diffuse =
			g_vertexWkMiniMap[MAP_WIDTH * i + j][3].diffuse = col;
		}
	}
}
