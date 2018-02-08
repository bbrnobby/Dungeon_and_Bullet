//=============================================================================
//
// �e���� [gun.cpp]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#include "main.h"
#include "gun.h"
#include "bullet.h"
#include "player.h"
#include "camera.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define INT_SHOT_PISTOL			(10)	// �s�X�g���̎ˌ��C���^�[�o��
#define INT_SHOT_SHOTOGUN		(30)	// �V���b�g�K���̎ˌ��C���^�[�o��
#define MAG_PISTOL				(10)	// �s�X�g���̃}�K�W���e��
#define MAG_SHOTGUN				(6)		// �V���b�g�K���̃}�K�W���e��
#define	MAG_MAX					(30)	// �ő�e�\����

#define TEXTURE_UI_GUN			_T("data/TEXTURE/gun000.png")	// �e�̃e�N�X�`��(UI����)
#define TEXTURE_UI_WINDOW		_T("data/TEXTURE/window001.png")	// �E�B���h�E�̃e�N�X�`��(UI����)
#define TEXTURE_UI_BULLET		_T("data/TEXTURE/bullet000.png")	// �e�̃e�N�X�`��(UI����)�@

#define TEXTURE_UI_GUN_SIZE			(128)	// �e(UI)�̃e�N�X�`���T�C�Y
#define TEXTURE_UI_WIN_SIZE			(144)	// �E�B���h�E�̃e�N�X�`���T�C�Y
#define TEXTURE_UI_BULLET_SIZE_X	(16)	// �e�e(UI)�̃e�N�X�`��X�T�C�Y
#define TEXTURE_UI_BULLET_SIZE_Y	(32)	// �e�e(UI)�̃e�N�X�`��Y�T�C�Y

#define POS_UI_GUN_X		(SCREEN_WIDTH - 30 - (TEXTURE_UI_GUN_SIZE + TEXTURE_UI_WIN_SIZE) / 2)	// �e(UI)��X���W
#define POS_UI_GUN_Y		(SCREEN_HEIGHT - 30 - (TEXTURE_UI_GUN_SIZE + TEXTURE_UI_WIN_SIZE) / 2)	// �e(UI)��Y���W
#define POS_UI_WINDOW_X		(SCREEN_WIDTH - 30 - TEXTURE_UI_WIN_SIZE)		// �E�B���h�E��X���W
#define POS_UI_WINDOW_Y		(SCREEN_HEIGHT - 30 - TEXTURE_UI_WIN_SIZE)		// �E�B���h�E��Y���W
#define POS_UI_BULLET_X		(SCREEN_WIDTH - 45 - TEXTURE_UI_BULLET_SIZE_X)		// �e�e(UI)��X���W
#define POS_UI_BULLET_Y		(SCREEN_HEIGHT - 45 - TEXTURE_UI_BULLET_SIZE_Y)		// �e�e(UI)��Y���W

