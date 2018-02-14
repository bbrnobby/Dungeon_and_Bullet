//=============================================================================
//
// �X�R�A���� [score.h]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FONT_SCORE_SIZE		(48)		// �t�H���g�T�C�Y
#define SCORE_DIGIT			(5)			// ����
#define SCORE_POS_X			(SCREEN_WIDTH - SCORE_DIGIT * FONT_SCORE_SIZE - 32)		// �|���S���̏����ʒuX
#define SCORE_POS_Y			(64)		// ����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(int type);
void DrawScore(void);

void AddScore( int add );
void SetScoreArea(float x, float y, float size);
int GetScore(void);

#endif
