//=============================================================================
//
// 背景処理 [bg.h]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

#include "main.h"
#include "dungeon.h"

// マクロ定義
#define TEXTURE_GAME_BG00		_T("data/TEXTURE/galaxy.jpg")	// サンプル用画像
#define BG00_SIZE_X			(1000)	// テクスチャサイズ
#define BG00_SIZE_Y			(1000)	// 同上
#define BG00_POS_X			(MAP_WIDTH * TEXTURE_DUNGEON_SIZE_X / 2)		// ポリゴンの初期位置X(左上)
#define BG00_POS_Y			(MAP_HEIGHT * TEXTURE_DUNGEON_SIZE_Y / 2)		// 同上
#define BG00_CENTER_X		(BG00_POS_X / 2)
#define BG00_CENTER_Y		(BG00_POS_Y / 2)

#define BG00_ROTATION		(0.0002f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBG(int type);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);

#endif
