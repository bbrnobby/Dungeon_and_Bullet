//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_PLAYER_SIZE_X		(48)	// テクスチャサイズ
#define TEXTURE_PLAYER_SIZE_Y		(48)	// 同上
#define PLAYER_SPEED				(6.0f)	// プレイヤーの速度

//*****************************************************************************
// 構造体宣言
//*****************************************************************************

typedef struct	// プレイヤー構造体
{
	bool			use;						// true:使用  false:未使用
	bool			isMove;						// 動いているか
	int				state;						// 状態
	D3DXVECTOR3		pos;						// 移動量
	float			subPosY;					// 縦移動
	D3DXVECTOR3		vec;						// 移動ベクトル
	D3DXVECTOR3		subVec;						// 反動ベクトル
	bool			invincible;					// 無敵
	int				interval;					// インターバル
	int				hp;							// 体力
	int				gunType;					// 銃のタイプ
	bool			movable;					// 動けるか

	int				dir;						// 方向
	D3DXVECTOR3		rot;						// 回転量
	int				PatternAnim;				// アニメーションパターンナンバー
	int				CountAnim;					// アニメーションカウント

	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

	float			Radius;						// 半径
	float			BaseAngle;					// 角度

	int				roomID;						// いる部屋のID

} PLAYER;

enum PLAYER_DIRECTION
{
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT,
	DIR_UP,
};

enum PLAYER_STATE
{
	PLAYER_WALK,
	PLAYER_LOCK,
	PLAYER_RUN,
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(int type);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);
void SetPlayer(float x, float y);
void KnockBackPlayer(PLAYER *player, float rotZ);
void SetInvincible(PLAYER *player);

#endif