const char *TEXTURE_GAME_GUN[] =	// �e�̃e�N�X�`��(�Q�[������)
{
	"data/TEXTURE/gun001.png",
	"data/TEXTURE/gun002.png",
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexGun(void);
void SetTextureGun(void);
void SetVertexGun(int dir);

void SetTextureGunUI(void);
void SetVertexGunUI(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureGun[GUN_MAX] = {};		// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		g_pD3DTextureUIGun;					// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		g_pD3DTextureUIBullet;				// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		g_pD3DTextureUIWindow;				// �e�N�X�`���ւ̃|���S��

GUN						gunWk;								// �e�\����
UI_GUN					uiGunWk;							// �e(UI)�\����
UI_WINDOW				uiWinWk;							// �E�B���h�E(UI)�\����
UI_BULLET				uiBulletWk[MAG_MAX];				// �e�e(UI)�\����
bool					g_bGun[GUN_MAX];					// �e�̏����t���O

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGun(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GUN *gun = &gunWk;							// �e�̃|�C���^
	UI_GUN *uiGun = &uiGunWk;					// �e(UI)�̃|�C���^
	UI_WINDOW *uiWin = &uiWinWk;				// �E�B���h�E(UI)�̃|�C���^
	UI_BULLET *uiBullet = uiBulletWk;			// �e�e(UI)�̃|�C���^

	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		for (int i = 0; i < GUN_MAX; i++)
		{
			D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
				TEXTURE_GAME_GUN[i],			// �t�@�C���̖��O
				&g_pD3DTextureGun[i]);			// �ǂݍ��ރ������̃|�C���^
		}

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
			TEXTURE_UI_GUN,					// �t�@�C���̖��O
			&g_pD3DTextureUIGun);			// �ǂݍ��ރ������̃|�C���^

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
			TEXTURE_UI_WINDOW,				// �t�@�C���̖��O
			&g_pD3DTextureUIWindow);		// �ǂݍ��ރ������̃|�C���^

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
			TEXTURE_UI_BULLET,				// �t�@�C���̖��O
			&g_pD3DTextureUIBullet);		// �ǂݍ��ރ������̃|�C���^

	}

	// �e�̏���������
	gun->use = false;
	gun->pos = D3DXVECTOR3(0.0f, TEXTURE_PLAYER_SIZE_Y / 4.0f, 0.0f);
	gun->subPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	gun->vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	gun->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	gun->dirRot = 0.0f;
	gun->subRot = 0.0f;
	gun->isLocked = false;

	D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_GUN_SIZE_X, TEXTURE_GUN_SIZE_Y);
	gun->Radius = D3DXVec2Length(&temp);								// �e�̔��a��������
	gun->BaseAngle = atan2f(TEXTURE_GUN_SIZE_Y, TEXTURE_GUN_SIZE_X);	// �e�̊p�x��������

	// �e(UI)�̏���������
	uiGun->use = false;
	uiGun->pos = D3DXVECTOR3(POS_UI_GUN_X, POS_UI_GUN_Y, 0.0f);
	uiGun->subPosX = 0.0f;
	uiGun->count = -1;
	uiGun->Texture = g_pD3DTextureUIGun;
	

	// �E�B���h�E(UI)�̏���������
	uiWin->pos = D3DXVECTOR3(POS_UI_WINDOW_X, POS_UI_WINDOW_Y, 0.0f);
	uiWin->Texture = g_pD3DTextureUIWindow;

	// �e�e(UI)�̏���������
	for (int i = 0; i < MAG_MAX; i++, uiBullet++)
	{
		uiBullet->use = false;
		uiBullet->pos = D3DXVECTOR3(POS_UI_BULLET_X, POS_UI_BULLET_Y, 0.0f);
		uiBullet->subPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		uiBullet->vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		uiBullet->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		uiBullet->vecRot = 0.0f;

		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_UI_BULLET_SIZE_X, TEXTURE_UI_BULLET_SIZE_Y);
		uiBullet->Radius = D3DXVec2Length(&temp);											// �e�̔��a��������
		uiBullet->BaseAngle = atan2f(TEXTURE_UI_BULLET_SIZE_Y, TEXTURE_UI_BULLET_SIZE_X);	// �e�̊p�x��������

		uiBullet->Texture = g_pD3DTextureUIBullet;
	}

	// ���_���̍쐬
	MakeVertexGun();	

	// �����t���O�̏�����
	for (int i = 0; i < GUN_MAX; i++)
	{
		g_bGun[i] = true;
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGun(void)
{
	for (int i = 0; i < GUN_MAX; i++)
	{
		if (g_pD3DTextureGun[i] != NULL)
		{	// �e�N�X�`���̊J��
			g_pD3DTextureGun[i]->Release();
			g_pD3DTextureGun[i] = NULL;
		}
	}

	if (g_pD3DTextureUIGun != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTextureUIGun->Release();
		g_pD3DTextureUIGun = NULL;
	}

	if (g_pD3DTextureUIWindow != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTextureUIWindow->Release();
		g_pD3DTextureUIWindow = NULL;
	}

	if (g_pD3DTextureUIBullet != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTextureUIBullet->Release();
		g_pD3DTextureUIBullet = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGun(void)
{
	GUN *gun = &gunWk;					
	PLAYER *player = GetPlayer();		
	UI_GUN *uiGun = &uiGunWk;
	UI_WINDOW *uiWin = &uiWinWk;
	UI_BULLET *uiBullet = uiBulletWk;

	// �v���C���[�̏�Ԃŏe�̕`��𔻒f
	switch (player->state)
	{
	case PLAYER_WALK:
	case PLAYER_LOCK:
		gun->use = true;
		break;
	case PLAYER_RUN:
		gun->use = false;
		break;
	}

	// �g�p���Ă����ԂȂ�X�V����
	if (gun->use)					
	{
		// �ː���Œ莞
		if (player->state != PLAYER_LOCK)
		{
			gun->isLocked = false;	// �e���Œ��
			gun->subRot = 0.0f;		// �΂߂̉�]�����Z�b�g
		}

		// �e��Œ莞
		if (!gun->isLocked)
		{
			gun->subPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			switch (player->dir)
			{
			case DIR_RIGHT:	// �E
				if (player->state == PLAYER_LOCK)
				{
					gun->isLocked = true;
					if (gun->type == GUN_SHOTGUN)
					{
						gun->subPos.x = -TEXTURE_PLAYER_SIZE_X * 0.125f;
						if (player->vec.y > 0)
						{
							gun->subPos.x += TEXTURE_PLAYER_SIZE_X * 0.125f;
							gun->subRot = D3DX_PI * 0.25f;
						}
						else if (player->vec.y < 0)
						{
							gun->subPos.x -= TEXTURE_PLAYER_SIZE_X * 0.125f;
							gun->subRot = -D3DX_PI * 0.25f;
						}
					}
					else
					{
						gun->subPos.x = TEXTURE_PLAYER_SIZE_X * 0.25f;
						if (player->vec.y > 0)
						{
							gun->subPos.x += TEXTURE_PLAYER_SIZE_X * 0.25f;
							gun->subRot = D3DX_PI * 0.25f;
						}
						else if (player->vec.y < 0)
						{
							gun->subPos.x -= TEXTURE_PLAYER_SIZE_X * 0.25f;
							gun->subRot = -D3DX_PI * 0.25f;
						}
					}
				}
				else
				{
					gun->subPos.x = TEXTURE_PLAYER_SIZE_X * 0.125;
					if (player->vec.y > 0)
					{
						gun->subPos.x += TEXTURE_PLAYER_SIZE_X * 0.125f;
						gun->subRot = D3DX_PI * 0.25f;
					}
					else if (player->vec.y < 0)
					{
						gun->subPos.x -= TEXTURE_PLAYER_SIZE_X * 0.125f;
						gun->subRot = -D3DX_PI * 0.25f;
					}
				}
				// �Ȃ߂炩�ɉ�]������
				if (gun->dirRot > D3DX_PI)
				{
					gun->dirRot -= D3DX_PI * 2;
				}
				break;
			case DIR_DOWN:	// ��
				gun->subRot = 0.0f;	// �΂߂̉�]�����Z�b�g
				if (player->state == PLAYER_LOCK)
				{
					gun->isLocked = true;
					if (gun->type == GUN_SHOTGUN)
					{
						gun->subPos.x = -TEXTURE_PLAYER_SIZE_X * 0.375f;
						gun->subPos.y = -TEXTURE_PLAYER_SIZE_Y * 0.125f;
					}
					else
					{
						gun->subPos.x = TEXTURE_PLAYER_SIZE_X * 0.125f;
						gun->subPos.y = TEXTURE_PLAYER_SIZE_Y * 0.375f;
					}
				}
				else
				{
					gun->subPos.x = -TEXTURE_PLAYER_SIZE_X * 0.25f;
					gun->subPos.y = TEXTURE_PLAYER_SIZE_Y * 0.25f;
				}
				break;
			case DIR_LEFT:	// ��
				if (player->state == PLAYER_LOCK)
				{
					gun->isLocked = true;
					if (gun->type == GUN_SHOTGUN)
					{
						gun->subPos.x = TEXTURE_PLAYER_SIZE_X * 0.125f;
						if (player->vec.y > 0)
						{
							gun->subPos.x -= TEXTURE_PLAYER_SIZE_X * 0.25f;
							gun->subPos.y -= TEXTURE_PLAYER_SIZE_Y * 0.25f;
							gun->subRot = -D3DX_PI * 0.25f;
						}
						else if (player->vec.y < 0)
						{
							gun->subPos.y += TEXTURE_PLAYER_SIZE_Y * 0.125f;
							gun->subRot = D3DX_PI * 0.25f;
						}
					}
					else
					{
						gun->subPos.x = -TEXTURE_PLAYER_SIZE_X * 0.375f;
						if (player->vec.y > 0)
						{
							gun->subPos.x -= TEXTURE_PLAYER_SIZE_X * 0.25f;
							gun->subRot = -D3DX_PI * 0.25f;
						}
						else if (player->vec.y < 0)
						{
							gun->subPos.x += TEXTURE_PLAYER_SIZE_X * 0.25f;
							gun->subPos.y -= TEXTURE_PLAYER_SIZE_Y * 0.25f;
							gun->subRot = D3DX_PI * 0.25f;
						}
					}
				}
				else
				{
					gun->subPos.x = -TEXTURE_PLAYER_SIZE_X * 0.125f;
					if (player->vec.y > 0)
					{
						gun->subPos.x -= TEXTURE_PLAYER_SIZE_X * 0.125f;
						gun->subRot = -D3DX_PI * 0.25f;
					}
					else if (player->vec.y < 0)
					{
						gun->subPos.x += TEXTURE_PLAYER_SIZE_X * 0.125f;
						gun->subPos.y -= TEXTURE_PLAYER_SIZE_Y * 0.125f;
						gun->subRot = D3DX_PI * 0.25f;
					}
				}
				break;
			case DIR_UP:	// ��
				gun->subRot = 0.0f;	// �΂߂̉�]�����Z�b�g
				if (player->state == PLAYER_LOCK)
				{
					gun->isLocked = true;
					if (gun->type == GUN_SHOTGUN)
					{
						gun->subPos.x = TEXTURE_PLAYER_SIZE_X * 0.375f;
						gun->subPos.y = TEXTURE_PLAYER_SIZE_Y * 0.25f;
					}
					else
					{
						gun->subPos.x = -TEXTURE_PLAYER_SIZE_X * 0.125f;
						gun->subPos.y = -TEXTURE_PLAYER_SIZE_Y * 0.25f;
					}
				}
				else
				{
					gun->subPos.x = TEXTURE_PLAYER_SIZE_X * 0.25f;
					gun->subPos.y = 0.0f;
				}
				// �Ȃ߂炩�ɉ�]������
				if (gun->dirRot < D3DX_PI)
				{
					gun->dirRot += D3DX_PI * 2;
				}
				break;
			}
		}
		// ��]�̍X�V
		gun->dirRot = (player->dir * D3DX_PI / 2 + gun->subRot + gun->dirRot * 5) / 6;
		gun->rot.z = player->rot.z * 2 + gun->dirRot;

		// �ʒu�̍X�V
		gun->pos.x = (gun->subPos.x + gun->pos.x * 2) / 3;
		gun->pos.y = (gun->subPos.y + player->subRot + gun->pos.y * 2) / 3;

		SetVertexGun(player->dir);	// �ړ���̍��W�Œ��_��ݒ�
	}
	// �ʒu�̍X�V
	if (TEXTURE_UI_GUN_SIZE * uiGun->count - uiGun->subPosX < 0.1f && uiGun->count % GUN_MAX == 0)
	{
		uiGun->subPosX = 0.0f;
		uiGun->count = 0;
	}
	else
	{
		uiGun->subPosX = (uiGun->subPosX * 3 + uiGun->count * TEXTURE_UI_GUN_SIZE) / 4;
	}
	PrintDebugProc("count:%d\n", uiGun->count);
	SetTextureGunUI();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGun(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GUN *gun = &gunWk;
	UI_GUN *uiGun = &uiGunWk;
	UI_WINDOW *uiWin = &uiWinWk;
	UI_BULLET *uiBullet = uiBulletWk;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < GUN_MAX; i++, gun++)
	{
		// �g�p���Ă����ԂȂ�`�悷��
		if (gun->use == true)	
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, gun->Texture);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, gun->vertexWk, sizeof(VERTEX_2D));
		}
	}

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, uiGun->Texture);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, uiGun->vertexWk, sizeof(VERTEX_2D));

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, uiWin->Texture);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, uiWin->vertexWk, sizeof(VERTEX_2D));

	for (int i = 0; i < MAG_MAX; i++, uiBullet++)
	{
		if (uiBullet->use)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, uiBullet->Texture);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, uiBullet->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexGun(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GUN *gun = &gunWk;
	UI_GUN *uiGun = &uiGunWk;
	UI_WINDOW *uiWin = &uiWinWk;
	UI_BULLET *uiBullet = uiBulletWk;

	// ���_���W�̐ݒ�
	SetVertexGun(GetPlayer()->dir);
	SetVertexGunUI();

	// rhw�̐ݒ�
	gun->vertexWk[0].rhw =
	gun->vertexWk[1].rhw =
	gun->vertexWk[2].rhw =
	gun->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	gun->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	gun->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	gun->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	gun->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// rhw�̐ݒ�
	uiGun->vertexWk[0].rhw =
	uiGun->vertexWk[1].rhw =
	uiGun->vertexWk[2].rhw =
	uiGun->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	uiGun->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	uiGun->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	uiGun->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	uiGun->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// rhw�̐ݒ�
	uiWin->vertexWk[0].rhw =
	uiWin->vertexWk[1].rhw =
	uiWin->vertexWk[2].rhw =
	uiWin->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	uiWin->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	uiWin->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	uiWin->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	uiWin->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	for (int i = 0; i < MAG_MAX; i++, uiBullet++)
	{
		// ���ˌ��̐ݒ�
		uiBullet->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		uiBullet->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		uiBullet->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		uiBullet->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// rhw�̐ݒ�
		uiBullet->vertexWk[0].rhw =
		uiBullet->vertexWk[1].rhw =
		uiBullet->vertexWk[2].rhw =
		uiBullet->vertexWk[3].rhw = 1.0f;
	}

	// �e�N�X�`�����W�̐ݒ�
	SetTextureGun();
	SetTextureGunUI();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureGun(void)
{
	GUN *gun = &gunWk;

	// �e�N�X�`�����W�̐ݒ�
	gun->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f);
	gun->vertexWk[1].tex = D3DXVECTOR2( 1.0f, 0.0f);
	gun->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f);
	gun->vertexWk[3].tex = D3DXVECTOR2( 1.0f, 1.0f);
}

