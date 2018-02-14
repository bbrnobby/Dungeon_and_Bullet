//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#include "camera.h"
#include "player.h"
#include "minimap.h"
#include "dungeon.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	CAMERA_DISTANCE		(72.0f)												// カメラの移動量

#define	RATE_CHASE_CAMERA	(50.0f)												// カメラの視点への補正係数

#define SCREEN_CENTER		D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f)	// 画面の中心

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
D3DXVECTOR3		g_posCamera;				// カメラの視点
D3DXVECTOR3		g_subPosCamera;				// カメラの揺れ
D3DXVECTOR3		g_posCameraRounded;			// カメラの視点(整数)
D3DXVECTOR3		g_vecCamera;				// カメラの方向
float			g_rotCamera;				// 揺れの値

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitCamera(void)
{
	g_posCamera = SCREEN_CENTER;					// 座標データを初期化
	g_subPosCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 揺れデータを初期化
	g_vecCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 方向データを初期化
	g_rotCamera = 0.0f;								// 揺れの値を初期化

	return S_OK;
}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateCamera(void)
{
	PLAYER *player = GetPlayer();

	// プレイヤーの射線固定時
	if (player->state == PLAYER_LOCK)
	{
		switch (player->dir)
		{
		case DIR_RIGHT:
			g_vecCamera.x = PLAYER_SPEED;
			break;
		case DIR_DOWN:
			g_vecCamera.y = PLAYER_SPEED;
			break;
		case DIR_LEFT:
			g_vecCamera.x = -PLAYER_SPEED;
			break;
		case DIR_UP:
			g_vecCamera.y = -PLAYER_SPEED;
			break;
		}
	}
	// プレイヤーが移動しているとき
	else if (player->isMove)
	{
		// カメラの方向をプレイヤーの移動方向にする
		g_vecCamera = player->vec;
	}
	// それ以外
	else
	{
		g_vecCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (player->hp != 0)
	{
		// カメラ位置の更新
		switch (player->state)
		{
		case PLAYER_WALK:
		case PLAYER_RUN:
			g_posCamera = (SCREEN_CENTER - g_vecCamera * CAMERA_DISTANCE / 2 + g_posCamera * RATE_CHASE_CAMERA * 2) / (RATE_CHASE_CAMERA * 2 + 1);
			break;
		case PLAYER_LOCK:
			g_posCamera = (SCREEN_CENTER - g_vecCamera * CAMERA_DISTANCE + g_posCamera * RATE_CHASE_CAMERA) / (RATE_CHASE_CAMERA + 1);
			break;
		}
		g_posCameraRounded = D3DXVECTOR3((int)(g_posCamera.x - player->pos.x), (int)(g_posCamera.y - player->pos.y), (int)(g_posCamera.z - player->pos.z));
		
		// 視界の範囲でミニマップ可視化
		SetMiniMap((int)((-g_posCameraRounded.x) / TEXTURE_DUNGEON_SIZE_X), (int)((-g_posCameraRounded.y) / TEXTURE_DUNGEON_SIZE_Y), (int)((-g_posCameraRounded.x + SCREEN_WIDTH) / TEXTURE_DUNGEON_SIZE_X), (int)((-g_posCameraRounded.y + SCREEN_HEIGHT) / TEXTURE_DUNGEON_SIZE_Y));
	}
	else
	{	// プレイヤーがやられたとき

		// カメラをプレイヤーの中心に向ける
		//g_vecCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (g_rotCamera != 0.0f)
	{
		g_rotCamera += D3DX_PI * 0.75;
		if (g_rotCamera < D3DX_PI * 20)
		{
			g_subPosCamera.x = (D3DX_PI * 20 - g_rotCamera) * cosf(g_rotCamera) * 0.25;
			g_subPosCamera.y = (D3DX_PI * 20 - g_rotCamera) * sinf(g_rotCamera) * 0.5;
		}
		else
		{
			g_subPosCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_rotCamera = 0.0f;
		}
	}
}

//=============================================================================
// カメラ座標取得関数
//=============================================================================
D3DXVECTOR3 *GetPosCamera(void)
{
	return(&g_posCameraRounded);
}

//=============================================================================
// 揺れ取得関数
//=============================================================================
D3DXVECTOR3 *GetSubPosCamera(void)
{
	return(&g_subPosCamera);
}

//=============================================================================
// カメラ座標設定関数
//=============================================================================
void SetCamera(float x, float y)
{
	g_posCamera = D3DXVECTOR3(x, y, 0.0f);			// 座標データを初期化
	g_vecCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 方向データを初期化
}

//=============================================================================
// カメラ揺れ設定関数
//=============================================================================
void SetShakeCamera(void)
{
	g_rotCamera += D3DX_PI * 0.75;
	g_subPosCamera.x = (D3DX_PI * 50 - g_rotCamera) * cosf(g_rotCamera) * 0.25;
	g_subPosCamera.y = (D3DX_PI * 50 - g_rotCamera) * sinf(g_rotCamera) * 0.5;
}