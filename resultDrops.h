//=============================================================================
//
// ���U���g�h���b�v���� [resultDrops.h]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#ifndef _RESULTDROPS_H_
#define _RESULTDROPS_H_

#include "main.h"
#include "score.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitResultDrops(int type);
void UninitResultDrops(void);
void UpdateResultDrops(void);
void DrawResultDrops(void);

void SetResultDropsArea(float x, float y, float sizeX, float sizeY);

#endif
