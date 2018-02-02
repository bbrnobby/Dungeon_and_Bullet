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

// �}�N����`
#define TEXTURE_RESULT_SCORE	_T("data/TEXTURE/number16x32.png")	// �T���v���p�摜
#define TEXTURE_RESULT_SCORE_SIZE_X	(16)	// �e�N�X�`���T�C�Y
#define TEXTURE_RESULT_SCORE_SIZE_Y	(32)	// ����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitResultScore(int type);
void UninitResultScore(void);
void UpdateResultScore(void);
void DrawResultScore(void);

void SetResultScoreArea(float x, float y, float sizeX, float sizeY);

#endif
