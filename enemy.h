//=============================================================================
//
// �G���� [enemy.h]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "dungeon.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_ENEMY_SIZE_X	(36)	// �e�N�X�`���T�C�Y
#define TEXTURE_ENEMY_SIZE_Y	(36)	// ����

#define ENEMY_INDEX				(25)	// �G�l�~�[�̊�l
#define ENEMY_ROOM_MIN_NUM		(3)		// �������̃G�l�~�[�̍ŏ���
#define ENEMY_MAX				(MAP_WIDTH * MAP_HEIGHT / ENEMY_INDEX) // �G�̍ő吔

#define ENEMY_KNOCKBACK_SPEED	(15.0f)	// �G�l�~�[1�̃m�b�N�o�b�N���x
#define ENEMY_AVOID_SPEED		(0.5f)	// �G�l�~�[1�̉����̂����x

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
// �\���̐錾
//*****************************************************************************
typedef struct	// �G�l�~�[�\����
{
	bool			use;						// true:�g�p  false:���g�p
	D3DXVECTOR3		pos;						// �ړ���
	float			subRot;						// �c�ړ�
	D3DXVECTOR3		vec;						// �ړ��x�N�g��
	D3DXVECTOR3		subVec;						// �⏕�ړ��x�N�g��
	D3DXVECTOR3		rot;						// ��]��
	int				dir;						// ����
	int				PatternAnim;				// �A�j���[�V�����p�^�[���i���o�[
	int				CountAnim;					// �A�j���[�V�����J�E���g

	bool			notice;						// �C�t���Ă��邩
	int				type;						// �G�l�~�[�̎��
	int				state;						// �G�l�~�[�̏��
	int				interval;					// �C���^�[�o��

	int				hp;							// �G�l�~�[�̗̑�
	int				maxhp;						// �G�l�~�[�̗̑�
	//int				score;					// �|�������̃X�R�A

	LPDIRECT3DTEXTURE9	Texture;				// �e�N�X�`�����
	VERTEX_2D		vertexWk[NUM_VERTEX];		// ���_���i�[���[�N

	float			Radius;						// ���a
	float			BaseAngle;					// �p�x

	int				roomID;						// �G�l�~�[�̂��镔����ID

} ENEMY;



//*****************************************************************************
// �v���g�^�C�v�錾
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
