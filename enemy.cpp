//=============================================================================
//
// �G���� [enemy.cpp]
// Author : GP11A243 06 ���c���l
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
// �}�N����`
//*****************************************************************************
#define TEXTURE_GAME_ENEMY		_T("data/TEXTURE/enemy001.png")	// �摜

#define ENEMY_TEXTURE_PATTERN_DIVIDE_X	(3)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define ENEMY_TEXTURE_PATTERN_DIVIDE_Y	(8)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ENEMY_ANIM_PATTERN_NUM			(ENEMY_TEXTURE_PATTERN_DIVIDE_X)	// �A�j���[�V�����p�^�[����
#define ENEMY_TIME_ANIMATION			(10)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define ENEMY_INTERVAL_WAIT				(30)	// �ҋ@�C���^�[�o��
#define ENEMY_INTERVAL_DAMAGE			(30)	// �_���[�W�C���^�[�o��
#define ENEMY_INTERVAL_MOVE				(90)	// �ړ��C���^�[�o��
#define ENEMY_INTERVAL_ATTACK			(90)	// �U���C���^�[�o��
#define ENEMY_INTERVAL_FADE				(60)	// �t�F�[�h�C���^�[�o��
#define ENEMY_INTERVAL_DEAD				(120)	// ���S�C���^�[�o��

#define ENEMY_HP_001					(3)		// �G�l�~�[1�̗̑�
//#define ENEMY_SCORE_001					(100)	// �G�l�~�[1�̃X�R�A
#define ENEMY_SPEED_001					(3.0f)	// �G�l�~�[1�̑��x
#define ENEMY_FRICTION_001				(0.85f)	// �G�l�~�[1�̊���

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexEnemy( int no );
void SetTextureEnemy( int no, int cntPattern );
void SetVertexEnemy( int no );
void SetDiffuseEnemy(int no);
void MoveEnemy(ENEMY *enemy);
void AttackEnemy(ENEMY *enemy);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEnemy = NULL;						// �e�N�X�`���ւ̃|���S��
VERTEX_2D				g_vertexWkHPEnemy[ENEMY_MAX][NUM_VERTEX];		// ���_���i�[���[�N
VERTEX_2D				g_vertexWkMaxHPEnemy[ENEMY_MAX][NUM_VERTEX];	// ���_���i�[���[�N

