//=============================================================================
//
// 体力処理 [heart.h]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#ifndef _HEART_H_
#define _HEART_H_

#include "main.h"

// マクロ定義
#define TEXTURE_HEART	_T("data/TEXTURE/heart001.png")	// サンプル用画像
#define TEXTURE_HEART_SIZE_X	(60)	// テクスチャサイズ
#define TEXTURE_HEART_SIZE_Y	(60)	// 同上
#define HEART_DIVIDE			(4)		// 体力の分割数

#define MAX_HP					(3)		// 体力の最大値

#define HEART_POS_X				(90)	// ポリゴンの初期位置X
#define HEART_POS_Y				(90)	// 同上

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitHeart(int type);
void UninitHeart(void);
void UpdateHeart(void);
void DrawHeart(void);

void SetHeartArea(float posX, float posY, float sizeX, float sizeY);

#endif
