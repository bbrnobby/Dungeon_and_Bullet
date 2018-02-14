//=============================================================================
//
// 体力処理 [heart.cpp]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#include "main.h"
#include "heart.h"
#include "player.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexHeart(void);
void SetTextureHeart(void);
void SetVertexHeart(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureHeart = NULL;		// テクスチャへのポリゴン
VERTEX_2D				g_vertexWkHeart[MAX_HP][NUM_VERTEX];	// 頂点情報格納ワーク

D3DXVECTOR3				g_posHeart;						// ポリゴンの移動量
D3DXVECTOR3				g_sizeHeart;

int						g_nHeart;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitHeart(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_HEART,				// ファイルの名前
			&g_pD3DTextureHeart);				// 読み込むメモリのポインタ
	}

	SetHeartArea(HEART_POS_X, HEART_POS_Y, TEXTURE_HEART_SIZE_X, TEXTURE_HEART_SIZE_Y);
	g_nHeart = 0;

	// 頂点情報の作成
	MakeVertexHeart();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitHeart(void)
{
	if (g_pD3DTextureHeart != NULL)
	{	// テクスチャの開放
		g_pD3DTextureHeart->Release();
		g_pD3DTextureHeart = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateHeart(void)
{
	// 毎フレーム実行される処理を記述する
	SetVertexHeart();
	SetTextureHeart();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawHeart(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureHeart);

	// HP
	for (int i = 0; i < MAX_HP; i++)
	{
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkHeart[i], sizeof(VERTEX_2D));
	}

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexHeart(void)
{
	// 桁数分処理する
	for (int i = 0; i < MAX_HP; i++)
	{
		// 頂点座標の設定
		g_vertexWkHeart[i][0].vtx.x = g_sizeHeart.x * i + g_posHeart.x;
		g_vertexWkHeart[i][0].vtx.y = g_posHeart.y;
		g_vertexWkHeart[i][0].vtx.z = 0.0f;
		g_vertexWkHeart[i][1].vtx.x = g_sizeHeart.x * i + g_posHeart.x + g_sizeHeart.x;
		g_vertexWkHeart[i][1].vtx.y = g_posHeart.y;
		g_vertexWkHeart[i][1].vtx.z = 0.0f;
		g_vertexWkHeart[i][2].vtx.x = g_sizeHeart.x * i + g_posHeart.x;
		g_vertexWkHeart[i][2].vtx.y = g_posHeart.y + g_sizeHeart.y;
		g_vertexWkHeart[i][2].vtx.z = 0.0f;
		g_vertexWkHeart[i][3].vtx.x = g_sizeHeart.x * i + g_posHeart.x + g_sizeHeart.x;
		g_vertexWkHeart[i][3].vtx.y = g_posHeart.y + g_sizeHeart.y;
		g_vertexWkHeart[i][3].vtx.z = 0.0f;

		// rhwの設定
		g_vertexWkHeart[i][0].rhw =
		g_vertexWkHeart[i][1].rhw =
		g_vertexWkHeart[i][2].rhw =
		g_vertexWkHeart[i][3].rhw = 1.0f;

		// 反射光の設定
		g_vertexWkHeart[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkHeart[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkHeart[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkHeart[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		g_vertexWkHeart[i][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		g_vertexWkHeart[i][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		g_vertexWkHeart[i][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		g_vertexWkHeart[i][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}
//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetTextureHeart(void)
{
	int number = GetPlayer()->hp;

	for (int i = 0; i < MAX_HP; i++)
	{
		// テクスチャ座標の設定
		int x = (number - (HEART_DIVIDE - 1) * i);
		if (x < 0) x = 0;
		else if (x > HEART_DIVIDE - 1) x = HEART_DIVIDE - 1;
		g_vertexWkHeart[i][0].tex = D3DXVECTOR2(1.0f / HEART_DIVIDE * x, 0.0f);
		g_vertexWkHeart[i][1].tex = D3DXVECTOR2(1.0f / HEART_DIVIDE * (x + 1), 0.0f);
		g_vertexWkHeart[i][2].tex = D3DXVECTOR2(1.0f / HEART_DIVIDE * x, 1.0f);
		g_vertexWkHeart[i][3].tex = D3DXVECTOR2(1.0f / HEART_DIVIDE * (x + 1), 1.0f);
	}
}

void SetVertexHeart(void)
{
	D3DXVECTOR3 *subPosCamera = GetSubPosCamera();

	for (int i = 0; i < MAX_HP; i++)
	{
		// 頂点座標の設定
		g_vertexWkHeart[i][0].vtx.x = g_sizeHeart.x * i + g_posHeart.x;
		g_vertexWkHeart[i][0].vtx.y = g_posHeart.y;
		g_vertexWkHeart[i][0].vtx.z = 0.0f;
		g_vertexWkHeart[i][0].vtx += *subPosCamera;

		g_vertexWkHeart[i][1].vtx.x = g_sizeHeart.x * i + g_posHeart.x + g_sizeHeart.x;
		g_vertexWkHeart[i][1].vtx.y = g_posHeart.y;
		g_vertexWkHeart[i][1].vtx.z = 0.0f;
		g_vertexWkHeart[i][1].vtx += *subPosCamera;

		g_vertexWkHeart[i][2].vtx.x = g_sizeHeart.x * i + g_posHeart.x;
		g_vertexWkHeart[i][2].vtx.y = g_posHeart.y + g_sizeHeart.y;
		g_vertexWkHeart[i][2].vtx.z = 0.0f;
		g_vertexWkHeart[i][2].vtx += *subPosCamera;

		g_vertexWkHeart[i][3].vtx.x = g_sizeHeart.x * i + g_posHeart.x + g_sizeHeart.x;
		g_vertexWkHeart[i][3].vtx.y = g_posHeart.y + g_sizeHeart.y;
		g_vertexWkHeart[i][3].vtx.z = 0.0f;
		g_vertexWkHeart[i][3].vtx += *subPosCamera;
	}
}

void SetHeartArea(float posX, float posY, float sizeX, float sizeY)
{
	g_posHeart = D3DXVECTOR3((float)posX, (float)posY, 0.0f);
	g_sizeHeart = D3DXVECTOR3((float)sizeX, (float)sizeY, 0.0f);
}
