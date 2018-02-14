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
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RESULT			_T("data/TEXTURE/TITLE/title_bg.png")			// 読み込むテクスチャファイル名
#define TEXTURE_GAMEOVER		_T("data/TEXTURE/TITLE/title_bg_mono.png")		// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexResult(void);
void SetDiffuseResult(int alpha);
void SetDiffuseGameover(int alpha);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureResult = NULL;			// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureGameover = NULL;		// テクスチャへのポインタ

VERTEX_2D				g_vertexWkResult[NUM_VERTEX];		// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkGameover[NUM_VERTEX];		// 頂点情報格納ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
		TEXTURE_RESULT,						// ファイルの名前
		&g_pD3DTextureResult);				// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
		TEXTURE_GAMEOVER,					// ファイルの名前
		&g_pD3DTextureGameover);			// 読み込むメモリー

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

	if (g_pD3DTextureGameover != NULL)
	{// テクスチャの開放
		g_pD3DTextureGameover->Release();
		g_pD3DTextureGameover = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	PLAYER *player = GetPlayer();

	if(IsAnyKeyTriggered() || IsAnyButtonTriggered(0))
	{
		SetFade(FADE_W_OUT, STAGE_REFRESH);
	}

	if (player->hp > 0)
	{
		SetDiffuseResult(255);
	}
	else
	{
		SetDiffuseGameover(255);
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
	pDevice->SetTexture(0, g_pD3DTextureGameover);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkGameover, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexResult(void)
{	
	// 頂点座標の設定
	g_vertexWkResult[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_vertexWkResult[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	g_vertexWkResult[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	g_vertexWkResult[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkResult[0].rhw =
	g_vertexWkResult[1].rhw =
	g_vertexWkResult[2].rhw =
	g_vertexWkResult[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkResult[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkResult[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkResult[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkResult[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

	// テクスチャ座標の設定
	g_vertexWkResult[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResult[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResult[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResult[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// 頂点座標の設定
	g_vertexWkGameover[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_vertexWkGameover[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	g_vertexWkGameover[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	g_vertexWkGameover[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkGameover[0].rhw =
	g_vertexWkGameover[1].rhw =
	g_vertexWkGameover[2].rhw =
	g_vertexWkGameover[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkGameover[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkGameover[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkGameover[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkGameover[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

	// テクスチャ座標の設定
	g_vertexWkGameover[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkGameover[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkGameover[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkGameover[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 反射光の設定
//=============================================================================
void SetDiffuseResult(int alpha)
{
	g_vertexWkResult[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	g_vertexWkResult[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	g_vertexWkResult[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	g_vertexWkResult[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
}

void SetDiffuseGameover(int alpha)
{
	g_vertexWkGameover[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	g_vertexWkGameover[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	g_vertexWkGameover[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	g_vertexWkGameover[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
}

