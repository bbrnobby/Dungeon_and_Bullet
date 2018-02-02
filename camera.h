//=============================================================================
//
// �J�������� [camera.h]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitCamera(void);
void UpdateCamera(void);
D3DXVECTOR3 *GetCameraPos(void);
D3DXVECTOR3 *GetCameraVec(void);
void SetCamera(float x, float y);

#endif
