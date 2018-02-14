//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "camera.h"
#include "pointer.h"
#include "particle.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_GAME_ENEMY		_T("data/TEXTURE/enemy001.png")	// 画像

#define ENEMY_TEXTURE_PATTERN_DIVIDE_X	(3)		// アニメパターンのテクスチャ内分割数（X)
#define ENEMY_TEXTURE_PATTERN_DIVIDE_Y	(8)		// アニメパターンのテクスチャ内分割数（Y)
#define ENEMY_ANIM_PATTERN_NUM			(ENEMY_TEXTURE_PATTERN_DIVIDE_X)	// アニメーションパターン数
#define ENEMY_TIME_ANIMATION			(10)	// アニメーションの切り替わるカウント

#define ENEMY_INTERVAL_WAIT				(30)	// 待機インターバル
#define ENEMY_INTERVAL_DAMAGE			(30)	// ダメージインターバル
#define ENEMY_INTERVAL_MOVE				(90)	// 移動インターバル
#define ENEMY_INTERVAL_ATTACK			(90)	// 攻撃インターバル
#define ENEMY_INTERVAL_FADE				(60)	// フェードインターバル
#define ENEMY_INTERVAL_DEAD				(120)	// 死亡インターバル

#define ENEMY_HP_001					(3)		// エネミー1の体力
//#define ENEMY_SCORE_001					(100)	// エネミー1のスコア
#define ENEMY_SPEED_001					(3.0f)	// エネミー1の速度
#define ENEMY_FRICTION_001				(0.85f)	// エネミー1の慣性

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEnemy( int no );
void SetTextureEnemy( int no, int cntPattern );
void SetVertexEnemy( int no );
void SetDiffuseEnemy(int no);
void MoveEnemy(ENEMY *enemy);
void AttackEnemy(ENEMY *enemy);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEnemy = NULL;						// テクスチャへのポリゴン
VERTEX_2D				g_vertexWkHPEnemy[ENEMY_MAX][NUM_VERTEX];		// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkMaxHPEnemy[ENEMY_MAX][NUM_VERTEX];	// 頂点情報格納ワーク

