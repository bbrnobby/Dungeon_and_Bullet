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
#define FONT_SCORE_SIZE		(48)		// フォントサイズ
#define SCORE_DIGIT			(5)			// 桁数
#define SCORE_POS_X			(SCREEN_WIDTH - SCORE_DIGIT * FONT_SCORE_SIZE - 32)		// ポリゴンの初期位置X
#define SCORE_POS_Y			(64)		// 同上

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(int type);
void DrawScore(void);

void AddScore( int add );
void SetScoreArea(float x, float y, float size);
int GetScore(void);

#endif
