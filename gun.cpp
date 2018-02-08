//=============================================================================
//
// 銃処理 [gun.cpp]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#include "main.h"
#include "gun.h"
#include "bullet.h"
#include "player.h"
#include "camera.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define INT_SHOT_PISTOL			(10)	// ピストルの射撃インターバル
#define INT_SHOT_SHOTOGUN		(30)	// ショットガンの射撃インターバル
#define MAG_PISTOL				(10)	// ピストルのマガジン弾数
#define MAG_SHOTGUN				(6)		// ショットガンのマガジン弾数
#define	MAG_MAX					(30)	// 最大弾表示数

#define TEXTURE_UI_GUN			_T("data/TEXTURE/gun000.png")	// 銃のテクスチャ(UI部分)
#define TEXTURE_UI_WINDOW		_T("data/TEXTURE/window001.png")	// ウィンドウのテクスチャ(UI部分)
#define TEXTURE_UI_BULLET		_T("data/TEXTURE/bullet000.png")	// 弾のテクスチャ(UI部分)　

#define TEXTURE_UI_GUN_SIZE			(128)	// 銃(UI)のテクスチャサイズ
#define TEXTURE_UI_WIN_SIZE			(144)	// ウィンドウのテクスチャサイズ
#define TEXTURE_UI_BULLET_SIZE_X	(16)	// 銃弾(UI)のテクスチャXサイズ
#define TEXTURE_UI_BULLET_SIZE_Y	(32)	// 銃弾(UI)のテクスチャYサイズ

#define POS_UI_GUN_X		(SCREEN_WIDTH - 30 - (TEXTURE_UI_GUN_SIZE + TEXTURE_UI_WIN_SIZE) / 2)	// 銃(UI)のX座標
#define POS_UI_GUN_Y		(SCREEN_HEIGHT - 30 - (TEXTURE_UI_GUN_SIZE + TEXTURE_UI_WIN_SIZE) / 2)	// 銃(UI)のY座標
#define POS_UI_WINDOW_X		(SCREEN_WIDTH - 30 - TEXTURE_UI_WIN_SIZE)		// ウィンドウのX座標
#define POS_UI_WINDOW_Y		(SCREEN_HEIGHT - 30 - TEXTURE_UI_WIN_SIZE)		// ウィンドウのY座標
#define POS_UI_BULLET_X		(SCREEN_WIDTH - 45 - TEXTURE_UI_BULLET_SIZE_X)		// 銃弾(UI)のX座標
#define POS_UI_BULLET_Y		(SCREEN_HEIGHT - 45 - TEXTURE_UI_BULLET_SIZE_Y)		// 銃弾(UI)のY座標