ENEMY					enemyWk[ENEMY_MAX];								// エネミー構造体

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = enemyWk;					// エネミーのポインタ

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
			TEXTURE_GAME_ENEMY,				// ファイルの名前
			&g_pD3DTextureEnemy);			// 読み込むメモリのポインタ
	}

	// エネミーの初期化処理
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		enemy->use = false;										// 使用
		enemy->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 座標データを初期化
		enemy->subRot = 0.0f;
		enemy->vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ベクトルデータを初期化
		enemy->subVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 回転データを初期化
		enemy->dir = 0;
		enemy->PatternAnim = 0;									// アニメパターン番号を初期化
		enemy->CountAnim = 0;									// アニメカウントを初期化
		enemy->notice = false;
		enemy->roomID = -1;
		enemy->type = 0;
		enemy->state = ENEMY_DEFAULT_STATE;
		enemy->interval = 0;
		enemy->hp = 0;

		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_ENEMY_SIZE_X, TEXTURE_ENEMY_SIZE_Y);
		enemy->Radius = D3DXVec2Length(&temp);					// エネミーの半径を初期化
		enemy->BaseAngle = atan2f(TEXTURE_ENEMY_SIZE_Y, TEXTURE_ENEMY_SIZE_X);	// エネミーの角度を初期化

		enemy->Texture = g_pD3DTextureEnemy;					// テクスチャ情報
		MakeVertexEnemy(i);										// 頂点情報の作成
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	if (g_pD3DTextureEnemy != NULL)
	{	// テクスチャの開放
		g_pD3DTextureEnemy->Release();
		g_pD3DTextureEnemy = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	ENEMY *enemy = enemyWk;		// エネミーのポインタ
	
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->state != ENEMY_DEFAULT_STATE)	
		{	// 出現している状態なら更新する
			if ((enemy->CountAnim % ENEMY_TIME_ANIMATION) == 0)
			{
				// パターンの切り替え
				enemy->PatternAnim = (enemy->PatternAnim + 1) % ENEMY_ANIM_PATTERN_NUM;
				enemy->CountAnim = 0;

				// テクスチャ座標を設定
				if (enemy->state == ENEMY_DAMAGE || enemy->state == ENEMY_DEAD)
				{
					SetTextureEnemy(i, enemy->PatternAnim + (enemy->dir + ENEMY_TEXTURE_PATTERN_DIVIDE_Y / 2) * ENEMY_ANIM_PATTERN_NUM);
				}
				else
				{
					SetTextureEnemy(i, enemy->PatternAnim + enemy->dir * ENEMY_ANIM_PATTERN_NUM);
				}
			}
			enemy->CountAnim++;

			switch (enemy->state)
			{
			case ENEMY_DAMAGE:
			case ENEMY_WAIT:
			case ENEMY_ATTACK:
				enemy->vec *= ENEMY_FRICTION_001;
			case ENEMY_MOVE:
				enemy->subVec *= ENEMY_FRICTION_001;
			default:
				// 座標の更新
				if (GetMapByPos(enemy->pos.x + enemy->vec.x, enemy->pos.y) != MAP_ROOM)
				{
					enemy->vec.x = 0.0f;
				}
				if (GetMapByPos(enemy->pos.x, enemy->pos.y + enemy->vec.y) != MAP_ROOM)
				{
					enemy->vec.y = 0.0f;
				}

				if (GetMapByPos(enemy->pos.x + enemy->subVec.x, enemy->pos.y) != MAP_ROOM)
				{
					enemy->subVec.x = 0.0f;
				}
				if (GetMapByPos(enemy->pos.x, enemy->pos.y + enemy->subVec.y) != MAP_ROOM)
				{
					enemy->subVec.y = 0.0f;
				}
				break;
			case ENEMY_DEAD:
				enemy->rot.z += ENEMY_KNOCKBACK_SPEED;
				SetParticleLine(enemy->pos, enemy->subVec, D3DCOLOR_RGBA(128, 128, 255, 255), TEXTURE_ENEMY_SIZE_X, TEXTURE_ENEMY_SIZE_Y, PARTICLE_LIFE);
				break;
			case ENEMY_SPAWN:
			case ENEMY_DESPAWN:
				SetDiffuseEnemy(i);
				break;
			}

			enemy->subRot = ((enemy->PatternAnim % 3 - 1) * 0.5f + enemy->subRot * 3) / 4;

			enemy->rot.z = ((enemy->PatternAnim % 3 - 1) * 0.25f + enemy->rot.z * 30) / 31;

			enemy->pos.x += enemy->vec.x + enemy->subVec.x;
			enemy->pos.y += enemy->vec.y + enemy->subVec.y + enemy->subRot;

			enemy->interval--;
			if (enemy->interval <= 0)
			{
				switch (enemy->state)
				{
				case ENEMY_MOVE:
					SetState(enemy, ENEMY_WAIT);
					break;
				case ENEMY_DEAD:
				case ENEMY_DESPAWN:
					enemy->state = ENEMY_DEFAULT_STATE;
					break;
				default:
					SetState(enemy, rand() % ENEMY_DEFAULT_STATE);
					break;
				}
			}
			SetVertexEnemy(i);	// 移動後の座標で頂点を設定
#ifdef _DEBUG
			PrintDebugProc("ENEMEY[%d] X:%f Y:%f ROOM:%d STATE:%d\n", i, enemy->pos.x, enemy->pos.y, enemy->roomID, enemy->state);
#endif
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = enemyWk;			// エネミーのポインタ

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->state != ENEMY_DEFAULT_STATE)
		{	// 出現している状態なら描画する
			// テクスチャの設定
			pDevice->SetTexture(0, enemy->Texture);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, enemy->vertexWk, sizeof(VERTEX_2D));

			if (enemy->state < ENEMY_SPAWN)
			{
				// テクスチャの設定
				pDevice->SetTexture(0, NULL);

				// ポリゴンの描画
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkMaxHPEnemy[i], sizeof(VERTEX_2D));

				// ポリゴンの描画
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkHPEnemy[i], sizeof(VERTEX_2D));
			}
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexEnemy( int no )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = &enemyWk[no];

	// 頂点座標の設定
	SetVertexEnemy( no );

	// rhwの設定
	enemy->vertexWk[0].rhw =
	enemy->vertexWk[1].rhw =
	enemy->vertexWk[2].rhw =
	enemy->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	enemy->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	enemy->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	enemy->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	enemy->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	enemy->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	enemy->vertexWk[1].tex = D3DXVECTOR2(1.0f / ENEMY_TEXTURE_PATTERN_DIVIDE_X, 0.0f);
	enemy->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / ENEMY_TEXTURE_PATTERN_DIVIDE_Y);
	enemy->vertexWk[3].tex = D3DXVECTOR2( 1.0f/ ENEMY_TEXTURE_PATTERN_DIVIDE_X, 1.0f/ENEMY_TEXTURE_PATTERN_DIVIDE_Y);

	// rhwの設定
	g_vertexWkHPEnemy[no][0].rhw =
	g_vertexWkHPEnemy[no][1].rhw =
	g_vertexWkHPEnemy[no][2].rhw =
	g_vertexWkHPEnemy[no][3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkHPEnemy[no][0].diffuse = D3DCOLOR_RGBA(0, 255, 0, 255);
	g_vertexWkHPEnemy[no][1].diffuse = D3DCOLOR_RGBA(0, 255, 0, 255);
	g_vertexWkHPEnemy[no][2].diffuse = D3DCOLOR_RGBA(0, 255, 0, 255);
	g_vertexWkHPEnemy[no][3].diffuse = D3DCOLOR_RGBA(0, 255, 0, 255);

	// rhwの設定
	g_vertexWkMaxHPEnemy[no][0].rhw =
	g_vertexWkMaxHPEnemy[no][1].rhw =
	g_vertexWkMaxHPEnemy[no][2].rhw =
	g_vertexWkMaxHPEnemy[no][3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkMaxHPEnemy[no][0].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
	g_vertexWkMaxHPEnemy[no][1].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
	g_vertexWkMaxHPEnemy[no][2].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
	g_vertexWkMaxHPEnemy[no][3].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureEnemy( int no, int cntPattern )
{
	ENEMY *enemy = &enemyWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % ENEMY_TEXTURE_PATTERN_DIVIDE_X;
	int y = cntPattern / ENEMY_TEXTURE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / ENEMY_TEXTURE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / ENEMY_TEXTURE_PATTERN_DIVIDE_Y;
	
	enemy->vertexWk[0].tex = D3DXVECTOR2( x * sizeX, y * sizeY );
	enemy->vertexWk[1].tex = D3DXVECTOR2( (x + 1) * sizeX, y * sizeY );
	enemy->vertexWk[2].tex = D3DXVECTOR2( x * sizeX, (y + 1) * sizeY );
	enemy->vertexWk[3].tex = D3DXVECTOR2( (x + 1) * sizeX, (y + 1) * sizeY );
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexEnemy( int no )
{
	ENEMY *enemy = &enemyWk[no];
	D3DXVECTOR3 *posCamera = GetCameraPos();

	// 頂点座標の設定
	enemy->vertexWk[0].vtx.x = enemy->pos.x + posCamera->x - cosf(enemy->BaseAngle + enemy->rot.z) * enemy->Radius;
	enemy->vertexWk[0].vtx.y = enemy->pos.y + posCamera->y - sinf(enemy->BaseAngle + enemy->rot.z) * enemy->Radius;
	enemy->vertexWk[0].vtx.z = 0.0f;

	enemy->vertexWk[1].vtx.x = enemy->pos.x + posCamera->x + cosf(enemy->BaseAngle - enemy->rot.z) * enemy->Radius;
	enemy->vertexWk[1].vtx.y = enemy->pos.y + posCamera->y - sinf(enemy->BaseAngle - enemy->rot.z) * enemy->Radius;
	enemy->vertexWk[1].vtx.z = 0.0f;

	enemy->vertexWk[2].vtx.x = enemy->pos.x + posCamera->x - cosf(enemy->BaseAngle - enemy->rot.z) * enemy->Radius;
	enemy->vertexWk[2].vtx.y = enemy->pos.y + posCamera->y + sinf(enemy->BaseAngle - enemy->rot.z) * enemy->Radius;
	enemy->vertexWk[2].vtx.z = 0.0f;

	enemy->vertexWk[3].vtx.x = enemy->pos.x + posCamera->x + cosf(enemy->BaseAngle + enemy->rot.z) * enemy->Radius;
	enemy->vertexWk[3].vtx.y = enemy->pos.y + posCamera->y + sinf(enemy->BaseAngle + enemy->rot.z) * enemy->Radius;
	enemy->vertexWk[3].vtx.z = 0.0f;

	// 頂点座標の設定
	g_vertexWkHPEnemy[no][0].vtx.x = enemy->pos.x + posCamera->x - TEXTURE_ENEMY_SIZE_X;
	g_vertexWkHPEnemy[no][0].vtx.y = enemy->pos.y + posCamera->y - TEXTURE_ENEMY_SIZE_Y;
	g_vertexWkHPEnemy[no][0].vtx.z = 0.0f;

	g_vertexWkHPEnemy[no][1].vtx.x = enemy->pos.x + posCamera->x + TEXTURE_ENEMY_SIZE_X * (1.0 - 2.0 * (1.0 - (float)enemy->hp / enemy->maxhp));
	g_vertexWkHPEnemy[no][1].vtx.y = enemy->pos.y + posCamera->y - TEXTURE_ENEMY_SIZE_Y;
	g_vertexWkHPEnemy[no][1].vtx.z = 0.0f;

	g_vertexWkHPEnemy[no][2].vtx.x = enemy->pos.x + posCamera->x - TEXTURE_ENEMY_SIZE_X;
	g_vertexWkHPEnemy[no][2].vtx.y = enemy->pos.y + posCamera->y - TEXTURE_ENEMY_SIZE_Y + 5;
	g_vertexWkHPEnemy[no][2].vtx.z = 0.0f;

	g_vertexWkHPEnemy[no][3].vtx.x = enemy->pos.x + posCamera->x + TEXTURE_ENEMY_SIZE_X * (1.0 - 2.0 * (1.0 - (float)enemy->hp / enemy->maxhp));
	g_vertexWkHPEnemy[no][3].vtx.y = enemy->pos.y + posCamera->y - TEXTURE_ENEMY_SIZE_Y + 5;
	g_vertexWkHPEnemy[no][3].vtx.z = 0.0f;

	// 頂点座標の設定
	g_vertexWkMaxHPEnemy[no][0].vtx.x = enemy->pos.x + posCamera->x - TEXTURE_ENEMY_SIZE_X;
	g_vertexWkMaxHPEnemy[no][0].vtx.y = enemy->pos.y + posCamera->y - TEXTURE_ENEMY_SIZE_Y;
	g_vertexWkMaxHPEnemy[no][0].vtx.z = 0.0f;

	g_vertexWkMaxHPEnemy[no][1].vtx.x = enemy->pos.x + posCamera->x + TEXTURE_ENEMY_SIZE_X;
	g_vertexWkMaxHPEnemy[no][1].vtx.y = enemy->pos.y + posCamera->y - TEXTURE_ENEMY_SIZE_Y;
	g_vertexWkMaxHPEnemy[no][1].vtx.z = 0.0f;

	g_vertexWkMaxHPEnemy[no][2].vtx.x = enemy->pos.x + posCamera->x - TEXTURE_ENEMY_SIZE_X;
	g_vertexWkMaxHPEnemy[no][2].vtx.y = enemy->pos.y + posCamera->y - TEXTURE_ENEMY_SIZE_Y + 5;
	g_vertexWkMaxHPEnemy[no][2].vtx.z = 0.0f;

	g_vertexWkMaxHPEnemy[no][3].vtx.x = enemy->pos.x + posCamera->x + TEXTURE_ENEMY_SIZE_X;
	g_vertexWkMaxHPEnemy[no][3].vtx.y = enemy->pos.y + posCamera->y - TEXTURE_ENEMY_SIZE_Y + 5;
	g_vertexWkMaxHPEnemy[no][3].vtx.z = 0.0f;

}

void SetDiffuseEnemy(int no)
{
	ENEMY *enemy = &enemyWk[no];

	int fade;

	if (enemy->state < ENEMY_DEFAULT_STATE)
	{
		fade = 255;
	}
	else if (enemy->state > ENEMY_SPAWN)
	{
		fade = 255 * enemy->interval / ENEMY_INTERVAL_FADE;
	}
	else
	{
		fade = 255 * (ENEMY_INTERVAL_FADE - enemy->interval) / ENEMY_INTERVAL_FADE;
	}

	// 反射光の設定
	enemy->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, fade);
	enemy->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, fade);
	enemy->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, fade);
	enemy->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, fade);
}

