//=============================================================================
//
// バレット処理 [bullet.h]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_GAME_BULLET				_T("data/TEXTURE/bullet001.png")	// 画像

#define TEXTURE_PATTERN_DIVIDE_X_BULLET	(2)			// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_BULLET	(2)			// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_BULLET			(TEXTURE_PATTERN_DIVIDE_X_BULLET*TEXTURE_PATTERN_DIVIDE_Y_BULLET)	// アニメーションパターン数
#define TIME_ANIMATION_BULLET			(4)			// アニメーションの切り替わるカウント

#define BULLET_MAX						(50)		// 表示される弾の最大数

#define BULLET_SIZE_PISTOL				(16)		// バレットの大きさ
#define BULLET_SIZE_SHOTGUN				(9)			// バレットの大きさ
#define BULLET_SIZE_ENEMY				(25)		// バレットの大きさ

#define BULLET_SPEED_PISTOL				(35.0f)		// バレットの移動スピード
#define BULLET_SPEED_SHOTGUN			(30.0f)		// バレットの移動スピード
#define BULLET_SPEED_ENEMY				(5.0f)		// 敵バレットの移動スピード
#define BULLET_SPINSPEED				(0.3f)		// 弾の回転速度

#define BULLET_DURATION_PISTOL			(120)		// バレットの持続時間
#define BULLET_DURATION_SHOTGUN			(15)		// バレットの持続時間
#define BULLET_DURATION_ENEMY			(1000)		// エネミーのバレット持続時間

enum type
{
	PLAYER_BULLET_PISTOL,
	PLAYER_BULLET_SHOTGUN,
	ENEMY_BULLET
};

//*****************************************************************************
// 構造体宣言
//*****************************************************************************

typedef struct	// バレット構造体
{
	bool			use;						// true:使用  false:未使用
	D3DXVECTOR3		pos;						// 移動量
	D3DXVECTOR3		vec;						// 移動ベクトル
	D3DXVECTOR3		rot;						// 回転量
	int				duration;
	int				PatternAnim;
	
	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

	bool			spin;						// 回転するか
	float			Radius;						// 半径
	float			BaseAngle;					// 角度

	int				type;						// 弾のタイプ
} BULLET;



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet(int type);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, float rot, int type);
BULLET *GetBullet(int no);


#endif
