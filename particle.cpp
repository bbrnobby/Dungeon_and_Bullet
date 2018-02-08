//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#include "particle.h"
#include "input.h"
#include "camera.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_PARTICLE	"data/TEXTURE/effect000.jpg"	// 読み込むテクスチャファイル名
#define	PARTICLE_SIZE_X		(50.0f)							// ビルボードの幅
#define	PARTICLE_SIZE_Y		(50.0f)							// ビルボードの高さ
#define	VALUE_MOVE_PARTICLE	(2.0f)							// 移動速度

#define	PARTICLE_MAX		(10000)							// パーティクル最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexParticle(int no);
void SetVertexParticle(int no);
void SetColorParticle(int no, D3DXCOLOR col);
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float sizeX, float sizeY, int life);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureParticle = NULL;	// テクスチャへのポインタ
PARTICLE				particleWk[PARTICLE_MAX];		// パーティクルワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitParticle(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PARTICLE *particle = particleWk;

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_PARTICLE,					// ファイルの名前
			&g_pD3DTextureParticle);			// 読み込むメモリー
	}

	for(int i = 0; i < PARTICLE_MAX; i++, particle++)
	{
		particle->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		particle->vec = D3DXVECTOR3(PARTICLE_SIZE_X, PARTICLE_SIZE_Y, 1.0f);
		particle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		particle->scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		particle->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		particle->life = 0;
		particle->use = false;
		particle->gravity = false;

		// 頂点情報の作成
		MakeVertexParticle(i);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitParticle(void)
{
	if(g_pD3DTextureParticle != NULL)
	{// テクスチャの開放
		g_pD3DTextureParticle->Release();
		g_pD3DTextureParticle = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateParticle(void)
{
	PARTICLE *particle = particleWk;

	for (int i = 0; i < PARTICLE_MAX; i++, particle++)
	{
		if (particle->use)
		{// 使用中
			//particle->vec -= particle->vec * 0.25f;
			particle->pos += particle->vec;

			// 頂点座標の設定
			SetVertexParticle(i);

			particle->life--;
			if (particle->life <= 0)
			{
				particle->use = false;
			}
			else
			{
				if (particle->life <= PARTICLE_LIFE)
				{
					particle->col.r *= (float)(particle->life) / PARTICLE_LIFE;
					particle->col.g *= (float)(particle->life) / PARTICLE_LIFE;
					particle->col.b *= (float)(particle->life) / PARTICLE_LIFE;
				}

				if (particle->life <= PARTICLE_LIFE / 5)
				{
					// α値設定
					particle->col.a -= 0.05f;
					if (particle->col.a < 0.0f)
					{
						particle->col.a = 0.0f;
					}
				}

				particle->scale *= (float) particle->life / (particle->life + 1);

				// 色の設定
				SetColorParticle(i, particle->col);
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PARTICLE *particle = particleWk;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ライティングを無効に
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 加算合成に設定
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for(int i = 0; i < PARTICLE_MAX; i++, particle++)
	{
		if(particle->use)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTextureParticle);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, particle->vertexWk, sizeof(VERTEX_2D));
		}
	}

	// ライティングを有効に
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 通常ブレンドに戻す
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexParticle(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PARTICLE *particle = &particleWk[no];			// パーティクルのポインタ

	// 頂点座標の設定
	SetVertexParticle(no);

	// rhwの設定
	particle->vertexWk[0].rhw =
	particle->vertexWk[1].rhw =
	particle->vertexWk[2].rhw =
	particle->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	particle->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	particle->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	particle->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	particle->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	particle->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	particle->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	particle->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	particle->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexParticle(int no)
{
	PARTICLE *particle = &particleWk[no];
	D3DXVECTOR3 *posCamera = GetCameraPos();

	// 頂点座標の設定
	particle->vertexWk[0].vtx.x = -particle->scale.x / 2 + particle->pos.x + posCamera->x;
	particle->vertexWk[0].vtx.y = -particle->scale.y / 2 + particle->pos.y + posCamera->y;
	particle->vertexWk[0].vtx.z = 0.0f;

	particle->vertexWk[1].vtx.x = particle->scale.x / 2 + particle->pos.x + posCamera->x;
	particle->vertexWk[1].vtx.y = -particle->scale.y / 2 + particle->pos.y + posCamera->y;
	particle->vertexWk[1].vtx.z = 0.0f;

	particle->vertexWk[2].vtx.x = -particle->scale.x / 2 + particle->pos.x + posCamera->x;
	particle->vertexWk[2].vtx.y = particle->scale.y / 2 + particle->pos.y + posCamera->y;
	particle->vertexWk[2].vtx.z = 0.0f;

	particle->vertexWk[3].vtx.x = particle->scale.x / 2 + particle->pos.x + posCamera->x;
	particle->vertexWk[3].vtx.y = particle->scale.y / 2 + particle->pos.y + posCamera->y;
	particle->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorParticle(int no, D3DXCOLOR col)
{
	PARTICLE *particle = &particleWk[no];

	// 反射光の設定
	particle->vertexWk[0].diffuse = col;
	particle->vertexWk[1].diffuse = col;
	particle->vertexWk[2].diffuse = col;
	particle->vertexWk[3].diffuse = col;
}

//=============================================================================
// パーティクルラインの作成
//=============================================================================
void SetParticleLine(D3DXVECTOR3 pos, D3DXVECTOR3 vec, D3DXCOLOR col, float sizeX, float sizeY, int life)
{
	if (sizeX*sizeX * 0.25f < vec.x*vec.x + vec.y*vec.y || sizeY*sizeY * 0.25f < vec.x*vec.x + vec.y*vec.y)
	{
		SetParticleLine(pos, vec / 2.0f, col, sizeX, sizeY, life - 1);
		SetParticleLine(pos - vec / 2.0f, vec / 2.0f, col, sizeX, sizeY, life - 2);
	}
	else
	{
		SetParticle(pos, col, sizeX, sizeY, life);
	}
}

//=============================================================================
// パーティクルの作成
//=============================================================================
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float sizeX, float sizeY, int life)
{
	PARTICLE *particle = particleWk;

	for (int i = 0; i < PARTICLE_MAX; i++, particle++)
	{
		if (!particle->use)
		{
			particle->pos = pos;
			particle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			particle->scale = D3DXVECTOR3(sizeX, sizeY, 1.0f);
			particle->vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			particle->col = col;
			particle->life = life;
			particle->use = true;

			// 頂点座標の設定
			SetVertexParticle(i);

			// 色の設定
			SetColorParticle(i, col);

			return;
		}
	}
}

//=============================================================================
// パーティクル取得関数
//=============================================================================
PARTICLE *GetParticle(int no)
{
	return(&particleWk[no]);
}