//=============================================================================
// エネミー取得関数
//=============================================================================
ENEMY *GetEnemy(int no)
{
	return(&enemyWk[no]);
}

//=============================================================================
// エネミー設定関数
//=============================================================================
void SetEnemy(int ID)
{
	ENEMY *enemy = enemyWk;

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (!enemy->use)
		{
			enemy->use = true;
			enemy->type = 0;
			enemy->roomID = ID;

			return;			// 1体セットしたので終了する
		}
	}
}

//=============================================================================
// エネミー出現関数
//=============================================================================
bool SpawnEnemy(float x, float y, int ID)
{
	ENEMY *enemy = enemyWk;

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->use && enemy->roomID == ID 
			&& (enemy->state == ENEMY_DEFAULT_STATE || enemy->state == ENEMY_DESPAWN))
		{
			if (enemy->state == ENEMY_DEFAULT_STATE)
			{
				enemy->pos.x = x;
				enemy->pos.y = y;

				enemy->notice = false;

				enemy->PatternAnim = rand() % ENEMY_ANIM_PATTERN_NUM;
				enemy->CountAnim = rand() % ENEMY_TIME_ANIMATION;
				enemy->dir = rand() % 4;
				enemy->rot.z = 0.0f;

				switch (enemy->type)
				{
				case 0:
					enemy->hp = enemy->maxhp = ENEMY_HP_001;
					//enemy->score = ENEMY_SCORE_001;
					break;
				}
			}

			enemy->vec.x = 0.0f;
			enemy->vec.y = 0.0f;
			enemy->subVec.x = 0.0f;
			enemy->subVec.y = 0.0f;

			SetState(enemy, ENEMY_SPAWN);

			SetVertexEnemy(i);		// 移動後の座標で頂点を設定
			SetDiffuseEnemy(i);
			SetPointer(i);			// ポインターを設定
			return true;			// 1体セットしたので終了する
		}
	}

	return false;
}