void SetTextureGunUI(void)
{
	UI_GUN *uiGun = &uiGunWk;
	UI_WINDOW *uiWin = &uiWinWk;
	UI_BULLET *uiBullet = uiBulletWk;

	// �e�N�X�`�����W�̐ݒ�
	float startX = uiGun->subPosX / TEXTURE_UI_GUN_SIZE / GUN_MAX;
	float sizeX = 1.0f / GUN_MAX;

	uiGun->vertexWk[0].tex = D3DXVECTOR2(startX, 0.0f);
	uiGun->vertexWk[1].tex = D3DXVECTOR2(startX + sizeX, 0.0f);
	uiGun->vertexWk[2].tex = D3DXVECTOR2(startX, 1.0f);
	uiGun->vertexWk[3].tex = D3DXVECTOR2(startX + sizeX, 1.0f);

	uiWin->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	uiWin->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	uiWin->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	uiWin->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	for (int i = 0; i < MAG_MAX; i++, uiBullet++)
	{
		float sizeX = 1.0f / GUN_MAX;

		uiBullet->vertexWk[0].tex = D3DXVECTOR2(uiBullet->type, 0.0f);
		uiBullet->vertexWk[1].tex = D3DXVECTOR2(uiBullet->type + sizeX, 0.0f);
		uiBullet->vertexWk[2].tex = D3DXVECTOR2(uiBullet->type, 1.0f);
		uiBullet->vertexWk[3].tex = D3DXVECTOR2(uiBullet->type + sizeX, 1.0f);
	}
}
//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexGun(int dir)
{
	GUN *gun = &gunWk;
	PLAYER *player = GetPlayer();
	D3DXVECTOR3 *posCamera = GetCameraPos();

	// ���_���W�̐ݒ�
	if (dir == DIR_LEFT)
	{
		gun->vertexWk[0].vtx.x = gun->pos.x + posCamera->x - cosf(gun->BaseAngle - gun->rot.z) * gun->Radius;
		gun->vertexWk[0].vtx.y = gun->pos.y + posCamera->y + sinf(gun->BaseAngle - gun->rot.z) * gun->Radius;
		gun->vertexWk[0].vtx.z = 0.0f;

		gun->vertexWk[1].vtx.x = gun->pos.x + posCamera->x + cosf(gun->BaseAngle + gun->rot.z) * gun->Radius;
		gun->vertexWk[1].vtx.y = gun->pos.y + posCamera->y + sinf(gun->BaseAngle + gun->rot.z) * gun->Radius;
		gun->vertexWk[1].vtx.z = 0.0f;

		gun->vertexWk[2].vtx.x = gun->pos.x + posCamera->x - cosf(gun->BaseAngle + gun->rot.z) * gun->Radius;
		gun->vertexWk[2].vtx.y = gun->pos.y + posCamera->y - sinf(gun->BaseAngle + gun->rot.z) * gun->Radius;
		gun->vertexWk[2].vtx.z = 0.0f;

		gun->vertexWk[3].vtx.x = gun->pos.x + posCamera->x + cosf(gun->BaseAngle - gun->rot.z) * gun->Radius;
		gun->vertexWk[3].vtx.y = gun->pos.y + posCamera->y - sinf(gun->BaseAngle - gun->rot.z) * gun->Radius;
		gun->vertexWk[3].vtx.z = 0.0f;
	}
	else
	{
		gun->vertexWk[0].vtx.x = gun->pos.x + posCamera->x - cosf(gun->BaseAngle + gun->rot.z) * gun->Radius;
		gun->vertexWk[0].vtx.y = gun->pos.y + posCamera->y - sinf(gun->BaseAngle + gun->rot.z) * gun->Radius;
		gun->vertexWk[0].vtx.z = 0.0f;

		gun->vertexWk[1].vtx.x = gun->pos.x + posCamera->x + cosf(gun->BaseAngle - gun->rot.z) * gun->Radius;
		gun->vertexWk[1].vtx.y = gun->pos.y + posCamera->y - sinf(gun->BaseAngle - gun->rot.z) * gun->Radius;
		gun->vertexWk[1].vtx.z = 0.0f;

		gun->vertexWk[2].vtx.x = gun->pos.x + posCamera->x - cosf(gun->BaseAngle - gun->rot.z) * gun->Radius;
		gun->vertexWk[2].vtx.y = gun->pos.y + posCamera->y + sinf(gun->BaseAngle - gun->rot.z) * gun->Radius;
		gun->vertexWk[2].vtx.z = 0.0f;

		gun->vertexWk[3].vtx.x = gun->pos.x + posCamera->x + cosf(gun->BaseAngle + gun->rot.z) * gun->Radius;
		gun->vertexWk[3].vtx.y = gun->pos.y + posCamera->y + sinf(gun->BaseAngle + gun->rot.z) * gun->Radius;
		gun->vertexWk[3].vtx.z = 0.0f;
	}

	gun->vertexWk[0].vtx += player->pos;
	gun->vertexWk[1].vtx += player->pos;
	gun->vertexWk[2].vtx += player->pos;
	gun->vertexWk[3].vtx += player->pos;
}

