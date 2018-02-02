//=============================================================================
//
// �~�j�}�b�v���� [minimap.cpp]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#include "minimap.h"
#include "player.h"
#include "camera.h"
#include "dungeon.h"
#include "particle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexMiniMap(void);
void SetVertexMiniMap(void);
void SetDiffuseMiniMap(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
VERTEX_2D		g_vertexWkMiniMap[MAP_WIDTH * MAP_HEIGHT][NUM_VERTEX];	// ���_���i�[���[�N
D3DXVECTOR3		g_posMiniMap;											// �~�j�}�b�v�̈ʒu
D3DXVECTOR3		g_scaleMiniMap;											// �p�l���̑傫��
bool			g_bMiniMap[MAP_WIDTH][MAP_HEIGHT];						// �~�j�}�b�v�`�攻��

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMiniMap(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posMiniMap = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_scaleMiniMap = D3DXVECTOR3(TEXTURE_MINIMAP_SIZE_X, TEXTURE_MINIMAP_SIZE_Y, 0.0f);

	for (int i = 0; i < MAP_WIDTH; i++)
	{

	}

	// ���_���̍쐬
	MakeVertexMiniMap();

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitMiniMap(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMiniMap(void)
{
	// �X�N���[������
	//g_posMiniMap = -GetPlayer(0)->pos + *GetCameraPos();
	g_posMiniMap = D3DXVECTOR3(30, SCREEN_HEIGHT - g_scaleMiniMap.y * MAP_HEIGHT - 30, 0.0f);
	SetVertexMiniMap();
	SetDiffuseMiniMap();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMiniMap(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ���C�e�B���O�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// �}�b�v�`�b�v
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkMiniMap[MAP_WIDTH * i + j], sizeof(VERTEX_2D));
		}
	}

	// ���C�e�B���O��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �ʏ�u�����h�ɖ߂�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexMiniMap(void)
{
	// �}�b�v�S�̂���������
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			// rhw�̐ݒ�
			g_vertexWkMiniMap[MAP_WIDTH * i + j][0].rhw =
			g_vertexWkMiniMap[MAP_WIDTH * i + j][1].rhw =
			g_vertexWkMiniMap[MAP_WIDTH * i + j][2].rhw =
			g_vertexWkMiniMap[MAP_WIDTH * i + j][3].rhw = 1.0f;

			// ���ˌ��̐ݒ�
			g_vertexWkMiniMap[MAP_WIDTH * i + j][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			g_vertexWkMiniMap[MAP_WIDTH * i + j][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			g_vertexWkMiniMap[MAP_WIDTH * i + j][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			g_vertexWkMiniMap[MAP_WIDTH * i + j][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
	}

	return S_OK;
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexMiniMap(void)
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			// ���_���W�̐ݒ�
			g_vertexWkMiniMap[MAP_WIDTH * i + j][0].vtx.x = g_scaleMiniMap.x * j;
			g_vertexWkMiniMap[MAP_WIDTH * i + j][0].vtx.y = g_scaleMiniMap.y * i;
			g_vertexWkMiniMap[MAP_WIDTH * i + j][0].vtx.z = 0.0f;
			g_vertexWkMiniMap[MAP_WIDTH * i + j][0].vtx += g_posMiniMap;

			g_vertexWkMiniMap[MAP_WIDTH * i + j][1].vtx.x = g_scaleMiniMap.x * (j + 1);
			g_vertexWkMiniMap[MAP_WIDTH * i + j][1].vtx.y = g_scaleMiniMap.y * i;
			g_vertexWkMiniMap[MAP_WIDTH * i + j][1].vtx.z = 0.0f;
			g_vertexWkMiniMap[MAP_WIDTH * i + j][1].vtx += g_posMiniMap;

			g_vertexWkMiniMap[MAP_WIDTH * i + j][2].vtx.x = g_scaleMiniMap.x * j;
			g_vertexWkMiniMap[MAP_WIDTH * i + j][2].vtx.y = g_scaleMiniMap.y * (i + 1);
			g_vertexWkMiniMap[MAP_WIDTH * i + j][2].vtx.z = 0.0f;
			g_vertexWkMiniMap[MAP_WIDTH * i + j][2].vtx += g_posMiniMap;

			g_vertexWkMiniMap[MAP_WIDTH * i + j][3].vtx.x = g_scaleMiniMap.x * (j + 1);
			g_vertexWkMiniMap[MAP_WIDTH * i + j][3].vtx.y = g_scaleMiniMap.y * (i + 1);
			g_vertexWkMiniMap[MAP_WIDTH * i + j][3].vtx.z = 0.0f;
			g_vertexWkMiniMap[MAP_WIDTH * i + j][3].vtx += g_posMiniMap;
		}
	}
}

//=============================================================================
// ���ˌ��̐ݒ�
//=============================================================================
void SetDiffuseMiniMap(void)
{
	D3DXCOLOR col = D3DCOLOR_RGBA(255, 255, 255, 255);
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			// ���ˌ��̐ݒ�
			switch (GetMap(i,j))
			{
			case MAP_NOT_OBJECT:
			case MAP_BOUNDARY:
			case MAP_WALL_UP:
			case MAP_WALL_DOWN:
			case MAP_WALL_RIGHT:
			case MAP_WALL_LEFT:
			case MAP_WALL_UPRIGHT:
			case MAP_WALL_UPLEFT:
			case MAP_WALL_DOWNLEFT:
			case MAP_WALL_DOWNRIGHT:
				col = D3DCOLOR_RGBA(0, 0, 0, 0);
				break;
			case MAP_ROOM:
			case MAP_START:
				if (GetRoom2(i, j) != NULL)
				{
					if (GetRoom2(i, j)->clear)
					{
						if (GetRoomID(i, j) == GetExitRoomID())
						{
							col = D3DCOLOR_RGBA(255, 255, 0, 128);
						}
						else
						{
							col = D3DCOLOR_RGBA(0, 255, 255, 128);
						}
					}
					else
					{
						col = D3DCOLOR_RGBA(0, 128, 255, 128);
					}
				}
				else

				{
					col = D3DCOLOR_RGBA(0, 128, 255, 128);

				}
				break;
			case MAP_TRAIL:
			case MAP_PATH:
				col = D3DCOLOR_RGBA(128, 0, 255, 128);
				break;
			case MAP_EXIT:
				col = D3DCOLOR_RGBA(255, 128, 0, 128);
				break;
			default:
				break;
			}

			g_vertexWkMiniMap[MAP_WIDTH * i + j][0].diffuse =
			g_vertexWkMiniMap[MAP_WIDTH * i + j][1].diffuse =
			g_vertexWkMiniMap[MAP_WIDTH * i + j][2].diffuse =
			g_vertexWkMiniMap[MAP_WIDTH * i + j][3].diffuse = col;
		}
	}
}
