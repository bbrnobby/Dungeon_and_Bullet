//=============================================================================
//
// 手処理 [hand.cpp]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#include "main.h"
#include "hand.h"
#include "player.h"
#include "gun.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
const char *TEXTURE_GAME_HAND[] =
{
	"data/TEXTURE/hand001.png",
	"data/TEXTURE/hand002.png"
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexHand();
void SetTextureHand();
void SetVertexHand(bool isLEFT);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureHand[GUN_MAX] = {};	// テクスチャへのポリゴン
HAND					handWk;								// 手構造体


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitHand(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HAND *hand = &handWk;						// 手のポインタ

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		for (int i = 0; i < GUN_MAX; i++)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
				TEXTURE_GAME_HAND[i],			// ファイルの名前
				&g_pD3DTextureHand[i]);			// 読み込むメモリのポインタ
		}
	}

	// 手の初期化処理
	hand->use = true;							
	hand->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	hand->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	hand->dirRot = 0.0f;
	hand->subRot = 0.0f;

	D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_HAND_SIZE_X, TEXTURE_HAND_SIZE_Y);
	hand->Radius = D3DXVec2Length(&temp);								// 手の半径を初期化
	hand->BaseAngle = atan2f(TEXTURE_HAND_SIZE_Y, TEXTURE_HAND_SIZE_X);	// 手の角度を初期化

	hand->Texture = NULL;												// テクスチャ情報を初期化
	MakeVertexHand();													// 頂点情報の作成

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitHand(void)
{
	for (int i = 0; i < GUN_MAX; i++)
	{
		if (g_pD3DTextureHand != NULL)
		{	// テクスチャの開放
			g_pD3DTextureHand[i]->Release();
			g_pD3DTextureHand[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateHand(void)
{
	HAND *hand = &handWk;			// 手のポインタ
	PLAYER *player = GetPlayer();	// プレイヤーのポインタ
	GUN *gun = GetGun();			// 銃のポインタ

	// プレイヤーの状態で銃の描画を判断
	switch (player->state)
	{
	case PLAYER_LOCK:
		hand->use = true;
		break;
	case PLAYER_WALK:
		hand->use = false;
		break;
	case PLAYER_RUN:
		hand->use = false;
		break;
	}

	// 使用している状態なら更新する
	if (hand->use == true)			
	{
		hand->pos = gun->pos;
		hand->pos.x += TEXTURE_PLAYER_SIZE_X / 2 * ((2 - player->dir) % 2);
		hand->rot = gun->rot;
		hand->dirRot = gun->dirRot;
		hand->subRot = gun->subRot;

		hand->Texture = g_pD3DTextureHand[gun->type];	// テクスチャ情報を更新
		SetVertexHand(player->dir == DIR_LEFT);			// 移動後の座標で頂点を設定
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawHand(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HAND *hand = &handWk;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 使用している状態なら描画する
	if (hand->use == true)			
	{
		// テクスチャの設定
		pDevice->SetTexture(0, hand->Texture);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, hand->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexHand(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HAND *hand = &handWk;

	// 頂点座標の設定
	SetVertexHand(false);

	// rhwの設定
	hand->vertexWk[0].rhw =
	hand->vertexWk[1].rhw =
	hand->vertexWk[2].rhw =
	hand->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	hand->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	hand->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	hand->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	hand->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	SetTextureHand();

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureHand()
{
	HAND *hand = &handWk;

	// テクスチャ座標の設定
	hand->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	hand->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	hand->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	hand->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexHand(bool isLEFT)
{
	PLAYER *player = GetPlayer();
	HAND *hand = &handWk;
	D3DXVECTOR3 *posCamera = GetCameraPos();

	// 頂点座標の設定
	if (isLEFT)
	{	// 左を向いている時
		hand->vertexWk[0].vtx.x = hand->pos.x + posCamera->x - cosf(hand->BaseAngle - hand->rot.z) * hand->Radius;
		hand->vertexWk[0].vtx.y = hand->pos.y + posCamera->y + sinf(hand->BaseAngle - hand->rot.z) * hand->Radius;
		hand->vertexWk[0].vtx.z = 0.0f;

		hand->vertexWk[1].vtx.x = hand->pos.x + posCamera->x + cosf(hand->BaseAngle + hand->rot.z) * hand->Radius;
		hand->vertexWk[1].vtx.y = hand->pos.y + posCamera->y + sinf(hand->BaseAngle + hand->rot.z) * hand->Radius;
		hand->vertexWk[1].vtx.z = 0.0f;

		hand->vertexWk[2].vtx.x = hand->pos.x + posCamera->x - cosf(hand->BaseAngle + hand->rot.z) * hand->Radius;
		hand->vertexWk[2].vtx.y = hand->pos.y + posCamera->y - sinf(hand->BaseAngle + hand->rot.z) * hand->Radius;
		hand->vertexWk[2].vtx.z = 0.0f;

		hand->vertexWk[3].vtx.x = hand->pos.x + posCamera->x + cosf(hand->BaseAngle - hand->rot.z) * hand->Radius;
		hand->vertexWk[3].vtx.y = hand->pos.y + posCamera->y - sinf(hand->BaseAngle - hand->rot.z) * hand->Radius;
		hand->vertexWk[3].vtx.z = 0.0f;
	}
	else
	{
		hand->vertexWk[0].vtx.x = hand->pos.x + posCamera->x - cosf(hand->BaseAngle + hand->rot.z) * hand->Radius;
		hand->vertexWk[0].vtx.y = hand->pos.y + posCamera->y - sinf(hand->BaseAngle + hand->rot.z) * hand->Radius;
		hand->vertexWk[0].vtx.z = 0.0f;

		hand->vertexWk[1].vtx.x = hand->pos.x + posCamera->x + cosf(hand->BaseAngle - hand->rot.z) * hand->Radius;
		hand->vertexWk[1].vtx.y = hand->pos.y + posCamera->y - sinf(hand->BaseAngle - hand->rot.z) * hand->Radius;
		hand->vertexWk[1].vtx.z = 0.0f;

		hand->vertexWk[2].vtx.x = hand->pos.x + posCamera->x - cosf(hand->BaseAngle - hand->rot.z) * hand->Radius;
		hand->vertexWk[2].vtx.y = hand->pos.y + posCamera->y + sinf(hand->BaseAngle - hand->rot.z) * hand->Radius;
		hand->vertexWk[2].vtx.z = 0.0f;

		hand->vertexWk[3].vtx.x = hand->pos.x + posCamera->x + cosf(hand->BaseAngle + hand->rot.z) * hand->Radius;
		hand->vertexWk[3].vtx.y = hand->pos.y + posCamera->y + sinf(hand->BaseAngle + hand->rot.z) * hand->Radius;
		hand->vertexWk[3].vtx.z = 0.0f;
	}

	hand->vertexWk[0].vtx += player->pos;
	hand->vertexWk[1].vtx += player->pos;
	hand->vertexWk[2].vtx += player->pos;
	hand->vertexWk[3].vtx += player->pos;
}

//=============================================================================
// 手取得関数
//=============================================================================
HAND *GetHand()
{
	return(&handWk);
}