void SetVertexGunUI(void)
{
	UI_GUN *uiGun = &uiGunWk;
	UI_WINDOW *uiWin = &uiWinWk;
	UI_BULLET *uiBullet = uiBulletWk;

	// ���_���W�̐ݒ�
	uiGun->vertexWk[0].vtx.x = uiGun->pos.x;
	uiGun->vertexWk[0].vtx.y = uiGun->pos.y;
	uiGun->vertexWk[0].vtx.z = 0.0f;

	uiGun->vertexWk[1].vtx.x = uiGun->pos.x + TEXTURE_UI_GUN_SIZE;
	uiGun->vertexWk[1].vtx.y = uiGun->pos.y;
	uiGun->vertexWk[1].vtx.z = 0.0f;

	uiGun->vertexWk[2].vtx.x = uiGun->pos.x;
	uiGun->vertexWk[2].vtx.y = uiGun->pos.y + TEXTURE_UI_GUN_SIZE;
	uiGun->vertexWk[2].vtx.z = 0.0f;

	uiGun->vertexWk[3].vtx.x = uiGun->pos.x + TEXTURE_UI_GUN_SIZE;
	uiGun->vertexWk[3].vtx.y = uiGun->pos.y + TEXTURE_UI_GUN_SIZE;
	uiGun->vertexWk[3].vtx.z = 0.0f;

	// ���_���W�̐ݒ�
	uiWin->vertexWk[0].vtx.x = uiWin->pos.x;
	uiWin->vertexWk[0].vtx.y = uiWin->pos.y;
	uiWin->vertexWk[0].vtx.z = 0.0f;

	uiWin->vertexWk[1].vtx.x = uiWin->pos.x + TEXTURE_UI_WIN_SIZE;
	uiWin->vertexWk[1].vtx.y = uiWin->pos.y;
	uiWin->vertexWk[1].vtx.z = 0.0f;

	uiWin->vertexWk[2].vtx.x = uiWin->pos.x;
	uiWin->vertexWk[2].vtx.y = uiWin->pos.y + TEXTURE_UI_WIN_SIZE;
	uiWin->vertexWk[2].vtx.z = 0.0f;

	uiWin->vertexWk[3].vtx.x = uiWin->pos.x + TEXTURE_UI_WIN_SIZE;
	uiWin->vertexWk[3].vtx.y = uiWin->pos.y + TEXTURE_UI_WIN_SIZE;
	uiWin->vertexWk[3].vtx.z = 0.0f;

	for (int i = 0; i < MAG_MAX; i++, uiBullet++)
	{
		// ���_���W�̐ݒ�
		uiBullet->vertexWk[0].vtx.x = uiBullet->pos.x - cosf(uiBullet->BaseAngle + uiBullet->rot.z) * uiBullet->Radius;
		uiBullet->vertexWk[0].vtx.y = uiBullet->pos.y - sinf(uiBullet->BaseAngle + uiBullet->rot.z) * uiBullet->Radius;
		uiBullet->vertexWk[0].vtx.z = 0.0f;

		uiBullet->vertexWk[1].vtx.x = uiBullet->pos.x + cosf(uiBullet->BaseAngle - uiBullet->rot.z) * uiBullet->Radius;
		uiBullet->vertexWk[1].vtx.y = uiBullet->pos.y - sinf(uiBullet->BaseAngle - uiBullet->rot.z) * uiBullet->Radius;
		uiBullet->vertexWk[1].vtx.z = 0.0f;

		uiBullet->vertexWk[2].vtx.x = uiBullet->pos.x - cosf(uiBullet->BaseAngle - uiBullet->rot.z) * uiBullet->Radius;
		uiBullet->vertexWk[2].vtx.y = uiBullet->pos.y + sinf(uiBullet->BaseAngle - uiBullet->rot.z) * uiBullet->Radius;
		uiBullet->vertexWk[2].vtx.z = 0.0f;

		uiBullet->vertexWk[3].vtx.x = uiBullet->pos.x + cosf(uiBullet->BaseAngle + uiBullet->rot.z) * uiBullet->Radius;
		uiBullet->vertexWk[3].vtx.y = uiBullet->pos.y + sinf(uiBullet->BaseAngle + uiBullet->rot.z) * uiBullet->Radius;
		uiBullet->vertexWk[3].vtx.z = 0.0f;
	}
}

