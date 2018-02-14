//=============================================================================
//
// ドロップ処理 [drop.cpp]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#include "drop.h"
#include "score.h"
#include "player.h"
#include "font.h"
#include "heart.h"
#include "camera.h"
#include "gun.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_GAME_DROP	_T("data/TEXTURE/drop001.png")	// 画像

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexDrop(int no);
void SetTextureDrop(int no);
void SetVertexDrop(int no);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureDrop = NULL;	// テクスチャへのポリゴン

DROP				dropWk[MAX_DROP];			// ドロップ構造体
int					mesDrop[MAX_DROP];			// メッセージの位置

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitDrop(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DROP *drop = dropWk;					// ドロップのポインターを初期化

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
			TEXTURE_GAME_DROP,				// ファイルの名前
			&g_pD3DTextureDrop);			// 読み込むメモリのポインタ
	}

	// ドロップの初期化処理
	for (int i = 0; i < MAX_DROP; i++, drop++)
	{
		drop->use = false;										// 使用
		drop->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 座標データを初期化
		drop->vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		drop->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		drop->subVec = 0.0f;
		drop->height = 0.0f;
		drop->size = TEXTURE_DROP_SIZE;
		drop->type = 0;

		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_DROP_SIZE, TEXTURE_DROP_SIZE);
		drop->Radius = D3DXVec2Length(&temp);							// 半径を初期化
		drop->BaseAngle = atan2f(TEXTURE_DROP_SIZE, TEXTURE_DROP_SIZE);	// 角度を初期化

		drop->Texture = g_pD3DTextureDrop;								// テクスチャ情報
		MakeVertexDrop(i);												// 頂点情報の作成

		mesDrop[i] = 0;
	}

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitDrop(void)
{
	if (g_pD3DTextureDrop != NULL)
	{	
		// テクスチャの開放
		g_pD3DTextureDrop->Release();
		g_pD3DTextureDrop = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateDrop(void)
{
	DROP *drop = dropWk;

	for (int i = 0; i < MAX_DROP; i++, drop++)
	{
		// 使用している状態なら更新する
		if (drop->use)			
		{
			drop->height += drop->subVec;
			drop->rot.z += drop->vecRot;
			if (drop->height <= 0)
			{
				drop->height = 0.0f;
				drop->subVec = 0.0f;
				drop->rot.z = 0.0f;
			}
			else
			{
				drop->pos += drop->vec;
				drop->vecRot -= 0.01f;
				drop->subVec -= 0.5f;
			}
			drop->pos.y -= drop->subVec;
			
			// 移動後の座標で頂点を設定
			SetVertexDrop(i);	
		}
		if (mesDrop[i] > 0)
		{
			mesDrop[i]--;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawDrop(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DROP *drop = dropWk;

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureDrop);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_DROP; i++, drop++)
	{
		// 使用している状態なら描画する
		if (drop->use)					
		{
			// テクスチャの設定
			pDevice->SetTexture(0, drop->Texture);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, drop->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexDrop(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DROP *drop = &dropWk[no];

	// 頂点座標の設定
	SetVertexDrop(no);

	// rhwの設定
	drop->vertexWk[0].rhw =
	drop->vertexWk[1].rhw =
	drop->vertexWk[2].rhw =
	drop->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	drop->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	drop->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	drop->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	drop->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	drop->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	drop->vertexWk[1].tex = D3DXVECTOR2(1.0f / DROP_MAX, 0.0f);
	drop->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	drop->vertexWk[3].tex = D3DXVECTOR2(1.0f / DROP_MAX, 1.0f);


	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureDrop(int no)
{
	DROP *drop = &dropWk[no];

	// テクスチャ座標の設定
	drop->vertexWk[0].tex = D3DXVECTOR2(drop->type * (1.0f / DROP_MAX), 0.0f);
	drop->vertexWk[1].tex = D3DXVECTOR2((drop->type + 1) * (1.0f / DROP_MAX), 0.0f);
	drop->vertexWk[2].tex = D3DXVECTOR2(drop->type * (1.0f / DROP_MAX), 1.0f);
	drop->vertexWk[3].tex = D3DXVECTOR2((drop->type + 1) * (1.0f /DROP_MAX), 1.0f);
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexDrop(int no)
{
	DROP *drop = &dropWk[no];
	D3DXVECTOR3 *posCamera = GetPosCamera();

	// 頂点座標の設定
	drop->vertexWk[0].vtx.x = drop->pos.x + posCamera->x - cosf(drop->BaseAngle + drop->rot.z) * drop->Radius;
	drop->vertexWk[0].vtx.y = drop->pos.y + posCamera->y - sinf(drop->BaseAngle + drop->rot.z) * drop->Radius;
	drop->vertexWk[0].vtx.z = 0.0f;

	drop->vertexWk[1].vtx.x = drop->pos.x + posCamera->x + cosf(drop->BaseAngle - drop->rot.z) * drop->Radius;
	drop->vertexWk[1].vtx.y = drop->pos.y + posCamera->y - sinf(drop->BaseAngle - drop->rot.z) * drop->Radius;
	drop->vertexWk[1].vtx.z = 0.0f;

	drop->vertexWk[2].vtx.x = drop->pos.x + posCamera->x - cosf(drop->BaseAngle - drop->rot.z) * drop->Radius;
	drop->vertexWk[2].vtx.y = drop->pos.y + posCamera->y + sinf(drop->BaseAngle - drop->rot.z) * drop->Radius;
	drop->vertexWk[2].vtx.z = 0.0f;

	drop->vertexWk[3].vtx.x = drop->pos.x + posCamera->x + cosf(drop->BaseAngle + drop->rot.z) * drop->Radius;
	drop->vertexWk[3].vtx.y = drop->pos.y + posCamera->y + sinf(drop->BaseAngle + drop->rot.z) * drop->Radius;
	drop->vertexWk[3].vtx.z = 0.0f;
}

DROP *GetDrop(int no)
{
	return (&dropWk[no]);
}

void SetDrop(float posX, float posY, int type, int size)
{
	DROP *drop = dropWk;

	for (int i = 0; i < MAX_DROP; i++, drop++)
	{
		if (!drop->use)
		{
			int theta = rand() % 360;
			drop->use = true;
			drop->pos.x = posX;
			drop->pos.y = posY;
			drop->vec.x = size * cosf(theta / 180.0f * D3DX_PI);
			drop->vec.y = size * sinf(theta / 180.0f * D3DX_PI);
			drop->vecRot = 0.50f;
			drop->subVec = 10.0f;
			drop->height = 5.0f;
			drop->type = type;
			drop->size = TEXTURE_DROP_SIZE;
			
			drop->pos.y -= drop->subVec;

			D3DXVECTOR2 temp = D3DXVECTOR2(drop->size, drop->size);
			drop->Radius = D3DXVec2Length(&temp);				// 半径を初期化
			drop->BaseAngle = atan2f(drop->size, drop->size);	// 角度を初期化

			SetTextureDrop(i);
			SetVertexDrop(i);	// 移動後の座標で頂点を設定

			return;
		}
	}
}

void PickDrop(DROP *drop)
{
	PLAYER *player = GetPlayer();
	for (int i = 0; i < MAX_DROP; i++)
	{
		if (mesDrop[i] == 0)
		{
			switch (drop->type)
			{
			case DROP_COIN001:
				SetString("どうか　１０バレ", SCREEN_CENTER_X, DROP_MES_SIZE * (i + 1), DROP_MES_SIZE, DROP_MES_INTERVVAL);
				AddScore(10);
				break;
			case DROP_COIN002:
				SetString("ぎんか　5０バレ", SCREEN_CENTER_X, DROP_MES_SIZE * (i + 1), DROP_MES_SIZE, DROP_MES_INTERVVAL);
				AddScore(50);
				break;
			case DROP_COIN003:
				SetString("きんか　１0０バレ", SCREEN_CENTER_X, DROP_MES_SIZE * (i + 1), DROP_MES_SIZE, DROP_MES_INTERVVAL);
				AddScore(100);
				break;
			//case DROP_PISTOL:
			//case DROP_SHOTGUN:
			//case DROP_LASERGUN:
			//	return;
			case DROP_JEWEL001:
				SetString("サファイア　75バレ", SCREEN_CENTER_X, DROP_MES_SIZE * (i + 1), DROP_MES_SIZE, DROP_MES_INTERVVAL);
				AddScore(75);
				break;
			case DROP_JEWEL002:
				SetString("エメラルド　85バレ", SCREEN_CENTER_X, DROP_MES_SIZE * (i + 1), DROP_MES_SIZE, DROP_MES_INTERVVAL);
				AddScore(85);
				break;
			case DROP_JEWEL003:
				SetString("トパーズ　90バレ", SCREEN_CENTER_X, DROP_MES_SIZE * (i + 1), DROP_MES_SIZE, DROP_MES_INTERVVAL);
				AddScore(90);
				break;
			case DROP_JEWEL004:
				SetString("ルビー　105バレ", SCREEN_CENTER_X, DROP_MES_SIZE * (i + 1), DROP_MES_SIZE, DROP_MES_INTERVVAL);
				AddScore(105);
				break;
			case DROP_JEWEL005:
				SetString("ダイヤモンド　120バレ", SCREEN_CENTER_X, DROP_MES_SIZE * (i + 1), DROP_MES_SIZE, DROP_MES_INTERVVAL);
				AddScore(120);
				break;
			case DROP_MINIHEART:
				SetString("ちいさいハート　たいりょく+1", SCREEN_CENTER_X, DROP_MES_SIZE * (i + 1), DROP_MES_SIZE, DROP_MES_INTERVVAL);
				if (player->hp < MAX_HP * (HEART_DIVIDE - 1)) player->hp++;
				break;
			case DROP_BIGHEART:
				SetString("おおきいハート　たいりょく+3", SCREEN_CENTER_X, DROP_MES_SIZE * (i + 1), DROP_MES_SIZE, DROP_MES_INTERVVAL);
				player->hp += HEART_DIVIDE - 1;
				if (player->hp > MAX_HP * (HEART_DIVIDE - 1)) player->hp = MAX_HP * (HEART_DIVIDE - 1);
				break;
			case DROP_SLUG:
				SetString("スラッグだん　ショットガン＋6", SCREEN_CENTER_X, DROP_MES_SIZE * (i + 1), DROP_MES_SIZE, DROP_MES_INTERVVAL);
				GetSlug();
				break;
			}
			mesDrop[i] = DROP_MES_INTERVVAL;
			drop->use = false;
			return;
		}
	}
}
