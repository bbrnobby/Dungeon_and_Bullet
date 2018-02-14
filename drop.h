//=============================================================================
//
// �h���b�v���� [drop.h]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#ifndef _DROP_H_
#define _DROP_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_DROP_SIZE				(48)		// �e�N�X�`���T�C�Y

#define MAX_DROP						(30)
#define DROP_MES_SIZE					(36)
#define DROP_MES_INTERVVAL				(120)

enum DROP_TYPE
{
	DROP_COIN001,
	DROP_COIN002,
	DROP_COIN003,
	//DROP_PISTOL,
	//DROP_SHOTGUN,
	//DROP_LASERGUN,
	DROP_MINIHEART,
	DROP_BIGHEART,
	DROP_JEWEL001,
	DROP_JEWEL002,
	DROP_JEWEL003,
	DROP_JEWEL004,
	DROP_JEWEL005,
	DROP_SLUG,
	DROP_MAX
};


//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct	// �t�H���g�\����
{
	bool			use;						// true:�g�p  false:���g�p
	D3DXVECTOR3		pos;						// �ʒu
	D3DXVECTOR3		vec;						// ���x
	D3DXVECTOR3		rot;						// ��]
	float			vecRot;						// ��]���x
	float			subVec;						// �c�ړ�
	float			height;						// ����
	int				type;						// ���
	int				size;						// �傫��

	int				interval;					// �C���^�[�o��

	LPDIRECT3DTEXTURE9	Texture;				// �e�N�X�`�����
	VERTEX_2D		vertexWk[NUM_VERTEX];		// ���_���i�[���[�N

	float			Radius;						// ���a
	float			BaseAngle;					// �p�x
} DROP;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitDrop(int type);
void UninitDrop(void);
void UpdateDrop(void);
void DrawDrop(void);

DROP *GetDrop(int no);
void SetDrop(float posX, float posY, int type, int size);
void PickDrop(DROP *drop);

#endif