//=============================================================================
// �e�擾�֐�
//=============================================================================
GUN *GetGun()
{
	// �e�̃A�h���X��Ԃ�
	return(&gunWk);		
}

//=============================================================================
// �e�ݒ�֐�
//=============================================================================
void SetGun(int no)
{
	GUN *gun = &gunWk;
	UI_GUN *uiGun = &uiGunWk;
	PLAYER *player = GetPlayer();

	// �e�̐ݒ�
	if (g_bGun[no])
	{	// �������Ă��鎞
		gun->pos = D3DXVECTOR3(0.0f, TEXTURE_PLAYER_SIZE_Y / 4.0f, 0.0f);
		gun->type = no;
		gun->dirRot -= D3DX_PI;

		gun->subPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		switch (player->dir)

		{
		case DIR_RIGHT:	// �E
			if (gun->isLocked)
			{
				if (gun->type == GUN_SHOTGUN)
				{
					gun->subPos.x = -TEXTURE_PLAYER_SIZE_X / 8.0f;
					if (gun->subRot > 0)
					{
						gun->subPos.x += TEXTURE_PLAYER_SIZE_X / 8.0f;
					}
					else if (gun->subRot < 0)
					{
						gun->subPos.x -= TEXTURE_PLAYER_SIZE_X / 8.0f;
					}
				}
				else
				{
					gun->subPos.x = TEXTURE_PLAYER_SIZE_X / 4.0f;
					if (gun->subRot > 0)
					{
						gun->subPos.x += TEXTURE_PLAYER_SIZE_X / 4.0f;
					}
					else if (gun->subRot < 0)
					{
						gun->subPos.x -= TEXTURE_PLAYER_SIZE_X / 4.0f;
					}
				}
			}
			else
			{
				gun->subPos.x = TEXTURE_PLAYER_SIZE_X / 8.0f;
				if (gun->subRot > 0)
				{
					gun->subPos.x += TEXTURE_PLAYER_SIZE_X / 8.0f;
				}
				else if (gun->subRot < 0)
				{
					gun->subPos.x -= TEXTURE_PLAYER_SIZE_X / 8.0f;
				}
			}
			break;
		case DIR_DOWN:	// ��
			if (gun->isLocked)
			{
				if (gun->type == GUN_SHOTGUN)
				{
					gun->subPos.x = -TEXTURE_PLAYER_SIZE_X / 4.0f;
					gun->subPos.y = -TEXTURE_PLAYER_SIZE_Y / 8.0f;
				}
				else
				{
					gun->subPos.x = TEXTURE_PLAYER_SIZE_X / 8.0f;
					gun->subPos.y = TEXTURE_PLAYER_SIZE_Y / 8.0f * 3;
				}
			}
			else
			{
				gun->subPos.x = -TEXTURE_PLAYER_SIZE_X / 4.0f;
				gun->subPos.y = TEXTURE_PLAYER_SIZE_Y / 4.0f;
			}
			break;
		case DIR_LEFT:	// ��
			if (gun->isLocked)
			{
				if (gun->type == GUN_SHOTGUN)
				{
					gun->subPos.x = TEXTURE_PLAYER_SIZE_X / 8.0f;
					if (gun->subRot < 0)
					{
						gun->subPos.x -= TEXTURE_PLAYER_SIZE_X / 4.0f;
						gun->subPos.y -= TEXTURE_PLAYER_SIZE_Y / 4.0f;
					}
					else if (gun->subRot > 0)
					{
						gun->subPos.y += TEXTURE_PLAYER_SIZE_Y / 8.0f;
					}
				}
				else
				{
					gun->subPos.x = -TEXTURE_PLAYER_SIZE_X / 8.0f * 3;
					if (gun->subRot < 0)
					{
						gun->subPos.x -= TEXTURE_PLAYER_SIZE_X / 4.0f;
					}
					else if (gun->subRot > 0)
					{
						gun->subPos.x += TEXTURE_PLAYER_SIZE_X / 4.0f;
						gun->subPos.y -= TEXTURE_PLAYER_SIZE_Y / 4.0f;
					}
				}
			}
			else
			{
				gun->subPos.x = -TEXTURE_PLAYER_SIZE_X / 8.0f;
				if (gun->subRot < 0)
				{
					gun->subPos.x -= TEXTURE_PLAYER_SIZE_X / 8.0f;
				}
				else if (gun->subRot > 0)
				{
					gun->subPos.x += TEXTURE_PLAYER_SIZE_X / 8.0f;
					gun->subPos.y -= TEXTURE_PLAYER_SIZE_Y / 8.0f;
				}
			}
			break;
		case DIR_UP:	// ��
			if (gun->isLocked)
			{
				if (gun->type == GUN_SHOTGUN)
				{
					gun->subPos.x = TEXTURE_PLAYER_SIZE_X / 4.0f;
					gun->subPos.y = TEXTURE_PLAYER_SIZE_Y / 4.0f;
				}
				else
				{
					gun->subPos.x = -TEXTURE_PLAYER_SIZE_X / 8.0f;
					gun->subPos.y = -TEXTURE_PLAYER_SIZE_Y / 4.0f;
				}
			}
			else
			{
				gun->subPos.x = TEXTURE_PLAYER_SIZE_X / 4.0f;
				gun->subPos.y = 0.0f;
			}
			break;
		}

		uiGun->count++;

		MakeVertexGun();
		gun->Texture = g_pD3DTextureGun[no];	// �e�N�X�`�������X�V
	}
}

