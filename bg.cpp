//=============================================================================
//
// 背景処理 [bg.cpp]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#include "bg.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBG(void);
void SetVertexBG(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBG = NULL;		// テクスチャへのポインタ
VERTEX_2D				g_vertexWkBG[NUM_VERTEX];	// 頂点情報格納ワーク

D3DXVECTOR3				g_posBG;					// 背景の位置
float					g_rotz;
float					g_radius;
float					g_baseAngleBG;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBG(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_GAME_BG00,		// ファイルの名前
			&g_pD3DTextureBG);		// 読み込むメモリー
	}

	g_posBG = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotz = 0.0f;
	D3DXVECTOR2 temp = D3DXVECTOR2(BG00_SIZE_X, BG00_SIZE_Y);
	g_radius = D3DXVec2Length(&temp);
	g_baseAngleBG = atan2f(BG00_SIZE_X, BG00_SIZE_Y);

	g_posBG.x = -GetPlayer()->pos.x / 4.0f;
	g_posBG.y = -GetPlayer()->pos.y / 4.0f;

	// 頂点情報の作成
	MakeVertexBG();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBG(void)
{
	if(g_pD3DTextureBG != NULL)
	{// テクスチャの開放
		g_pD3DTextureBG->Release();
		g_pD3DTextureBG = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBG(void)
{
	// 毎フレーム実行する処理

	// スクロール処理
	g_posBG.x = -GetPlayer()->pos.x / 4.0f;
	g_posBG.y = -GetPlayer()->pos.y / 4.0f;

	g_rotz += BG00_ROTATION;

	SetVertexBG();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureBG);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkBG, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexBG(void)
{
	// 頂点座標の設定
	SetVertexBG();

	// rhwの設定
	g_vertexWkBG[0].rhw =
	g_vertexWkBG[1].rhw =
	g_vertexWkBG[2].rhw =
	g_vertexWkBG[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkBG[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 45);
	g_vertexWkBG[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 45);
	g_vertexWkBG[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 45);
	g_vertexWkBG[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 45);

	// テクスチャ座標の設定
	g_vertexWkBG[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkBG[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkBG[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkBG[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexBG(void)
{
	// 頂点座標の設定
	g_vertexWkBG[0].vtx = D3DXVECTOR3(BG00_CENTER_X - cosf(g_baseAngleBG + g_rotz) * g_radius, BG00_CENTER_Y - sinf(g_baseAngleBG + g_rotz) * g_radius, 0.0f) + g_posBG;
	g_vertexWkBG[1].vtx = D3DXVECTOR3(BG00_CENTER_X + cosf(g_baseAngleBG - g_rotz) * g_radius, BG00_CENTER_Y - sinf(g_baseAngleBG - g_rotz) * g_radius, 0.0f) + g_posBG;
	g_vertexWkBG[2].vtx = D3DXVECTOR3(BG00_CENTER_X - cosf(g_baseAngleBG - g_rotz) * g_radius, BG00_CENTER_Y + sinf(g_baseAngleBG - g_rotz) * g_radius, 0.0f) + g_posBG;
	g_vertexWkBG[3].vtx = D3DXVECTOR3(BG00_CENTER_X + cosf(g_baseAngleBG + g_rotz) * g_radius, BG00_CENTER_Y + sinf(g_baseAngleBG + g_rotz) * g_radius, 0.0f) + g_posBG;
}