const char *TEXTURE_GAME_GUN[] =	// 銃のテクスチャ(ゲーム部分)
{
	"data/TEXTURE/gun001.png",
	"data/TEXTURE/gun002.png",
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexGun(void);
void SetTextureGun(void);
void SetVertexGun(int dir);

void SetTextureGunUI(void);
void SetVertexGunUI(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureGun[GUN_MAX] = {};		// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		g_pD3DTextureUIGun;					// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		g_pD3DTextureUIBullet;				// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		g_pD3DTextureUIWindow;				// テクスチャへのポリゴン

GUN						gunWk;								// 銃構造体
UI_GUN					uiGunWk;							// 銃(UI)構造体
UI_WINDOW				uiWinWk;							// ウィンドウ(UI)構造体
UI_BULLET				uiBulletWk[MAG_MAX];				// 銃弾(UI)構造体
bool					g_bGun[GUN_MAX];					// 銃の所持フラグ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGun(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GUN *gun = &gunWk;							// 銃のポインタ
	UI_GUN *uiGun = &uiGunWk;					// 銃(UI)のポインタ
	UI_WINDOW *uiWin = &uiWinWk;				// ウィンドウ(UI)のポインタ
	UI_BULLET *uiBullet = uiBulletWk;			// 銃弾(UI)のポインタ

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		for (int i = 0; i < GUN_MAX; i++)
		{
			D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
				TEXTURE_GAME_GUN[i],			// ファイルの名前
				&g_pD3DTextureGun[i]);			// 読み込むメモリのポインタ
		}

		D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
			TEXTURE_UI_GUN,					// ファイルの名前
			&g_pD3DTextureUIGun);			// 読み込むメモリのポインタ

		D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
			TEXTURE_UI_WINDOW,				// ファイルの名前
			&g_pD3DTextureUIWindow);		// 読み込むメモリのポインタ

		D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
			TEXTURE_UI_BULLET,				// ファイルの名前
			&g_pD3DTextureUIBullet);		// 読み込むメモリのポインタ

	}

	// 銃の初期化処理
	gun->use = false;
	gun->pos = D3DXVECTOR3(0.0f, TEXTURE_PLAYER_SIZE_Y / 4.0f, 0.0f);
	gun->subPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	gun->vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	gun->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	gun->dirRot = 0.0f;
	gun->subRot = 0.0f;
	gun->isLocked = false;

	D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_GUN_SIZE_X, TEXTURE_GUN_SIZE_Y);
	gun->Radius = D3DXVec2Length(&temp);								// 銃の半径を初期化
	gun->BaseAngle = atan2f(TEXTURE_GUN_SIZE_Y, TEXTURE_GUN_SIZE_X);	// 銃の角度を初期化

	// 銃(UI)の初期化処理
	uiGun->use = false;
	uiGun->pos = D3DXVECTOR3(POS_UI_GUN_X, POS_UI_GUN_Y, 0.0f);
	uiGun->subPosX = 0.0f;
	uiGun->count = -1;
	uiGun->Texture = g_pD3DTextureUIGun;
	

	// ウィンドウ(UI)の初期化処理
	uiWin->pos = D3DXVECTOR3(POS_UI_WINDOW_X, POS_UI_WINDOW_Y, 0.0f);
	uiWin->Texture = g_pD3DTextureUIWindow;

	// 銃弾(UI)の初期化処理
	for (int i = 0; i < MAG_MAX; i++, uiBullet++)
	{
		uiBullet->use = false;
		uiBullet->pos = D3DXVECTOR3(POS_UI_BULLET_X, POS_UI_BULLET_Y, 0.0f);
		uiBullet->subPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		uiBullet->vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		uiBullet->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		uiBullet->vecRot = 0.0f;

		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_UI_BULLET_SIZE_X, TEXTURE_UI_BULLET_SIZE_Y);
		uiBullet->Radius = D3DXVec2Length(&temp);											// 銃の半径を初期化
		uiBullet->BaseAngle = atan2f(TEXTURE_UI_BULLET_SIZE_Y, TEXTURE_UI_BULLET_SIZE_X);	// 銃の角度を初期化

		uiBullet->Texture = g_pD3DTextureUIBullet;
	}

	// 頂点情報の作成
	MakeVertexGun();	

	// 所持フラグの初期化
	for (int i = 0; i < GUN_MAX; i++)
	{
		g_bGun[i] = true;
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGun(void)
{
	for (int i = 0; i < GUN_MAX; i++)
	{
		if (g_pD3DTextureGun[i] != NULL)
		{	// テクスチャの開放
			g_pD3DTextureGun[i]->Release();
			g_pD3DTextureGun[i] = NULL;
		}
	}

	if (g_pD3DTextureUIGun != NULL)
	{	// テクスチャの開放
		g_pD3DTextureUIGun->Release();
		g_pD3DTextureUIGun = NULL;
	}

	if (g_pD3DTextureUIWindow != NULL)
	{	// テクスチャの開放
		g_pD3DTextureUIWindow->Release();
		g_pD3DTextureUIWindow = NULL;
	}

	if (g_pD3DTextureUIBullet != NULL)
	{	// テクスチャの開放
		g_pD3DTextureUIBullet->Release();
		g_pD3DTextureUIBullet = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGun(void)
{
	GUN *gun = &gunWk;					
	PLAYER *player = GetPlayer();		
	UI_GUN *uiGun = &uiGunWk;
	UI_WINDOW *uiWin = &uiWinWk;
	UI_BULLET *uiBullet = uiBulletWk;

	// プレイヤーの状態で銃の描画を判断
	switch (player->state)
	{
	case PLAYER_WALK:
	case PLAYER_LOCK:
		gun->use = true;
		break;
	case PLAYER_RUN:
		gun->use = false;
		break;
	}

	// 使用している状態なら更新する
	if (gun->use)					
	{
		// 射線非固定時
		if (player->state != PLAYER_LOCK)
		{
			gun->isLocked = false;	// 銃を非固定に
			gun->subRot = 0.0f;		// 斜めの回転をリセット
		}

		// 銃非固定時
		if (!gun->isLocked)
		{
			gun->subPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			switch (player->dir)
			{
			case DIR_RIGHT:	// 右
				if (player->state == PLAYER_LOCK)
				{
					gun->isLocked = true;
					if (gun->type == GUN_SHOTGUN)
					{
						gun->subPos.x = -TEXTURE_PLAYER_SIZE_X * 0.125f;
						if (player->vec.y > 0)
						{
							gun->subPos.x += TEXTURE_PLAYER_SIZE_X * 0.125f;
							gun->subRot = D3DX_PI * 0.25f;
						}
						else if (player->vec.y < 0)
						{
							gun->subPos.x -= TEXTURE_PLAYER_SIZE_X * 0.125f;
							gun->subRot = -D3DX_PI * 0.25f;
						}
					}
					else
					{
						gun->subPos.x = TEXTURE_PLAYER_SIZE_X * 0.25f;
						if (player->vec.y > 0)
						{
							gun->subPos.x += TEXTURE_PLAYER_SIZE_X * 0.25f;
							gun->subRot = D3DX_PI * 0.25f;
						}
						else if (player->vec.y < 0)
						{
							gun->subPos.x -= TEXTURE_PLAYER_SIZE_X * 0.25f;
							gun->subRot = -D3DX_PI * 0.25f;
						}
					}
				}
				else
				{
					gun->subPos.x = TEXTURE_PLAYER_SIZE_X * 0.125;
					if (player->vec.y > 0)
					{
						gun->subPos.x += TEXTURE_PLAYER_SIZE_X * 0.125f;
						gun->subRot = D3DX_PI * 0.25f;
					}
					else if (player->vec.y < 0)
					{
						gun->subPos.x -= TEXTURE_PLAYER_SIZE_X * 0.125f;
						gun->subRot = -D3DX_PI * 0.25f;
					}
				}
				// なめらかに回転させる
				if (gun->dirRot > D3DX_PI)
				{
					gun->dirRot -= D3DX_PI * 2;
				}
				break;
			case DIR_DOWN:	// 下
				gun->subRot = 0.0f;	// 斜めの回転をリセット
				if (player->state == PLAYER_LOCK)
				{
					gun->isLocked = true;
					if (gun->type == GUN_SHOTGUN)
					{
						gun->subPos.x = -TEXTURE_PLAYER_SIZE_X * 0.375f;
						gun->subPos.y = -TEXTURE_PLAYER_SIZE_Y * 0.125f;
					}
					else
					{
						gun->subPos.x = TEXTURE_PLAYER_SIZE_X * 0.125f;
						gun->subPos.y = TEXTURE_PLAYER_SIZE_Y * 0.375f;
					}
				}
				else
				{
					gun->subPos.x = -TEXTURE_PLAYER_SIZE_X * 0.25f;
					gun->subPos.y = TEXTURE_PLAYER_SIZE_Y * 0.25f;
				}
				break;
			case DIR_LEFT:	// 左
				if (player->state == PLAYER_LOCK)
				{
					gun->isLocked = true;
					if (gun->type == GUN_SHOTGUN)
					{
						gun->subPos.x = TEXTURE_PLAYER_SIZE_X * 0.125f;
						if (player->vec.y > 0)
						{
							gun->subPos.x -= TEXTURE_PLAYER_SIZE_X * 0.25f;
							gun->subPos.y -= TEXTURE_PLAYER_SIZE_Y * 0.25f;
							gun->subRot = -D3DX_PI * 0.25f;
						}
						else if (player->vec.y < 0)
						{
							gun->subPos.y += TEXTURE_PLAYER_SIZE_Y * 0.125f;
							gun->subRot = D3DX_PI * 0.25f;
						}
					}
					else
					{
						gun->subPos.x = -TEXTURE_PLAYER_SIZE_X * 0.375f;
						if (player->vec.y > 0)
						{
							gun->subPos.x -= TEXTURE_PLAYER_SIZE_X * 0.25f;
							gun->subRot = -D3DX_PI * 0.25f;
						}
						else if (player->vec.y < 0)
						{
							gun->subPos.x += TEXTURE_PLAYER_SIZE_X * 0.25f;
							gun->subPos.y -= TEXTURE_PLAYER_SIZE_Y * 0.25f;
							gun->subRot = D3DX_PI * 0.25f;
						}
					}
				}
				else
				{
					gun->subPos.x = -TEXTURE_PLAYER_SIZE_X * 0.125f;
					if (player->vec.y > 0)
					{
						gun->subPos.x -= TEXTURE_PLAYER_SIZE_X * 0.125f;
						gun->subRot = -D3DX_PI * 0.25f;
					}
					else if (player->vec.y < 0)
					{
						gun->subPos.x += TEXTURE_PLAYER_SIZE_X * 0.125f;
						gun->subPos.y -= TEXTURE_PLAYER_SIZE_Y * 0.125f;
						gun->subRot = D3DX_PI * 0.25f;
					}
				}
				break;
			case DIR_UP:	// 上
				gun->subRot = 0.0f;	// 斜めの回転をリセット
				if (player->state == PLAYER_LOCK)
				{
					gun->isLocked = true;
					if (gun->type == GUN_SHOTGUN)
					{
						gun->subPos.x = TEXTURE_PLAYER_SIZE_X * 0.375f;
						gun->subPos.y = TEXTURE_PLAYER_SIZE_Y * 0.25f;
					}
					else
					{
						gun->subPos.x = -TEXTURE_PLAYER_SIZE_X * 0.125f;
						gun->subPos.y = -TEXTURE_PLAYER_SIZE_Y * 0.25f;
					}
				}
				else
				{
					gun->subPos.x = TEXTURE_PLAYER_SIZE_X * 0.25f;
					gun->subPos.y = 0.0f;
				}
				// なめらかに回転させる
				if (gun->dirRot < D3DX_PI)
				{
					gun->dirRot += D3DX_PI * 2;
				}
				break;
			}
		}
		// 回転の更新
		gun->dirRot = (player->dir * D3DX_PI / 2 + gun->subRot + gun->dirRot * 5) / 6;
		gun->rot.z = player->rot.z * 2 + gun->dirRot;

		// 位置の更新
		gun->pos.x = (gun->subPos.x + gun->pos.x * 2) / 3;
		gun->pos.y = (gun->subPos.y + player->subRot + gun->pos.y * 2) / 3;

		SetVertexGun(player->dir);	// 移動後の座標で頂点を設定
	}
	// 位置の更新
	if (TEXTURE_UI_GUN_SIZE * uiGun->count - uiGun->subPosX < 0.1f && uiGun->count % GUN_MAX == 0)
	{
		uiGun->subPosX = 0.0f;
		uiGun->count = 0;
	}
	else
	{
		uiGun->subPosX = (uiGun->subPosX * 3 + uiGun->count * TEXTURE_UI_GUN_SIZE) / 4;
	}
	PrintDebugProc("count:%d\n", uiGun->count);
	SetTextureGunUI();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGun(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GUN *gun = &gunWk;
	UI_GUN *uiGun = &uiGunWk;
	UI_WINDOW *uiWin = &uiWinWk;
	UI_BULLET *uiBullet = uiBulletWk;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < GUN_MAX; i++, gun++)
	{
		// 使用している状態なら描画する
		if (gun->use == true)	
		{
			// テクスチャの設定
			pDevice->SetTexture(0, gun->Texture);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, gun->vertexWk, sizeof(VERTEX_2D));
		}
	}

	// テクスチャの設定
	pDevice->SetTexture(0, uiGun->Texture);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, uiGun->vertexWk, sizeof(VERTEX_2D));

	// テクスチャの設定
	pDevice->SetTexture(0, uiWin->Texture);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, uiWin->vertexWk, sizeof(VERTEX_2D));

	for (int i = 0; i < MAG_MAX; i++, uiBullet++)
	{
		if (uiBullet->use)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, uiBullet->Texture);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, uiBullet->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexGun(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GUN *gun = &gunWk;
	UI_GUN *uiGun = &uiGunWk;
	UI_WINDOW *uiWin = &uiWinWk;
	UI_BULLET *uiBullet = uiBulletWk;

	// 頂点座標の設定
	SetVertexGun(GetPlayer()->dir);
	SetVertexGunUI();

	// rhwの設定
	gun->vertexWk[0].rhw =
	gun->vertexWk[1].rhw =
	gun->vertexWk[2].rhw =
	gun->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	gun->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	gun->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	gun->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	gun->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// rhwの設定
	uiGun->vertexWk[0].rhw =
	uiGun->vertexWk[1].rhw =
	uiGun->vertexWk[2].rhw =
	uiGun->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	uiGun->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	uiGun->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	uiGun->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	uiGun->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// rhwの設定
	uiWin->vertexWk[0].rhw =
	uiWin->vertexWk[1].rhw =
	uiWin->vertexWk[2].rhw =
	uiWin->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	uiWin->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	uiWin->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	uiWin->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	uiWin->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	for (int i = 0; i < MAG_MAX; i++, uiBullet++)
	{
		// 反射光の設定
		uiBullet->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		uiBullet->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		uiBullet->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		uiBullet->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// rhwの設定
		uiBullet->vertexWk[0].rhw =
		uiBullet->vertexWk[1].rhw =
		uiBullet->vertexWk[2].rhw =
		uiBullet->vertexWk[3].rhw = 1.0f;
	}

	// テクスチャ座標の設定
	SetTextureGun();
	SetTextureGunUI();

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureGun(void)
{
	GUN *gun = &gunWk;

	// テクスチャ座標の設定
	gun->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f);
	gun->vertexWk[1].tex = D3DXVECTOR2( 1.0f, 0.0f);
	gun->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f);
	gun->vertexWk[3].tex = D3DXVECTOR2( 1.0f, 1.0f);
}

