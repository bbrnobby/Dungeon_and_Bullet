//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "gun.h"
#include "dungeon.h"
#include "heart.h"
#include "font.h"
#include "camera.h"
#include "particle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define TEXTURE_GAME_PLAYER	_T("data/TEXTURE/player001.png")	// �摜

#define PLAYER_TEXTURE_PATTERN_DIVIDE_X	(4)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define PLAYER_TEXTURE_PATTERN_DIVIDE_Y	(12)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define PLAYER_ANIM_PATTERN_NUM			(4)		// �A�j���[�V�����p�^�[����
#define PLAYER_TIME_ANIMATION			(8)		// �A�j���[�V�����̐؂�ւ��J�E���g
#define PLAYER_TIME_ANIMATION_RUN		(6)		// �A�j���[�V�����̐؂�ւ��J�E���g(����)

#define PLAYER_KNOCKBACK_SPEED		(15.0f)		// �v���C���[�̃m�b�N�o�b�N���x
#define PLAYER_FRICTION				(0.85f)		// �v���C���[�̖��C�W��
#define PLAYER_INTERVAL_INVINCIBLE	(200)		// �v���C���[�̖��G����


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexPlayer( void );
void SetTexturePlayer( int cntPattern );
void SetDiffusePLAYER( int alpha );
void SetVertexPlayer( void );

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTexturePlayer = NULL;	// �e�N�X�`���ւ̃|���S��
PLAYER				playerWk;					// �v���C���[�\����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = &playerWk;

	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_PLAYER,				// �t�@�C���̖��O
			&g_pD3DTexturePlayer);				// �ǂݍ��ރ������̃|�C���^
	}

	// �v���C���[�̏���������
	player->use = true;									
	player->isMove = false;								
	player->state = PLAYER_WALK;
	player->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		
	player->subRot = 0.0f;								
	player->vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		
	player->subVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		
	player->hp = 0;					
	player->gunType = GUN_PISTOL;
	player->invincible = false;							
	player->interval = 0;								
	player->dir = 0;									
	player->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		
	player->PatternAnim = 1;							
	player->CountAnim = 0;								
	player->roomID = -1;								
	player->movable = false;							

	D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_PLAYER_SIZE_X,
								TEXTURE_PLAYER_SIZE_Y);
	player->Radius = D3DXVec2Length(&temp);				// �v���C���[�̔��a��������
	player->BaseAngle = atan2f(TEXTURE_PLAYER_SIZE_Y,
								TEXTURE_PLAYER_SIZE_X);	// �v���C���[�̊p�x��������

	player->Texture = g_pD3DTexturePlayer;				// �e�N�X�`�����
	MakeVertexPlayer();									// ���_���̍쐬
	
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	if (g_pD3DTexturePlayer != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTexturePlayer->Release();
		g_pD3DTexturePlayer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	PLAYER *player = &playerWk;
	int roomID;

	// �g�p���Ă����ԂȂ�X�V����
	if (player->use == true)
	{
		// �x�N�g���f�[�^��������
		player->vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		if (player->movable)
		{
			// �L�[�{�[�h���͂ňړ�(�㉺�ƍ��E�̓��������͍l���Ȃ�)
			if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN))
			{	// ��
				if (!player->isMove)
				{
					player->isMove = true;
					player->CountAnim = 0;
				}
				if (player->state != PLAYER_LOCK)
				{
					player->dir = DIR_DOWN;
				}
				player->vec.y = PLAYER_SPEED;
			}
			else if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP))
			{	// ��
				if (!player->isMove)
				{
					player->isMove = true;
					player->CountAnim = 0;
				}
				if (player->state != PLAYER_LOCK)
				{
						player->dir = DIR_UP;
				}
				player->vec.y = -PLAYER_SPEED;
			}

			if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT))
			{	// �E
				if (!player->isMove)
				{
					player->isMove = true;
					player->CountAnim = 0;
				}
				if (player->state != PLAYER_LOCK)
				{
					player->dir = DIR_RIGHT;
				}
				player->vec.x = PLAYER_SPEED;
			}
			else if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT))
			{	// ��
				if (!player->isMove)
				{
					player->isMove = true;
					player->CountAnim = 0;
				}
				if (player->state != PLAYER_LOCK)
				{
					player->dir = DIR_LEFT;
				}
				player->vec.x = -PLAYER_SPEED;
			}

			// �΂ߏ���
			if (player->vec.x != 0.0f && player->vec.y != 0.0f)
			{
				// 45��
				player->vec.x /= 1.41421356f;
				player->vec.y /= 1.41421356f;
			}

			// �ː��Œ�
			if ((GetMapByPos(player->pos.x, player->pos.y) == MAP_ROOM)
			 &&(GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, BUTTON_Y) || IsButtonPressed(0, BUTTON_Z)))
			{				
				if (player->state == PLAYER_WALK)
				{	// ������Ԃ̎��A�\�����[�V�����ɕω�
					player->state = PLAYER_LOCK;
					SetTexturePlayer(player->PatternAnim + (player->dir + player->state * 4) * PLAYER_ANIM_PATTERN_NUM);
				}	
			}
			else if (player->state == PLAYER_LOCK)
			{	// ���������Ɏː��Œ���������A���[�V������ʏ�ɖ߂�
				player->state = PLAYER_WALK;
				player->rot.z = 0.0f;
				player->PatternAnim = 1;
				SetTexturePlayer(player->PatternAnim + player->dir * PLAYER_ANIM_PATTERN_NUM);
			}

			// ����
			if ((GetMapByPos(player->pos.x, player->pos.y) == MAP_PATH
				|| GetMapByPos(player->pos.x, player->pos.y) == MAP_TRAIL
				|| (GetRoom(GetRoomIDByPos(player->pos.x, player->pos.y))
					&& GetRoom(GetRoomIDByPos(player->pos.x, player->pos.y))->clear))
				&& (GetKeyboardPress(DIK_LCONTROL) || IsButtonPressed(0, BUTTON_B))) {
				if (player->state == PLAYER_WALK)
				{	// ������Ԃ̎��A���胂�[�V�����ɕω�
					player->state = PLAYER_RUN;
					player->PatternAnim = 1;
					SetTexturePlayer(player->PatternAnim + (player->dir + player->state * 4) * PLAYER_ANIM_PATTERN_NUM);
				}
			}
			else if (player->state == PLAYER_RUN)
			{	// ���������ɑ�����������A���[�V������ʏ�ɖ߂�
				player->state = PLAYER_WALK;
				player->rot.z = 0.0f;
				player->PatternAnim = 1;
				SetTexturePlayer(player->PatternAnim + player->dir * PLAYER_ANIM_PATTERN_NUM);
			}

			// �e����
			if (player->state != PLAYER_RUN && GetMapByPos(player->pos.x, player->pos.y) == MAP_ROOM && (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(0, BUTTON_X)))
			{
				SetShot();
			}

			// �Î~���͗������[�V������
			if (player->vec.x == 0.0f && player->vec.y == 0.0f)
			{
				if (player->state != PLAYER_LOCK) player->PatternAnim = 0;
				else player->PatternAnim = -1;
				player->CountAnim = 0;
			}

			// �A�j���[�V����
			if (player->isMove && ((player->CountAnim % PLAYER_TIME_ANIMATION) == 0 ||
				(player->state == PLAYER_RUN && player->CountAnim % (PLAYER_TIME_ANIMATION_RUN) == 0)))
			{
				// �p�^�[���̐؂�ւ�
				player->PatternAnim = (player->PatternAnim + 1) % PLAYER_ANIM_PATTERN_NUM;
				player->CountAnim = 0;

				// �e�N�X�`�����W��ݒ�
				SetTexturePlayer(player->PatternAnim + (player->dir + player->state * 4) * PLAYER_ANIM_PATTERN_NUM);
			}
			player->CountAnim++;

			if (player->invincible)
			{	// ���G���A�L�����N�^�[��_�ł�����
				SetDiffusePLAYER((player->interval % 2) * 255);
			}
			player->interval--;
			if (player->interval <= 0)
			{	// �C���^�[�o���I�����A���G����
				SetDiffusePLAYER(255);
				player->invincible = false;
			}

			if (player->vec.x == 0.0f && player->vec.y == 0.0f)
			{
				player->isMove = false;
			}

			// �����̕ω�
			switch (player->state)
			{
			case PLAYER_LOCK:
				player->vec = player->vec / 4 * 3;
				break;
			case PLAYER_RUN:
				player->vec = player->vec * 2;
				break;
			}

			// ���W�̍X�V
			if (GetMapByPos(player->pos.x + player->vec.x + player->subVec.x, player->pos.y) < MAP_ROOM)
			{
				player->vec.x = 0.0f;
				player->subVec.x = 0.0f;
			}
			if (GetMapByPos(player->pos.x, player->pos.y + player->vec.y + player->subVec.y) < MAP_ROOM)
			{
				player->vec.y = 0.0f;
				player->subVec.y = 0.0f;
			}

			player->subVec *= PLAYER_FRICTION;

		}
		else if (player->hp == 0)
		{	// �v���C���[�̗̑͂�0�ɂȂ����Ƃ�
			player->state = PLAYER_WALK;
			player->vec.x = 0.0f;
			player->vec.y = 0.0f;
			player->rot.z += PLAYER_SPEED;
			SetParticleLine(player->pos, player->subVec, D3DCOLOR_RGBA(128, 128, 128, 255), TEXTURE_PLAYER_SIZE_X, TEXTURE_PLAYER_SIZE_X, PARTICLE_LIFE);
		}
		else
		{
			player->PatternAnim = 0;
		}

		// �v���C���[�̈ʒu���X�V
		player->pos += player->vec + player->subVec;

		// �����ɓ��锻��
		roomID = GetRoomIDByPos(player->pos.x, player->pos.y);
		if (player->roomID != roomID)
		{
			if (roomID != -1) EnterRoom(roomID);
			else LeaveRoom(player->roomID);
			player->roomID = roomID;
		}

		// �c�h��
		switch (player->state)
		{
		case PLAYER_WALK:
		case PLAYER_LOCK:
			player->subRot = (-(player->PatternAnim % 2) * 3.0f + player->subRot * 3) / 4;
			break;
		case PLAYER_RUN:
			player->subRot = (-(player->PatternAnim % 2) * 6.0f + player->subRot * 3) / 4;
			break;
		}

		// ��]�̍X�V
		switch (player->state)
		{
		case PLAYER_WALK:
			player->rot.z = ((1.5f - player->PatternAnim) * 0.25f + player->rot.z * 20) / 21;
			break;
		case PLAYER_LOCK:
			player->rot.z = player->rot.z * 20 / 21;
			break;
		case PLAYER_RUN:
			player->rot.z = ((1.5f - player->PatternAnim) * 0.5f + ((1 - player->dir) % 2) * 0.3f + player->rot.z * 20) / 21;
			break;
		}

		if (!player->isMove && player->hp != 0)
		{
			player->rot.z = 0.0f;
		}

		// �ړ���̍��W�Œ��_��ݒ�
		SetVertexPlayer();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = &playerWk;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (player->use)
	{	// �g�p���Ă����ԂȂ�`�悷��
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, player->Texture);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, player->vertexWk, sizeof(VERTEX_2D));
	}

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = &playerWk;

	// ���_���W�̐ݒ�
	SetVertexPlayer();

	// rhw�̐ݒ�
	player->vertexWk[0].rhw =
	player->vertexWk[1].rhw =
	player->vertexWk[2].rhw =
	player->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	player->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	player->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	player->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	player->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	SetTexturePlayer(player->PatternAnim);
	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTexturePlayer( int cntPattern )
{
	PLAYER *player = &playerWk;
	
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % PLAYER_TEXTURE_PATTERN_DIVIDE_X;
	int y = cntPattern / PLAYER_TEXTURE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / PLAYER_TEXTURE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / PLAYER_TEXTURE_PATTERN_DIVIDE_Y;
	
	player->vertexWk[0].tex = D3DXVECTOR2(x * sizeX, y * sizeY);
	player->vertexWk[1].tex = D3DXVECTOR2((x + 1) * sizeX, y * sizeY);
	player->vertexWk[2].tex = D3DXVECTOR2(x * sizeX, (y + 1) * sizeY);
	player->vertexWk[3].tex = D3DXVECTOR2((x + 1) * sizeX, (y + 1) * sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexPlayer(void)
{
	PLAYER *player = &playerWk;
	D3DXVECTOR3 *posCamera = GetCameraPos();
	
	// ���_���W�̐ݒ�
	player->vertexWk[0].vtx.x = (int)(posCamera->x - cosf(player->BaseAngle + player->rot.z) * player->Radius);
	player->vertexWk[0].vtx.y = (int)(posCamera->y - sinf(player->BaseAngle + player->rot.z) * player->Radius + player->subRot);
	player->vertexWk[0].vtx.z = 0.0f;
	player->vertexWk[0].vtx	+= player->pos;

	player->vertexWk[1].vtx.x = (int)(posCamera->x + cosf(player->BaseAngle - player->rot.z) * player->Radius);
	player->vertexWk[1].vtx.y = (int)(posCamera->y - sinf(player->BaseAngle - player->rot.z) * player->Radius + player->subRot);
	player->vertexWk[1].vtx.z = 0.0f;
	player->vertexWk[1].vtx += player->pos;

	player->vertexWk[2].vtx.x = (int)(posCamera->x - cosf(player->BaseAngle - player->rot.z) * player->Radius);
	player->vertexWk[2].vtx.y = (int)(posCamera->y + sinf(player->BaseAngle - player->rot.z) * player->Radius + player->subRot);
	player->vertexWk[2].vtx.z = 0.0f;
	player->vertexWk[2].vtx += player->pos;

	player->vertexWk[3].vtx.x = (int)(posCamera->x + cosf(player->BaseAngle + player->rot.z) * player->Radius);
	player->vertexWk[3].vtx.y = (int)(posCamera->y + sinf(player->BaseAngle + player->rot.z) * player->Radius + player->subRot);
	player->vertexWk[3].vtx.z = 0.0f;
	player->vertexWk[3].vtx += player->pos;
}

//=============================================================================
// ���ˌ��̐ݒ�
//=============================================================================
void SetDiffusePLAYER(int alpha)
{
	PLAYER *player = &playerWk;	

	// ���ˌ��̐ݒ�
	player->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	player->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	player->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	player->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
}

//=============================================================================
// �v���C���[�擾�֐�
//=============================================================================
PLAYER *GetPlayer(void)
{
	return(&playerWk);
}

//=============================================================================
// �v���C���[�ݒ�֐�
//=============================================================================
void SetPlayer(float x, float y)
{
	PLAYER *player = &playerWk;

	player->isMove = false;
	player->state = PLAYER_WALK;

	player->pos.x = x;
	player->pos.y = y;

	player->subRot = 0.0f;
	player->vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player->dir = DIR_RIGHT;
	player->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player->PatternAnim = 0;
	player->CountAnim = 0;
	player->roomID = -1;
	player->subVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player->invincible = false;
	player->interval = MESSAGE_INTERVAL;
	player->hp = MAX_HP * (HEART_DIVIDE - 1);
	player->movable = true;

	SetGun(player->gunType);
}

void KnockBackPlayer(PLAYER *player, float rotZ)
{
	player->subVec.x += PLAYER_KNOCKBACK_SPEED * cosf(rotZ);
	player->subVec.y += PLAYER_KNOCKBACK_SPEED * sinf(rotZ);
}

void SetInvincible(PLAYER *player)
{
	player->invincible = true;
	player->interval = PLAYER_INTERVAL_INVINCIBLE;
}
