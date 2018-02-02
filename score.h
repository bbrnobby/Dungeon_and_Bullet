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
#define TEXTURE_GAME_SCORE	_T("data/TEXTURE/number224.png")	// �T���v���p�摜
#define TEXTURE_SCORE_SIZE_X	(32)	// �e�N�X�`���T�C�Y
#define TEXTURE_SCORE_SIZE_Y	(32)	// ����
#define SCORE_MAX			(99999)		// �X�R�A�̍ő�l
#define SCORE_DIGIT			(5)			// ����

#define SCORE_POS_X			(SCREEN_WIDTH - SCORE_DIGIT * TEXTURE_SCORE_SIZE_X - 30)		// �|���S���̏����ʒuX
#define SCORE_POS_Y			(40)		// ����
#define SCORE_SIZE_X		(TEXTURE_SCORE_SIZE_X)
#define SCORE_SIZE_Y		(TEXTURE_SCORE_SIZE_Y)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitScore(int type);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);

void AddScore( int add );
void SetScoreArea(float x, float y, float sizeX, float sizeY);
int GetScore(void);

#endif
