//=============================================================================
//
// �e���� [gun.h]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#ifndef _GUN_H_
#define _GUN_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_GUN_SIZE_X			(96)		// �e�N�X�`���T�C�Y
#define TEXTURE_GUN_SIZE_Y			(96)		// ����
#define SLUG_MAX					(10)		// �U�e��

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct	// �e�\����
{
	bool			use;						// true:�g�p  false:���g�p
	int				type;						// �e�̎��							
	D3DXVECTOR3		pos;						// �ʒu
	D3DXVECTOR3		subPos;						// �ʒu����
	D3DXVECTOR3		vec;						// �ړ��x�N�g��
	D3DXVECTOR3		rot;						// ��]��
	float			dirRot;						// ��]��(����)
	float			subRot;						// �΂߂̉�]
	bool			isLocked;					// �����̌Œ�

	LPDIRECT3DTEXTURE9	Texture;				// �e�N�X�`�����
	VERTEX_2D		vertexWk[NUM_VERTEX];		// ���_���i�[���[�N

	float			Radius;						// ���a
	float			BaseAngle;					// �p�x

	int				ammo;						// ���e��
	int				interval;					// �C���^�[�o��
	int				index;						// ��ԕ\���p
	int				numBullets;					// �s�X�g���e��
	int				numSlug;					// �V���b�g�K���e��
} GUN;

typedef struct	// �e(UI)�\����
{
	bool			use;						// true:�g�p  false:���g�p
	D3DXVECTOR3		pos;						// �ʒu
	float			subPosX;					// �ʒu����
	int				count;

	LPDIRECT3DTEXTURE9	Texture;				// �e�N�X�`�����
	VERTEX_2D		vertexWk[NUM_VERTEX];		// ���_���i�[���[�N
} UI_GUN;

typedef struct	// �e�E�B���h�E(UI)�\����
{
	D3DXVECTOR3		pos;						// �ʒu

	LPDIRECT3DTEXTURE9	Texture;				// �e�N�X�`�����
	VERTEX_2D		vertexWk[NUM_VERTEX];		// ���_���i�[���[�N
} UI_WINDOW;

typedef struct	// �e�e(UI)�\����
{
	bool			use;						// true:�g�p  false:���g�p
	D3DXVECTOR3		pos;						// �ʒu
	D3DXVECTOR3		vec;						// �ړ��x�N�g��
	bool			move;						// �ړ��t���O
	D3DXVECTOR3		rot;						// ��]��
	float			vecRot;						// ��]���x

	LPDIRECT3DTEXTURE9	Texture;				// �e�N�X�`�����
	VERTEX_2D		vertexWk[NUM_VERTEX];		// ���_���i�[���[�N

	float			Radius;						// ���a
	float			BaseAngle;					// �p�x
} UI_BULLET;

enum GUN_TYPE
{
	GUN_PISTOL,
	GUN_SHOTGUN,
	//GUN_LASERGUN,
	GUN_MAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGun(int type);	// ����������
void UninitGun(void);		// �I������
void UpdateGun(void);		// �X�V����
void DrawGun(void);			// �`�揈��

GUN *GetGun();				// �e�̎擾
void SetGun(int no);		// �e�̐ݒ�
void SetShot();				// �e�̔���
#endif
