//=============================================================================
//
// リザルトスコア処理 [resultScore.h]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#ifndef _RESULTSCORE_H_
#define _RESULTSCORE_H_

#include "main.h"
#include "score.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitResultScore(int type);
void UninitResultScore(void);
void UpdateResultScore(void);
void DrawResultScore(void);

void SetResultScoreArea(float x, float y, float sizeX, float sizeY);

#endif