void SetTextureGunUI(void)
{
	UI_GUN *uiGun = &uiGunWk;
	UI_WINDOW *uiWin = &uiWinWk;
	UI_BULLET *uiBullet = uiBulletWk;

	// テクスチャ座標の設定
	float startX = uiGun->subPosX / TEXTURE_UI_GUN_SIZE / GUN_MAX;
	float sizeX = 1.0f / GUN_MAX;

	uiGun->vertexWk[0].tex = D3DXVECTOR2(startX, 0.0f);
	uiGun->vertexWk[1].tex = D3DXVECTOR2(startX + sizeX, 0.0f);
	uiGun->vertexWk[2].tex = D3DXVECTOR2(startX, 1.0f);
	uiGun->vertexWk[3].tex = D3DXVECTOR2(startX + sizeX, 1.0f);

	uiWin->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	uiWin->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	uiWin->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	uiWin->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	for (int i = 0; i < MAG_MAX; i++, uiBullet++)
	{
		float sizeX = 1.0f / GUN_MAX;

		uiBullet->vertexWk[0].tex = D3DXVECTOR2(uiBullet->type, 0.0f);
		uiBullet->vertexWk[1].tex = D3DXVECTOR2(uiBullet->type + sizeX, 0.0f);
		uiBullet->vertexWk[2].tex = D3DXVECTOR2(uiBullet->type, 1.0f);
		uiBullet->vertexWk[3].tex = D3DXVECTOR2(uiBullet->type + sizeX, 1.0f);
	}
}
//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexGun(int dir)
{
	GUN *gun = &gunWk;
	PLAYER *player = GetPlayer();
	D3DXVECTOR3 *posCamera = GetCameraPos();

	// 頂点座標の設定
	if (dir == DIR_LEFT)
	{
		gun->vertexWk[0].vtx.x = gun->pos.x + posCamera->x - cosf(gun->BaseAngle - gun->rot.z) * gun->Radius;
		gun->vertexWk[0].vtx.y = gun->pos.y + posCamera->y + sinf(gun->BaseAngle - gun->rot.z) * gun->Radius;
		gun->vertexWk[0].vtx.z = 0.0f;

		gun->vertexWk[1].vtx.x = gun->pos.x + posCamera->x + cosf(gun->BaseAngle + gun->rot.z) * gun->Radius;
		gun->vertexWk[1].vtx.y = gun->pos.y + posCamera->y + sinf(gun->BaseAngle + gun->rot.z) * gun->Radius;
		gun->vertexWk[1].vtx.z = 0.0f;

		gun->vertexWk[2].vtx.x = gun->pos.x + posCamera->x - cosf(gun->BaseAngle + gun->rot.z) * gun->Radius;
		gun->vertexWk[2].vtx.y = gun->pos.y + posCamera->y - sinf(gun->BaseAngle + gun->rot.z) * gun->Radius;
		gun->vertexWk[2].vtx.z = 0.0f;

		gun->vertexWk[3].vtx.x = gun->pos.x + posCamera->x + cosf(gun->BaseAngle - gun->rot.z) * gun->Radius;
		gun->vertexWk[3].vtx.y = gun->pos.y + posCamera->y - sinf(gun->BaseAngle - gun->rot.z) * gun->Radius;
		gun->vertexWk[3].vtx.z = 0.0f;
	}
	else
	{
		gun->vertexWk[0].vtx.x = gun->pos.x + posCamera->x - cosf(gun->BaseAngle + gun->rot.z) * gun->Radius;
		gun->vertexWk[0].vtx.y = gun->pos.y + posCamera->y - sinf(gun->BaseAngle + gun->rot.z) * gun->Radius;
		gun->vertexWk[0].vtx.z = 0.0f;

		gun->vertexWk[1].vtx.x = gun->pos.x + posCamera->x + cosf(gun->BaseAngle - gun->rot.z) * gun->Radius;
		gun->vertexWk[1].vtx.y = gun->pos.y + posCamera->y - sinf(gun->BaseAngle - gun->rot.z) * gun->Radius;
		gun->vertexWk[1].vtx.z = 0.0f;

		gun->vertexWk[2].vtx.x = gun->pos.x + posCamera->x - cosf(gun->BaseAngle - gun->rot.z) * gun->Radius;
		gun->vertexWk[2].vtx.y = gun->pos.y + posCamera->y + sinf(gun->BaseAngle - gun->rot.z) * gun->Radius;
		gun->vertexWk[2].vtx.z = 0.0f;

		gun->vertexWk[3].vtx.x = gun->pos.x + posCamera->x + cosf(gun->BaseAngle + gun->rot.z) * gun->Radius;
		gun->vertexWk[3].vtx.y = gun->pos.y + posCamera->y + sinf(gun->BaseAngle + gun->rot.z) * gun->Radius;
		gun->vertexWk[3].vtx.z = 0.0f;
	}

	gun->vertexWk[0].vtx += player->pos;
	gun->vertexWk[1].vtx += player->pos;
	gun->vertexWk[2].vtx += player->pos;
	gun->vertexWk[3].vtx += player->pos;
}

