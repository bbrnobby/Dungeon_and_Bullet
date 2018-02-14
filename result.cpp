//=============================================================================
//
// ���U���g��ʏ��� [result.cpp]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#include "main.h"
#include "result.h"
#include "input.h"
#include "fade.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RESULT			_T("data/TEXTURE/TITLE/title_bg.png")			// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_GAMEOVER		_T("data/TEXTURE/TITLE/title_bg_mono.png")		// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexResult(void);
void SetDiffuseResult(int alpha);
void SetDiffuseGameover(int alpha);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureResult = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureGameover = NULL;		// �e�N�X�`���ւ̃|�C���^

VERTEX_2D				g_vertexWkResult[NUM_VERTEX];		// ���_���i�[���[�N
VERTEX_2D				g_vertexWkGameover[NUM_VERTEX];		// ���_���i�[���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
		TEXTURE_RESULT,						// �t�@�C���̖��O
		&g_pD3DTextureResult);				// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
		TEXTURE_GAMEOVER,					// �t�@�C���̖��O
		&g_pD3DTextureGameover);			// �ǂݍ��ރ������[

	// ���_���̍쐬
	MakeVertexResult();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
{
	if(g_pD3DTextureResult != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureResult->Release();
		g_pD3DTextureResult = NULL;
	}

	if (g_pD3DTextureGameover != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureGameover->Release();
		g_pD3DTextureGameover = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult(void)
{
	PLAYER *player = GetPlayer();

	if(IsAnyKeyTriggered() || IsAnyButtonTriggered(0))
	{
		SetFade(FADE_W_OUT, STAGE_REFRESH);
	}

	if (player->hp > 0)
	{
		SetDiffuseResult(255);
	}
	else
	{
		SetDiffuseGameover(255);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureResult);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkResult, sizeof(VERTEX_2D));

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureGameover);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkGameover, sizeof(VERTEX_2D));
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexResult(void)
{	
	// ���_���W�̐ݒ�
	g_vertexWkResult[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_vertexWkResult[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	g_vertexWkResult[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	g_vertexWkResult[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkResult[0].rhw =
	g_vertexWkResult[1].rhw =
	g_vertexWkResult[2].rhw =
	g_vertexWkResult[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkResult[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkResult[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkResult[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkResult[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkResult[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResult[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResult[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResult[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// ���_���W�̐ݒ�
	g_vertexWkGameover[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_vertexWkGameover[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	g_vertexWkGameover[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	g_vertexWkGameover[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkGameover[0].rhw =
	g_vertexWkGameover[1].rhw =
	g_vertexWkGameover[2].rhw =
	g_vertexWkGameover[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkGameover[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkGameover[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkGameover[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkGameover[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkGameover[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkGameover[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkGameover[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkGameover[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// ���ˌ��̐ݒ�
//=============================================================================
void SetDiffuseResult(int alpha)
{
	g_vertexWkResult[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	g_vertexWkResult[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	g_vertexWkResult[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	g_vertexWkResult[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
}

void SetDiffuseGameover(int alpha)
{
	g_vertexWkGameover[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	g_vertexWkGameover[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	g_vertexWkGameover[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	g_vertexWkGameover[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
}

