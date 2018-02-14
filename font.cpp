//=============================================================================
//
// �������� [font.cpp]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#include "font.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexFont(int no);
void SetTextureFont(int no);
void SetVertexFont(int no);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureFont = NULL;					// �e�N�X�`���ւ̃|���S��

FONT					fontWk[FONT_BUFFER];						// �t�H���g�\����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitFont(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FONT *font = fontWk;					// �t�H���g�̃|�C���^�[��������

	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_FONT,				// �t�@�C���̖��O
			&g_pD3DTextureFont);			// �ǂݍ��ރ������̃|�C���^
	}

	// �t�H���g�̏���������
	for (int i = 0; i < FONT_BUFFER; i++, font++)
	{
		font->use = false;										// �g�p
		font->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ���W�f�[�^��������
		font->size = TEXTURE_FONT_SIZE;
		font->index = 0;

		font->duration = 0;										// �A�j���J�E���g��������

		font->Texture = g_pD3DTextureFont;						// �e�N�X�`�����
		MakeVertexFont(i);										// ���_���̍쐬
	}

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitFont(void)
{
	if (g_pD3DTextureFont != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTextureFont->Release();
		g_pD3DTextureFont = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFont(void)
{
	FONT *font = fontWk;

	for (int i = 0; i < FONT_BUFFER; i++, font++)
	{
		if (font->use)			// �g�p���Ă����ԂȂ�X�V����
		{
			if (font->duration > 0) font->duration--;
			else if (font->duration == 0)
			{
				font->use = false;
			}
			SetVertexFont(i);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawFont(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FONT *font = fontWk;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < FONT_BUFFER; i++, font++)
	{
		if (font->use)					// �g�p���Ă����ԂȂ�`�悷��
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, font->Texture);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, font->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexFont( int no )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FONT *font = &fontWk[no];

	// ���_���W�̐ݒ�
	SetVertexFont(no);

	// rhw�̐ݒ�
	font->vertexWk[0].rhw =
	font->vertexWk[1].rhw =
	font->vertexWk[2].rhw =
	font->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	font->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	font->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	font->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	font->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	font->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	font->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_FONT_DIVIDE_X, 0.0f);
	font->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_FONT_DIVIDE_Y);
	font->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_FONT_DIVIDE_X, 1.0f / TEXTURE_PATTERN_FONT_DIVIDE_Y);


	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureFont(int no)
{
	FONT *font = &fontWk[no];

	int x,y;
	x = (TEXTURE_PATTERN_FONT_DIVIDE_X - 1) - (font->index / TEXTURE_PATTERN_FONT_DIVIDE_Y);
	y = font->index % TEXTURE_PATTERN_FONT_DIVIDE_Y;

	// �e�N�X�`�����W�̐ݒ�
	font->vertexWk[0].tex = D3DXVECTOR2(x * (1.0f / TEXTURE_PATTERN_FONT_DIVIDE_X), y * (1.0f / TEXTURE_PATTERN_FONT_DIVIDE_Y));
	font->vertexWk[1].tex = D3DXVECTOR2((x + 1) * (1.0f / TEXTURE_PATTERN_FONT_DIVIDE_X), y * (1.0f / TEXTURE_PATTERN_FONT_DIVIDE_Y));
	font->vertexWk[2].tex = D3DXVECTOR2(x * (1.0f / TEXTURE_PATTERN_FONT_DIVIDE_X), (y + 1) * (1.0f / TEXTURE_PATTERN_FONT_DIVIDE_Y));
	font->vertexWk[3].tex = D3DXVECTOR2((x + 1) * (1.0f / TEXTURE_PATTERN_FONT_DIVIDE_X), (y + 1) * (1.0f / TEXTURE_PATTERN_FONT_DIVIDE_Y));
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexFont(int no)
{
	FONT *font = &fontWk[no];
	D3DXVECTOR3 *subPosCamera = GetSubPosCamera();

	// ���_���W�̐ݒ�
	font->vertexWk[0].vtx.x = font->pos.x;
	font->vertexWk[0].vtx.y = font->pos.y;
	font->vertexWk[0].vtx.z = 0.0f;
	font->vertexWk[0].vtx += *subPosCamera;

	font->vertexWk[1].vtx.x = font->pos.x + font->size * 0.75f;
	font->vertexWk[1].vtx.y = font->pos.y;
	font->vertexWk[1].vtx.z = 0.0f;
	font->vertexWk[1].vtx += *subPosCamera;

	font->vertexWk[2].vtx.x = font->pos.x;
	font->vertexWk[2].vtx.y = font->pos.y + font->size;
	font->vertexWk[2].vtx.z = 0.0f;
	font->vertexWk[2].vtx += *subPosCamera;

	font->vertexWk[3].vtx.x = font->pos.x + font->size * 0.75f;
	font->vertexWk[3].vtx.y = font->pos.y + font->size;
	font->vertexWk[3].vtx.z = 0.0f;
	font->vertexWk[3].vtx += *subPosCamera;
}

void SetString(char *string, float posX, float posY, int size, int interval)
{
	char *buf;
	int length, index;

	for (length = 0, buf = string; *buf != '\0'; length++, buf++)
	{
		if (*buf == -125 || *buf == -126 || *buf == -127) buf++;
	}

	for (index = 0, buf = string; *buf != '\0'; index = 0, buf++)
	{
		if (*buf == -125)
		{
			buf++;
			// �u�@�v�`�u�~�v
			if (*buf >= 64 && *buf <= 126)
			{
				index = *buf + 20;
			}
			// �u���v�`�u���v
			else if (*buf >= -128 && *buf <= -108)
			{
				index = *buf + 275;
			}
		}
		else if (*buf == -126)
		{
			buf++;
			// �u���v�`�u��v
			if (*buf >= -97 && *buf <= -15)
			{
				index = *buf + 97;
			}
			// �u�O�v�`�u�X�v
			else if (*buf >= 79 && *buf <= 88)
			{
				index = *buf + 89;
			}
		}
		else if (*buf == -127)
		{
			buf++;
			// �u�@�v
			if (*buf == 64)
			{
				index = 83;
			}
			// �u�{�v
			else if (*buf == 123)
			{
				index = 207;
			}
			// �u�[�v�E�u�\�v�E�u�|�v
			else if (*buf == 91 || *buf == 92 || *buf == 124)
			{
				index = 206;
			}
			// �u�~�v�E�u���v
			else if (*buf == 126 || *buf == -106)
			{
				index = 208;
			}
			// �u���v�E�u�^�v
			else if (*buf == -128 || *buf == 94)
			{
				index = 209;
			}
			// �u�I�v
			else if (*buf == 73)
			{
				index = 178;
			}
			// �u�H�v
			else if (*buf == 72)
			{
				index = 179;
			}
		}
		// �u0�v�`�u9�v
		else if (*buf >= 48 && *buf <= 57)
		{
			index = *buf + 120;
		}
		// �uA�v�`�uZ�v
		else if (*buf >= 65 && *buf <= 90)
		{
			index = *buf + 115;
		}
		// �ua�v�`�uz�v
		else if (*buf >= 97 && *buf <= 122)
		{
			index = *buf + 83;
		}
		// �u �v
		else if (*buf == 32)
		{
			index = 83;
		}
		// �u+�v
		else if (*buf == 43)
		{
			index = 207;
		}
		// �u-�v
		else if (*buf == 45)
		{
			index = 206;
		}
		// �u*�v
		else if (*buf == 42)
		{
			index = 208;
		}
		// �u/�v
		else if (*buf == 47)
		{
			index = 209;
		}
		// �u!�v
		else if (*buf == 33)
		{
			index = 178;
		}
		// �u?�v
		else if (*buf == 63)
		{
			index = 179;
		}

		if (index != 0)
		{
			FONT *font = fontWk;
			for (int i = 0; i < FONT_BUFFER; i++, font++)
			{
				if (!font->use)
				{
					font->use = true;
					font->pos.x = posX + size * (-length * 0.5f)* 0.75f;
					font->pos.y = posY - size * 0.5f;
					font->size = size;
					font->index = index;

					font->duration = interval;

					SetTextureFont(i);
					SetVertexFont(i);
					break;
				}
			}
		}
		posX += size * 0.75f;
	}
}

void RefreshFont(void)
{
	FONT *font = fontWk;

	for (int i = 0; i < FONT_BUFFER; i++, font++)
	{
		if (font->use && font->duration > 0)
		{
			font->use = false;
			font->pos.x = 0.0f;
			font->pos.y = 0.0f;
			font->size = TEXTURE_FONT_SIZE;
			font->index = 0;

			font->duration = 0;	

			SetTextureFont(i);
			SetVertexFont(i);
		}
	}
}