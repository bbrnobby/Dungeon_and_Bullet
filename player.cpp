//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "gun.h"
#include "dungeon.h"
#include "heart.h"
#include "font.h"
#include "camera.h"
#include "particle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define TEXTURE_GAME_PLAYER	_T("data/TEXTURE/player001.png")	// 画像

#define PLAYER_TEXTURE_PATTERN_DIVIDE_X	(4)		// アニメパターンのテクスチャ内分割数（X)
#define PLAYER_TEXTURE_PATTERN_DIVIDE_Y	(12)	// アニメパターンのテクスチャ内分割数（Y)
#define PLAYER_ANIM_PATTERN_NUM			(4)		// アニメーションパターン数
#define PLAYER_TIME_ANIMATION			(8)		// アニメーションの切り替わるカウント
#define PLAYER_TIME_ANIMATION_RUN		(6)		// アニメーションの切り替わるカウント(走り)

#define PLAYER_KNOCKBACK_SPEED		(15.0f)		// プレイヤーのノックバック速度
#define PLAYER_FRICTION				(0.85f)		// プレイヤーの摩擦係数
#define PLAYER_INTERVAL_INVINCIBLE	(200)		// プレイヤーの無敵時間


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPlayer( void );
void SetTexturePlayer( int cntPattern );
void SetDiffusePLAYER( int alpha );
void SetVertexPlayer( void );

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTexturePlayer = NULL;	// テクスチャへのポリゴン
PLAYER				playerWk;					// プレイヤー構造体

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = &playerWk;

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_PLAYER,				// ファイルの名前
			&g_pD3DTexturePlayer);				// 読み込むメモリのポインタ
	}

	// プレイヤーの初期化処理
	player->use = true;									
	player->isMove = false;								
	player->state = PLAYER_WALK;
	player->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		
	player->subRot = 0.0f;								
	player->vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		
	player->subVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		
	player->hp = 0;					
	player->gunType = GUN_PISTOL;
	player->invincible = false;							
	player->interval = 0;								
	player->dir = 0;									
	player->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		
	player->PatternAnim = 1;							
	player->CountAnim = 0;								
	player->roomID = -1;								
	player->movable = false;							

	D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_PLAYER_SIZE_X,
								TEXTURE_PLAYER_SIZE_Y);
	player->Radius = D3DXVec2Length(&temp);				// プレイヤーの半径を初期化
	player->BaseAngle = atan2f(TEXTURE_PLAYER_SIZE_Y,
								TEXTURE_PLAYER_SIZE_X);	// プレイヤーの角度を初期化

	player->Texture = g_pD3DTexturePlayer;				// テクスチャ情報
	MakeVertexPlayer();									// 頂点情報の作成
	
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	if (g_pD3DTexturePlayer != NULL)
	{	// テクスチャの開放
		g_pD3DTexturePlayer->Release();
		g_pD3DTexturePlayer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	PLAYER *player = &playerWk;
	int roomID;

	// 使用している状態なら更新する
	if (player->use == true)
	{
		// ベクトルデータを初期化
		player->vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		if (player->movable)
		{
			// キーボード入力で移動(上下と左右の同時押しは考えない)
			if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN))
			{	// 下
				if (!player->isMove)
				{
					player->isMove = true;
					player->CountAnim = 0;
				}
				if (player->state != PLAYER_LOCK)
				{
					player->dir = DIR_DOWN;
				}
				player->vec.y = PLAYER_SPEED;
			}
			else if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP))
			{	// 上
				if (!player->isMove)
				{
					player->isMove = true;
					player->CountAnim = 0;
				}
				if (player->state != PLAYER_LOCK)
				{
						player->dir = DIR_UP;
				}
				player->vec.y = -PLAYER_SPEED;
			}

			if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT))
			{	// 右
				if (!player->isMove)
				{
					player->isMove = true;
					player->CountAnim = 0;
				}
				if (player->state != PLAYER_LOCK)
				{
					player->dir = DIR_RIGHT;
				}
				player->vec.x = PLAYER_SPEED;
			}
			else if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT))
			{	// 左
				if (!player->isMove)
				{
					player->isMove = true;
					player->CountAnim = 0;
				}
				if (player->state != PLAYER_LOCK)
				{
					player->dir = DIR_LEFT;
				}
				player->vec.x = -PLAYER_SPEED;
			}

			// 斜め処理
			if (player->vec.x != 0.0f && player->vec.y != 0.0f)
			{
				// 45°
				player->vec.x /= 1.41421356f;
				player->vec.y /= 1.41421356f;
			}

			// 射線固定
			if ((GetMapByPos(player->pos.x, player->pos.y) == MAP_ROOM)
			 &&(GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, BUTTON_Y) || IsButtonPressed(0, BUTTON_Z)))
			{				
				if (player->state == PLAYER_WALK)
				{	// 歩き状態の時、構えモーションに変化
					player->state = PLAYER_LOCK;
					SetTexturePlayer(player->PatternAnim + (player->dir + player->state * 4) * PLAYER_ANIM_PATTERN_NUM);
				}	
			}
			else if (player->state == PLAYER_LOCK)
			{	// 離した時に射線固定を解除し、モーションを通常に戻す
				player->state = PLAYER_WALK;
				player->rot.z = 0.0f;
				player->PatternAnim = 1;
				SetTexturePlayer(player->PatternAnim + player->dir * PLAYER_ANIM_PATTERN_NUM);
			}

			// 走る
			if ((GetMapByPos(player->pos.x, player->pos.y) == MAP_PATH
				|| GetMapByPos(player->pos.x, player->pos.y) == MAP_TRAIL
				|| (GetRoom(GetRoomIDByPos(player->pos.x, player->pos.y))
					&& GetRoom(GetRoomIDByPos(player->pos.x, player->pos.y))->clear))
				&& (GetKeyboardPress(DIK_LCONTROL) || IsButtonPressed(0, BUTTON_B))) {
				if (player->state == PLAYER_WALK)
				{	// 歩き状態の時、走りモーションに変化
					player->state = PLAYER_RUN;
					player->PatternAnim = 1;
					SetTexturePlayer(player->PatternAnim + (player->dir + player->state * 4) * PLAYER_ANIM_PATTERN_NUM);
				}
			}
			else if (player->state == PLAYER_RUN)
			{	// 離した時に走りを解除し、モーションを通常に戻す
				player->state = PLAYER_WALK;
				player->rot.z = 0.0f;
				player->PatternAnim = 1;
				SetTexturePlayer(player->PatternAnim + player->dir * PLAYER_ANIM_PATTERN_NUM);
			}

			// 弾発射
			if (player->state != PLAYER_RUN && GetMapByPos(player->pos.x, player->pos.y) == MAP_ROOM && (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(0, BUTTON_X)))
			{
				SetShot();
			}

			// 静止時は立ちモーションに
			if (player->vec.x == 0.0f && player->vec.y == 0.0f)
			{
				if (player->state != PLAYER_LOCK) player->PatternAnim = 0;
				else player->PatternAnim = -1;
				player->CountAnim = 0;
			}

			// アニメーション
			if (player->isMove && ((player->CountAnim % PLAYER_TIME_ANIMATION) == 0 ||
				(player->state == PLAYER_RUN && player->CountAnim % (PLAYER_TIME_ANIMATION_RUN) == 0)))
			{
				// パターンの切り替え
				player->PatternAnim = (player->PatternAnim + 1) % PLAYER_ANIM_PATTERN_NUM;
				player->CountAnim = 0;

				// テクスチャ座標を設定
				SetTexturePlayer(player->PatternAnim + (player->dir + player->state * 4) * PLAYER_ANIM_PATTERN_NUM);
			}
			player->CountAnim++;

			if (player->invincible)
			{	// 無敵時、キャラクターを点滅させる
				SetDiffusePLAYER((player->interval % 2) * 255);
			}
			player->interval--;
			if (player->interval <= 0)
			{	// インターバル終了時、無敵解除
				SetDiffusePLAYER(255);
				player->invincible = false;
			}

			if (player->vec.x == 0.0f && player->vec.y == 0.0f)
			{
				player->isMove = false;
			}

			// 速さの変化
			switch (player->state)
			{
			case PLAYER_LOCK:
				player->vec = player->vec / 4 * 3;
				break;
			case PLAYER_RUN:
				player->vec = player->vec * 2;
				break;
			}

			// 座標の更新
			if (GetMapByPos(player->pos.x + player->vec.x + player->subVec.x, player->pos.y) < MAP_ROOM)
			{
				player->vec.x = 0.0f;
				player->subVec.x = 0.0f;
			}
			if (GetMapByPos(player->pos.x, player->pos.y + player->vec.y + player->subVec.y) < MAP_ROOM)
			{
				player->vec.y = 0.0f;
				player->subVec.y = 0.0f;
			}

			player->subVec *= PLAYER_FRICTION;

		}
		else if (player->hp == 0)
		{	// プレイヤーの体力が0になったとき
			player->state = PLAYER_WALK;
			player->vec.x = 0.0f;
			player->vec.y = 0.0f;
			player->rot.z += PLAYER_SPEED;
			SetParticleLine(player->pos, player->subVec, D3DCOLOR_RGBA(128, 128, 128, 255), TEXTURE_PLAYER_SIZE_X, TEXTURE_PLAYER_SIZE_X, PARTICLE_LIFE);
		}
		else
		{
			player->PatternAnim = 0;
		}

		// プレイヤーの位置を更新
		player->pos += player->vec + player->subVec;

		// 部屋に入る判定
		roomID = GetRoomIDByPos(player->pos.x, player->pos.y);
		if (player->roomID != roomID)
		{
			if (roomID != -1) EnterRoom(roomID);
			else LeaveRoom(player->roomID);
			player->roomID = roomID;
		}

		// 縦揺れ
		switch (player->state)
		{
		case PLAYER_WALK:
		case PLAYER_LOCK:
			player->subRot = (-(player->PatternAnim % 2) * 3.0f + player->subRot * 3) / 4;
			break;
		case PLAYER_RUN:
			player->subRot = (-(player->PatternAnim % 2) * 6.0f + player->subRot * 3) / 4;
			break;
		}

		// 回転の更新
		switch (player->state)
		{
		case PLAYER_WALK:
			player->rot.z = ((1.5f - player->PatternAnim) * 0.25f + player->rot.z * 20) / 21;
			break;
		case PLAYER_LOCK:
			player->rot.z = player->rot.z * 20 / 21;
			break;
		case PLAYER_RUN:
			player->rot.z = ((1.5f - player->PatternAnim) * 0.5f + ((1 - player->dir) % 2) * 0.3f + player->rot.z * 20) / 21;
			break;
		}

		if (!player->isMove && player->hp != 0)
		{
			player->rot.z = 0.0f;
		}

		// 移動後の座標で頂点を設定
		SetVertexPlayer();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = &playerWk;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (player->use)
	{	// 使用している状態なら描画する
		// テクスチャの設定
		pDevice->SetTexture(0, player->Texture);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, player->vertexWk, sizeof(VERTEX_2D));
	}

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = &playerWk;

	// 頂点座標の設定
	SetVertexPlayer();

	// rhwの設定
	player->vertexWk[0].rhw =
	player->vertexWk[1].rhw =
	player->vertexWk[2].rhw =
	player->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	player->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	player->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	player->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	player->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	SetTexturePlayer(player->PatternAnim);
	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTexturePlayer( int cntPattern )
{
	PLAYER *player = &playerWk;
	
	// テクスチャ座標の設定
	int x = cntPattern % PLAYER_TEXTURE_PATTERN_DIVIDE_X;
	int y = cntPattern / PLAYER_TEXTURE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / PLAYER_TEXTURE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / PLAYER_TEXTURE_PATTERN_DIVIDE_Y;
	
	player->vertexWk[0].tex = D3DXVECTOR2(x * sizeX, y * sizeY);
	player->vertexWk[1].tex = D3DXVECTOR2((x + 1) * sizeX, y * sizeY);
	player->vertexWk[2].tex = D3DXVECTOR2(x * sizeX, (y + 1) * sizeY);
	player->vertexWk[3].tex = D3DXVECTOR2((x + 1) * sizeX, (y + 1) * sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexPlayer(void)
{
	PLAYER *player = &playerWk;
	D3DXVECTOR3 *posCamera = GetCameraPos();
	
	// 頂点座標の設定
	player->vertexWk[0].vtx.x = (int)(posCamera->x - cosf(player->BaseAngle + player->rot.z) * player->Radius);
	player->vertexWk[0].vtx.y = (int)(posCamera->y - sinf(player->BaseAngle + player->rot.z) * player->Radius + player->subRot);
	player->vertexWk[0].vtx.z = 0.0f;
	player->vertexWk[0].vtx	+= player->pos;

	player->vertexWk[1].vtx.x = (int)(posCamera->x + cosf(player->BaseAngle - player->rot.z) * player->Radius);
	player->vertexWk[1].vtx.y = (int)(posCamera->y - sinf(player->BaseAngle - player->rot.z) * player->Radius + player->subRot);
	player->vertexWk[1].vtx.z = 0.0f;
	player->vertexWk[1].vtx += player->pos;

	player->vertexWk[2].vtx.x = (int)(posCamera->x - cosf(player->BaseAngle - player->rot.z) * player->Radius);
	player->vertexWk[2].vtx.y = (int)(posCamera->y + sinf(player->BaseAngle - player->rot.z) * player->Radius + player->subRot);
	player->vertexWk[2].vtx.z = 0.0f;
	player->vertexWk[2].vtx += player->pos;

	player->vertexWk[3].vtx.x = (int)(posCamera->x + cosf(player->BaseAngle + player->rot.z) * player->Radius);
	player->vertexWk[3].vtx.y = (int)(posCamera->y + sinf(player->BaseAngle + player->rot.z) * player->Radius + player->subRot);
	player->vertexWk[3].vtx.z = 0.0f;
	player->vertexWk[3].vtx += player->pos;
}

//=============================================================================
// 反射光の設定
//=============================================================================
void SetDiffusePLAYER(int alpha)
{
	PLAYER *player = &playerWk;	

	// 反射光の設定
	player->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	player->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	player->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	player->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
}

//=============================================================================
// プレイヤー取得関数
//=============================================================================
PLAYER *GetPlayer(void)
{
	return(&playerWk);
}

//=============================================================================
// プレイヤー設定関数
//=============================================================================
void SetPlayer(float x, float y)
{
	PLAYER *player = &playerWk;

	player->isMove = false;
	player->state = PLAYER_WALK;

	player->pos.x = x;
	player->pos.y = y;

	player->subRot = 0.0f;
	player->vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player->dir = DIR_RIGHT;
	player->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player->PatternAnim = 0;
	player->CountAnim = 0;
	player->roomID = -1;
	player->subVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player->invincible = false;
	player->interval = MESSAGE_INTERVAL;
	player->hp = MAX_HP * (HEART_DIVIDE - 1);
	player->movable = true;

	SetGun(player->gunType);
}

void KnockBackPlayer(PLAYER *player, float rotZ)
{
	player->subVec.x += PLAYER_KNOCKBACK_SPEED * cosf(rotZ);
	player->subVec.y += PLAYER_KNOCKBACK_SPEED * sinf(rotZ);
}

void SetInvincible(PLAYER *player)
{
	player->invincible = true;
	player->interval = PLAYER_INTERVAL_INVINCIBLE;
}
