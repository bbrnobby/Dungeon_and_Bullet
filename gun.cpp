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

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define INTERVAL_PISTOL		(10)
#define INTERVAL_SHOTOGUN	(30)

const char *TEXTURE_GAME_GUN[] =
{
	"data/TEXTURE/gun001.png",
	"data/TEXTURE/gun002.png"
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexGun();
void SetTextureGun();
void SetVertexGun(int dir);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureGun[GUN_MAX] = {};	// テクスチャへのポリゴン
GUN						gunWk;							// 銃構造体
bool					g_bGun[GUN_MAX];				// 銃の所持フラグ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGun(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GUN *gun = &gunWk;							// 銃のポインター

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		for (int i = 0; i < GUN_MAX; i++)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
				TEXTURE_GAME_GUN[i],			// ファイルの名前
				&g_pD3DTextureGun[i]);			// 読み込むメモリのポインタ
		}
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

	MakeVertexGun();													// 頂点情報の作成

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
		if (g_pD3DTextureGun != NULL)
		{	// テクスチャの開放
			g_pD3DTextureGun[i]->Release();
			g_pD3DTextureGun[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGun(void)
{
	GUN *gun = &gunWk;				// 銃のポインタ
	PLAYER *player = GetPlayer();	// プレイヤーのポインタ

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
		gun->pos.y = (gun->subPos.y + player->subPosY + gun->pos.y * 2) / 3;

		SetVertexGun(player->dir);	// 移動後の座標で頂点を設定
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGun(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GUN *gun = &gunWk;

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
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexGun()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GUN *gun = &gunWk;

	// 頂点座標の設定
	SetVertexGun(GetPlayer()->dir);

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

	// テクスチャ座標の設定
	SetTextureGun();

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureGun()
{
	GUN *gun = &gunWk;

	// テクスチャ座標の設定
	gun->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f);
	gun->vertexWk[1].tex = D3DXVECTOR2( 1.0f, 0.0f);
	gun->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f);
	gun->vertexWk[3].tex = D3DXVECTOR2( 1.0f, 1.0f);
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
			float subRot = D3DX_PI * 0.33 * (rand() % 360) / 360 - D3DX_PI * 0.166;
			SetBullet(pos + GetGun()->pos, GetGun()->rot.z + subRot, PLAYER_BULLET_SHOTGUN);
		}
		break;
	}
}