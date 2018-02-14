//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#include "main.h"
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_TITLE_BG		_T("data/TEXTURE/TITLE/title_bg.png")			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_BG_MONO	_T("data/TEXTURE/TITLE/title_bg_mono.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_BG_CHROMA	_T("data/TEXTURE/TITLE/title_bg_chroma.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_CHROMA	_T("data/TEXTURE/TITLE/title_logo_chroma.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_TEXT		_T("data/TEXTURE/TITLE/title_text.png")			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_VERSION	_T("data/TEXTURE/TITLE/title_version.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_TITLE_HOLE		_T("data/TEXTURE/TITLE/title_bullethole.png")	// �ǂݍ��ރe�N�X�`���t�@�C����

const char *TEXTURE_TITLE_LOGO[] =
{
	"data/TEXTURE/TITLE/title_logo_00.png",
	"data/TEXTURE/TITLE/title_logo_01.png",
	"data/TEXTURE/TITLE/title_logo_02.png",
	"data/TEXTURE/TITLE/title_logo_03.png",
	"data/TEXTURE/TITLE/title_logo_04.png",
	"data/TEXTURE/TITLE/title_logo_05.png",
	"data/TEXTURE/TITLE/title_logo_06.png",
	"data/TEXTURE/TITLE/title_logo_07.png",
	"data/TEXTURE/TITLE/title_logo_08.png",
	"data/TEXTURE/TITLE/title_logo_09.png",
	"data/TEXTURE/TITLE/title_logo_10.png",
	"data/TEXTURE/TITLE/title_logo_11.png",
	"data/TEXTURE/TITLE/title_logo_12.png",
	"data/TEXTURE/TITLE/title_logo_13.png",
};

#define LOGO_MAX				(14)		// ���S�̕�����
#define TITLE_INTERVAL_LOGO		(4)			// �����̏o���Ԋu
#define TITLE_INTERVAL			(100)		// �^�C�g���A�E�g�̃C���^�[�o��
#define POS_HOLE_X				(192)		// �e����X���W
#define POS_HOLE_Y				(256)		// �e����Y���W

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTitle(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureTitleBG = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9	g_pD3DTextureTitleBGMono = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9	g_pD3DTextureTitleBGChroma = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9	g_pD3DTextureTitleHole = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9	g_pD3DTextureTitleChroma = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9	g_pD3DTextureTitleText = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9	g_pD3DTextureTitleVersion = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9	g_pD3DTextureTitleLogo[LOGO_MAX] = {};	// �e�N�X�`���ւ̃|�C���^

VERTEX_2D		g_vertexWkTitleBG[NUM_VERTEX];				// ���_���i�[���[�N
VERTEX_2D		g_vertexWkTitleBGMono[NUM_VERTEX];			// ���_���i�[���[�N
VERTEX_2D		g_vertexWkTitleBGChroma[NUM_VERTEX];		// ���_���i�[���[�N
VERTEX_2D		g_vertexWkTitleHole[NUM_VERTEX];			// ���_���i�[���[�N
VERTEX_2D		g_vertexWkTitleChroma[NUM_VERTEX];			// ���_���i�[���[�N
VERTEX_2D		g_vertexWkTitleText[NUM_VERTEX];			// ���_���i�[���[�N
VERTEX_2D		g_vertexWkTitleVersion[NUM_VERTEX];			// ���_���i�[���[�N
VERTEX_2D		g_vertexWkTitleLogo[NUM_VERTEX * LOGO_MAX];	// ���_���i�[���[�N

D3DXVECTOR3		g_posTitleLogo[LOGO_MAX];					// ���S�̈ʒu
int				g_stageState;								// �^�C�g���̏��
int				g_interval;									// �C���^�[�o��
int				g_staLogo;									// ���S�ԍ�(�J�n)
int				g_endLogo;									// ���S�ԍ�(�I��)
float			g_rotTitle;										// ��](�h�炬�Ɏg�p)

LPDIRECTSOUNDBUFFER8	g_pTitleBGM;						// BGM�p�o�b�t�@
LPDIRECTSOUNDBUFFER8	g_pTitleSE_Shot;					// SE�p�o�b�t�@
LPDIRECTSOUNDBUFFER8	g_pTitleSE_Wind[LOGO_MAX];			// SE�p�o�b�t�@

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitle(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TITLE_BG,					// �t�@�C���̖��O
			&g_pD3DTextureTitleBG);				// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TITLE_BG_MONO,				// �t�@�C���̖��O
			&g_pD3DTextureTitleBGMono);			// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TITLE_BG_CHROMA,			// �t�@�C���̖��O
			&g_pD3DTextureTitleBGChroma);		// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TITLE_HOLE,					// �t�@�C���̖��O
			&g_pD3DTextureTitleHole);			// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TITLE_CHROMA,				// �t�@�C���̖��O
			&g_pD3DTextureTitleChroma);			// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TITLE_TEXT,					// �t�@�C���̖��O
			&g_pD3DTextureTitleText);			// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TITLE_VERSION,				// �t�@�C���̖��O
			&g_pD3DTextureTitleVersion);		// �ǂݍ��ރ������[

		for (int i = 0; i < LOGO_MAX; i++)
		{
			D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
				TEXTURE_TITLE_LOGO[i],			// �t�@�C���̖��O
				&g_pD3DTextureTitleLogo[i]);	// �ǂݍ��ރ������[
		}
	}

	// �T�E���h�̃��[�h
	g_pTitleBGM = LoadSound(BGM_TITLE);
	g_pTitleSE_Shot = LoadSound(SE_BOMB);

	for (int i = 0; i < LOGO_MAX; i++)
	{
		// �T�E���h�̃��[�h
		g_pTitleSE_Wind[i] = LoadSound(SE_WIND);

		g_posTitleLogo[i] = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	}

	// ���_���̍쐬
	MakeVertexTitle();

	g_stageState = TITLE_IN;
	g_interval = 10;
	g_staLogo = g_endLogo = 0;
	g_rotTitle = 0.0f;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
{
	if (g_pD3DTextureTitleBG != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTextureTitleBG->Release();
		g_pD3DTextureTitleBG = NULL;
	}

	if (g_pD3DTextureTitleBGMono != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTextureTitleBGMono->Release();
		g_pD3DTextureTitleBGMono = NULL;
	}

	if (g_pD3DTextureTitleBGChroma != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTextureTitleBGChroma->Release();
		g_pD3DTextureTitleBGChroma = NULL;
	}

	if (g_pD3DTextureTitleChroma != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTextureTitleChroma->Release();
		g_pD3DTextureTitleChroma = NULL;
	}

	if (g_pD3DTextureTitleText != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTextureTitleText->Release();
		g_pD3DTextureTitleText = NULL;
	}

	if (g_pD3DTextureTitleVersion != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTextureTitleVersion->Release();
		g_pD3DTextureTitleVersion = NULL;
	}

	for (int i = 0; i < LOGO_MAX; i++)
	{
		if (g_pD3DTextureTitleLogo[i] != NULL)
		{	// �e�N�X�`���̊J��
			g_pD3DTextureTitleLogo[i]->Release();
			g_pD3DTextureTitleLogo[i] = NULL;
		}
	}

	if (g_pTitleBGM != NULL)
	{	// �T�E���h�̊J��
		g_pTitleBGM->Release();
		g_pTitleBGM = NULL;
	}

	if (g_pTitleSE_Shot != NULL)
	{// �T�E���h�̊J��
		g_pTitleSE_Shot->Release();
		g_pTitleSE_Shot = NULL;
	}
	for (int i = 0; i < LOGO_MAX; i++)
	{
		if (g_pTitleSE_Wind[i] != NULL)
		{// �T�E���h�̊J��
			g_pTitleSE_Wind[i]->Release();
			g_pTitleSE_Wind[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitle(void)
{
	if (IsAnyKeyTriggered() || IsAnyButtonTriggered(0)) 
	{
		switch (g_stageState)
		{
		case TITLE_IN:
			// �T�E���h�̍Đ�
			PlaySound(g_pTitleBGM, E_DS8_FLAG_LOOP);
			
			for (int i = 0; i < LOGO_MAX; i++)
			{
				g_posTitleLogo[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				// ���_���W�̐ݒ�
				g_vertexWkTitleLogo[0 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7), 32 + 96 * (i / 7), 0.0f);
				g_vertexWkTitleLogo[1 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7) + 256, 32 + 96 * (i / 7), 0.0f);
				g_vertexWkTitleLogo[2 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7), 32 + 96 * (i / 7) + 256, 0.0f);
				g_vertexWkTitleLogo[3 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7) + 256, 32 + 96 * (i / 7) + 256, 0.0f);

				// ���ˌ��̐ݒ�
				g_vertexWkTitleVersion[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				g_vertexWkTitleVersion[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				g_vertexWkTitleVersion[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				g_vertexWkTitleVersion[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			}
			g_interval = 255;

			// ���ˌ��̐ݒ�
			g_vertexWkTitleBG[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, g_interval);
			g_vertexWkTitleBG[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, g_interval);
			g_vertexWkTitleBG[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, g_interval);
			g_vertexWkTitleBG[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, g_interval);

			// ���ˌ��̐ݒ�
			g_vertexWkTitleHole[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 - g_interval);
			g_vertexWkTitleHole[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 - g_interval);
			g_vertexWkTitleHole[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 - g_interval);
			g_vertexWkTitleHole[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 - g_interval);

			g_stageState++;
			break;
		case TITLE_WAIT:
			g_stageState++;
			g_interval = 0;
			SetFade(FADE_W_OUT, STAGE_RESET);
			break;
		case TITLE_OUT:
			break;
		}
	}
	
	switch (g_stageState)
	{
	case TITLE_IN:
		g_interval--;
		if (g_interval <= 0 && g_endLogo < LOGO_MAX)
		{
			// �T�E���h�̍Đ�
			PlaySound(g_pTitleSE_Wind[g_endLogo], E_DS8_FLAG_NONE);

			g_interval = TITLE_INTERVAL_LOGO;
			g_endLogo++;
		}
		for (int i = g_staLogo; i < g_endLogo; i++)
		{
			g_posTitleLogo[i].x = g_posTitleLogo[i].x * TITLE_INTERVAL_LOGO / (TITLE_INTERVAL_LOGO + 1);

			if (g_posTitleLogo[i].x <= 0.1f)
			{
				g_posTitleLogo[i].x = 0.0f;
				g_staLogo++;
			}
			// ���_���W�̐ݒ�
			g_vertexWkTitleLogo[0 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7), 32 + 96 * (i / 7), 0.0f);
			g_vertexWkTitleLogo[1 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7) + 256, 32 + 96 * (i / 7), 0.0f);
			g_vertexWkTitleLogo[2 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7), 32 + 96 * (i / 7) + 256, 0.0f);
			g_vertexWkTitleLogo[3 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7) + 256, 32 + 96 * (i / 7) + 256, 0.0f);
		}
		if (g_staLogo == LOGO_MAX)
		{
			if (g_rotTitle == 0.0f)
			{
				// �T�E���h�̍Đ�
				PlaySound(g_pTitleSE_Shot, E_DS8_FLAG_NONE);

				// ���ˌ��̐ݒ�
				g_vertexWkTitleHole[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				g_vertexWkTitleHole[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				g_vertexWkTitleHole[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				g_vertexWkTitleHole[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			}

			g_rotTitle += D3DX_PI * 0.75;
			if (g_rotTitle < D3DX_PI * 50)
			{
				for (int i = 0; i < LOGO_MAX; i++)
				{
					g_posTitleLogo[i].x = (D3DX_PI * 50 - g_rotTitle) * cosf(g_rotTitle) * 0.25;
					g_posTitleLogo[i].y = (D3DX_PI * 50 - g_rotTitle) * sinf(g_rotTitle) * 0.5;

					// ���_���W�̐ݒ�
					g_vertexWkTitleLogo[0 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7), 32 + 96 * (i / 7), 0.0f);
					g_vertexWkTitleLogo[1 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7) + 256, 32 + 96 * (i / 7), 0.0f);
					g_vertexWkTitleLogo[2 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7), 32 + 96 * (i / 7) + 256, 0.0f);
					g_vertexWkTitleLogo[3 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7) + 256, 32 + 96 * (i / 7) + 256, 0.0f);
				}
			}
			else
			{
				// �T�E���h�̍Đ�
				PlaySound(g_pTitleBGM, E_DS8_FLAG_LOOP);

				for (int i = 0; i < LOGO_MAX; i++)
				{
					g_posTitleLogo[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				
					// ���_���W�̐ݒ�
					g_vertexWkTitleLogo[0 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7), 32 + 96 * (i / 7), 0.0f);
					g_vertexWkTitleLogo[1 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7) + 256, 32 + 96 * (i / 7), 0.0f);
					g_vertexWkTitleLogo[2 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7), 32 + 96 * (i / 7) + 256, 0.0f);
					g_vertexWkTitleLogo[3 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7) + 256, 32 + 96 * (i / 7) + 256, 0.0f);
				}

				// ���ˌ��̐ݒ�
				g_vertexWkTitleVersion[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				g_vertexWkTitleVersion[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				g_vertexWkTitleVersion[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				g_vertexWkTitleVersion[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

				g_interval = 0;
				g_stageState++;
			}
		}
		break;
	case TITLE_WAIT:
		if (g_interval < 255)
		{
			g_interval ++;

			// ���ˌ��̐ݒ�
			g_vertexWkTitleBG[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, g_interval);
			g_vertexWkTitleBG[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, g_interval);
			g_vertexWkTitleBG[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, g_interval);
			g_vertexWkTitleBG[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, g_interval);

			// ���ˌ��̐ݒ�
			g_vertexWkTitleHole[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 - g_interval);
			g_vertexWkTitleHole[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 - g_interval);
			g_vertexWkTitleHole[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 - g_interval);
			g_vertexWkTitleHole[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 - g_interval);
		}
		g_rotTitle += 0.02f;
		if (g_rotTitle > D3DX_PI * 2) g_rotTitle -= D3DX_PI * 2;

		// ���ˌ��̐ݒ�
		g_vertexWkTitleText[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 * (int)(1 + cosf(g_rotTitle * 2)));
		g_vertexWkTitleText[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 * (int)(1 + cosf(g_rotTitle * 2)));
		g_vertexWkTitleText[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 * (int)(1 + cosf(g_rotTitle * 2)));
		g_vertexWkTitleText[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 * (int)(1 + cosf(g_rotTitle * 2)));

		// ���ˌ��̐ݒ�
		g_vertexWkTitleChroma[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, (int)(255 * (1 - cosf(g_rotTitle)) / 2.0f));
		g_vertexWkTitleChroma[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, (int)(255 * (1 - cosf(g_rotTitle)) / 2.0f));
		g_vertexWkTitleChroma[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, (int)(255 * (1 - cosf(g_rotTitle)) / 2.0f));
		g_vertexWkTitleChroma[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, (int)(255 * (1 - cosf(g_rotTitle)) / 2.0f));

		// ���ˌ��̐ݒ�
		g_vertexWkTitleBGChroma[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, (int)(g_interval * (1 - cosf(g_rotTitle)) / 2.0f));
		g_vertexWkTitleBGChroma[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, (int)(g_interval * (1 - cosf(g_rotTitle)) / 2.0f));
		g_vertexWkTitleBGChroma[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, (int)(g_interval * (1 - cosf(g_rotTitle)) / 2.0f));
		g_vertexWkTitleBGChroma[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, (int)(g_interval * (1 - cosf(g_rotTitle)) / 2.0f));
		break;
	case TITLE_OUT:
		if (g_interval == 50)
		{
			// �T�E���h�̒�~
			StopSound(g_pTitleBGM);
		}
		else
		{
			g_interval++;
		}
		break;
	}
	
	PrintDebugProc("TITLE_STATE:%d\n", g_stageState);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureTitleBG);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleBG, sizeof(VERTEX_2D));


	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureTitleBGMono);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleBGMono, sizeof(VERTEX_2D));
	
	
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureTitleBGChroma);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleBGChroma, sizeof(VERTEX_2D));


	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureTitleHole);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleHole, sizeof(VERTEX_2D));


	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureTitleChroma);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleChroma, sizeof(VERTEX_2D));


	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureTitleText);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleText, sizeof(VERTEX_2D));


	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureTitleVersion);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleVersion, sizeof(VERTEX_2D));
	
	for (int i = 0; i < LOGO_MAX; i++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureTitleLogo[i]);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &g_vertexWkTitleLogo[i * NUM_VERTEX], sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexTitle(void)
{
	// ���_���W�̐ݒ�
	g_vertexWkTitleBG[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_vertexWkTitleBG[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	g_vertexWkTitleBG[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	g_vertexWkTitleBG[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkTitleBG[0].rhw =
	g_vertexWkTitleBG[1].rhw =
	g_vertexWkTitleBG[2].rhw =
	g_vertexWkTitleBG[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkTitleBG[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleBG[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleBG[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleBG[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkTitleBG[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitleBG[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitleBG[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitleBG[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// ���_���W�̐ݒ�
	g_vertexWkTitleBGMono[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_vertexWkTitleBGMono[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	g_vertexWkTitleBGMono[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	g_vertexWkTitleBGMono[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkTitleBGMono[0].rhw =
	g_vertexWkTitleBGMono[1].rhw =
	g_vertexWkTitleBGMono[2].rhw =
	g_vertexWkTitleBGMono[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkTitleBGMono[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleBGMono[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleBGMono[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleBGMono[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkTitleBGMono[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitleBGMono[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitleBGMono[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitleBGMono[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// ���_���W�̐ݒ�
	g_vertexWkTitleBGChroma[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_vertexWkTitleBGChroma[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	g_vertexWkTitleBGChroma[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	g_vertexWkTitleBGChroma[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkTitleBGChroma[0].rhw =
	g_vertexWkTitleBGChroma[1].rhw =
	g_vertexWkTitleBGChroma[2].rhw =
	g_vertexWkTitleBGChroma[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkTitleBGChroma[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleBGChroma[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleBGChroma[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleBGChroma[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkTitleBGChroma[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitleBGChroma[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitleBGChroma[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitleBGChroma[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// ���_���W�̐ݒ�
	g_vertexWkTitleHole[0].vtx = D3DXVECTOR3(POS_HOLE_X - 256, POS_HOLE_Y - 256, 0.0f);
	g_vertexWkTitleHole[1].vtx = D3DXVECTOR3(POS_HOLE_X + 256, POS_HOLE_Y - 256, 0.0f);
	g_vertexWkTitleHole[2].vtx = D3DXVECTOR3(POS_HOLE_X - 256, POS_HOLE_Y + 256, 0.0f);
	g_vertexWkTitleHole[3].vtx = D3DXVECTOR3(POS_HOLE_X + 256, POS_HOLE_Y + 256, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkTitleHole[0].rhw =
	g_vertexWkTitleHole[1].rhw =
	g_vertexWkTitleHole[2].rhw =
	g_vertexWkTitleHole[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkTitleHole[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleHole[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleHole[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleHole[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkTitleHole[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitleHole[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitleHole[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitleHole[3].tex = D3DXVECTOR2(1.0f, 1.0f);



	// ���_���W�̐ݒ�
	g_vertexWkTitleChroma[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_vertexWkTitleChroma[1].vtx = D3DXVECTOR3(1024, 0.0f, 0.0f);
	g_vertexWkTitleChroma[2].vtx = D3DXVECTOR3(0.0f, 512, 0.0f);
	g_vertexWkTitleChroma[3].vtx = D3DXVECTOR3(1024, 512, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkTitleChroma[0].rhw =
	g_vertexWkTitleChroma[1].rhw =
	g_vertexWkTitleChroma[2].rhw =
	g_vertexWkTitleChroma[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkTitleChroma[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleChroma[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleChroma[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleChroma[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkTitleChroma[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitleChroma[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitleChroma[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitleChroma[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// ���_���W�̐ݒ�
	g_vertexWkTitleText[0].vtx = D3DXVECTOR3(SCREEN_CENTER_X - 256, SCREEN_HEIGHT - 224, 0.0f);
	g_vertexWkTitleText[1].vtx = D3DXVECTOR3(SCREEN_CENTER_X + 256, SCREEN_HEIGHT - 224, 0.0f);
	g_vertexWkTitleText[2].vtx = D3DXVECTOR3(SCREEN_CENTER_X - 256, SCREEN_HEIGHT - 128, 0.0f);
	g_vertexWkTitleText[3].vtx = D3DXVECTOR3(SCREEN_CENTER_X + 256, SCREEN_HEIGHT - 128, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkTitleText[0].rhw =
	g_vertexWkTitleText[1].rhw =
	g_vertexWkTitleText[2].rhw =
	g_vertexWkTitleText[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkTitleText[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleText[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleText[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleText[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkTitleText[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitleText[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitleText[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitleText[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// ���_���W�̐ݒ�
	g_vertexWkTitleVersion[0].vtx = D3DXVECTOR3(SCREEN_WIDTH - 160, SCREEN_HEIGHT - 64, 0.0f);
	g_vertexWkTitleVersion[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 64, 0.0f);
	g_vertexWkTitleVersion[2].vtx = D3DXVECTOR3(SCREEN_WIDTH - 160, SCREEN_HEIGHT, 0.0f);
	g_vertexWkTitleVersion[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkTitleVersion[0].rhw =
	g_vertexWkTitleVersion[1].rhw =
	g_vertexWkTitleVersion[2].rhw =
	g_vertexWkTitleVersion[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkTitleVersion[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleVersion[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleVersion[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleVersion[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkTitleVersion[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitleVersion[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitleVersion[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitleVersion[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	for (int i = 0; i < LOGO_MAX; i++)
	{
		// ���_���W�̐ݒ�
		g_vertexWkTitleLogo[0 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7), 32 + 96 * (i / 7), 0.0f);
		g_vertexWkTitleLogo[1 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7) + 256, 32 + 96 * (i / 7), 0.0f);
		g_vertexWkTitleLogo[2 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7), 32 + 96 * (i / 7) + 256, 0.0f);
		g_vertexWkTitleLogo[3 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7) + 256, 32 + 96 * (i / 7) + 256, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		g_vertexWkTitleLogo[0 + i * NUM_VERTEX].rhw =
		g_vertexWkTitleLogo[1 + i * NUM_VERTEX].rhw =
		g_vertexWkTitleLogo[2 + i * NUM_VERTEX].rhw =
		g_vertexWkTitleLogo[3 + i * NUM_VERTEX].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		g_vertexWkTitleLogo[0 + i * NUM_VERTEX].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleLogo[1 + i * NUM_VERTEX].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleLogo[2 + i * NUM_VERTEX].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleLogo[3 + i * NUM_VERTEX].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		g_vertexWkTitleLogo[0 + i * NUM_VERTEX].tex = D3DXVECTOR2(0.0f, 0.0f);
		g_vertexWkTitleLogo[1 + i * NUM_VERTEX].tex = D3DXVECTOR2(1.0f, 0.0f);
		g_vertexWkTitleLogo[2 + i * NUM_VERTEX].tex = D3DXVECTOR2(0.0f, 1.0f);
		g_vertexWkTitleLogo[3 + i * NUM_VERTEX].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	return S_OK;
}

