//=============================================================================
//
// �T�E���h���� [sound.cpp]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <windows.h>
#include <tchar.h>
#include <dsound.h>
#include <mmsystem.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �\���̐錾
//*****************************************************************************
enum
{	// �T�E���h�ʂ��i���o�[
	BGM_TITLE,
	BGM_MAZE,
	BGM_HARI,
	SE_BOMB,
	SE_WIND,
	SOUND_MAX
};

enum
{	// �Đ��p�t���O
	E_DS8_FLAG_NONE,
	E_DS8_FLAG_LOOP,	// DSBPLAY_LOOPING=1
	E_DS8_FLAG_MAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT					InitSound( HWND hWnd );	// ������
void					UninitSound();			// ��Еt��
LPDIRECTSOUNDBUFFER8	LoadSound( int no );	// �T�E���h�̃��[�h
void					PlaySound( LPDIRECTSOUNDBUFFER8 pBuffer, int flag = 0 );	// �����ƂɍĐ�
void					StopSound(LPDIRECTSOUNDBUFFER8 pBuffer);					// �����~�߂�
bool					IsPlaying( LPDIRECTSOUNDBUFFER8 pBuffer );					// �Đ������ǂ���

#endif