//=============================================================================
//
// ポインター処理 [pointer.cpp]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#include "pointer.h"
#include "enemy.h"
#include "camera.h"
#include "player.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_GAME_POINTER			_T("data/TEXTURE/pointer001.png")	// 画像
#define TEXTURE_POINTER_SIZE_X			(16)	// テクスチャサイズ
#define TEXTURE_POINTER_SIZE_Y			(16)	// 同上

#define POINTER_TEXTURE_PATTERN_DIVIDE	(2)		// アニメパターンのテクスチャ内分割数（X)

#define POINTER_MAX						(10)	// ポインターの最大数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPointer(int no);
void SetTexturePointer(int no, int cntPattern);
void SetVertexPointer(int no);
void SetDiffusePointer(int no, int fade);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTexturePointer = NULL;		// テクスチャへのポリゴン

POINTER					pointerWk[POINTER_MAX];				// ポインター構造体

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPointer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	POINTER *pointer = pointerWk;			// ポインターのポインタ

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
			TEXTURE_GAME_POINTER,			// ファイルの名前
			&g_pD3DTexturePointer);			// 読み込むメモリのポインタ
	}

	// ポインターの初期化処理
	for (int i = 0; i < POINTER_MAX; i++, pointer++)
	{
		pointer->use = false;										// 使用
		pointer->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 座標データを初期化
		pointer->subRot = 0.0f;
		pointer->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 回転データを初期化

		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_POINTER_SIZE_X, TEXTURE_POINTER_SIZE_Y);
		pointer->Radius = D3DXVec2Length(&temp);					// ポインターの半径を初期化
		pointer->BaseAngle = atan2f(TEXTURE_POINTER_SIZE_Y, TEXTURE_POINTER_SIZE_X);	// ポインターの角度を初期化

		pointer->Texture = g_pD3DTexturePointer;					// テクスチャ情報
		MakeVertexPointer(i);										// 頂点情報の作成
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPointer(void)
{
	if (g_pD3DTexturePointer != NULL)
	{	// テクスチャの開放
		g_pD3DTexturePointer->Release();
		g_pD3DTexturePointer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePointer(void)
{
	POINTER *pointer = pointerWk;		// ポインターのポインターを初期化
	PLAYER *player = GetPlayer();
	ENEMY *enemy;
	D3DXVECTOR3 cameraPos = *GetCameraPos();

	int fade;

	for (int i = 0; i < POINTER_MAX; i++, pointer++)
	{
		if (pointer->use)	
		{	// 使用している状態なら更新する
			enemy = GetEnemy(pointer->no);

			if (enemy->state == ENEMY_DESPAWN || enemy->state == ENEMY_DEAD)
			{
				pointer->use = false;
				continue;
			}

			if (enemy->pos.x > -cameraPos.x &&
				enemy->pos.x < -cameraPos.x + SCREEN_WIDTH &&
				enemy->pos.y > -cameraPos.y &&
				enemy->pos.y < -cameraPos.y + SCREEN_HEIGHT)
			{
				fade = 0;
			}
			else
			{
				fade = 255;

				pointer->subRot = 0.02f;
				if (pointer->subRot > D3DX_PI)
				{
					pointer->subRot -= D3DX_PI;
				}

				pointer->rot.z = -D3DX_PI * 0.5 + atan2(enemy->pos.y - player->pos.y, enemy->pos.x - player->pos.x);

				pointer->pos.x = player->pos.x + cosf(pointer->rot.z + D3DX_PI * 0.5f) * TEXTURE_PLAYER_SIZE_X * 2;
				pointer->pos.y = player->pos.y + sinf(pointer->rot.z + D3DX_PI * 0.5f) * TEXTURE_PLAYER_SIZE_Y * 2;

				// テクスチャ座標を設定
				SetTexturePointer(i, (int)(sinf(pointer->subRot) + 0.3f));
			}
			// 反射光を設定
			SetDiffusePointer(i, fade);
			
			// 頂点を設定
			SetVertexPointer(i);	
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPointer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	POINTER *pointer = pointerWk;		// ポインターのポインタ

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < POINTER_MAX; i++, pointer++)
	{
		if (pointer->use)					
		{	// 使用している状態なら描画する
			// テクスチャの設定
			pDevice->SetTexture(0, pointer->Texture);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, pointer->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexPointer(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	POINTER *pointer = &pointerWk[no];

	// 頂点座標の設定
	SetVertexPointer(no);

	// rhwの設定
	pointer->vertexWk[0].rhw =
	pointer->vertexWk[1].rhw =
	pointer->vertexWk[2].rhw =
	pointer->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	pointer->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pointer->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pointer->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pointer->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	pointer->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pointer->vertexWk[1].tex = D3DXVECTOR2(1.0f / POINTER_TEXTURE_PATTERN_DIVIDE, 0.0f);
	pointer->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pointer->vertexWk[3].tex = D3DXVECTOR2(1.0f / POINTER_TEXTURE_PATTERN_DIVIDE, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTexturePointer(int no, int cntPattern)
{
	POINTER *pointer = &pointerWk[no];

	// テクスチャ座標の設定
	int x = cntPattern;
	float sizeX = 1.0f / POINTER_TEXTURE_PATTERN_DIVIDE;

	pointer->vertexWk[0].tex = D3DXVECTOR2(x * sizeX, 0.0f);
	pointer->vertexWk[1].tex = D3DXVECTOR2((x + 1) * sizeX, 0.0f);
	pointer->vertexWk[2].tex = D3DXVECTOR2(x * sizeX, 1.0f);
	pointer->vertexWk[3].tex = D3DXVECTOR2((x + 1) * sizeX, 1.0f);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexPointer(int no)
{
	POINTER *pointer = &pointerWk[no];
	D3DXVECTOR3 *posCamera = GetCameraPos();

	// 頂点座標の設定
	pointer->vertexWk[0].vtx.x = pointer->pos.x + posCamera->x - cosf(pointer->BaseAngle + pointer->rot.z) * pointer->Radius;
	pointer->vertexWk[0].vtx.y = pointer->pos.y + posCamera->y - sinf(pointer->BaseAngle + pointer->rot.z) * pointer->Radius;
	pointer->vertexWk[0].vtx.z = 0.0f;

	pointer->vertexWk[1].vtx.x = pointer->pos.x + posCamera->x + cosf(pointer->BaseAngle - pointer->rot.z) * pointer->Radius;
	pointer->vertexWk[1].vtx.y = pointer->pos.y + posCamera->y - sinf(pointer->BaseAngle - pointer->rot.z) * pointer->Radius;
	pointer->vertexWk[1].vtx.z = 0.0f;

	pointer->vertexWk[2].vtx.x = pointer->pos.x + posCamera->x - cosf(pointer->BaseAngle - pointer->rot.z) * pointer->Radius;
	pointer->vertexWk[2].vtx.y = pointer->pos.y + posCamera->y + sinf(pointer->BaseAngle - pointer->rot.z) * pointer->Radius;
	pointer->vertexWk[2].vtx.z = 0.0f;

	pointer->vertexWk[3].vtx.x = pointer->pos.x + posCamera->x + cosf(pointer->BaseAngle + pointer->rot.z) * pointer->Radius;
	pointer->vertexWk[3].vtx.y = pointer->pos.y + posCamera->y + sinf(pointer->BaseAngle + pointer->rot.z) * pointer->Radius;
	pointer->vertexWk[3].vtx.z = 0.0f;
}

void SetDiffusePointer(int no, int fade)
{
	POINTER *pointer = &pointerWk[no];

	// 反射光の設定
	pointer->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, fade);
	pointer->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, fade);
	pointer->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, fade);
	pointer->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, fade);
}

//=============================================================================
// ポインター取得関数
//=============================================================================
POINTER *GetPointer(int no)
{
	return(&pointerWk[no]);
}

//=============================================================================
// ポインター設定関数
//=============================================================================
void SetPointer(int no)
{
	POINTER *pointer = pointerWk;

	for (int i = 0; i < POINTER_MAX; i++, pointer++)
	{
		if (!pointer->use)
		{
			pointer->use = true;
			pointer->no = no;

			return;			// 1つセットしたので終了する
		}
	}
}
