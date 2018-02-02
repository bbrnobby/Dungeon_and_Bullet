//=============================================================================
//
// �J�������� [camera.cpp]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#include "camera.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	CAMERA_DISTANCE		(72.0f)												// �J�����̈ړ���

#define	RATE_CHASE_CAMERA	(50.0f)												// �J�����̎��_�ւ̕␳�W��

#define SCREEN_CENTER		D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f)	// ��ʂ̒��S

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
D3DXVECTOR3		g_posCamera;				// �J�����̎��_
D3DXVECTOR3		g_posCameraRounded;			// �J�����̎��_(����)
D3DXVECTOR3		g_vecCamera;				// �J�����̕���

//=============================================================================
// ����������
//=============================================================================
HRESULT InitCamera(void)
{
	g_posCamera = SCREEN_CENTER;					// ���W�f�[�^��������
	g_vecCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����f�[�^��������

	return S_OK;
}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateCamera(void)
{
	PLAYER *player = GetPlayer();

	// �v���C���[�̎ː��Œ莞
	if (player->state == PLAYER_LOCK)
	{
		switch (player->dir)
		{
		case DIR_RIGHT:
			g_vecCamera.x = PLAYER_SPEED;
			break;
		case DIR_DOWN:
			g_vecCamera.y = PLAYER_SPEED;
			break;
		case DIR_LEFT:
			g_vecCamera.x = -PLAYER_SPEED;
			break;
		case DIR_UP:
			g_vecCamera.y = -PLAYER_SPEED;
			break;
		}
	}
	// �v���C���[���ړ����Ă���Ƃ�
	else if (player->isMove)
	{
		// �J�����̕������v���C���[�̈ړ������ɂ���
		g_vecCamera = player->vec;
	}
	// ����ȊO
	else
	{
		g_vecCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (player->hp != 0)
	{
		// �J�����ʒu�̍X�V
		switch (player->state)
		{
		case PLAYER_WALK:
		case PLAYER_RUN:
			g_posCamera = (SCREEN_CENTER - g_vecCamera * CAMERA_DISTANCE / 2 + g_posCamera * RATE_CHASE_CAMERA * 2) / (RATE_CHASE_CAMERA * 2 + 1);
			break;
		case PLAYER_LOCK:
			g_posCamera = (SCREEN_CENTER - g_vecCamera * CAMERA_DISTANCE + g_posCamera * RATE_CHASE_CAMERA) / (RATE_CHASE_CAMERA + 1);
			break;
		}
		g_posCameraRounded = D3DXVECTOR3((int)(g_posCamera.x - player->pos.x), (int)(g_posCamera.y - player->pos.y), (int)(g_posCamera.z - player->pos.z));
	}
	else
	{	// �v���C���[�����ꂽ�Ƃ�

		// �J�������v���C���[�̒��S�Ɍ�����
		//g_vecCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//=============================================================================
// �J�������W�擾�֐�
//=============================================================================
D3DXVECTOR3 *GetCameraPos(void)
{
	return(&g_posCameraRounded);
}

//=============================================================================
// �J���������擾�֐�
//=============================================================================
D3DXVECTOR3 *GetCameraVec(void)
{
	return(&g_vecCamera);
}


//=============================================================================
// �J�������W�ݒ�֐�
//=============================================================================
void SetCamera(float x, float y)
{
	g_posCamera = D3DXVECTOR3(x, y, 0.0f);			// ���W�f�[�^��������
	g_vecCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����f�[�^��������
}