void SetVertexGunUI(void)
{
	UI_GUN *uiGun = &uiGunWk;
	UI_WINDOW *uiWin = &uiWinWk;
	UI_BULLET *uiBullet = uiBulletWk;

	// 頂点座標の設定
	uiGun->vertexWk[0].vtx.x = uiGun->pos.x;
	uiGun->vertexWk[0].vtx.y = uiGun->pos.y;
	uiGun->vertexWk[0].vtx.z = 0.0f;

	uiGun->vertexWk[1].vtx.x = uiGun->pos.x + TEXTURE_UI_GUN_SIZE;
	uiGun->vertexWk[1].vtx.y = uiGun->pos.y;
	uiGun->vertexWk[1].vtx.z = 0.0f;

	uiGun->vertexWk[2].vtx.x = uiGun->pos.x;
	uiGun->vertexWk[2].vtx.y = uiGun->pos.y + TEXTURE_UI_GUN_SIZE;
	uiGun->vertexWk[2].vtx.z = 0.0f;

	uiGun->vertexWk[3].vtx.x = uiGun->pos.x + TEXTURE_UI_GUN_SIZE;
	uiGun->vertexWk[3].vtx.y = uiGun->pos.y + TEXTURE_UI_GUN_SIZE;
	uiGun->vertexWk[3].vtx.z = 0.0f;

	// 頂点座標の設定
	uiWin->vertexWk[0].vtx.x = uiWin->pos.x;
	uiWin->vertexWk[0].vtx.y = uiWin->pos.y;
	uiWin->vertexWk[0].vtx.z = 0.0f;

	uiWin->vertexWk[1].vtx.x = uiWin->pos.x + TEXTURE_UI_WIN_SIZE;
	uiWin->vertexWk[1].vtx.y = uiWin->pos.y;
	uiWin->vertexWk[1].vtx.z = 0.0f;

	uiWin->vertexWk[2].vtx.x = uiWin->pos.x;
	uiWin->vertexWk[2].vtx.y = uiWin->pos.y + TEXTURE_UI_WIN_SIZE;
	uiWin->vertexWk[2].vtx.z = 0.0f;

	uiWin->vertexWk[3].vtx.x = uiWin->pos.x + TEXTURE_UI_WIN_SIZE;
	uiWin->vertexWk[3].vtx.y = uiWin->pos.y + TEXTURE_UI_WIN_SIZE;
	uiWin->vertexWk[3].vtx.z = 0.0f;

	for (int i = 0; i < MAG_MAX; i++, uiBullet++)
	{
		// 頂点座標の設定
		uiBullet->vertexWk[0].vtx.x = uiBullet->pos.x - cosf(uiBullet->BaseAngle + uiBullet->rot.z) * uiBullet->Radius;
		uiBullet->vertexWk[0].vtx.y = uiBullet->pos.y - sinf(uiBullet->BaseAngle + uiBullet->rot.z) * uiBullet->Radius;
		uiBullet->vertexWk[0].vtx.z = 0.0f;

		uiBullet->vertexWk[1].vtx.x = uiBullet->pos.x + cosf(uiBullet->BaseAngle - uiBullet->rot.z) * uiBullet->Radius;
		uiBullet->vertexWk[1].vtx.y = uiBullet->pos.y - sinf(uiBullet->BaseAngle - uiBullet->rot.z) * uiBullet->Radius;
		uiBullet->vertexWk[1].vtx.z = 0.0f;

		uiBullet->vertexWk[2].vtx.x = uiBullet->pos.x - cosf(uiBullet->BaseAngle - uiBullet->rot.z) * uiBullet->Radius;
		uiBullet->vertexWk[2].vtx.y = uiBullet->pos.y + sinf(uiBullet->BaseAngle - uiBullet->rot.z) * uiBullet->Radius;
		uiBullet->vertexWk[2].vtx.z = 0.0f;

		uiBullet->vertexWk[3].vtx.x = uiBullet->pos.x + cosf(uiBullet->BaseAngle + uiBullet->rot.z) * uiBullet->Radius;
		uiBullet->vertexWk[3].vtx.y = uiBullet->pos.y + sinf(uiBullet->BaseAngle + uiBullet->rot.z) * uiBullet->Radius;
		uiBullet->vertexWk[3].vtx.z = 0.0f;
	}
}