//=============================================================================
// エネミー消滅関数
//=============================================================================
void DeSpawnEnemy(int ID)
{
	ENEMY *enemy = enemyWk;

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->use && enemy->roomID == ID && enemy->state < ENEMY_DESPAWN)
		{
			SetState(enemy, ENEMY_DESPAWN);

			return;			// 1体セットしたので終了する
		}
	}

	return;
}

//=============================================================================
// エネミー削除関数
//=============================================================================
void DeleteEnemy(ENEMY *enemy)
{
	ROOM *room = GetRoom(enemy->roomID);

	SetState(enemy, ENEMY_DEAD);
	enemy->use = false;

	room->cntEnemy--;
	room->spawnEnemy--;

	if (room->cntEnemy > 0)
	{
		float Ex, Ey;
		float Px, Py;

		Px = GetPlayer()->pos.x;
		Py = GetPlayer()->pos.y;
		do
		{
			Ex = (room->baseX + 2 + (room->width - 4) * (rand() % 100 / 100.0f)) * TEXTURE_DUNGEON_SIZE_X;
			Ey = (room->baseY + 2 + (room->height - 3) * (rand() % 100 / 100.0f)) * TEXTURE_DUNGEON_SIZE_Y;
		} while ((Ex - Px)*(Ex - Px) + (Ey - Py)*(Ey - Py) < SPAWN_DISTANCE * SPAWN_DISTANCE);
		if (SpawnEnemy(Ex, Ey, enemy->roomID))
		{
			room->spawnEnemy++;
		}
	}
	else
	{
		ClearRoom(enemy->roomID);
	}
}

