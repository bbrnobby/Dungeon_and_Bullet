//=============================================================================
//
// �h���b�v���� [drop.cpp]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#include "drop.h"
#include "score.h"
#include "player.h"
#include "font.h"
#include "heart.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexDrop(int no);
void SetTextureDrop(int no);
void SetVertexDrop(int no);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureDrop = NULL;				// �e�N�X�`���ւ̃|���S��

DROP				dropWk[MAX_DROP];						// �h���b�v�\����
int					mesDrop[MAX_DROP];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitDrop(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DROP *drop = dropWk;					// �h���b�v�̃|�C���^�[��������

	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_DROP,				// �t�@�C���̖��O
			&g_pD3DTextureDrop);			// �ǂݍ��ރ������̃|�C���^
	}

	// �h���b�v�̏���������
	for (int i = 0; i < MAX_DROP; i++, drop++)
	{
		drop->use = false;										// �g�p
		drop->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ���W�f�[�^��������
		drop->vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		drop->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		drop->subVec = 0.0f;
		drop->height = 0.0f;
		drop->size = TEXTURE_DROP_SIZE;
		drop->type = 0;

		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_DROP_SIZE, TEXTURE_DROP_SIZE);
		drop->Radius = D3DXVec2Length(&temp);							// ���a��������
		drop->BaseAngle = atan2f(TEXTURE_DROP_SIZE, TEXTURE_DROP_SIZE);	// �p�x��������

		drop->Texture = g_pD3DTextureDrop;								// �e�N�X�`�����
		MakeVertexDrop(i);												// ���_���̍쐬

		mesDrop[i] = 0;
	}

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitDrop(void)
{
	if (g_pD3DTextureDrop != NULL)
	{	
		// �e�N�X�`���̊J��
		g_pD3DTextureDrop->Release();
		g_pD3DTextureDrop = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateDrop(void)
{
	DROP *drop = dropWk;

	for (int i = 0; i < MAX_DROP; i++, drop++)
	{
		// �g�p���Ă����ԂȂ�X�V����
		if (drop->use)			
		{
			drop->height += drop->subVec;
			if (drop->height <= 0)
			{
				drop->height = 0.0f;
				drop->subVec = 0.0f;
			}
			else
			{
				drop->pos += drop->vec;
				//drop->rot.z = 0.25f;
				drop->subVec -= 0.5f;
			}
			drop->pos.y -= drop->subVec;
			
			// �ړ���̍��W�Œ��_��ݒ�
			SetVertexDrop(i);	
		}
		if (mesDrop[i] > 0)
		{
			mesDrop[i]--;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawDrop(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DROP *drop = dropWk;

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureDrop);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_DROP; i++, drop++)
	{
		// �g�p���Ă����ԂȂ�`�悷��
		if (drop->use)					
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, drop->Texture);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, drop->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexDrop(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DROP *drop = &dropWk[no];

	// ���_���W�̐ݒ�
	SetVertexDrop(no);

	// rhw�̐ݒ�
	drop->vertexWk[0].rhw =
	drop->vertexWk[1].rhw =
	drop->vertexWk[2].rhw =
	drop->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	drop->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	drop->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	drop->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	drop->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	drop->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	drop->vertexWk[1].tex = D3DXVECTOR2(1.0f / DROP_MAX, 0.0f);
	drop->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	drop->vertexWk[3].tex = D3DXVECTOR2(1.0f / DROP_MAX, 1.0f);


	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureDrop(int no)
{
	DROP *drop = &dropWk[no];

	// �e�N�X�`�����W�̐ݒ�
	drop->vertexWk[0].tex = D3DXVECTOR2(drop->type * (1.0f / DROP_MAX), 0.0f);
	drop->vertexWk[1].tex = D3DXVECTOR2((drop->type + 1) * (1.0f / DROP_MAX), 0.0f);
	drop->vertexWk[2].tex = D3DXVECTOR2(drop->type * (1.0f / DROP_MAX), 1.0f);
	drop->vertexWk[3].tex = D3DXVECTOR2((drop->type + 1) * (1.0f /DROP_MAX), 1.0f);
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexDrop(int no)
{
	DROP *drop = &dropWk[no];
	D3DXVECTOR3 *posCamera = GetCameraPos();

	// ���_���W�̐ݒ�
	drop->vertexWk[0].vtx.x = drop->pos.x + posCamera->x - cosf(drop->BaseAngle + drop->rot.z) * drop->Radius;
	drop->vertexWk[0].vtx.y = drop->pos.y + posCamera->y - sinf(drop->BaseAngle + drop->rot.z) * drop->Radius;
	drop->vertexWk[0].vtx.z = 0.0f;

	drop->vertexWk[1].vtx.x = drop->pos.x + posCamera->x + cosf(drop->BaseAngle - drop->rot.z) * drop->Radius;
	drop->vertexWk[1].vtx.y = drop->pos.y + posCamera->y - sinf(drop->BaseAngle - drop->rot.z) * drop->Radius;
	drop->vertexWk[1].vtx.z = 0.0f;

	drop->vertexWk[2].vtx.x = drop->pos.x + posCamera->x - cosf(drop->BaseAngle - drop->rot.z) * drop->Radius;
	drop->vertexWk[2].vtx.y = drop->pos.y + posCamera->y + sinf(drop->BaseAngle - drop->rot.z) * drop->Radius;
	drop->vertexWk[2].vtx.z = 0.0f;

	drop->vertexWk[3].vtx.x = drop->pos.x + posCamera->x + cosf(drop->BaseAngle + drop->rot.z) * drop->Radius;
	drop->vertexWk[3].vtx.y = drop->pos.y + posCamera->y + sinf(drop->BaseAngle + drop->rot.z) * drop->Radius;
	drop->vertexWk[3].vtx.z = 0.0f;
}

DROP *GetDrop(int no)
{
	return (&dropWk[no]);
}

void SetDrop(float posX, float posY, int type, int size)
{
	DROP *drop = dropWk;

	for (int i = 0; i < MAX_DROP; i++, drop++)
	{
		if (!drop->use)
		{
			int theta = rand() % 360;
			drop->use = true;
			drop->pos.x = posX;
			drop->pos.y = posY;
			drop->vec.x = size * cosf(theta / 180.0f * D3DX_PI);
			drop->vec.y = size * sinf(theta / 180.0f * D3DX_PI);
			drop->subVec = 10.0f;
			drop->height = 5.0f;
			drop->type = type;									// �A�j���J�E���g��������

			//if (drop->type >= DROP_PISTOL && drop->type <= DROP_BIGHEART)
			//{
			//	drop->size = TEXTURE_DROP_SIZE;
			//}
			//else
			{
				drop->size = TEXTURE_DROP_SIZE / 4 * 3;
			}

			drop->pos.y -= drop->subVec;

			D3DXVECTOR2 temp = D3DXVECTOR2(drop->size, drop->size);
			drop->Radius = D3DXVec2Length(&temp);				// ���a��������
			drop->BaseAngle = atan2f(drop->size, drop->size);	// �p�x��������

			SetTextureDrop(i);
			SetVertexDrop(i);	// �ړ���̍��W�Œ��_��ݒ�

			return;
		}
	}
}

void PickDrop(DROP *drop)
{
	PLAYER *player = GetPlayer();
	//if (drop->type >= DROP_PISTOL && drop->type <= DROP_LASERGUN)
	//{
	//	SetString("�݂��������ł��@���߂��", SCREEN_CENTER_X, SCREEN_CENTER_Y, DROP_MES_SIZE, 1);
	//}
	for (int i = 0; i < MAX_DROP; i++)
	{
		if (mesDrop[i] == 0)
		{
			switch (drop->type)
			{
			case DROP_COIN001:
				SetString("�ǂ��̃R�C���@�{�P�O", SCREEN_CENTER_X, DROP_MES_SIZE * (i + 1), DROP_MES_SIZE, DROP_MES_INTERVVAL);
				AddScore(10);
				break;
			case DROP_COIN002:
				SetString("����̃R�C���@�{5�O", SCREEN_CENTER_X, DROP_MES_SIZE * (i + 1), DROP_MES_SIZE, DROP_MES_INTERVVAL);
				AddScore(50);
				break;
			case DROP_COIN003:
				SetString("����̃R�C���@�{�P0�O", SCREEN_CENTER_X, DROP_MES_SIZE * (i + 1), DROP_MES_SIZE, DROP_MES_INTERVVAL);
				AddScore(100);
				break;
			//case DROP_PISTOL:
			//case DROP_SHOTGUN:
			//case DROP_LASERGUN:
			//	return;
			case DROP_MINIHEART:
				SetString("���������n�[�g�@������傭+1", SCREEN_CENTER_X, DROP_MES_SIZE * (i + 1), DROP_MES_SIZE, DROP_MES_INTERVVAL);
				if (player->hp < MAX_HP * (HEART_DIVIDE - 1)) player->hp++;
				break;
			case DROP_BIGHEART:
				SetString("���������n�[�g�@������傭+�P�Q�[�W", SCREEN_CENTER_X, DROP_MES_SIZE * (i + 1), DROP_MES_SIZE, DROP_MES_INTERVVAL);
				player->hp += HEART_DIVIDE - 1;
				if (player->hp > MAX_HP * (HEART_DIVIDE - 1)) player->hp = MAX_HP * (HEART_DIVIDE - 1);
				break;
			case DROP_JEWEL001:
				SetString("�T�t�@�C�A�@�{150", SCREEN_CENTER_X, DROP_MES_SIZE * (i + 1), DROP_MES_SIZE, DROP_MES_INTERVVAL);
				AddScore(150);
				break;
			case DROP_JEWEL002:
				SetString("�G�������h�@�{200", SCREEN_CENTER_X, DROP_MES_SIZE * (i + 1), DROP_MES_SIZE, DROP_MES_INTERVVAL);
				AddScore(200);
				break;
			case DROP_JEWEL003:
				SetString("�g�p�[�Y�@+250", SCREEN_CENTER_X, DROP_MES_SIZE * (i + 1), DROP_MES_SIZE, DROP_MES_INTERVVAL);
				AddScore(250);
				break;
			case DROP_JEWEL004:
				SetString("���r�[�@�{300", SCREEN_CENTER_X, DROP_MES_SIZE * (i + 1), DROP_MES_SIZE, DROP_MES_INTERVVAL);
				AddScore(300);
				break;
			case DROP_JEWEL005:
				SetString("�_�C�������h�@�{500", SCREEN_CENTER_X, DROP_MES_SIZE * (i + 1), DROP_MES_SIZE, DROP_MES_INTERVVAL);
				AddScore(500);
				break;
			}
			mesDrop[i] = DROP_MES_INTERVVAL;
			drop->use = false;
			return;
		}
	}
}
