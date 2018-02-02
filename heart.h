//=============================================================================
//
// �̗͏��� [heart.h]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#ifndef _HEART_H_
#define _HEART_H_

#include "main.h"

// �}�N����`
#define TEXTURE_HEART	_T("data/TEXTURE/heart001.png")	// �T���v���p�摜
#define TEXTURE_HEART_SIZE_X	(60)	// �e�N�X�`���T�C�Y
#define TEXTURE_HEART_SIZE_Y	(60)	// ����
#define HEART_DIVIDE			(4)		// �̗͂̕�����

#define MAX_HP					(3)		// �̗͂̍ő�l

#define HEART_POS_X				(90)	// �|���S���̏����ʒuX
#define HEART_POS_Y				(90)	// ����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitHeart(int type);
void UninitHeart(void);
void UpdateHeart(void);
void DrawHeart(void);

void SetHeartArea(float posX, float posY, float sizeX, float sizeY);

#endif
