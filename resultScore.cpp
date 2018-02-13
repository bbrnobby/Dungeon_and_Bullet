//=============================================================================
//
// リザルトスコア処理 [resultScore.cpp]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#include "main.h"
#include "resultScore.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_RESULT_SCORE	_T("data/TEXTURE/number16x32.png")	// サンプル用画像
#define TEXTURE_RESULT_SCORE_SIZE_X	(16)	// テクスチャサイズ
#define TEXTURE_RESULT_SCORE_SIZE_Y	(32)	// 同上

#define RESULTSCORE_POS_X		(SCREEN_CENTER_X - 160)	// リザルトスコアの表示位置
#define RESULTSCORE_POS_Y		(SCREEN_CENTER_Y - 64)	// リザルトスコアの表示位置
#define RESULTSCORE_SIZE_X		(64)					// リザルトスコアの文字横サイズ
#define RESULTSCORE_SIZE_Y		(128)					// リザルトスコアの文字縦サイズ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexResultScore(void);
void SetTextureResultScore(void);
void SetVertexResultScore(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureResultScore = NULL;		// テクスチャへのポリゴン
VERTEX_2D				g_vertexWkResultScore[SCORE_DIGIT][NUM_VERTEX];	// 頂点情報格納ワーク

D3DXVECTOR3				g_posResultScore;						// ポリゴンの移動量
D3DXVECTOR3				g_sizeResultScore;

int						g_nResultScore;							// リザルトスコア

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResultScore(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_RESULT_SCORE,				// ファイルの名前
			&g_pD3DTextureResultScore);			// 読み込むメモリのポインタ
	}

	SetResultScoreArea(RESULTSCORE_POS_X, RESULTSCORE_POS_Y, RESULTSCORE_SIZE_X, RESULTSCORE_SIZE_Y);
	g_nResultScore = 0;

	// 頂点情報の作成
	MakeVertexResultScore();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResultScore(void)
{
	if (g_pD3DTextureResultScore != NULL)
	{	// テクスチャの開放
		g_pD3DTextureResultScore->Release();
		g_pD3DTextureResultScore = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResultScore(void)
{
	g_nResultScore = GetScore();

	SetVertexResultScore();
	SetTextureResultScore();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureResultScore);

	// スコア
	for (int i = 0; i < SCORE_DIGIT; i++)
	{
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkResultScore[i], sizeof(VERTEX_2D));
	}

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexResultScore(void)
{
	// 桁数分処理する
	for (int i = 0; i < SCORE_DIGIT; i++)
	{
		// 頂点座標の設定
		g_vertexWkResultScore[i][0].vtx.x = TEXTURE_RESULT_SCORE_SIZE_X * (SCORE_DIGIT - i - 1) + SCORE_POS_X;
		g_vertexWkResultScore[i][0].vtx.y = SCORE_POS_Y;
		g_vertexWkResultScore[i][0].vtx.z = 0.0f;
		g_vertexWkResultScore[i][1].vtx.x = TEXTURE_RESULT_SCORE_SIZE_X * (SCORE_DIGIT - i - 1) + SCORE_POS_X + TEXTURE_RESULT_SCORE_SIZE_X;
		g_vertexWkResultScore[i][1].vtx.y = SCORE_POS_Y;
		g_vertexWkResultScore[i][1].vtx.z = 0.0f;
		g_vertexWkResultScore[i][2].vtx.x = TEXTURE_RESULT_SCORE_SIZE_X * (SCORE_DIGIT - i - 1) + SCORE_POS_X;
		g_vertexWkResultScore[i][2].vtx.y = SCORE_POS_Y + TEXTURE_RESULT_SCORE_SIZE_Y;
		g_vertexWkResultScore[i][2].vtx.z = 0.0f;
		g_vertexWkResultScore[i][3].vtx.x = TEXTURE_RESULT_SCORE_SIZE_X * (SCORE_DIGIT - i - 1) + SCORE_POS_X + TEXTURE_RESULT_SCORE_SIZE_X;
		g_vertexWkResultScore[i][3].vtx.y = SCORE_POS_Y + TEXTURE_RESULT_SCORE_SIZE_Y;
		g_vertexWkResultScore[i][3].vtx.z = 0.0f;

		// rhwの設定
		g_vertexWkResultScore[i][0].rhw =
		g_vertexWkResultScore[i][1].rhw =
		g_vertexWkResultScore[i][2].rhw =
		g_vertexWkResultScore[i][3].rhw = 1.0f;

		// 反射光の設定
		g_vertexWkResultScore[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultScore[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultScore[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultScore[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		g_vertexWkResultScore[i][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		g_vertexWkResultScore[i][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		g_vertexWkResultScore[i][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		g_vertexWkResultScore[i][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}
//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetTextureResultScore(void)
{
	int number = g_nResultScore;

	for (int i = 0; i < SCORE_DIGIT; i++)
	{
		// テクスチャ座標の設定
		float x = (float)(number % 10);
		g_vertexWkResultScore[i][0].tex = D3DXVECTOR2(0.1f * x, 0.0f);
		g_vertexWkResultScore[i][1].tex = D3DXVECTOR2(0.1f * (x + 1), 0.0f);
		g_vertexWkResultScore[i][2].tex = D3DXVECTOR2(0.1f * x, 1.0f);
		g_vertexWkResultScore[i][3].tex = D3DXVECTOR2(0.1f * (x + 1), 1.0f);
		number /= 10;
	}
}

void SetVertexResultScore(void)
{
	for (int i = 0; i < SCORE_DIGIT; i++)
	{
		// 頂点座標の設定
		g_vertexWkResultScore[i][0].vtx.x = g_sizeResultScore.x * (SCORE_DIGIT - i - 1) + g_posResultScore.x;
		g_vertexWkResultScore[i][0].vtx.y = g_posResultScore.y;
		g_vertexWkResultScore[i][0].vtx.z = 0.0f;
		g_vertexWkResultScore[i][1].vtx.x = g_sizeResultScore.x * (SCORE_DIGIT - i - 1) + g_posResultScore.x + g_sizeResultScore.x;
		g_vertexWkResultScore[i][1].vtx.y = g_posResultScore.y;
		g_vertexWkResultScore[i][1].vtx.z = 0.0f;
		g_vertexWkResultScore[i][2].vtx.x = g_sizeResultScore.x * (SCORE_DIGIT - i - 1) + g_posResultScore.x;
		g_vertexWkResultScore[i][2].vtx.y = g_posResultScore.y + g_sizeResultScore.y;
		g_vertexWkResultScore[i][2].vtx.z = 0.0f;
		g_vertexWkResultScore[i][3].vtx.x = g_sizeResultScore.x * (SCORE_DIGIT - i - 1) + g_posResultScore.x + g_sizeResultScore.x;
		g_vertexWkResultScore[i][3].vtx.y = g_posResultScore.y + g_sizeResultScore.y;
		g_vertexWkResultScore[i][3].vtx.z = 0.0f;
	}
}

void SetResultScoreArea(float x, float y, float sizeX, float sizeY)
{
	g_posResultScore = D3DXVECTOR3((float)x, (float)y, 0.0f);
	g_sizeResultScore = D3DXVECTOR3((float)sizeX, (float)sizeY, 0.0f);
}
