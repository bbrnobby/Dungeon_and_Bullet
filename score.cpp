//=============================================================================
//
// スコア処理 [score.cpp]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#include "main.h"
#include "score.h"
#include <stdio.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexScore(void);
void SetTextureScore(void);
void SetVertexScore(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureScore = NULL;		// テクスチャへのポリゴン
VERTEX_2D				g_vertexWkScore[SCORE_DIGIT][NUM_VERTEX];	// 頂点情報格納ワーク

D3DXVECTOR3				g_posScore;						// ポリゴンの移動量
D3DXVECTOR3				g_sizeScore;

int						g_nScore;
int						g_nSymbolScore;						// スコア
int						g_addPoint;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitScore(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_SCORE,					// ファイルの名前
			&g_pD3DTextureScore);				// 読み込むメモリのポインタ
	}

	SetScoreArea(SCORE_POS_X, SCORE_POS_Y, SCORE_SIZE_X, SCORE_SIZE_Y);
	g_nScore = 0;
	g_nSymbolScore = 0;
	g_addPoint = 0;

	// 頂点情報の作成
	MakeVertexScore();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitScore(void)
{
	if( g_pD3DTextureScore != NULL )
	{	// テクスチャの開放
		g_pD3DTextureScore->Release();
		g_pD3DTextureScore = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateScore(void)
{
	// 毎フレーム実行される処理を記述する
	if (g_nSymbolScore < g_nScore)
	{
		g_nSymbolScore += g_addPoint;
	}
	else
	{
		g_nSymbolScore = g_nScore;
		g_addPoint = 0;
	}

	SetVertexScore();
	SetTextureScore();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの設定
	pDevice->SetTexture( 0, g_pD3DTextureScore );

	// スコア
	for(int i = 0; i < SCORE_DIGIT; i++ )
	{
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkScore[i], sizeof(VERTEX_2D));
	}

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexScore(void)
{	
	// 桁数分処理する
	for(int i = 0; i < SCORE_DIGIT; i++ )
	{
		// 頂点座標の設定
		g_vertexWkScore[i][0].vtx.x = TEXTURE_SCORE_SIZE_X * (SCORE_DIGIT - i - 1) + SCORE_POS_X;
		g_vertexWkScore[i][0].vtx.y = SCORE_POS_Y;
		g_vertexWkScore[i][0].vtx.z = 0.0f;
		g_vertexWkScore[i][1].vtx.x = TEXTURE_SCORE_SIZE_X * (SCORE_DIGIT - i - 1) + SCORE_POS_X + TEXTURE_SCORE_SIZE_X;
		g_vertexWkScore[i][1].vtx.y = SCORE_POS_Y;
		g_vertexWkScore[i][1].vtx.z = 0.0f;
		g_vertexWkScore[i][2].vtx.x = TEXTURE_SCORE_SIZE_X * (SCORE_DIGIT - i - 1) + SCORE_POS_X;
		g_vertexWkScore[i][2].vtx.y = SCORE_POS_Y + TEXTURE_SCORE_SIZE_Y;
		g_vertexWkScore[i][2].vtx.z = 0.0f;
		g_vertexWkScore[i][3].vtx.x = TEXTURE_SCORE_SIZE_X * (SCORE_DIGIT - i - 1) + SCORE_POS_X + TEXTURE_SCORE_SIZE_X;
		g_vertexWkScore[i][3].vtx.y = SCORE_POS_Y + TEXTURE_SCORE_SIZE_Y;
		g_vertexWkScore[i][3].vtx.z = 0.0f;

		// rhwの設定
		g_vertexWkScore[i][0].rhw =
		g_vertexWkScore[i][1].rhw =
		g_vertexWkScore[i][2].rhw =
		g_vertexWkScore[i][3].rhw = 1.0f;

		// 反射光の設定
		g_vertexWkScore[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkScore[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkScore[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkScore[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		g_vertexWkScore[i][0].tex = D3DXVECTOR2( 0.0f, 0.0f );
		g_vertexWkScore[i][1].tex = D3DXVECTOR2( 1.0f, 0.0f );
		g_vertexWkScore[i][2].tex = D3DXVECTOR2( 0.0f, 1.0f );
		g_vertexWkScore[i][3].tex = D3DXVECTOR2( 1.0f, 1.0f );
	}

	return S_OK;
}
//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetTextureScore(void)
{
	int number = g_nSymbolScore;
	
	for(int i = 0; i < SCORE_DIGIT; i++ )
	{
		// テクスチャ座標の設定
		float y = (float)(number % 10);
		g_vertexWkScore[i][0].tex = D3DXVECTOR2( 0.0f, 0.1f * y);
		g_vertexWkScore[i][1].tex = D3DXVECTOR2( 1.0f, 0.1f * y);
		g_vertexWkScore[i][2].tex = D3DXVECTOR2( 0.0f, 0.1f * ( y + 1 ));
		g_vertexWkScore[i][3].tex = D3DXVECTOR2( 1.0f, 0.1f * ( y + 1 ));
		number /= 10;
	}
}

void SetVertexScore(void)
{
	for (int i = 0; i < SCORE_DIGIT; i++)
	{
		// 頂点座標の設定
		g_vertexWkScore[i][0].vtx.x = g_sizeScore.x * (SCORE_DIGIT - i - 1) + g_posScore.x;
		g_vertexWkScore[i][0].vtx.y = g_posScore.y;
		g_vertexWkScore[i][0].vtx.z = 0.0f;
		g_vertexWkScore[i][1].vtx.x = g_sizeScore.x * (SCORE_DIGIT - i - 1) + g_posScore.x + g_sizeScore.x;
		g_vertexWkScore[i][1].vtx.y = g_posScore.y;
		g_vertexWkScore[i][1].vtx.z = 0.0f;
		g_vertexWkScore[i][2].vtx.x = g_sizeScore.x * (SCORE_DIGIT - i - 1) + g_posScore.x;
		g_vertexWkScore[i][2].vtx.y = g_posScore.y + g_sizeScore.y;
		g_vertexWkScore[i][2].vtx.z = 0.0f;
		g_vertexWkScore[i][3].vtx.x = g_sizeScore.x * (SCORE_DIGIT - i - 1) + g_posScore.x + g_sizeScore.x;
		g_vertexWkScore[i][3].vtx.y = g_posScore.y + g_sizeScore.y;
		g_vertexWkScore[i][3].vtx.z = 0.0f;
	}
}


//=============================================================================
// HPデータをセットする
// 引数:add :追加する点数。マイナスも可能、初期化などに。
//=============================================================================
void AddScore( int add )
{
	g_nScore += add;
	if( g_nScore > SCORE_MAX )
	{
		g_nScore = SCORE_MAX;
	}
	else if( g_nScore < 0 )
	{
		g_nScore = 0;
	}
	g_addPoint = (g_nScore - g_nSymbolScore) / 30;
	if (g_addPoint < 1) g_addPoint = 1;
}

void SetScoreArea(float x, float y, float sizeX, float sizeY)
{
	g_posScore = D3DXVECTOR3((float)x, (float)y, 0.0f);
	g_sizeScore = D3DXVECTOR3((float)sizeX, (float)sizeY, 0.0f);
}

int GetScore(void)
{
	return g_nScore;
}