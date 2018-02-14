//=============================================================================
//
// �̗͏��� [heart.cpp]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#include "main.h"
#include "heart.h"
#include "player.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexHeart(void);
void SetTextureHeart(void);
void SetVertexHeart(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureHeart = NULL;		// �e�N�X�`���ւ̃|���S��
VERTEX_2D				g_vertexWkHeart[MAX_HP][NUM_VERTEX];	// ���_���i�[���[�N

D3DXVECTOR3				g_posHeart;						// �|���S���̈ړ���
D3DXVECTOR3				g_sizeHeart;

int						g_nHeart;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitHeart(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_HEART,				// �t�@�C���̖��O
			&g_pD3DTextureHeart);				// �ǂݍ��ރ������̃|�C���^
	}

	SetHeartArea(HEART_POS_X, HEART_POS_Y, TEXTURE_HEART_SIZE_X, TEXTURE_HEART_SIZE_Y);
	g_nHeart = 0;

	// ���_���̍쐬
	MakeVertexHeart();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitHeart(void)
{
	if (g_pD3DTextureHeart != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTextureHeart->Release();
		g_pD3DTextureHeart = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateHeart(void)
{
	// ���t���[�����s����鏈�����L�q����
	SetVertexHeart();
	SetTextureHeart();

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawHeart(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureHeart);

	// HP
	for (int i = 0; i < MAX_HP; i++)
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkHeart[i], sizeof(VERTEX_2D));
	}

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexHeart(void)
{
	// ��������������
	for (int i = 0; i < MAX_HP; i++)
	{
		// ���_���W�̐ݒ�
		g_vertexWkHeart[i][0].vtx.x = g_sizeHeart.x * i + g_posHeart.x;
		g_vertexWkHeart[i][0].vtx.y = g_posHeart.y;
		g_vertexWkHeart[i][0].vtx.z = 0.0f;
		g_vertexWkHeart[i][1].vtx.x = g_sizeHeart.x * i + g_posHeart.x + g_sizeHeart.x;
		g_vertexWkHeart[i][1].vtx.y = g_posHeart.y;
		g_vertexWkHeart[i][1].vtx.z = 0.0f;
		g_vertexWkHeart[i][2].vtx.x = g_sizeHeart.x * i + g_posHeart.x;
		g_vertexWkHeart[i][2].vtx.y = g_posHeart.y + g_sizeHeart.y;
		g_vertexWkHeart[i][2].vtx.z = 0.0f;
		g_vertexWkHeart[i][3].vtx.x = g_sizeHeart.x * i + g_posHeart.x + g_sizeHeart.x;
		g_vertexWkHeart[i][3].vtx.y = g_posHeart.y + g_sizeHeart.y;
		g_vertexWkHeart[i][3].vtx.z = 0.0f;

		// rhw�̐ݒ�
		g_vertexWkHeart[i][0].rhw =
		g_vertexWkHeart[i][1].rhw =
		g_vertexWkHeart[i][2].rhw =
		g_vertexWkHeart[i][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		g_vertexWkHeart[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkHeart[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkHeart[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkHeart[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		g_vertexWkHeart[i][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		g_vertexWkHeart[i][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		g_vertexWkHeart[i][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		g_vertexWkHeart[i][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}
//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetTextureHeart(void)
{
	int number = GetPlayer()->hp;

	for (int i = 0; i < MAX_HP; i++)
	{
		// �e�N�X�`�����W�̐ݒ�
		int x = (number - (HEART_DIVIDE - 1) * i);
		if (x < 0) x = 0;
		else if (x > HEART_DIVIDE - 1) x = HEART_DIVIDE - 1;
		g_vertexWkHeart[i][0].tex = D3DXVECTOR2(1.0f / HEART_DIVIDE * x, 0.0f);
		g_vertexWkHeart[i][1].tex = D3DXVECTOR2(1.0f / HEART_DIVIDE * (x + 1), 0.0f);
		g_vertexWkHeart[i][2].tex = D3DXVECTOR2(1.0f / HEART_DIVIDE * x, 1.0f);
		g_vertexWkHeart[i][3].tex = D3DXVECTOR2(1.0f / HEART_DIVIDE * (x + 1), 1.0f);
	}
}

void SetVertexHeart(void)
{
	D3DXVECTOR3 *subPosCamera = GetSubPosCamera();

	for (int i = 0; i < MAX_HP; i++)
	{
		// ���_���W�̐ݒ�
		g_vertexWkHeart[i][0].vtx.x = g_sizeHeart.x * i + g_posHeart.x;
		g_vertexWkHeart[i][0].vtx.y = g_posHeart.y;
		g_vertexWkHeart[i][0].vtx.z = 0.0f;
		g_vertexWkHeart[i][0].vtx += *subPosCamera;

		g_vertexWkHeart[i][1].vtx.x = g_sizeHeart.x * i + g_posHeart.x + g_sizeHeart.x;
		g_vertexWkHeart[i][1].vtx.y = g_posHeart.y;
		g_vertexWkHeart[i][1].vtx.z = 0.0f;
		g_vertexWkHeart[i][1].vtx += *subPosCamera;

		g_vertexWkHeart[i][2].vtx.x = g_sizeHeart.x * i + g_posHeart.x;
		g_vertexWkHeart[i][2].vtx.y = g_posHeart.y + g_sizeHeart.y;
		g_vertexWkHeart[i][2].vtx.z = 0.0f;
		g_vertexWkHeart[i][2].vtx += *subPosCamera;

		g_vertexWkHeart[i][3].vtx.x = g_sizeHeart.x * i + g_posHeart.x + g_sizeHeart.x;
		g_vertexWkHeart[i][3].vtx.y = g_posHeart.y + g_sizeHeart.y;
		g_vertexWkHeart[i][3].vtx.z = 0.0f;
		g_vertexWkHeart[i][3].vtx += *subPosCamera;
	}
}

void SetHeartArea(float posX, float posY, float sizeX, float sizeY)
{
	g_posHeart = D3DXVECTOR3((float)posX, (float)posY, 0.0f);
	g_sizeHeart = D3DXVECTOR3((float)sizeX, (float)sizeY, 0.0f);
}
