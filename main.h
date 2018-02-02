//=============================================================================
//
// Mainヘッダー [main.h]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止

#include "windows.h"
#include "d3dx9.h"
#include <tchar.h>
#include <stdio.h>

#define DIRECTINPUT_VERSION 0x0800		// 警告対処
#include "dinput.h"
#include "mmsystem.h"

#if 1	// [ここを"0"にした場合、"構成プロパティ" -> "リンカ" -> "入力" -> "追加の依存ファイル"に対象ライブラリを設定する]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#endif

#define SCREEN_WIDTH	(1920)				// ウインドウの幅
#define SCREEN_HEIGHT	(1080)				// ウインドウの高さ
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// ウインドウの中心Ｙ座標

#define	NUM_VERTEX		(4)					// 頂点数

// 頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define FONT_SIZE (10)

// 上記頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	float rhw;				// テクスチャのパースペクティブコレクト用
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_2D;

// 画面遷移定数
enum E_STAGE
{
	STAGE_TITLE,
	STAGE_GAME,
	STAGE_GAME_END,
	STAGE_RESULT,
	STAGE_RESET,
	STAGE_REFRESH,
	STAGE_MAX
};

#define NUM_POLYGON			(2)

#define MESSAGE_INTERVAL	(180)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);		// デバイス取得関数

void SetStage(int stage);


#endif