//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#include "main.h"
#include "score.h"
#include <stdio.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexScore(void);
void SetTextureScore(void);
void SetVertexScore(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureScore = NULL;		// �e�N�X�`���ւ̃|���S��
VERTEX_2D				g_vertexWkScore[SCORE_DIGIT][NUM_VERTEX];	// ���_���i�[���[�N

D3DXVECTOR3				g_posScore;						// �|���S���̈ړ���
D3DXVECTOR3				g_sizeScore;

int						g_nScore;
int						g_nSymbolScore;						// �X�R�A
int						g_addPoint;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitScore(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_SCORE,					// �t�@�C���̖��O
			&g_pD3DTextureScore);				// �ǂݍ��ރ������̃|�C���^
	}

	SetScoreArea(SCORE_POS_X, SCORE_POS_Y, SCORE_SIZE_X, SCORE_SIZE_Y);
	g_nScore = 0;
	g_nSymbolScore = 0;
	g_addPoint = 0;

	// ���_���̍쐬
	MakeVertexScore();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitScore(void)
{
	if( g_pD3DTextureScore != NULL )
	{	// �e�N�X�`���̊J��
		g_pD3DTextureScore->Release();
		g_pD3DTextureScore = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateScore(void)
{
	// ���t���[�����s����鏈�����L�q����
	if (g_nSymbolScore < g_nScore)
	{
		g_nSymbolScore += g_addPoint;
	}
	else
	{
		g_nSymbolScore = g_nScore;
		g_addPoint = 0;
	}

	SetVertexScore();
	SetTextureScore();

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture( 0, g_pD3DTextureScore );

	// �X�R�A
	for(int i = 0; i < SCORE_DIGIT; i++ )
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkScore[i], sizeof(VERTEX_2D));
	}

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexScore(void)
{	
	// ��������������
	for(int i = 0; i < SCORE_DIGIT; i++ )
	{
		// ���_���W�̐ݒ�
		g_vertexWkScore[i][0].vtx.x = TEXTURE_SCORE_SIZE_X * (SCORE_DIGIT - i - 1) + SCORE_POS_X;
		g_vertexWkScore[i][0].vtx.y = SCORE_POS_Y;
		g_vertexWkScore[i][0].vtx.z = 0.0f;
		g_vertexWkScore[i][1].vtx.x = TEXTURE_SCORE_SIZE_X * (SCORE_DIGIT - i - 1) + SCORE_POS_X + TEXTURE_SCORE_SIZE_X;
		g_vertexWkScore[i][1].vtx.y = SCORE_POS_Y;
		g_vertexWkScore[i][1].vtx.z = 0.0f;
		g_vertexWkScore[i][2].vtx.x = TEXTURE_SCORE_SIZE_X * (SCORE_DIGIT - i - 1) + SCORE_POS_X;
		g_vertexWkScore[i][2].vtx.y = SCORE_POS_Y + TEXTURE_SCORE_SIZE_Y;
		g_vertexWkScore[i][2].vtx.z = 0.0f;
		g_vertexWkScore[i][3].vtx.x = TEXTURE_SCORE_SIZE_X * (SCORE_DIGIT - i - 1) + SCORE_POS_X + TEXTURE_SCORE_SIZE_X;
		g_vertexWkScore[i][3].vtx.y = SCORE_POS_Y + TEXTURE_SCORE_SIZE_Y;
		g_vertexWkScore[i][3].vtx.z = 0.0f;

		// rhw�̐ݒ�
		g_vertexWkScore[i][0].rhw =
		g_vertexWkScore[i][1].rhw =
		g_vertexWkScore[i][2].rhw =
		g_vertexWkScore[i][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		g_vertexWkScore[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkScore[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkScore[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkScore[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		g_vertexWkScore[i][0].tex = D3DXVECTOR2( 0.0f, 0.0f );
		g_vertexWkScore[i][1].tex = D3DXVECTOR2( 1.0f, 0.0f );
		g_vertexWkScore[i][2].tex = D3DXVECTOR2( 0.0f, 1.0f );
		g_vertexWkScore[i][3].tex = D3DXVECTOR2( 1.0f, 1.0f );
	}

	return S_OK;
}
//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetTextureScore(void)
{
	int number = g_nSymbolScore;
	
	for(int i = 0; i < SCORE_DIGIT; i++ )
	{
		// �e�N�X�`�����W�̐ݒ�
		float y = (float)(number % 10);
		g_vertexWkScore[i][0].tex = D3DXVECTOR2( 0.0f, 0.1f * y);
		g_vertexWkScore[i][1].tex = D3DXVECTOR2( 1.0f, 0.1f * y);
		g_vertexWkScore[i][2].tex = D3DXVECTOR2( 0.0f, 0.1f * ( y + 1 ));
		g_vertexWkScore[i][3].tex = D3DXVECTOR2( 1.0f, 0.1f * ( y + 1 ));
		number /= 10;
	}
}

void SetVertexScore(void)
{
	for (int i = 0; i < SCORE_DIGIT; i++)
	{
		// ���_���W�̐ݒ�
		g_vertexWkScore[i][0].vtx.x = g_sizeScore.x * (SCORE_DIGIT - i - 1) + g_posScore.x;
		g_vertexWkScore[i][0].vtx.y = g_posScore.y;
		g_vertexWkScore[i][0].vtx.z = 0.0f;
		g_vertexWkScore[i][1].vtx.x = g_sizeScore.x * (SCORE_DIGIT - i - 1) + g_posScore.x + g_sizeScore.x;
		g_vertexWkScore[i][1].vtx.y = g_posScore.y;
		g_vertexWkScore[i][1].vtx.z = 0.0f;
		g_vertexWkScore[i][2].vtx.x = g_sizeScore.x * (SCORE_DIGIT - i - 1) + g_posScore.x;
		g_vertexWkScore[i][2].vtx.y = g_posScore.y + g_sizeScore.y;
		g_vertexWkScore[i][2].vtx.z = 0.0f;
		g_vertexWkScore[i][3].vtx.x = g_sizeScore.x * (SCORE_DIGIT - i - 1) + g_posScore.x + g_sizeScore.x;
		g_vertexWkScore[i][3].vtx.y = g_posScore.y + g_sizeScore.y;
		g_vertexWkScore[i][3].vtx.z = 0.0f;
	}
}


//=============================================================================
// HP�f�[�^���Z�b�g����
// ����:add :�ǉ�����_���B�}�C�i�X���\�A�������ȂǂɁB
//=============================================================================
void AddScore( int add )
{
	g_nScore += add;
	if( g_nScore > SCORE_MAX )
	{
		g_nScore = SCORE_MAX;
	}
	else if( g_nScore < 0 )
	{
		g_nScore = 0;
	}
	g_addPoint = (g_nScore - g_nSymbolScore) / 30;
	if (g_addPoint < 1) g_addPoint = 1;
}

void SetScoreArea(float x, float y, float sizeX, float sizeY)
{
	g_posScore = D3DXVECTOR3((float)x, (float)y, 0.0f);
	g_sizeScore = D3DXVECTOR3((float)sizeX, (float)sizeY, 0.0f);
}

int GetScore(void)
{
	return g_nScore;
}