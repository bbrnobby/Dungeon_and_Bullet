//=============================================================================
//
// �菈�� [hand.cpp]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#include "main.h"
#include "hand.h"
#include "player.h"
#include "gun.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
const char *TEXTURE_GAME_HAND[] =
{
	"data/TEXTURE/hand001.png",
	"data/TEXTURE/hand002.png"
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexHand();
void SetTextureHand();
void SetVertexHand(bool isLEFT);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureHand[GUN_MAX] = {};	// �e�N�X�`���ւ̃|���S��
HAND					handWk;								// ��\����


//=============================================================================
// ����������
//=============================================================================
HRESULT InitHand(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HAND *hand = &handWk;						// ��̃|�C���^

	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		for (int i = 0; i < GUN_MAX; i++)
		{
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
				TEXTURE_GAME_HAND[i],			// �t�@�C���̖��O
				&g_pD3DTextureHand[i]);			// �ǂݍ��ރ������̃|�C���^
		}
	}

	// ��̏���������
	hand->use = true;							
	hand->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	hand->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	hand->dirRot = 0.0f;
	hand->subRot = 0.0f;

	D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_HAND_SIZE_X, TEXTURE_HAND_SIZE_Y);
	hand->Radius = D3DXVec2Length(&temp);								// ��̔��a��������
	hand->BaseAngle = atan2f(TEXTURE_HAND_SIZE_Y, TEXTURE_HAND_SIZE_X);	// ��̊p�x��������

	hand->Texture = NULL;												// �e�N�X�`������������
	MakeVertexHand();													// ���_���̍쐬

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitHand(void)
{
	for (int i = 0; i < GUN_MAX; i++)
	{
		if (g_pD3DTextureHand != NULL)
		{	// �e�N�X�`���̊J��
			g_pD3DTextureHand[i]->Release();
			g_pD3DTextureHand[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateHand(void)
{
	HAND *hand = &handWk;			// ��̃|�C���^
	PLAYER *player = GetPlayer();	// �v���C���[�̃|�C���^
	GUN *gun = GetGun();			// �e�̃|�C���^

	// �v���C���[�̏�Ԃŏe�̕`��𔻒f
	switch (player->state)
	{
	case PLAYER_LOCK:
		hand->use = true;
		break;
	case PLAYER_WALK:
		hand->use = false;
		break;
	case PLAYER_RUN:
		hand->use = false;
		break;
	}

	// �g�p���Ă����ԂȂ�X�V����
	if (hand->use == true)			
	{
		hand->pos = gun->pos;
		hand->pos.x += TEXTURE_PLAYER_SIZE_X / 2 * ((2 - player->dir) % 2);
		hand->rot = gun->rot;
		hand->dirRot = gun->dirRot;
		hand->subRot = gun->subRot;

		hand->Texture = g_pD3DTextureHand[gun->type];	// �e�N�X�`�������X�V
		SetVertexHand(player->dir == DIR_LEFT);			// �ړ���̍��W�Œ��_��ݒ�
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawHand(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HAND *hand = &handWk;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �g�p���Ă����ԂȂ�`�悷��
	if (hand->use == true)			
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, hand->Texture);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, hand->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexHand(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HAND *hand = &handWk;

	// ���_���W�̐ݒ�
	SetVertexHand(false);

	// rhw�̐ݒ�
	hand->vertexWk[0].rhw =
	hand->vertexWk[1].rhw =
	hand->vertexWk[2].rhw =
	hand->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	hand->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	hand->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	hand->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	hand->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	SetTextureHand();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureHand()
{
	HAND *hand = &handWk;

	// �e�N�X�`�����W�̐ݒ�
	hand->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	hand->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	hand->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	hand->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexHand(bool isLEFT)
{
	PLAYER *player = GetPlayer();
	HAND *hand = &handWk;
	D3DXVECTOR3 *posCamera = GetCameraPos();

	// ���_���W�̐ݒ�
	if (isLEFT)
	{	// ���������Ă��鎞
		hand->vertexWk[0].vtx.x = hand->pos.x + posCamera->x - cosf(hand->BaseAngle - hand->rot.z) * hand->Radius;
		hand->vertexWk[0].vtx.y = hand->pos.y + posCamera->y + sinf(hand->BaseAngle - hand->rot.z) * hand->Radius;
		hand->vertexWk[0].vtx.z = 0.0f;

		hand->vertexWk[1].vtx.x = hand->pos.x + posCamera->x + cosf(hand->BaseAngle + hand->rot.z) * hand->Radius;
		hand->vertexWk[1].vtx.y = hand->pos.y + posCamera->y + sinf(hand->BaseAngle + hand->rot.z) * hand->Radius;
		hand->vertexWk[1].vtx.z = 0.0f;

		hand->vertexWk[2].vtx.x = hand->pos.x + posCamera->x - cosf(hand->BaseAngle + hand->rot.z) * hand->Radius;
		hand->vertexWk[2].vtx.y = hand->pos.y + posCamera->y - sinf(hand->BaseAngle + hand->rot.z) * hand->Radius;
		hand->vertexWk[2].vtx.z = 0.0f;

		hand->vertexWk[3].vtx.x = hand->pos.x + posCamera->x + cosf(hand->BaseAngle - hand->rot.z) * hand->Radius;
		hand->vertexWk[3].vtx.y = hand->pos.y + posCamera->y - sinf(hand->BaseAngle - hand->rot.z) * hand->Radius;
		hand->vertexWk[3].vtx.z = 0.0f;
	}
	else
	{
		hand->vertexWk[0].vtx.x = hand->pos.x + posCamera->x - cosf(hand->BaseAngle + hand->rot.z) * hand->Radius;
		hand->vertexWk[0].vtx.y = hand->pos.y + posCamera->y - sinf(hand->BaseAngle + hand->rot.z) * hand->Radius;
		hand->vertexWk[0].vtx.z = 0.0f;

		hand->vertexWk[1].vtx.x = hand->pos.x + posCamera->x + cosf(hand->BaseAngle - hand->rot.z) * hand->Radius;
		hand->vertexWk[1].vtx.y = hand->pos.y + posCamera->y - sinf(hand->BaseAngle - hand->rot.z) * hand->Radius;
		hand->vertexWk[1].vtx.z = 0.0f;

		hand->vertexWk[2].vtx.x = hand->pos.x + posCamera->x - cosf(hand->BaseAngle - hand->rot.z) * hand->Radius;
		hand->vertexWk[2].vtx.y = hand->pos.y + posCamera->y + sinf(hand->BaseAngle - hand->rot.z) * hand->Radius;
		hand->vertexWk[2].vtx.z = 0.0f;

		hand->vertexWk[3].vtx.x = hand->pos.x + posCamera->x + cosf(hand->BaseAngle + hand->rot.z) * hand->Radius;
		hand->vertexWk[3].vtx.y = hand->pos.y + posCamera->y + sinf(hand->BaseAngle + hand->rot.z) * hand->Radius;
		hand->vertexWk[3].vtx.z = 0.0f;
	}

	hand->vertexWk[0].vtx += player->pos;
	hand->vertexWk[1].vtx += player->pos;
	hand->vertexWk[2].vtx += player->pos;
	hand->vertexWk[3].vtx += player->pos;
}

//=============================================================================
// ��擾�֐�
//=============================================================================
HAND *GetHand()
{
	return(&handWk);
}


