//=============================================================================
//
// �w�i���� [bg.cpp]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#include "bg.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBG(void);
void SetVertexBG(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBG = NULL;		// �e�N�X�`���ւ̃|�C���^
VERTEX_2D				g_vertexWkBG[NUM_VERTEX];	// ���_���i�[���[�N

D3DXVECTOR3				g_posBG;					// �w�i�̈ʒu
float					g_rotz;
float					g_radius;
float					g_baseAngleBG;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBG(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_GAME_BG00,		// �t�@�C���̖��O
			&g_pD3DTextureBG);		// �ǂݍ��ރ������[
	}

	g_posBG = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotz = 0.0f;
	D3DXVECTOR2 temp = D3DXVECTOR2(BG00_SIZE_X, BG00_SIZE_Y);
	g_radius = D3DXVec2Length(&temp);
	g_baseAngleBG = atan2f(BG00_SIZE_X, BG00_SIZE_Y);

	g_posBG.x = -GetPlayer()->pos.x / 4.0f;
	g_posBG.y = -GetPlayer()->pos.y / 4.0f;

	// ���_���̍쐬
	MakeVertexBG();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBG(void)
{
	if(g_pD3DTextureBG != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureBG->Release();
		g_pD3DTextureBG = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBG(void)
{
	// ���t���[�����s���鏈��

	// �X�N���[������
	g_posBG.x = -GetPlayer()->pos.x / 4.0f;
	g_posBG.y = -GetPlayer()->pos.y / 4.0f;

	g_rotz += BG00_ROTATION;

	SetVertexBG();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureBG);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkBG, sizeof(VERTEX_2D));
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexBG(void)
{
	// ���_���W�̐ݒ�
	SetVertexBG();

	// rhw�̐ݒ�
	g_vertexWkBG[0].rhw =
	g_vertexWkBG[1].rhw =
	g_vertexWkBG[2].rhw =
	g_vertexWkBG[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkBG[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 45);
	g_vertexWkBG[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 45);
	g_vertexWkBG[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 45);
	g_vertexWkBG[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 45);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkBG[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkBG[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkBG[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkBG[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBG(void)
{
	// ���_���W�̐ݒ�
	g_vertexWkBG[0].vtx = D3DXVECTOR3(BG00_CENTER_X - cosf(g_baseAngleBG + g_rotz) * g_radius, BG00_CENTER_Y - sinf(g_baseAngleBG + g_rotz) * g_radius, 0.0f) + g_posBG;
	g_vertexWkBG[1].vtx = D3DXVECTOR3(BG00_CENTER_X + cosf(g_baseAngleBG - g_rotz) * g_radius, BG00_CENTER_Y - sinf(g_baseAngleBG - g_rotz) * g_radius, 0.0f) + g_posBG;
	g_vertexWkBG[2].vtx = D3DXVECTOR3(BG00_CENTER_X - cosf(g_baseAngleBG - g_rotz) * g_radius, BG00_CENTER_Y + sinf(g_baseAngleBG - g_rotz) * g_radius, 0.0f) + g_posBG;
	g_vertexWkBG[3].vtx = D3DXVECTOR3(BG00_CENTER_X + cosf(g_baseAngleBG + g_rotz) * g_radius, BG00_CENTER_Y + sinf(g_baseAngleBG + g_rotz) * g_radius, 0.0f) + g_posBG;
}
