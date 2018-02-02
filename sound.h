//=============================================================================
//
// サウンド処理 [sound.cpp]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <windows.h>
#include <tchar.h>
#include <dsound.h>
#include <mmsystem.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// 構造体宣言
//*****************************************************************************
enum
{	// サウンド通しナンバー
	BGM_TITLE,
	BGM_MAZE,
	BGM_HARI,
	SE_BOMB,
	SE_WIND,
	SOUND_MAX
};

enum
{	// 再生用フラグ
	E_DS8_FLAG_NONE,
	E_DS8_FLAG_LOOP,	// DSBPLAY_LOOPING=1
	E_DS8_FLAG_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT					InitSound( HWND hWnd );	// 初期化
void					UninitSound();			// 後片付け
LPDIRECTSOUNDBUFFER8	LoadSound( int no );	// サウンドのロード
void					PlaySound( LPDIRECTSOUNDBUFFER8 pBuffer, int flag = 0 );	// 音ごとに再生
void					StopSound(LPDIRECTSOUNDBUFFER8 pBuffer);					// 音を止める
bool					IsPlaying( LPDIRECTSOUNDBUFFER8 pBuffer );					// 再生中かどうか

#endif