//=============================================================================
// 銃取得関数
//=============================================================================
GUN *GetGun()
{
	// 銃のアドレスを返す
	return(&gunWk);		
}

//=============================================================================
// 銃設定関数
//=============================================================================
void SetGun(int no)
{
	GUN *gun = &gunWk;
	UI_GUN *uiGun = &uiGunWk;
	PLAYER *player = GetPlayer();

	// 銃の設定
	if (g_bGun[no])
	{	// 所持している時
		gun->pos = D3DXVECTOR3(0.0f, TEXTURE_PLAYER_SIZE_Y / 4.0f, 0.0f);
		gun->type = no;
		gun->dirRot -= D3DX_PI;

		gun->subPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		switch (player->dir)

		{
		case DIR_RIGHT:	// 右
			if (gun->isLocked)
			{
				if (gun->type == GUN_SHOTGUN)
				{
					gun->subPos.x = -TEXTURE_PLAYER_SIZE_X / 8.0f;
					if (gun->subRot > 0)
					{
						gun->subPos.x += TEXTURE_PLAYER_SIZE_X / 8.0f;
					}
					else if (gun->subRot < 0)
					{
						gun->subPos.x -= TEXTURE_PLAYER_SIZE_X / 8.0f;
					}
				}
				else
				{
					gun->subPos.x = TEXTURE_PLAYER_SIZE_X / 4.0f;
					if (gun->subRot > 0)
					{
						gun->subPos.x += TEXTURE_PLAYER_SIZE_X / 4.0f;
					}
					else if (gun->subRot < 0)
					{
						gun->subPos.x -= TEXTURE_PLAYER_SIZE_X / 4.0f;
					}
				}
			}
			else
			{
				gun->subPos.x = TEXTURE_PLAYER_SIZE_X / 8.0f;
				if (gun->subRot > 0)
				{
					gun->subPos.x += TEXTURE_PLAYER_SIZE_X / 8.0f;
				}
				else if (gun->subRot < 0)
				{
					gun->subPos.x -= TEXTURE_PLAYER_SIZE_X / 8.0f;
				}
			}
			break;
		case DIR_DOWN:	// 下
			if (gun->isLocked)
			{
				if (gun->type == GUN_SHOTGUN)
				{
					gun->subPos.x = -TEXTURE_PLAYER_SIZE_X / 4.0f;
					gun->subPos.y = -TEXTURE_PLAYER_SIZE_Y / 8.0f;
				}
				else
				{
					gun->subPos.x = TEXTURE_PLAYER_SIZE_X / 8.0f;
					gun->subPos.y = TEXTURE_PLAYER_SIZE_Y / 8.0f * 3;
				}
			}
			else
			{
				gun->subPos.x = -TEXTURE_PLAYER_SIZE_X / 4.0f;
				gun->subPos.y = TEXTURE_PLAYER_SIZE_Y / 4.0f;
			}
			break;
		case DIR_LEFT:	// 左
			if (gun->isLocked)
			{
				if (gun->type == GUN_SHOTGUN)
				{
					gun->subPos.x = TEXTURE_PLAYER_SIZE_X / 8.0f;
					if (gun->subRot < 0)
					{
						gun->subPos.x -= TEXTURE_PLAYER_SIZE_X / 4.0f;
						gun->subPos.y -= TEXTURE_PLAYER_SIZE_Y / 4.0f;
					}
					else if (gun->subRot > 0)
					{
						gun->subPos.y += TEXTURE_PLAYER_SIZE_Y / 8.0f;
					}
				}
				else
				{
					gun->subPos.x = -TEXTURE_PLAYER_SIZE_X / 8.0f * 3;
					if (gun->subRot < 0)
					{
						gun->subPos.x -= TEXTURE_PLAYER_SIZE_X / 4.0f;
					}
					else if (gun->subRot > 0)
					{
						gun->subPos.x += TEXTURE_PLAYER_SIZE_X / 4.0f;
						gun->subPos.y -= TEXTURE_PLAYER_SIZE_Y / 4.0f;
					}
				}
			}
			else
			{
				gun->subPos.x = -TEXTURE_PLAYER_SIZE_X / 8.0f;
				if (gun->subRot < 0)
				{
					gun->subPos.x -= TEXTURE_PLAYER_SIZE_X / 8.0f;
				}
				else if (gun->subRot > 0)
				{
					gun->subPos.x += TEXTURE_PLAYER_SIZE_X / 8.0f;
					gun->subPos.y -= TEXTURE_PLAYER_SIZE_Y / 8.0f;
				}
			}
			break;
		case DIR_UP:	// 上
			if (gun->isLocked)
			{
				if (gun->type == GUN_SHOTGUN)
				{
					gun->subPos.x = TEXTURE_PLAYER_SIZE_X / 4.0f;
					gun->subPos.y = TEXTURE_PLAYER_SIZE_Y / 4.0f;
				}
				else
				{
					gun->subPos.x = -TEXTURE_PLAYER_SIZE_X / 8.0f;
					gun->subPos.y = -TEXTURE_PLAYER_SIZE_Y / 4.0f;
				}
			}
			else
			{
				gun->subPos.x = TEXTURE_PLAYER_SIZE_X / 4.0f;
				gun->subPos.y = 0.0f;
			}
			break;
		}

		uiGun->count++;

		MakeVertexGun();
		gun->Texture = g_pD3DTextureGun[no];	// テクスチャ情報を更新
	}
}

