//=============================================================================
//
// バレット処理 [bullet.cpp]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#include "main.h"
#include "bullet.h"
#include "sound.h"
#include "player.h"
#include "gun.h"
#include "dungeon.h"
#include "camera.h"
#include "particle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBullet( int no );
void SetTextureBullet( int no, int cntPattern );
void SetVertexBullet( int no );

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBullet = NULL;		// テクスチャへのポリゴン

BULLET					bulletWk[BULLET_MAX];			// バレット構造体

LPDIRECTSOUNDBUFFER8	g_pSE;							// SE用バッファ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBullet(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLET *bullet = bulletWk;			// エネミーのポインターを初期化

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
			TEXTURE_GAME_BULLET,			// ファイルの名前
			&g_pD3DTextureBullet);			// 読み込むメモリのポインタ
		
		g_pSE = LoadSound(SE_BOMB);
	}

	// バレットの初期化処理
	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{
		bullet->use = false;									// 未使用（発射されていない弾）
		bullet->pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);			// 座標データを初期化
		bullet->vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ベクトルデータを初期化
		bullet->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 回転データを初期化
		bullet->PatternAnim = 0;

		bullet->spin = false;
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_BULLET_SIZE, TEXTURE_BULLET_SIZE);
		bullet->Radius = D3DXVec2Length(&temp);					// エネミーの半径を初期化
		bullet->BaseAngle = atan2f(TEXTURE_BULLET_SIZE, TEXTURE_BULLET_SIZE);	// エネミーの角度を初期化

		bullet->Texture = g_pD3DTextureBullet;					// テクスチャ情報
		MakeVertexBullet(i);									// 頂点情報の作成
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBullet(void)
{
	if (g_pD3DTextureBullet != NULL)
	{	// テクスチャの開放
		g_pD3DTextureBullet->Release();
		g_pD3DTextureBullet = NULL;
	}

	if (g_pSE != NULL)
	{	// サウンドの開放
		g_pSE->Release();
		g_pSE = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBullet(void)
{
	BULLET *bullet = bulletWk;				// バレットのポインターを初期化
	
	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{
		if (bullet->use)			// 使用している状態なら更新する
		{
			// 座標の更新
			bullet->pos += bullet->vec;

			bullet->duration--;
			if (bullet->duration == 0 || (
				GetMapByPos(bullet->pos.x - TEXTURE_BULLET_SIZE * 1.5, bullet->pos.y - TEXTURE_BULLET_SIZE * 2) != MAP_ROOM &&
				GetMapByPos(bullet->pos.x + TEXTURE_BULLET_SIZE * 1.5, bullet->pos.y - TEXTURE_BULLET_SIZE * 2) != MAP_ROOM &&
				GetMapByPos(bullet->pos.x - TEXTURE_BULLET_SIZE * 1.5, bullet->pos.y + TEXTURE_BULLET_SIZE * 1) != MAP_ROOM &&
				GetMapByPos(bullet->pos.x + TEXTURE_BULLET_SIZE * 1.5, bullet->pos.y + TEXTURE_BULLET_SIZE * 1) != MAP_ROOM))
			{
				bullet->use = false;
				bullet->pos.x = -100.0f;
				bullet->vec.x = 0.0f;
				bullet->vec.y = 0.0f;
				continue;
			}

			if (bullet->spin) bullet->rot.z += BULLET_SPINSPEED_001;

			SetVertexBullet(i);				// 移動後の座標で頂点を設定
			switch(bullet->shooter)
			{
			case PLAYER_BULLET_PISTOL:
				SetParticleLine(bullet->pos, bullet->vec, D3DCOLOR_RGBA(255, 128, 0, 255), TEXTURE_BULLET_SIZE, TEXTURE_BULLET_SIZE, PARTICLE_LIFE);
				break;
			case ENEMY_BULLET:
				SetParticleLine(bullet->pos, bullet->vec, D3DCOLOR_RGBA(255, 0, 0, 255), TEXTURE_BULLET_SIZE, TEXTURE_BULLET_SIZE, PARTICLE_LIFE);
				break;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLET *bullet = bulletWk;				// バレットのポインターを初期化

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{
		if (bullet->use)			// 使用している状態なら更新する
		{
			// テクスチャの設定
			pDevice->SetTexture(0, bullet->Texture);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, bullet->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexBullet( int no )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLET *bullet = &bulletWk[no];			// バレットのポインターを初期化

	// 頂点座標の設定
	SetVertexBullet( no );

	// rhwの設定
	bullet->vertexWk[0].rhw =
	bullet->vertexWk[1].rhw =
	bullet->vertexWk[2].rhw =
	bullet->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	bullet->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	bullet->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	bullet->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	bullet->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	bullet->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	bullet->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_BULLET, 0.0f);
	bullet->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BULLET);
	bullet->vertexWk[3].tex = D3DXVECTOR2(1.0f/ TEXTURE_PATTERN_DIVIDE_X_BULLET, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_BULLET);

	//bullet->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//bullet->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//bullet->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//bullet->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureBullet( int no, int cntPattern )
{
	BULLET *bullet = &bulletWk[no];			// バレットのポインターを初期化

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_BULLET;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_BULLET;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_BULLET;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BULLET;
	
	bullet->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	bullet->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	bullet->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	bullet->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexBullet( int no )
{
	BULLET *bullet = &bulletWk[no];
	D3DXVECTOR3 *posCamera = GetCameraPos();

	// 頂点座標の設定
	bullet->vertexWk[0].vtx.x = bullet->pos.x + posCamera->x - cosf(bullet->BaseAngle + bullet->rot.z) * bullet->Radius;
	bullet->vertexWk[0].vtx.y = bullet->pos.y + posCamera->y - sinf(bullet->BaseAngle + bullet->rot.z) * bullet->Radius;
	bullet->vertexWk[0].vtx.z = 0.0f;

	bullet->vertexWk[1].vtx.x = bullet->pos.x + posCamera->x + cosf(bullet->BaseAngle - bullet->rot.z) * bullet->Radius;
	bullet->vertexWk[1].vtx.y = bullet->pos.y + posCamera->y - sinf(bullet->BaseAngle - bullet->rot.z) * bullet->Radius;
	bullet->vertexWk[1].vtx.z = 0.0f;

	bullet->vertexWk[2].vtx.x = bullet->pos.x + posCamera->x - cosf(bullet->BaseAngle - bullet->rot.z) * bullet->Radius;
	bullet->vertexWk[2].vtx.y = bullet->pos.y + posCamera->y + sinf(bullet->BaseAngle - bullet->rot.z) * bullet->Radius;
	bullet->vertexWk[2].vtx.z = 0.0f;

	bullet->vertexWk[3].vtx.x = bullet->pos.x + posCamera->x + cosf(bullet->BaseAngle + bullet->rot.z) * bullet->Radius;
	bullet->vertexWk[3].vtx.y = bullet->pos.y + posCamera->y + sinf(bullet->BaseAngle + bullet->rot.z) * bullet->Radius;
	bullet->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// バレットの発射設定
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, float rot, int shooter, int duration)
{
	BULLET *bullet = bulletWk;				// バレットのポインターを初期化

	// もし未使用の弾が無かったら発射しない( = これ以上撃てないって事 )
	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{
		if (!bullet->use)					// 未使用状態のバレットを見つける
		{
			bullet->use = true;				// 使用状態へ変更する
			bullet->shooter = shooter;
			bullet->pos = pos;				// 座標をセット
			bullet->rot.z = 0.0f;

			// バレットの移動処理
			if (shooter == PLAYER_BULLET_PISTOL)
			{
				bullet->spin = true;
				bullet->PatternAnim = 1;
				bullet->duration = duration;

				bullet->vec.x = cosf(rot) * BULLET_SPEED_001;
				bullet->vec.y = sinf(rot) * BULLET_SPEED_001;
			}
			else if(shooter == ENEMY_BULLET)
			{
				bullet->spin = false;
				bullet->PatternAnim = 2;
				bullet->duration = duration;

				bullet->vec.x = cosf(rot) * BULLET_SPEED_ENEMY;
				bullet->vec.y = sinf(rot) * BULLET_SPEED_ENEMY;

			}
			SetTextureBullet(i, bullet->PatternAnim);

			// 発射音再生
			g_pSE -> SetCurrentPosition(0);
			PlaySound(g_pSE, E_DS8_FLAG_NONE);

			return;							// 1発セットしたので終了する
		}
	}
}

//=============================================================================
// バレット取得関数
//=============================================================================
BULLET *GetBullet(int no)
{
	return(&bulletWk[no]);
}

