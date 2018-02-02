//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define PARTICLE_LIFE		(60)							// ����

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	bool		use;		// �g�p���Ă��邩�ǂ���
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 vec;		// �ړ���
	D3DXVECTOR3 rot;		// ��]
	D3DXVECTOR3 scale;		// �X�P�[��
	D3DXCOLOR	col;		// �F
	//float		sizeX;		// ��
	//float		sizeY;		// ����
	//int nIdxShadow;			// �eID
	int			life;		// ����
	bool		gravity;	// �d�͂����邩

	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
} PARTICLE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitParticle(int type);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

void SetParticleLine(D3DXVECTOR3 pos, D3DXVECTOR3 vec, D3DXCOLOR col, float sizeX, float sizeY, int life);
PARTICLE *GetParticle(int no);

#endif
