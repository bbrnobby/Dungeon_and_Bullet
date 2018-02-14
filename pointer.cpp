//=============================================================================
//
// �|�C���^�[���� [pointer.cpp]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#include "pointer.h"
#include "enemy.h"
#include "camera.h"
#include "player.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_GAME_POINTER			_T("data/TEXTURE/pointer001.png")	// �摜
#define TEXTURE_POINTER_SIZE_X			(16)	// �e�N�X�`���T�C�Y
#define TEXTURE_POINTER_SIZE_Y			(16)	// ����

#define POINTER_TEXTURE_PATTERN_DIVIDE	(2)		// �A�j���p�^�[���̃e�N�X�`�����������iX)

#define POINTER_MAX						(10)	// �|�C���^�[�̍ő吔


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexPointer(int no);
void SetTexturePointer(int no, int cntPattern);
void SetVertexPointer(int no);
void SetDiffusePointer(int no, int fade);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTexturePointer = NULL;		// �e�N�X�`���ւ̃|���S��

POINTER					pointerWk[POINTER_MAX];				// �|�C���^�[�\����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPointer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	POINTER *pointer = pointerWk;			// �|�C���^�[�̃|�C���^

	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_POINTER,			// �t�@�C���̖��O
			&g_pD3DTexturePointer);			// �ǂݍ��ރ������̃|�C���^
	}

	// �|�C���^�[�̏���������
	for (int i = 0; i < POINTER_MAX; i++, pointer++)
	{
		pointer->use = false;										// �g�p
		pointer->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ���W�f�[�^��������
		pointer->subRot = 0.0f;
		pointer->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ��]�f�[�^��������

		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_POINTER_SIZE_X, TEXTURE_POINTER_SIZE_Y);
		pointer->Radius = D3DXVec2Length(&temp);					// �|�C���^�[�̔��a��������
		pointer->BaseAngle = atan2f(TEXTURE_POINTER_SIZE_Y, TEXTURE_POINTER_SIZE_X);	// �|�C���^�[�̊p�x��������

		pointer->Texture = g_pD3DTexturePointer;					// �e�N�X�`�����
		MakeVertexPointer(i);										// ���_���̍쐬
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPointer(void)
{
	if (g_pD3DTexturePointer != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTexturePointer->Release();
		g_pD3DTexturePointer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePointer(void)
{
	POINTER *pointer = pointerWk;		// �|�C���^�[�̃|�C���^�[��������
	PLAYER *player = GetPlayer();
	ENEMY *enemy;
	D3DXVECTOR3 cameraPos = *GetCameraPos();

	int fade;

	for (int i = 0; i < POINTER_MAX; i++, pointer++)
	{
		if (pointer->use)	
		{	// �g�p���Ă����ԂȂ�X�V����
			enemy = GetEnemy(pointer->no);

			if (enemy->state == ENEMY_DESPAWN || enemy->state == ENEMY_DEAD)
			{
				pointer->use = false;
				continue;
			}

			if (enemy->pos.x > -cameraPos.x &&
				enemy->pos.x < -cameraPos.x + SCREEN_WIDTH &&
				enemy->pos.y > -cameraPos.y &&
				enemy->pos.y < -cameraPos.y + SCREEN_HEIGHT)
			{
				fade = 0;
			}
			else
			{
				fade = 255;

				pointer->subRot = 0.02f;
				if (pointer->subRot > D3DX_PI)
				{
					pointer->subRot -= D3DX_PI;
				}

				pointer->rot.z = -D3DX_PI * 0.5 + atan2(enemy->pos.y - player->pos.y, enemy->pos.x - player->pos.x);

				pointer->pos.x = player->pos.x + cosf(pointer->rot.z + D3DX_PI * 0.5f) * TEXTURE_PLAYER_SIZE_X * 2;
				pointer->pos.y = player->pos.y + sinf(pointer->rot.z + D3DX_PI * 0.5f) * TEXTURE_PLAYER_SIZE_Y * 2;

				// �e�N�X�`�����W��ݒ�
				SetTexturePointer(i, (int)(sinf(pointer->subRot) + 0.3f));
			}
			// ���ˌ���ݒ�
			SetDiffusePointer(i, fade);
			
			// ���_��ݒ�
			SetVertexPointer(i);	
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPointer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	POINTER *pointer = pointerWk;		// �|�C���^�[�̃|�C���^

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < POINTER_MAX; i++, pointer++)
	{
		if (pointer->use)					
		{	// �g�p���Ă����ԂȂ�`�悷��
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, pointer->Texture);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, pointer->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexPointer(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	POINTER *pointer = &pointerWk[no];

	// ���_���W�̐ݒ�
	SetVertexPointer(no);

	// rhw�̐ݒ�
	pointer->vertexWk[0].rhw =
	pointer->vertexWk[1].rhw =
	pointer->vertexWk[2].rhw =
	pointer->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	pointer->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pointer->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pointer->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pointer->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	pointer->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pointer->vertexWk[1].tex = D3DXVECTOR2(1.0f / POINTER_TEXTURE_PATTERN_DIVIDE, 0.0f);
	pointer->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pointer->vertexWk[3].tex = D3DXVECTOR2(1.0f / POINTER_TEXTURE_PATTERN_DIVIDE, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTexturePointer(int no, int cntPattern)
{
	POINTER *pointer = &pointerWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern;
	float sizeX = 1.0f / POINTER_TEXTURE_PATTERN_DIVIDE;

	pointer->vertexWk[0].tex = D3DXVECTOR2(x * sizeX, 0.0f);
	pointer->vertexWk[1].tex = D3DXVECTOR2((x + 1) * sizeX, 0.0f);
	pointer->vertexWk[2].tex = D3DXVECTOR2(x * sizeX, 1.0f);
	pointer->vertexWk[3].tex = D3DXVECTOR2((x + 1) * sizeX, 1.0f);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexPointer(int no)
{
	POINTER *pointer = &pointerWk[no];
	D3DXVECTOR3 *posCamera = GetCameraPos();

	// ���_���W�̐ݒ�
	pointer->vertexWk[0].vtx.x = pointer->pos.x + posCamera->x - cosf(pointer->BaseAngle + pointer->rot.z) * pointer->Radius;
	pointer->vertexWk[0].vtx.y = pointer->pos.y + posCamera->y - sinf(pointer->BaseAngle + pointer->rot.z) * pointer->Radius;
	pointer->vertexWk[0].vtx.z = 0.0f;

	pointer->vertexWk[1].vtx.x = pointer->pos.x + posCamera->x + cosf(pointer->BaseAngle - pointer->rot.z) * pointer->Radius;
	pointer->vertexWk[1].vtx.y = pointer->pos.y + posCamera->y - sinf(pointer->BaseAngle - pointer->rot.z) * pointer->Radius;
	pointer->vertexWk[1].vtx.z = 0.0f;

	pointer->vertexWk[2].vtx.x = pointer->pos.x + posCamera->x - cosf(pointer->BaseAngle - pointer->rot.z) * pointer->Radius;
	pointer->vertexWk[2].vtx.y = pointer->pos.y + posCamera->y + sinf(pointer->BaseAngle - pointer->rot.z) * pointer->Radius;
	pointer->vertexWk[2].vtx.z = 0.0f;

	pointer->vertexWk[3].vtx.x = pointer->pos.x + posCamera->x + cosf(pointer->BaseAngle + pointer->rot.z) * pointer->Radius;
	pointer->vertexWk[3].vtx.y = pointer->pos.y + posCamera->y + sinf(pointer->BaseAngle + pointer->rot.z) * pointer->Radius;
	pointer->vertexWk[3].vtx.z = 0.0f;
}

void SetDiffusePointer(int no, int fade)
{
	POINTER *pointer = &pointerWk[no];

	// ���ˌ��̐ݒ�
	pointer->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, fade);
	pointer->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, fade);
	pointer->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, fade);
	pointer->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, fade);
}

//=============================================================================
// �|�C���^�[�擾�֐�
//=============================================================================
POINTER *GetPointer(int no)
{
	return(&pointerWk[no]);
}

//=============================================================================
// �|�C���^�[�ݒ�֐�
//=============================================================================
void SetPointer(int no)
{
	POINTER *pointer = pointerWk;

	for (int i = 0; i < POINTER_MAX; i++, pointer++)
	{
		if (!pointer->use)
		{
			pointer->use = true;
			pointer->no = no;

			return;			// 1�Z�b�g�����̂ŏI������
		}
	}
}