ENEMY					enemyWk[ENEMY_MAX];								// �G�l�~�[�\����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = enemyWk;					// �G�l�~�[�̃|�C���^

	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_ENEMY,				// �t�@�C���̖��O
			&g_pD3DTextureEnemy);			// �ǂݍ��ރ������̃|�C���^
	}

	// �G�l�~�[�̏���������
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		enemy->use = false;										// �g�p
		enemy->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ���W�f�[�^��������
		enemy->subRot = 0.0f;
		enemy->vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �x�N�g���f�[�^��������
		enemy->subVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ��]�f�[�^��������
		enemy->dir = 0;
		enemy->PatternAnim = 0;									// �A�j���p�^�[���ԍ���������
		enemy->CountAnim = 0;									// �A�j���J�E���g��������
		enemy->notice = false;
		enemy->roomID = -1;
		enemy->type = 0;
		enemy->state = ENEMY_DEFAULT_STATE;
		enemy->interval = 0;
		enemy->hp = 0;

		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_ENEMY_SIZE_X, TEXTURE_ENEMY_SIZE_Y);
		enemy->Radius = D3DXVec2Length(&temp);					// �G�l�~�[�̔��a��������
		enemy->BaseAngle = atan2f(TEXTURE_ENEMY_SIZE_Y, TEXTURE_ENEMY_SIZE_X);	// �G�l�~�[�̊p�x��������

		enemy->Texture = g_pD3DTextureEnemy;					// �e�N�X�`�����
		MakeVertexEnemy(i);										// ���_���̍쐬
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	if (g_pD3DTextureEnemy != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTextureEnemy->Release();
		g_pD3DTextureEnemy = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	ENEMY *enemy = enemyWk;		// �G�l�~�[�̃|�C���^
	
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->state != ENEMY_DEFAULT_STATE)	
		{	// �o�����Ă����ԂȂ�X�V����
			if ((enemy->CountAnim % ENEMY_TIME_ANIMATION) == 0)
			{
				// �p�^�[���̐؂�ւ�
				enemy->PatternAnim = (enemy->PatternAnim + 1) % ENEMY_ANIM_PATTERN_NUM;
				enemy->CountAnim = 0;

				// �e�N�X�`�����W��ݒ�
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
				// ���W�̍X�V
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
			SetVertexEnemy(i);	// �ړ���̍��W�Œ��_��ݒ�
#ifdef _DEBUG
			PrintDebugProc("ENEMEY[%d] X:%f Y:%f ROOM:%d STATE:%d\n", i, enemy->pos.x, enemy->pos.y, enemy->roomID, enemy->state);
#endif
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = enemyWk;			// �G�l�~�[�̃|�C���^

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->state != ENEMY_DEFAULT_STATE)
		{	// �o�����Ă����ԂȂ�`�悷��
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, enemy->Texture);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, enemy->vertexWk, sizeof(VERTEX_2D));

			if (enemy->state < ENEMY_SPAWN)
			{
				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, NULL);

				// �|���S���̕`��
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkMaxHPEnemy[i], sizeof(VERTEX_2D));

				// �|���S���̕`��
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkHPEnemy[i], sizeof(VERTEX_2D));
			}
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexEnemy( int no )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = &enemyWk[no];

	// ���_���W�̐ݒ�
	SetVertexEnemy( no );

	// rhw�̐ݒ�
	enemy->vertexWk[0].rhw =
	enemy->vertexWk[1].rhw =
	enemy->vertexWk[2].rhw =
	enemy->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	enemy->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	enemy->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	enemy->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	enemy->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	enemy->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	enemy->vertexWk[1].tex = D3DXVECTOR2(1.0f / ENEMY_TEXTURE_PATTERN_DIVIDE_X, 0.0f);
	enemy->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / ENEMY_TEXTURE_PATTERN_DIVIDE_Y);
	enemy->vertexWk[3].tex = D3DXVECTOR2( 1.0f/ ENEMY_TEXTURE_PATTERN_DIVIDE_X, 1.0f/ENEMY_TEXTURE_PATTERN_DIVIDE_Y);

	// rhw�̐ݒ�
	g_vertexWkHPEnemy[no][0].rhw =
	g_vertexWkHPEnemy[no][1].rhw =
	g_vertexWkHPEnemy[no][2].rhw =
	g_vertexWkHPEnemy[no][3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkHPEnemy[no][0].diffuse = D3DCOLOR_RGBA(0, 255, 0, 255);
	g_vertexWkHPEnemy[no][1].diffuse = D3DCOLOR_RGBA(0, 255, 0, 255);
	g_vertexWkHPEnemy[no][2].diffuse = D3DCOLOR_RGBA(0, 255, 0, 255);
	g_vertexWkHPEnemy[no][3].diffuse = D3DCOLOR_RGBA(0, 255, 0, 255);

	// rhw�̐ݒ�
	g_vertexWkMaxHPEnemy[no][0].rhw =
	g_vertexWkMaxHPEnemy[no][1].rhw =
	g_vertexWkMaxHPEnemy[no][2].rhw =
	g_vertexWkMaxHPEnemy[no][3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkMaxHPEnemy[no][0].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
	g_vertexWkMaxHPEnemy[no][1].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
	g_vertexWkMaxHPEnemy[no][2].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
	g_vertexWkMaxHPEnemy[no][3].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureEnemy( int no, int cntPattern )
{
	ENEMY *enemy = &enemyWk[no];

	// �e�N�X�`�����W�̐ݒ�
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
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexEnemy( int no )
{
	ENEMY *enemy = &enemyWk[no];
	D3DXVECTOR3 *posCamera = GetCameraPos();

	// ���_���W�̐ݒ�
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

	// ���_���W�̐ݒ�
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

	// ���_���W�̐ݒ�
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

	// ���ˌ��̐ݒ�
	enemy->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, fade);
	enemy->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, fade);
	enemy->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, fade);
	enemy->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, fade);
}

//=============================================================================
// �G�l�~�[�擾�֐�
//=============================================================================
ENEMY *GetEnemy(int no)
{
	return(&enemyWk[no]);
}

//=============================================================================
// �G�l�~�[�ݒ�֐�
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

			return;			// 1�̃Z�b�g�����̂ŏI������
		}
	}
}

//=============================================================================
// �G�l�~�[�o���֐�
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

			SetVertexEnemy(i);		// �ړ���̍��W�Œ��_��ݒ�
			SetDiffuseEnemy(i);
			SetPointer(i);			// �|�C���^�[��ݒ�
			return true;			// 1�̃Z�b�g�����̂ŏI������
		}
	}

	return false;
}

//=============================================================================
// �G�l�~�[���Ŋ֐�
//=============================================================================
void DeSpawnEnemy(int ID)
{
	ENEMY *enemy = enemyWk;

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->use && enemy->roomID == ID && enemy->state < ENEMY_DESPAWN)
		{
			SetState(enemy, ENEMY_DESPAWN);

			return;			// 1�̃Z�b�g�����̂ŏI������
		}
	}

	return;
}

//=============================================================================
// �G�l�~�[�폜�֐�
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
// �G�l�~�[��Ԑݒ�֐�
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
// �G�l�~�[�ړ��֐�
//=============================================================================
void MoveEnemy(ENEMY *enemy)
{
	float rotZ = D3DX_PI * 2 * (rand() % 360) / 360.0f;

	enemy->vec.x = ENEMY_SPEED_001 * cosf(rotZ);
	enemy->vec.y = ENEMY_SPEED_001 * sinf(rotZ);

	enemy->dir = (int)(rotZ / D3DX_PI * 2 + 0.5) % 4;
}

//=============================================================================
// �G�l�~�[�����֐�
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
// �G�l�~�[�U���֐�
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
