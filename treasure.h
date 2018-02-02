//=============================================================================
//
// �󔠏��� [treasure.h]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#ifndef _TREASURE_H_
#define _TREASURE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_GAME_TREASURE				_T("data/TEXTURE/treasure001.png")	// �T���v���p�摜
#define TEXTURE_TREASURE_SIZE				(48)	// �e�N�X�`���T�C�Y

#define TEXTURE_PATTERN_TREASURE_DIVIDE_X	(3)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_TREASURE_DIVIDE_Y	(4)		// �A�j���p�^�[���̃e�N�X�`�����������iY)

#define TREASURE_MAX						(30)
#define TREASURE_TIME_ANIMATION				(12)	// �A�j���[�V�����̐؂�ւ��J�E���g

enum TREASURE_TYPE
{
	TREASURE_HEART,
	//TREASURE_GUNBOX,
	TREASURE_COMMON,
	TREASURE_RARE,
	TREASURE_BOX_MAX
};

enum BOX_STATE
{
	BOX_NONE,
	BOX_SPAWNED,
	BOX_WAITING,
	BOX_OPENED,
	BOX_EMPTY
};

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct	// �t�H���g�\����
{
	bool			use;						// true:�g�p  false:���g�p
	D3DXVECTOR3		pos;						// �ʒu
	D3DXVECTOR3		rot;
	float			vec;
	float			height;
	int				type;
	int				state;
	int				size;

	int				PatternAnim;				// �A�j���[�V�����p�^�[���i���o�[
	int				CountAnim;					// �A�j���[�V�����J�E���g

	LPDIRECT3DTEXTURE9	Texture;				// �e�N�X�`�����
	VERTEX_2D		vertexWk[NUM_VERTEX];		// ���_���i�[���[�N

	float			Radius;						// ���a
	float			BaseAngle;					// �p�x
} TREASURE;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTreasure(int type);
void UninitTreasure(void);
void UpdateTreasure(void);
void DrawTreasure(void);

TREASURE *GetTreasure(int no);
void SetTreasure(float posX, float posY, int type, int size);

#endif
