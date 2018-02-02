//=============================================================================
//
// �v���C���[���� [player.h]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_PLAYER_SIZE_X		(48)	// �e�N�X�`���T�C�Y
#define TEXTURE_PLAYER_SIZE_Y		(48)	// ����
#define PLAYER_SPEED				(6.0f)	// �v���C���[�̑��x

//*****************************************************************************
// �\���̐錾
//*****************************************************************************

typedef struct	// �v���C���[�\����
{
	bool			use;						// true:�g�p  false:���g�p
	bool			isMove;						// �����Ă��邩
	int				state;						// ���
	D3DXVECTOR3		pos;						// �ړ���
	float			subPosY;					// �c�ړ�
	D3DXVECTOR3		vec;						// �ړ��x�N�g��
	D3DXVECTOR3		subVec;						// �����x�N�g��
	bool			invincible;					// ���G
	int				interval;					// �C���^�[�o��
	int				hp;							// �̗�
	int				gunType;					// �e�̃^�C�v
	bool			movable;					// �����邩

	int				dir;						// ����
	D3DXVECTOR3		rot;						// ��]��
	int				PatternAnim;				// �A�j���[�V�����p�^�[���i���o�[
	int				CountAnim;					// �A�j���[�V�����J�E���g

	LPDIRECT3DTEXTURE9	Texture;				// �e�N�X�`�����
	VERTEX_2D		vertexWk[NUM_VERTEX];		// ���_���i�[���[�N

	float			Radius;						// ���a
	float			BaseAngle;					// �p�x

	int				roomID;						// ���镔����ID

} PLAYER;

enum PLAYER_DIRECTION
{
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT,
	DIR_UP,
};

enum PLAYER_STATE
{
	PLAYER_WALK,
	PLAYER_LOCK,
	PLAYER_RUN,
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(int type);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);
void SetPlayer(float x, float y);
void KnockBackPlayer(PLAYER *player, float rotZ);
void SetInvincible(PLAYER *player);

#endif