//=============================================================================
// エネミー状態設定関数
//=============================================================================
void SetState(ENEMY *enemy, int state)
{
	if (enemy->use)
	{
		switch (state)
		{
		case ENEMY_WAIT:
			enemy->interval = ENEMY_INTERVAL_WAIT;
			break;
		case ENEMY_DAMAGE:
			enemy->interval = ENEMY_INTERVAL_DAMAGE;
			break;
		case ENEMY_MOVE:
			MoveEnemy(enemy);
			enemy->interval = rand() % ENEMY_INTERVAL_MOVE;
			break;
		case ENEMY_ATTACK:
			AttackEnemy(enemy);
			enemy->interval = ENEMY_INTERVAL_ATTACK;
			break;
		case ENEMY_DEAD:
			enemy->interval = ENEMY_INTERVAL_DEAD;
			break;
		case ENEMY_SPAWN:
			if (enemy->state == ENEMY_DESPAWN) break;
		case ENEMY_DESPAWN:
			if (enemy->state == ENEMY_SPAWN) break;
		default:
			enemy->interval = ENEMY_INTERVAL_FADE;
			break;
		}
		enemy->state = state;
	}
}

//=============================================================================
// エネミー移動関数
//=============================================================================
void MoveEnemy(ENEMY *enemy)
{
	float rotZ = D3DX_PI * 2 * (rand() % 360) / 360.0f;

	enemy->vec.x = ENEMY_SPEED_001 * cosf(rotZ);
	enemy->vec.y = ENEMY_SPEED_001 * sinf(rotZ);

	enemy->dir = (int)(rotZ / D3DX_PI * 2 + 0.5) % 4;
}

