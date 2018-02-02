//=============================================================================
//
// �w�i���� [bg.h]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

#include "main.h"
#include "dungeon.h"

// �}�N����`
#define TEXTURE_GAME_BG00		_T("data/TEXTURE/galaxy.jpg")	// �T���v���p�摜
#define BG00_SIZE_X			(1000)	// �e�N�X�`���T�C�Y
#define BG00_SIZE_Y			(1000)	// ����
#define BG00_POS_X			(MAP_WIDTH * TEXTURE_DUNGEON_SIZE_X / 2)		// �|���S���̏����ʒuX(����)
#define BG00_POS_Y			(MAP_HEIGHT * TEXTURE_DUNGEON_SIZE_Y / 2)		// ����
#define BG00_CENTER_X		(BG00_POS_X / 2)
#define BG00_CENTER_Y		(BG00_POS_Y / 2)

#define BG00_ROTATION		(0.0002f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBG(int type);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);

#endif
