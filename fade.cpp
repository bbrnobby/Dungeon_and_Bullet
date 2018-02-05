//=============================================================================
//
// フェード処理 [fade.cpp]
// Author : 
//
//=============================================================================
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	FADE_RATE		(0.02f)		// フェード係数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexFade(void);
void SetColor(D3DCOLOR col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_p3DTextureFade = NULL;	// テクスチャへのポインタ
VERTEX_2D				g_vertexWkFade[NUM_VERTEX];	// 頂点情報格納ワーク

D3DXCOLOR				g_color;
FADE					g_eFade = FADE_IN;
E_STAGE					g_eState = STAGE_TITLE;		// 次に飛ぶ予定のステート

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の設定
	MakeVertexFade();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitFade(void)
{
	if(g_p3DTextureFade != NULL)
	{// テクスチャの開放
		g_p3DTextureFade->Release();
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateFade(void)
{
	switch(g_eFade)
	{
	case FADE_NONE:
	case FADE_MAX:
		break;
	case FADE_W_OUT:
	case FADE_B_OUT:
		// フェードアウト処理
		g_color.a += FADE_RATE;		// α値を加算して画面を消していく
		if(g_color.a >= 1.0f)
		{
			// 状態を切り替え
			SetStage( g_eState );

			// フェードイン処理に切り替え
			g_color.a = 1.0f;
			SetFade(FADE_IN, STAGE_MAX);
		}

		// 色を設定
		SetColor(g_color);		
		break;
	case FADE_IN:
		// フェードイン処理
		g_color.a -= FADE_RATE;		// α値を減算して画面を浮き上がらせる
		if (g_color.a <= 0.0f)
		{
			// フェード処理終了
			g_color.a = 0.0f;
			SetFade(FADE_NONE, STAGE_MAX);
		}

		// 色を設定
		SetColor(g_color);
		break;
	}
}

//=============================================================================
// フェード画面
//=============================================================================
void DrawFade()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkFade, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャのパースペクティブコレクト用
	g_vertexWkFade[0].rhw =
	g_vertexWkFade[1].rhw =
	g_vertexWkFade[2].rhw =
	g_vertexWkFade[3].rhw = 1.0f;

	// 反射光の設定
	g_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	g_vertexWkFade[0].diffuse = g_color;
	g_vertexWkFade[1].diffuse = g_color;
	g_vertexWkFade[2].diffuse = g_color;
	g_vertexWkFade[3].diffuse = g_color;

	// 頂点座標の設定
	g_vertexWkFade[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_vertexWkFade[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	g_vertexWkFade[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	g_vertexWkFade[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// テクスチャ座標の設定
	g_vertexWkFade[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkFade[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkFade[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkFade[3].tex = D3DXVECTOR2(1.0f, 1.0f);	

	return S_OK;
}

//=============================================================================
// 色を設定
//=============================================================================
void SetColor(D3DCOLOR col)
{
	// 反射光の設定
	g_vertexWkFade[0].diffuse = col;
	g_vertexWkFade[1].diffuse = col;
	g_vertexWkFade[2].diffuse = col;
	g_vertexWkFade[3].diffuse = col;
}

//=============================================================================
// フェードの状態設定
//=============================================================================
void SetFade(FADE fade, E_STAGE next )
{
	switch (fade)
	{
	case FADE_NONE:
	case FADE_MAX:
	case FADE_IN:
		break;
	case FADE_W_OUT:
		g_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		break;
	case FADE_B_OUT:
		g_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		break;
	}
	g_eFade = fade;
	g_eState = next;
}

//=============================================================================
// フェードの状態取得
//=============================================================================
FADE GetFade(void)
{
	return g_eFade;
}