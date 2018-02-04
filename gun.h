//=============================================================================
//
// 銃処理 [gun.h]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#ifndef _GUN_H_
#define _GUN_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_GUN_SIZE_X			(96)		// テクスチャサイズ
#define TEXTURE_GUN_SIZE_Y			(96)		// 同上
#define SLUG_MAX					(8)			// 散弾数

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct	// 銃構造体
{
	bool			use;						// true:使用  false:未使用
	int				type;						// 銃の種類							
	D3DXVECTOR3		pos;						// 位置
	D3DXVECTOR3		subPos;						// 位置調整
	D3DXVECTOR3		vec;						// 移動ベクトル
	D3DXVECTOR3		rot;						// 回転量
	float			dirRot;						// 回転量(方向)
	float			subRot;						// 斜めの回転
	bool			isLocked;					// 方向の固定

	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

	float			Radius;						// 半径
	float			BaseAngle;					// 角度
} GUN;

enum GUN_TYPE
{
	GUN_PISTOL,
	GUN_SHOTGUN,
	//GUN_LASERGUN,
	GUN_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGun(int type);	// 初期化処理
void UninitGun(void);		// 終了処理
void UpdateGun(void);		// 更新処理
void DrawGun(void);			// 描画処理

GUN *GetGun();				// 銃の取得
void SetGun(int no);		// 銃の設定
void SetShot();				// 銃の発射
#endif