//=============================================================================
// �e���ˊ֐�
//=============================================================================
void SetShot()
{
	GUN *gun = &gunWk;
	PLAYER *player = GetPlayer();

	D3DXVECTOR3 pos = player->pos;

	switch (gun->type)
	{
	case GUN_PISTOL:
		pos.x += -TEXTURE_PLAYER_SIZE_X * ((2 - player->dir) % 2) * 0.2 + TEXTURE_PLAYER_SIZE_X * cosf(GetGun()->rot.z) * 0.9;
		pos.y += TEXTURE_PLAYER_SIZE_Y * ((1 + player->dir) % 2) * 0.2 + TEXTURE_PLAYER_SIZE_Y * sinf(GetGun()->rot.z) * 0.9;
		SetBullet(pos + GetGun()->pos, GetGun()->rot.z, PLAYER_BULLET_PISTOL);
		break;
	case GUN_SHOTGUN:
		pos.x += -TEXTURE_PLAYER_SIZE_X * ((2 - player->dir) % 2) * 0.125 + TEXTURE_PLAYER_SIZE_X * cosf(GetGun()->rot.z) * 1.5;
		pos.y += TEXTURE_PLAYER_SIZE_Y * ((1 + player->dir) % 2) * 0.125 + TEXTURE_PLAYER_SIZE_Y * sinf(GetGun()->rot.z) * 1.5;
		for (int i = 0; i < SLUG_MAX; i++)
		{
			float subRot = D3DX_PI * 0.25 * (rand() % 360) / 360 - D3DX_PI * 0.125;
			SetBullet(pos + GetGun()->pos, GetGun()->rot.z + subRot, PLAYER_BULLET_SHOTGUN);
		}
		break;
	}
}