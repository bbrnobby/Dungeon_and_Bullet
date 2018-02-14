//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#include "main.h"
#include "score.h"
#include "camera.h"
#include "font.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCORE_MAX			(99999)		// �X�R�A�̍ő�l
#define TIME_MAX			(99999)		// ���Ԃ̍ő�l

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
D3DXVECTOR3		g_posScore;			// �X�R�A�̍��W
float			g_sizeScore;		// �X�R�A�̃T�C�Y

int				g_nScore;			// �X�R�A
int				g_nSymbolScore;		// �X�R�A�i�\���p�j
int				g_addPoint;			// ���Z�|�C���g
int				g_time;				// ����
int				g_nTimeCount;		// �J�E���g

//=============================================================================
// ����������
//=============================================================================
HRESULT InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	g_posScore = D3DXVECTOR3(SCORE_POS_X, SCORE_POS_Y, 0.0f);
	g_sizeScore = FONT_SCORE_SIZE;

	g_nScore = 0;
	g_nSymbolScore = 0;
	g_addPoint = 0;
	g_time = 0;
	g_nTimeCount = 0;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitScore(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateScore(int type)
{
	char str[32];
	D3DXVECTOR3 pos = g_posScore;

	if (g_nSymbolScore < g_nScore)
	{
		g_nSymbolScore += g_addPoint;
	}
	else
	{
		g_nSymbolScore = g_nScore;
		g_addPoint = 0;
	}

	if (type == 0)
	{
		if (g_nTimeCount < 60)
		{
			g_nTimeCount++;
		}
		else
		{
			g_time++;
			g_nTimeCount = 0;
		}


		sprintf(str, "�^�C���@%02d:%02d  ", g_time / 60, g_time % 60);
		SetString(str, pos.x, pos.y, g_sizeScore, 1);

		pos.y += g_sizeScore;

		sprintf(str, "�����ˁ@%05d�o��", g_nSymbolScore);
		SetString(str, pos.x, pos.y, g_sizeScore, 1);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawScore(void)
{
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

void SetScoreArea(float x, float y, float size)
{
	g_posScore = D3DXVECTOR3((float)x, (float)y, 0.0f);
	g_sizeScore = size;
}

int GetScore(void)
{
	return g_nScore;
}