//=============================================================================
// 銃発射関数
//=============================================================================
void SetShot()
{
	GUN *gun = &gunWk;
	PLAYER *player = GetPlayer();

	D3DXVECTOR3 pos = player->pos;

	switch (gun->type)
	{
	case GUN_PISTOL:
		pos.x += -TEXTURE_PLAYER_SIZE_X * ((2 - player->dir) % 2) * 0.2 + TEXTURE_PLAYER_SIZE_X * cosf(GetGun()->rot.z) * 0.9;
		pos.y += TEXTURE_PLAYER_SIZE_Y * ((1 + player->dir) % 2) * 0.2 + TEXTURE_PLAYER_SIZE_Y * sinf(GetGun()->rot.z) * 0.9;
		SetBullet(pos + GetGun()->pos, GetGun()->rot.z, PLAYER_BULLET_PISTOL);
		break;
	case GUN_SHOTGUN:
		pos.x += -TEXTURE_PLAYER_SIZE_X * ((2 - player->dir) % 2) * 0.125 + TEXTURE_PLAYER_SIZE_X * cosf(GetGun()->rot.z) * 1.5;
		pos.y += TEXTURE_PLAYER_SIZE_Y * ((1 + player->dir) % 2) * 0.125 + TEXTURE_PLAYER_SIZE_Y * sinf(GetGun()->rot.z) * 1.5;
		for (int i = 0; i < SLUG_MAX; i++)
		{
			float subRot = D3DX_PI * 0.25 * (rand() % 360) / 360 - D3DX_PI * 0.125;
			SetBullet(pos + GetGun()->pos, GetGun()->rot.z + subRot, PLAYER_BULLET_SHOTGUN);
		}
		break;
	}
}