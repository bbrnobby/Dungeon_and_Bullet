//=============================================================================
//
// �󔠏��� [treasure.cpp]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#include "treasure.h"
#include "player.h"
#include "drop.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTreasure(int no);
void SetTextureTreasure(int no);
void SetVertexTreasure(int no);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTreasure = NULL;					// �e�N�X�`���ւ̃|���S��

TREASURE				treasureWk[TREASURE_MAX];						// �󔠍\����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTreasure(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TREASURE *treasure = treasureWk;					// �󔠂̃|�C���^�[��������

	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_TREASURE,				// �t�@�C���̖��O
			&g_pD3DTextureTreasure);			// �ǂݍ��ރ������̃|�C���^
	}

	// �󔠂̏���������
	for (int i = 0; i < TREASURE_MAX; i++, treasure++)
	{
		treasure->use = false;										// �g�p
		treasure->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ���W�f�[�^��������
		treasure->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		treasure->vec = 0.0f;
		treasure->height = 0.0f;
		treasure->type = TREASURE_COMMON;
		treasure->state = BOX_NONE;
		treasure->size = 0;

		treasure->PatternAnim = 0;
		treasure->CountAnim = 0;										// �A�j���J�E���g��������

		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_TREASURE_SIZE, TEXTURE_TREASURE_SIZE);
		treasure->Radius = D3DXVec2Length(&temp);						// ���a��������
		treasure->BaseAngle = atan2f(TEXTURE_TREASURE_SIZE, TEXTURE_TREASURE_SIZE);	// �p�x��������

		treasure->Texture = g_pD3DTextureTreasure;						// �e�N�X�`�����
		MakeVertexTreasure(i);									// ���_���̍쐬
	}

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitTreasure(void)
{
	if (g_pD3DTextureTreasure != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTextureTreasure->Release();
		g_pD3DTextureTreasure = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTreasure(void)
{
	TREASURE *treasure = treasureWk;

	for (int i = 0; i < TREASURE_MAX; i++, treasure++)
	{
		if (treasure->use)			// �g�p���Ă����ԂȂ�X�V����
		{
			switch(treasure->state)
			{
			case BOX_SPAWNED:
				treasure->height += treasure->vec;
				if (treasure->height <= 0)
				{
					treasure->rot.z = 0.0f;
					treasure->height = 0.0f;
					treasure->vec = 0.0f;
					treasure->PatternAnim = 0;
					treasure->state = BOX_WAITING;
					// �e�N�X�`�����W��ݒ�
					SetTextureTreasure(i);
				}
				else
				{
					treasure->rot.z = (1 - treasure->PatternAnim) * 0.25f;
					treasure->vec -= 0.5f;
				}
			case BOX_OPENED:
				if (treasure->PatternAnim < TEXTURE_PATTERN_TREASURE_DIVIDE_Y - 2)
				{
					treasure->CountAnim++;
					if ((treasure->CountAnim % TREASURE_TIME_ANIMATION) == 0)
					{
						// �p�^�[���̐؂�ւ�
						treasure->PatternAnim++;
						treasure->CountAnim = 0;

						// �e�N�X�`�����W��ݒ�
						SetTextureTreasure(i);
					}
				}
				else if(treasure->state == BOX_OPENED)
				{
					int drop;
					for (int j = 0; j < treasure->size; j++)
					{
						switch (treasure->type)
						{
						case TREASURE_COMMON:
							drop = rand() % 3;
							break;
						//case TREASURE_GUNBOX:
						//	if (treasure->size == TEXTURE_TREASURE_SIZE / 4 * 3 / 8) drop = DROP_PISTOL;
						//	else if (treasure->size == TEXTURE_TREASURE_SIZE / 8) drop = DROP_SHOTGUN;
						//	else if (treasure->size == TEXTURE_TREASURE_SIZE / 2 * 3 / 8) drop = DROP_LASERGUN;
						//	j = treasure->size;
						//	break;
						case TREASURE_HEART:
							drop = rand() % 2 + DROP_MINIHEART;
							j++;
							break;
						case TREASURE_RARE:
							drop = rand() % 5 + DROP_JEWEL001;
							break;
						}
						SetDrop(treasure->pos.x, treasure->pos.y, drop, treasure->size * 0.5);
					}
					treasure->state = BOX_EMPTY;
				}
				break;
			}


			treasure->pos.y -= treasure->vec;

			SetVertexTreasure(i);	// �ړ���̍��W�Œ��_��ݒ�
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTreasure(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TREASURE *treasure = treasureWk;

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureTreasure);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < TREASURE_MAX; i++, treasure++)
	{
		if (treasure->use)					// �g�p���Ă����ԂȂ�`�悷��
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, treasure->Texture);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, treasure->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexTreasure(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TREASURE *treasure = &treasureWk[no];

	// ���_���W�̐ݒ�
	SetVertexTreasure(no);

	// rhw�̐ݒ�
	treasure->vertexWk[0].rhw =
	treasure->vertexWk[1].rhw =
	treasure->vertexWk[2].rhw =
	treasure->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	treasure->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	treasure->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	treasure->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	treasure->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	treasure->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	treasure->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_X, 0.0f);
	treasure->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_Y);
	treasure->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_X, 1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_Y);


	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureTreasure(int no)
{
	TREASURE *treasure = &treasureWk[no];

	// �e�N�X�`�����W�̐ݒ�
	treasure->vertexWk[0].tex = D3DXVECTOR2(treasure->type * (1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_X), treasure->PatternAnim * (1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_Y));
	treasure->vertexWk[1].tex = D3DXVECTOR2((treasure->type + 1) * (1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_X), treasure->PatternAnim * (1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_Y));
	treasure->vertexWk[2].tex = D3DXVECTOR2(treasure->type * (1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_X), (treasure->PatternAnim + 1) * (1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_Y));
	treasure->vertexWk[3].tex = D3DXVECTOR2((treasure->type + 1) * (1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_X), (treasure->PatternAnim + 1) * (1.0f / TEXTURE_PATTERN_TREASURE_DIVIDE_Y));
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexTreasure(int no)
{
	TREASURE *treasure = &treasureWk[no];
	D3DXVECTOR3 *posCamera = GetCameraPos();

	// ���_���W�̐ݒ�
	treasure->vertexWk[0].vtx.x = treasure->pos.x + posCamera->x - cosf(treasure->BaseAngle + treasure->rot.z) * treasure->Radius;;
	treasure->vertexWk[0].vtx.y = treasure->pos.y + posCamera->y - sinf(treasure->BaseAngle + treasure->rot.z) * treasure->Radius;
	treasure->vertexWk[0].vtx.z = 0.0f;

	treasure->vertexWk[1].vtx.x = treasure->pos.x + posCamera->x + cosf(treasure->BaseAngle - treasure->rot.z) * treasure->Radius;
	treasure->vertexWk[1].vtx.y = treasure->pos.y + posCamera->y - sinf(treasure->BaseAngle - treasure->rot.z) * treasure->Radius;
	treasure->vertexWk[1].vtx.z = 0.0f;

	treasure->vertexWk[2].vtx.x = treasure->pos.x + posCamera->x - cosf(treasure->BaseAngle - treasure->rot.z) * treasure->Radius;
	treasure->vertexWk[2].vtx.y = treasure->pos.y + posCamera->y + sinf(treasure->BaseAngle - treasure->rot.z) * treasure->Radius;
	treasure->vertexWk[2].vtx.z = 0.0f;

	treasure->vertexWk[3].vtx.x = treasure->pos.x + posCamera->x + cosf(treasure->BaseAngle + treasure->rot.z) * treasure->Radius;
	treasure->vertexWk[3].vtx.y = treasure->pos.y + posCamera->y + sinf(treasure->BaseAngle + treasure->rot.z) * treasure->Radius;
	treasure->vertexWk[3].vtx.z = 0.0f;
}

TREASURE *GetTreasure(int no)
{
	return (&treasureWk[no]);
}

void SetTreasure(float posX, float posY, int type, int size)
{
	TREASURE *treasure = treasureWk;

	for (int i = 0; i < TREASURE_MAX; i++, treasure++)
	{
		if (!treasure->use)
		{
			treasure->use = true;
			treasure->pos.x = posX;
			treasure->pos.y = posY;
			treasure->vec = 10.0f;
			treasure->height = 50.0f;
			treasure->type = type;
			treasure->state = BOX_SPAWNED;
			treasure->size = size / 8;

			treasure->PatternAnim = 0;
			treasure->CountAnim = 0;									// �A�j���J�E���g��������

			D3DXVECTOR2 temp = D3DXVECTOR2(size, size);
			treasure->Radius = D3DXVec2Length(&temp);					// ���a��������
			treasure->BaseAngle = atan2f(size, size);					// �p�x��������

			treasure->pos.y -= treasure->vec;

			SetVertexTreasure(i);	// �ړ���̍��W�Œ��_��ݒ�

			SetTextureTreasure(i);	// �e�N�X�`�����W��ݒ�
			return;
		}
	}
}