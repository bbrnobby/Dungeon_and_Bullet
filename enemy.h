//=============================================================================
//
// 敵処理 [enemy.h]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "dungeon.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_ENEMY_SIZE_X	(36)	// テクスチャサイズ
#define TEXTURE_ENEMY_SIZE_Y	(36)	// 同上

#define ENEMY_INDEX				(25)	// エネミーの基準値
#define ENEMY_ROOM_MIN_NUM		(3)		// 部屋中のエネミーの最小数
#define ENEMY_MAX				(MAP_WIDTH * MAP_HEIGHT / ENEMY_INDEX) // 敵の最大数

#define ENEMY_KNOCKBACK_SPEED	(15.0f)	// エネミー1のノックバック速度
#define ENEMY_AVOID_SPEED		(0.5f)	// エネミー1の押しのけ速度

enum ENEMY_STATE
{
	ENEMY_WAIT,
	ENEMY_MOVE,
	ENEMY_ATTACK,
	ENEMY_DEFAULT_STATE,
	ENEMY_DAMAGE,
	ENEMY_SPAWN,
	ENEMY_DESPAWN,
	ENEMY_DEAD
};


//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct	// エネミー構造体
{
	bool			use;						// true:使用  false:未使用
	D3DXVECTOR3		pos;						// 移動量
	float			subRot;						// 縦移動
	D3DXVECTOR3		vec;						// 移動ベクトル
	D3DXVECTOR3		subVec;						// 補助移動ベクトル
	D3DXVECTOR3		rot;						// 回転量
	int				dir;						// 方向
	int				PatternAnim;				// アニメーションパターンナンバー
	int				CountAnim;					// アニメーションカウント

	bool			notice;						// 気付いているか
	int				type;						// エネミーの種類
	int				state;						// エネミーの状態
	int				interval;					// インターバル

	int				hp;							// エネミーの体力
	int				maxhp;						// エネミーの体力
	//int				score;					// 倒した時のスコア

	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

	float			Radius;						// 半径
	float			BaseAngle;					// 角度

	int				roomID;						// エネミーのいる部屋のID

} ENEMY;



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(int type);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY *GetEnemy(int no);
void SetEnemy(int ID);
bool SpawnEnemy(float x, float y, int ID);
void DeSpawnEnemy(int ID);
void DeleteEnemy(ENEMY *enemy);
void SetState(ENEMY *enemy, int state);
void KnockBackEnemy(ENEMY *enemy, float speed, float rotZ);

#endif
