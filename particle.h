//=============================================================================
//
// パーティクル処理 [particle.h]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define PARTICLE_LIFE		(60)							// 寿命

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	bool		use;		// 使用しているかどうか
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 vec;		// 移動量
	D3DXVECTOR3 rot;		// 回転
	D3DXVECTOR3 scale;		// スケール
	D3DXCOLOR	col;		// 色
	//float		sizeX;		// 幅
	//float		sizeY;		// 高さ
	//int nIdxShadow;			// 影ID
	int			life;		// 寿命
	bool		gravity;	// 重力があるか

	VERTEX_2D				vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
} PARTICLE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitParticle(int type);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

void SetParticleLine(D3DXVECTOR3 pos, D3DXVECTOR3 vec, D3DXCOLOR col, float sizeX, float sizeY, int life);
PARTICLE *GetParticle(int no);

#endif
