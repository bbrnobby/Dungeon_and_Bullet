//=============================================================================
//
// スコア処理 [score.h]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_GAME_SCORE	_T("data/TEXTURE/number224.png")	// サンプル用画像
#define TEXTURE_SCORE_SIZE_X	(32)	// テクスチャサイズ
#define TEXTURE_SCORE_SIZE_Y	(32)	// 同上
#define SCORE_MAX			(99999)		// スコアの最大値
#define SCORE_DIGIT			(5)			// 桁数

#define SCORE_POS_X			(SCREEN_WIDTH - SCORE_DIGIT * TEXTURE_SCORE_SIZE_X - 30)		// ポリゴンの初期位置X
#define SCORE_POS_Y			(40)		// 同上
#define SCORE_SIZE_X		(TEXTURE_SCORE_SIZE_X)
#define SCORE_SIZE_Y		(TEXTURE_SCORE_SIZE_Y)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitScore(int type);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);

void AddScore( int add );
void SetScoreArea(float x, float y, float sizeX, float sizeY);
int GetScore(void);

#endif
