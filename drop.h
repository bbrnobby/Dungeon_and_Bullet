//=============================================================================
//
// ドロップ処理 [drop.h]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#ifndef _DROP_H_
#define _DROP_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_DROP_SIZE				(48)		// テクスチャサイズ

#define MAX_DROP						(30)
#define DROP_MES_SIZE					(36)
#define DROP_MES_INTERVVAL				(120)

enum DROP_TYPE
{
	DROP_COIN001,
	DROP_COIN002,
	DROP_COIN003,
	//DROP_PISTOL,
	//DROP_SHOTGUN,
	//DROP_LASERGUN,
	DROP_MINIHEART,
	DROP_BIGHEART,
	DROP_JEWEL001,
	DROP_JEWEL002,
	DROP_JEWEL003,
	DROP_JEWEL004,
	DROP_JEWEL005,
	DROP_SLUG,
	DROP_MAX
};


//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct	// フォント構造体
{
	bool			use;						// true:使用  false:未使用
	D3DXVECTOR3		pos;						// 位置
	D3DXVECTOR3		vec;						// 速度
	D3DXVECTOR3		rot;						// 回転
	float			vecRot;						// 回転速度
	float			subVec;						// 縦移動
	float			height;						// 高さ
	int				type;						// 種類
	int				size;						// 大きさ

	int				interval;					// インターバル

	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

	float			Radius;						// 半径
	float			BaseAngle;					// 角度
} DROP;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitDrop(int type);
void UninitDrop(void);
void UpdateDrop(void);
void DrawDrop(void);

DROP *GetDrop(int no);
void SetDrop(float posX, float posY, int type, int size);
void PickDrop(DROP *drop);

#endif
