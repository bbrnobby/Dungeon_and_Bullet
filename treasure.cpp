//=============================================================================
//
// 宝箱処理 [treasure.cpp]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#include "treasure.h"
#include "player.h"
#include "drop.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTreasure(int no);
void SetTextureTreasure(int no);
void SetVertexTreasure(int no);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTreasure = NULL;					// テクスチャへのポリゴン

TREASURE				treasureWk[TREASURE_MAX];						// 宝箱構造体

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTreasure(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TREASURE *treasure = treasureWk;					// 宝箱のポインターを初期化

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_TREASURE,				// ファイルの名前
			&g_pD3DTextureTreasure);			// 読み込むメモリのポインタ
	}

	// 宝箱の初期化処理
	for (int i = 0; i < TREASURE_MAX; i++, treasure++)
	{
		treasure->use = false;										// 使用
		treasure->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 座標データを初期化
		treasure->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		treasure->vec = 0.0f;
		treasure->height = 0.0f;
		treasure->type = TREASURE_COMMON;
		treasure->state = BOX_NONE;
		treasure->size = 0;

		treasure->PatternAnim = 0;
		treasure->CountAnim = 0;										// アニメカウントを初期化

		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_TREASURE_SIZE, TEXTURE_TREASURE_SIZE);
		treasure->Radius = D3DXVec2Length(&temp);						// 半径を初期化
		treasure->BaseAngle = atan2f(TEXTURE_TREASURE_SIZE, TEXTURE_TREASURE_SIZE);	// 角度を初期化

		treasure->Texture = g_pD3DTextureTreasure;						// テクスチャ情報
		MakeVertexTreasure(i);									// 頂点情報の作成
	}

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitTreasure(void)
{
	if (g_pD3DTextureTreasure != NULL)
	{	// テクスチャの開放
		g_pD3DTextureTreasure->Release();
		g_pD3DTextureTreasure = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTreasure(void)
{
	TREASURE *treasure = treasureWk;

	for (int i = 0; i < TREASURE_MAX; i++, treasure++)
	{
		if (treasure->use)			// 使用している状態なら更新する
		{
			switch(treasure->state)
			{
			case BOX_SPAWNED:
				treasure->height += treasure->vec;
				if (treasure->height <= 0)
				{
					treasure->rot.z = 0.0f;
					treasure->height = 0.0f;
					treasure->vec = 0.0f;
					treasure->PatternAnim = 0;
					treasure->state = BOX_WAITING;
					// テクスチャ座標を設定
					SetTextureTreasure(i);
				}
				else
				{
					treasure->rot.z = (1 - treasure->PatternAnim) * 0.25f;
					treasure->vec -= 0.5f;
				}
			case BOX_OPENED:
				if (treasure->PatternAnim < TEXTURE_PATTERN_TREASURE_DIVIDE_Y - 2)
				{
					treasure->CountAnim++;
					if ((treasure->CountAnim % TREASURE_TIME_ANIMATION) == 0)
					{
						// パターンの切り替え
						treasure->PatternAnim++;
						treasure->CountAnim = 0;

						// テクスチャ座標を設定
						SetTextureTreasure(i);
					}
				}
				else if(treasure->state == BOX_OPENED)
				{
					int drop;
					for (int j = 0; j < treasure->size; j++)
					{
						switch (treasure->type)
						{
						case TREASURE_COMMON:
							drop = rand() % 3;
							break;
						//case TREASURE_GUNBOX:
						//	if (treasure->size == TEXTURE_TREASURE_SIZE / 4 * 3 / 8) drop = DROP_PISTOL;
						//	else if (treasure->size == TEXTURE_TREASURE_SIZE / 8) drop = DROP_SHOTGUN;
						//	else if (treasure->size == TEXTURE_TREASURE_SIZE / 2 * 3 / 8) drop = DROP_LASERGUN;
						//	j = treasure->size;
						//	break;
						case TREASURE_HEART:
							drop = rand() % 2 + DROP_MINIHEART;
							j++;
							break;
						case TREASURE_RARE:
							drop = rand() % 5 + DROP_JEWEL001;
							break;
						}
						SetDrop(treasure->pos.x, treasure->pos.y, drop, treasure->size * 0.5);
					}
					treasure->state = BOX_EMPTY;
				}
				break;
			}


			treasure->pos.y -= treasure->vec;

			SetVertexTreasure(i);	// 移動後の座標で頂点を設定
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTreasure(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TREASURE *treasure = treasureWk;

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureTreasure);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < TREASURE_MAX; i++, treasure++)
	{
		if (treasure->use)					// 使用している状態なら描画する
		{
			// テクスチャの設定
			pDevice->SetTexture(0, treasure->Texture);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, treasure->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTreasure(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TREASURE *treasure = &treasureWk[no];

	// 頂点座標の設定
	SetVertexTreasure(no);

	// rhwの設定
	treasure->vertexWk[0].rhw =
	treasure->vertexWk[1].rhw =
	treasure->vertexWk[2].rhw =
	treasure->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	treasure->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	treasure->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	treasure->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	treasure->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	treasure->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	treasure->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_X, 0.0f);
	treasure->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_Y);
	treasure->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_X, 1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_Y);


	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureTreasure(int no)
{
	TREASURE *treasure = &treasureWk[no];

	// テクスチャ座標の設定
	treasure->vertexWk[0].tex = D3DXVECTOR2(treasure->type * (1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_X), treasure->PatternAnim * (1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_Y));
	treasure->vertexWk[1].tex = D3DXVECTOR2((treasure->type + 1) * (1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_X), treasure->PatternAnim * (1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_Y));
	treasure->vertexWk[2].tex = D3DXVECTOR2(treasure->type * (1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_X), (treasure->PatternAnim + 1) * (1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_Y));
	treasure->vertexWk[3].tex = D3DXVECTOR2((treasure->type + 1) * (1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_X), (treasure->PatternAnim + 1) * (1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_Y));
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexTreasure(int no)
{
	TREASURE *treasure = &treasureWk[no];
	D3DXVECTOR3 *posCamera = GetCameraPos();

	// 頂点座標の設定
	treasure->vertexWk[0].vtx.x = treasure->pos.x + posCamera->x - cosf(treasure->BaseAngle + treasure->rot.z) * treasure->Radius;;
	treasure->vertexWk[0].vtx.y = treasure->pos.y + posCamera->y - sinf(treasure->BaseAngle + treasure->rot.z) * treasure->Radius;
	treasure->vertexWk[0].vtx.z = 0.0f;

	treasure->vertexWk[1].vtx.x = treasure->pos.x + posCamera->x + cosf(treasure->BaseAngle - treasure->rot.z) * treasure->Radius;
	treasure->vertexWk[1].vtx.y = treasure->pos.y + posCamera->y - sinf(treasure->BaseAngle - treasure->rot.z) * treasure->Radius;
	treasure->vertexWk[1].vtx.z = 0.0f;

	treasure->vertexWk[2].vtx.x = treasure->pos.x + posCamera->x - cosf(treasure->BaseAngle - treasure->rot.z) * treasure->Radius;
	treasure->vertexWk[2].vtx.y = treasure->pos.y + posCamera->y + sinf(treasure->BaseAngle - treasure->rot.z) * treasure->Radius;
	treasure->vertexWk[2].vtx.z = 0.0f;

	treasure->vertexWk[3].vtx.x = treasure->pos.x + posCamera->x + cosf(treasure->BaseAngle + treasure->rot.z) * treasure->Radius;
	treasure->vertexWk[3].vtx.y = treasure->pos.y + posCamera->y + sinf(treasure->BaseAngle + treasure->rot.z) * treasure->Radius;
	treasure->vertexWk[3].vtx.z = 0.0f;
}

TREASURE *GetTreasure(int no)
{
	return (&treasureWk[no]);
}

void SetTreasure(float posX, float posY, int type, int size)
{
	TREASURE *treasure = treasureWk;

	for (int i = 0; i < TREASURE_MAX; i++, treasure++)
	{
		if (!treasure->use)
		{
			treasure->use = true;
			treasure->pos.x = posX;
			treasure->pos.y = posY;
			treasure->vec = 10.0f;
			treasure->height = 50.0f;
			treasure->type = type;
			treasure->state = BOX_SPAWNED;
			treasure->size = size / 8;

			treasure->PatternAnim = 0;
			treasure->CountAnim = 0;									// アニメカウントを初期化

			D3DXVECTOR2 temp = D3DXVECTOR2(size, size);
			treasure->Radius = D3DXVec2Length(&temp);					// 半径を初期化
			treasure->BaseAngle = atan2f(size, size);					// 角度を初期化

			treasure->pos.y -= treasure->vec;

			SetVertexTreasure(i);	// 移動後の座標で頂点を設定

			SetTextureTreasure(i);	// テクスチャ座標を設定
			return;
		}
	}
}