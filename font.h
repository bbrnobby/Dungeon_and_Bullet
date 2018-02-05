//=============================================================================
//
// 文字処理 [font.h]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#ifndef _FONT_H_
#define _FONT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_GAME_FONT		_T("data/TEXTURE/font001.png")	// サンプル用画像
#define TEXTURE_FONT_SIZE				(64)	// テクスチャサイズ

#define TEXTURE_PATTERN_FONT_DIVIDE_X	(35)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_FONT_DIVIDE_Y	(6)		// アニメパターンのテクスチャ内分割数（Y)

#define FONT_BUFFER						(128)	// 表示できる文字数

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct	// フォント構造体
{
	bool			use;						// true:使用  false:未使用
	D3DXVECTOR3		pos;						// 位置
	int				size;						// 大きさ
	int				index;						// 文字インデックス

	int				duration;					// アニメーションカウント

	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク
} FONT;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitFont(int type);
void UninitFont(void);
void UpdateFont(void);
void DrawFont(void);

void SetString(char *string, float posX, float posY, int size, int interval);
void RefreshFont(void);

#endif
