//=============================================================================
//
// ���U���g�X�R�A���� [resultScore.h]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#ifndef _RESULTSCORE_H_
#define _RESULTSCORE_H_

#include "main.h"
#include "score.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitResultScore(int type);
void UninitResultScore(void);
void UpdateResultScore(void);
void DrawResultScore(void);

void SetResultScoreArea(float x, float y, float sizeX, float sizeY);

#endif