//=============================================================================
// エネミー反発関数
//=============================================================================
void KnockBackEnemy(ENEMY *enemy, float speed, float rotZ)
{
	enemy->vec.x = 0.0f;
	enemy->vec.y = 0.0f;

	//if (enemy->state != ENEMY_DEAD)
	//{
	//	enemy->subVec.x += speed * cosf(rotZ);
	//	enemy->subVec.y += speed * sinf(rotZ);
	//}
	//else
	//{
	//	enemy->subVec.x = speed * cosf(rotZ);
	//	enemy->subVec.y = speed * sinf(rotZ);
	//}

	enemy->subVec.x = speed * cosf(rotZ);
	enemy->subVec.y = speed * sinf(rotZ);
	
	if (enemy->state != ENEMY_DAMAGE)
	{
		enemy->dir = (int)(rotZ / D3DX_PI * 2 + 4.5) % 4;
	}
}

//=============================================================================
// エネミー攻撃関数
//=============================================================================
void AttackEnemy(ENEMY *enemy)
{
	PLAYER *player = GetPlayer();
	float rotZ = atan2(player->pos.y - enemy->pos.y, player->pos.x - enemy->pos.x);

	//enemy->subVec.x = -BULLET_SPEED_PISTOL * cosf(rotZ);
	//enemy->subVec.y = -BULLET_SPEED_PISTOL * sinf(rotZ);

	SetBullet(enemy->pos-enemy->subVec, rotZ, ENEMY_BULLET);

	enemy->dir = (int)(rotZ / D3DX_PI * 2 + 4.5) % 4;
}
