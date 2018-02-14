//=============================================================================
//
// リザルトドロップ処理 [resultDrops.h]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#ifndef _RESULTDROPS_H_
#define _RESULTDROPS_H_

#include "main.h"
#include "score.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitResultDrops(int type);
void UninitResultDrops(void);
void UpdateResultDrops(void);
void DrawResultDrops(void);

void SetResultDropsArea(float x, float y, float sizeX, float sizeY);

#endif
