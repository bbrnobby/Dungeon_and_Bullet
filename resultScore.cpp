//=============================================================================
//
// ���U���g�X�R�A���� [resultScore.cpp]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#include "main.h"
#include "resultScore.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_RESULT_SCORE	_T("data/TEXTURE/number16x32.png")	// �T���v���p�摜
#define TEXTURE_RESULT_SCORE_SIZE_X	(16)	// �e�N�X�`���T�C�Y
#define TEXTURE_RESULT_SCORE_SIZE_Y	(32)	// ����

#define RESULTSCORE_POS_X		(SCREEN_CENTER_X - 160)	// ���U���g�X�R�A�̕\���ʒu
#define RESULTSCORE_POS_Y		(SCREEN_CENTER_Y - 64)	// ���U���g�X�R�A�̕\���ʒu
#define RESULTSCORE_SIZE_X		(64)					// ���U���g�X�R�A�̕������T�C�Y
#define RESULTSCORE_SIZE_Y		(128)					// ���U���g�X�R�A�̕����c�T�C�Y

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexResultScore(void);
void SetTextureResultScore(void);
void SetVertexResultScore(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureResultScore = NULL;		// �e�N�X�`���ւ̃|���S��
VERTEX_2D				g_vertexWkResultScore[SCORE_DIGIT][NUM_VERTEX];	// ���_���i�[���[�N

D3DXVECTOR3				g_posResultScore;						// �|���S���̈ړ���
D3DXVECTOR3				g_sizeResultScore;

int						g_nResultScore;							// ���U���g�X�R�A

//=============================================================================
// ����������
//=============================================================================
HRESULT InitResultScore(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_RESULT_SCORE,				// �t�@�C���̖��O
			&g_pD3DTextureResultScore);			// �ǂݍ��ރ������̃|�C���^
	}

	SetResultScoreArea(RESULTSCORE_POS_X, RESULTSCORE_POS_Y, RESULTSCORE_SIZE_X, RESULTSCORE_SIZE_Y);
	g_nResultScore = 0;

	// ���_���̍쐬
	MakeVertexResultScore();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitResultScore(void)
{
	if (g_pD3DTextureResultScore != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTextureResultScore->Release();
		g_pD3DTextureResultScore = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResultScore(void)
{
	g_nResultScore = GetScore();

	SetVertexResultScore();
	SetTextureResultScore();

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureResultScore);

	// �X�R�A
	for (int i = 0; i < SCORE_DIGIT; i++)
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkResultScore[i], sizeof(VERTEX_2D));
	}

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexResultScore(void)
{
	// ��������������
	for (int i = 0; i < SCORE_DIGIT; i++)
	{
		// ���_���W�̐ݒ�
		g_vertexWkResultScore[i][0].vtx.x = TEXTURE_RESULT_SCORE_SIZE_X * (SCORE_DIGIT - i - 1) + SCORE_POS_X;
		g_vertexWkResultScore[i][0].vtx.y = SCORE_POS_Y;
		g_vertexWkResultScore[i][0].vtx.z = 0.0f;
		g_vertexWkResultScore[i][1].vtx.x = TEXTURE_RESULT_SCORE_SIZE_X * (SCORE_DIGIT - i - 1) + SCORE_POS_X + TEXTURE_RESULT_SCORE_SIZE_X;
		g_vertexWkResultScore[i][1].vtx.y = SCORE_POS_Y;
		g_vertexWkResultScore[i][1].vtx.z = 0.0f;
		g_vertexWkResultScore[i][2].vtx.x = TEXTURE_RESULT_SCORE_SIZE_X * (SCORE_DIGIT - i - 1) + SCORE_POS_X;
		g_vertexWkResultScore[i][2].vtx.y = SCORE_POS_Y + TEXTURE_RESULT_SCORE_SIZE_Y;
		g_vertexWkResultScore[i][2].vtx.z = 0.0f;
		g_vertexWkResultScore[i][3].vtx.x = TEXTURE_RESULT_SCORE_SIZE_X * (SCORE_DIGIT - i - 1) + SCORE_POS_X + TEXTURE_RESULT_SCORE_SIZE_X;
		g_vertexWkResultScore[i][3].vtx.y = SCORE_POS_Y + TEXTURE_RESULT_SCORE_SIZE_Y;
		g_vertexWkResultScore[i][3].vtx.z = 0.0f;

		// rhw�̐ݒ�
		g_vertexWkResultScore[i][0].rhw =
		g_vertexWkResultScore[i][1].rhw =
		g_vertexWkResultScore[i][2].rhw =
		g_vertexWkResultScore[i][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		g_vertexWkResultScore[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultScore[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultScore[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultScore[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		g_vertexWkResultScore[i][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		g_vertexWkResultScore[i][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		g_vertexWkResultScore[i][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		g_vertexWkResultScore[i][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}
//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetTextureResultScore(void)
{
	int number = g_nResultScore;

	for (int i = 0; i < SCORE_DIGIT; i++)
	{
		// �e�N�X�`�����W�̐ݒ�
		float x = (float)(number % 10);
		g_vertexWkResultScore[i][0].tex = D3DXVECTOR2(0.1f * x, 0.0f);
		g_vertexWkResultScore[i][1].tex = D3DXVECTOR2(0.1f * (x + 1), 0.0f);
		g_vertexWkResultScore[i][2].tex = D3DXVECTOR2(0.1f * x, 1.0f);
		g_vertexWkResultScore[i][3].tex = D3DXVECTOR2(0.1f * (x + 1), 1.0f);
		number /= 10;
	}
}

void SetVertexResultScore(void)
{
	for (int i = 0; i < SCORE_DIGIT; i++)
	{
		// ���_���W�̐ݒ�
		g_vertexWkResultScore[i][0].vtx.x = g_sizeResultScore.x * (SCORE_DIGIT - i - 1) + g_posResultScore.x;
		g_vertexWkResultScore[i][0].vtx.y = g_posResultScore.y;
		g_vertexWkResultScore[i][0].vtx.z = 0.0f;
		g_vertexWkResultScore[i][1].vtx.x = g_sizeResultScore.x * (SCORE_DIGIT - i - 1) + g_posResultScore.x + g_sizeResultScore.x;
		g_vertexWkResultScore[i][1].vtx.y = g_posResultScore.y;
		g_vertexWkResultScore[i][1].vtx.z = 0.0f;
		g_vertexWkResultScore[i][2].vtx.x = g_sizeResultScore.x * (SCORE_DIGIT - i - 1) + g_posResultScore.x;
		g_vertexWkResultScore[i][2].vtx.y = g_posResultScore.y + g_sizeResultScore.y;
		g_vertexWkResultScore[i][2].vtx.z = 0.0f;
		g_vertexWkResultScore[i][3].vtx.x = g_sizeResultScore.x * (SCORE_DIGIT - i - 1) + g_posResultScore.x + g_sizeResultScore.x;
		g_vertexWkResultScore[i][3].vtx.y = g_posResultScore.y + g_sizeResultScore.y;
		g_vertexWkResultScore[i][3].vtx.z = 0.0f;
	}
}

void SetResultScoreArea(float x, float y, float sizeX, float sizeY)
{
	g_posResultScore = D3DXVECTOR3((float)x, (float)y, 0.0f);
	g_sizeResultScore = D3DXVECTOR3((float)sizeX, (float)sizeY, 0.0f);
}
