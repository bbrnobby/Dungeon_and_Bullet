//=============================================================================
//
// 宝箱処理 [treasure.h]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#ifndef _TREASURE_H_
#define _TREASURE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_GAME_TREASURE				_T("data/TEXTURE/treasure001.png")	// サンプル用画像
#define TEXTURE_TREASURE_SIZE				(48)	// テクスチャサイズ

#define TEXTURE_PATTERN_TREASURE_DIVIDE_X	(3)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_TREASURE_DIVIDE_Y	(4)		// アニメパターンのテクスチャ内分割数（Y)

#define TREASURE_MAX						(30)
#define TREASURE_TIME_ANIMATION				(12)	// アニメーションの切り替わるカウント

enum TREASURE_TYPE
{
	TREASURE_HEART,
	//TREASURE_GUNBOX,
	TREASURE_COMMON,
	TREASURE_RARE,
	TREASURE_BOX_MAX
};

enum BOX_STATE
{
	BOX_NONE,
	BOX_SPAWNED,
	BOX_WAITING,
	BOX_OPENED,
	BOX_EMPTY
};

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct	// フォント構造体
{
	bool			use;						// true:使用  false:未使用
	D3DXVECTOR3		pos;						// 位置
	D3DXVECTOR3		rot;
	float			vec;
	float			height;
	int				type;
	int				state;
	int				size;

	int				PatternAnim;				// アニメーションパターンナンバー
	int				CountAnim;					// アニメーションカウント

	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

	float			Radius;						// 半径
	float			BaseAngle;					// 角度
} TREASURE;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTreasure(int type);
void UninitTreasure(void);
void UpdateTreasure(void);
void DrawTreasure(void);

TREASURE *GetTreasure(int no);
void SetTreasure(float posX, float posY, int type, int size);

#endif
