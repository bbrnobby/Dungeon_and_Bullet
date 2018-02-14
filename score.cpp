//=============================================================================
//
// スコア処理 [score.cpp]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#include "main.h"
#include "score.h"
#include "camera.h"
#include "font.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCORE_MAX			(99999)		// スコアの最大値
#define TIME_MAX			(99999)		// 時間の最大値

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
D3DXVECTOR3		g_posScore;			// スコアの座標
float			g_sizeScore;		// スコアのサイズ

int				g_nScore;			// スコア
int				g_nSymbolScore;		// スコア（表示用）
int				g_addPoint;			// 加算ポイント
int				g_time;				// 時間
int				g_nTimeCount;		// カウント

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	g_posScore = D3DXVECTOR3(SCORE_POS_X, SCORE_POS_Y, 0.0f);
	g_sizeScore = FONT_SCORE_SIZE;

	g_nScore = 0;
	g_nSymbolScore = 0;
	g_addPoint = 0;
	g_time = 0;
	g_nTimeCount = 0;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitScore(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateScore(int type)
{
	char str[32];
	D3DXVECTOR3 pos = g_posScore;

	if (g_nSymbolScore < g_nScore)
	{
		g_nSymbolScore += g_addPoint;
	}
	else
	{
		g_nSymbolScore = g_nScore;
		g_addPoint = 0;
	}

	if (type == 0)
	{
		if (g_nTimeCount < 60)
		{
			g_nTimeCount++;
		}
		else
		{
			g_time++;
			g_nTimeCount = 0;
		}


		sprintf(str, "タイム　%02d:%02d  ", g_time / 60, g_time % 60);
		SetString(str, pos.x, pos.y, g_sizeScore, 1);

		pos.y += g_sizeScore;

		sprintf(str, "おかね　%05dバレ", g_nSymbolScore);
		SetString(str, pos.x, pos.y, g_sizeScore, 1);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawScore(void)
{
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

void SetScoreArea(float x, float y, float size)
{
	g_posScore = D3DXVECTOR3((float)x, (float)y, 0.0f);
	g_sizeScore = size;
}

int GetScore(void)
{
	return g_nScore;
}