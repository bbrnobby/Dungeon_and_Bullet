//=============================================================================
//
// �o���b�g���� [bullet.h]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_GAME_BULLET				_T("data/TEXTURE/bullet001.png")	// �摜
#define TEXTURE_BULLET_SIZE				(12)		// �e�N�X�`���T�C�Y

#define TEXTURE_PATTERN_DIVIDE_X_BULLET	(2)			// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_BULLET	(2)			// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_BULLET			(TEXTURE_PATTERN_DIVIDE_X_BULLET*TEXTURE_PATTERN_DIVIDE_Y_BULLET)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_BULLET			(4)			// �A�j���[�V�����̐؂�ւ��J�E���g

#define BULLET_MAX						(50)		// �\�������e�̍ő吔

#define BULLET_SPEED_001				(25.0f)		// �o���b�g�̈ړ��X�s�[�h
#define BULLET_SPEED_ENEMY				(5.0f)		// �G�o���b�g�̈ړ��X�s�[�h
#define BULLET_SPINSPEED_001			(0.3f)		// �e�̉�]���x

enum shooter
{
	PLAYER_BULLET_PISTOL,
	PLAYER_BULLET_SHOTGUN,
	ENEMY_BULLET
};

//*****************************************************************************
// �\���̐錾
//*****************************************************************************

typedef struct	// �o���b�g�\����
{
	bool			use;						// true:�g�p  false:���g�p
	D3DXVECTOR3		pos;						// �ړ���
	D3DXVECTOR3		vec;						// �ړ��x�N�g��
	D3DXVECTOR3		rot;						// ��]��
	int				duration;
	int				PatternAnim;
	
	LPDIRECT3DTEXTURE9	Texture;				// �e�N�X�`�����
	VERTEX_2D		vertexWk[NUM_VERTEX];		// ���_���i�[���[�N

	bool			spin;						// ��]���邩
	float			Radius;						// ���a
	float			BaseAngle;					// �p�x

	int				shooter;

} BULLET;



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBullet(int type);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, float rot, int shooter, int duration);
BULLET *GetBullet(int no);


#endif
