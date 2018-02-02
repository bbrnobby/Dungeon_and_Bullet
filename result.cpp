//=============================================================================
//
// リザルト画面処理 [result.cpp]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#include "main.h"
#include "result.h"
#include "input.h"
#include "fade.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexResult(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureResult = NULL;			// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureResultLogo = NULL;		// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureResultImage = NULL;	// テクスチャへのポインタ

VERTEX_2D				g_vertexWkResult[NUM_VERTEX];		// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkResultImage[NUM_VERTEX];	// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkResultLogo[NUM_VERTEX];	// 頂点情報格納ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
								TEXTURE_RESULT,				// ファイルの名前
								&g_pD3DTextureResult);		// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
								TEXTURE_RESULT_LOGO,		// ファイルの名前
								&g_pD3DTextureResultLogo);	// 読み込むメモリー
	
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
								TEXTURE_RESULT_IMAGE,		// ファイルの名前
								&g_pD3DTextureResultImage);	// 読み込むメモリー

	// 頂点情報の作成
	MakeVertexResult();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	if(g_pD3DTextureResult != NULL)
	{// テクスチャの開放
		g_pD3DTextureResult->Release();
		g_pD3DTextureResult = NULL;
	}

	if(g_pD3DTextureResultLogo != NULL)
	{// テクスチャの開放
		g_pD3DTextureResultLogo->Release();
		g_pD3DTextureResultLogo = NULL;
	}

	if (g_pD3DTextureResultLogo != NULL)
	{// テクスチャの開放
		g_pD3DTextureResultLogo->Release();
		g_pD3DTextureResultLogo = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	if(IsAnyKeyTriggered() || IsAnyButtonTriggered(0))
	{
		SetFade(FADE_W_OUT, STAGE_REFRESH);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureResult);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkResult, sizeof(VERTEX_2D));

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureResultImage);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkResultImage, sizeof(VERTEX_2D));

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureResultLogo);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkResultLogo, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexResult(void)
{	
	// 頂点座標の設定
	g_vertexWkResult[0].vtx = D3DXVECTOR3(RESULT_POS_X, RESULT_POS_Y, 0.0f);
	g_vertexWkResult[1].vtx = D3DXVECTOR3(RESULT_POS_X + RESULT_SIZE_X, RESULT_POS_Y, 0.0f);
	g_vertexWkResult[2].vtx = D3DXVECTOR3(RESULT_POS_X, RESULT_POS_Y + RESULT_SIZE_Y, 0.0f);
	g_vertexWkResult[3].vtx = D3DXVECTOR3(RESULT_POS_X + RESULT_SIZE_X, RESULT_POS_Y + RESULT_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkResult[0].rhw =
	g_vertexWkResult[1].rhw =
	g_vertexWkResult[2].rhw =
	g_vertexWkResult[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkResult[0].diffuse = D3DCOLOR_RGBA(200, 200, 200, 255);
	g_vertexWkResult[1].diffuse = D3DCOLOR_RGBA(200, 200, 200, 255);
	g_vertexWkResult[2].diffuse = D3DCOLOR_RGBA(200, 200, 200, 255);
	g_vertexWkResult[3].diffuse = D3DCOLOR_RGBA(200, 200, 200, 255);

	// テクスチャ座標の設定
	g_vertexWkResult[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResult[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResult[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResult[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標の設定
	g_vertexWkResultLogo[0].vtx = D3DXVECTOR3(RESULTLOGO_POS_X, RESULTLOGO_POS_Y, 0.0f);
	g_vertexWkResultLogo[1].vtx = D3DXVECTOR3(RESULTLOGO_POS_X + RESULTLOGO_SIZE_X, RESULTLOGO_POS_Y, 0.0f);
	g_vertexWkResultLogo[2].vtx = D3DXVECTOR3(RESULTLOGO_POS_X, RESULTLOGO_POS_Y + RESULTLOGO_SIZE_Y, 0.0f);
	g_vertexWkResultLogo[3].vtx = D3DXVECTOR3(RESULTLOGO_POS_X + RESULTLOGO_SIZE_X, RESULTLOGO_POS_Y + RESULTLOGO_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkResultLogo[0].rhw =
	g_vertexWkResultLogo[1].rhw =
	g_vertexWkResultLogo[2].rhw =
	g_vertexWkResultLogo[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkResultLogo[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultLogo[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultLogo[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultLogo[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkResultLogo[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResultLogo[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResultLogo[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResultLogo[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標の設定
	g_vertexWkResultImage[0].vtx = D3DXVECTOR3(RESULTIMAGE_POS_X, RESULTIMAGE_POS_Y, 0.0f);
	g_vertexWkResultImage[1].vtx = D3DXVECTOR3(RESULTIMAGE_POS_X + RESULTIMAGE_SIZE_X, RESULTIMAGE_POS_Y, 0.0f);
	g_vertexWkResultImage[2].vtx = D3DXVECTOR3(RESULTIMAGE_POS_X, RESULTIMAGE_POS_Y + RESULTIMAGE_SIZE_Y, 0.0f);
	g_vertexWkResultImage[3].vtx = D3DXVECTOR3(RESULTIMAGE_POS_X + RESULTIMAGE_SIZE_X, RESULTIMAGE_POS_Y + RESULTIMAGE_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkResultImage[0].rhw =
	g_vertexWkResultImage[1].rhw =
	g_vertexWkResultImage[2].rhw =
	g_vertexWkResultImage[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkResultImage[0].diffuse = D3DCOLOR_RGBA(200, 200, 200, 255);
	g_vertexWkResultImage[1].diffuse = D3DCOLOR_RGBA(200, 200, 200, 255);
	g_vertexWkResultImage[2].diffuse = D3DCOLOR_RGBA(200, 200, 200, 255);
	g_vertexWkResultImage[3].diffuse = D3DCOLOR_RGBA(200, 200, 200, 255);

	// テクスチャ座標の設定
	g_vertexWkResultImage[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResultImage[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResultImage[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResultImage[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

