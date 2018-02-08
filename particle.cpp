//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#include "particle.h"
#include "input.h"
#include "camera.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_PARTICLE	"data/TEXTURE/effect000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	PARTICLE_SIZE_X		(50.0f)							// �r���{�[�h�̕�
#define	PARTICLE_SIZE_Y		(50.0f)							// �r���{�[�h�̍���
#define	VALUE_MOVE_PARTICLE	(2.0f)							// �ړ����x

#define	PARTICLE_MAX		(10000)							// �p�[�e�B�N���ő吔

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexParticle(int no);
void SetVertexParticle(int no);
void SetColorParticle(int no, D3DXCOLOR col);
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float sizeX, float sizeY, int life);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureParticle = NULL;	// �e�N�X�`���ւ̃|�C���^
PARTICLE				particleWk[PARTICLE_MAX];		// �p�[�e�B�N�����[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitParticle(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PARTICLE *particle = particleWk;

	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
			TEXTURE_PARTICLE,					// �t�@�C���̖��O
			&g_pD3DTextureParticle);			// �ǂݍ��ރ������[
	}

	for(int i = 0; i < PARTICLE_MAX; i++, particle++)
	{
		particle->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		particle->vec = D3DXVECTOR3(PARTICLE_SIZE_X, PARTICLE_SIZE_Y, 1.0f);
		particle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		particle->scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		particle->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		particle->life = 0;
		particle->use = false;
		particle->gravity = false;

		// ���_���̍쐬
		MakeVertexParticle(i);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitParticle(void)
{
	if(g_pD3DTextureParticle != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureParticle->Release();
		g_pD3DTextureParticle = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateParticle(void)
{
	PARTICLE *particle = particleWk;

	for (int i = 0; i < PARTICLE_MAX; i++, particle++)
	{
		if (particle->use)
		{// �g�p��
			//particle->vec -= particle->vec * 0.25f;
			particle->pos += particle->vec;

			// ���_���W�̐ݒ�
			SetVertexParticle(i);

			particle->life--;
			if (particle->life <= 0)
			{
				particle->use = false;
			}
			else
			{
				if (particle->life <= PARTICLE_LIFE)
				{
					particle->col.r *= (float)(particle->life) / PARTICLE_LIFE;
					particle->col.g *= (float)(particle->life) / PARTICLE_LIFE;
					particle->col.b *= (float)(particle->life) / PARTICLE_LIFE;
				}

				if (particle->life <= PARTICLE_LIFE / 5)
				{
					// ���l�ݒ�
					particle->col.a -= 0.05f;
					if (particle->col.a < 0.0f)
					{
						particle->col.a = 0.0f;
					}
				}

				particle->scale *= (float) particle->life / (particle->life + 1);

				// �F�̐ݒ�
				SetColorParticle(i, particle->col);
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PARTICLE *particle = particleWk;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ���C�e�B���O�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for(int i = 0; i < PARTICLE_MAX; i++, particle++)
	{
		if(particle->use)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pD3DTextureParticle);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, particle->vertexWk, sizeof(VERTEX_2D));
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
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexParticle(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PARTICLE *particle = &particleWk[no];			// �p�[�e�B�N���̃|�C���^

	// ���_���W�̐ݒ�
	SetVertexParticle(no);

	// rhw�̐ݒ�
	particle->vertexWk[0].rhw =
	particle->vertexWk[1].rhw =
	particle->vertexWk[2].rhw =
	particle->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	particle->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	particle->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	particle->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	particle->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	particle->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	particle->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	particle->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	particle->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexParticle(int no)
{
	PARTICLE *particle = &particleWk[no];
	D3DXVECTOR3 *posCamera = GetCameraPos();

	// ���_���W�̐ݒ�
	particle->vertexWk[0].vtx.x = -particle->scale.x / 2 + particle->pos.x + posCamera->x;
	particle->vertexWk[0].vtx.y = -particle->scale.y / 2 + particle->pos.y + posCamera->y;
	particle->vertexWk[0].vtx.z = 0.0f;

	particle->vertexWk[1].vtx.x = particle->scale.x / 2 + particle->pos.x + posCamera->x;
	particle->vertexWk[1].vtx.y = -particle->scale.y / 2 + particle->pos.y + posCamera->y;
	particle->vertexWk[1].vtx.z = 0.0f;

	particle->vertexWk[2].vtx.x = -particle->scale.x / 2 + particle->pos.x + posCamera->x;
	particle->vertexWk[2].vtx.y = particle->scale.y / 2 + particle->pos.y + posCamera->y;
	particle->vertexWk[2].vtx.z = 0.0f;

	particle->vertexWk[3].vtx.x = particle->scale.x / 2 + particle->pos.x + posCamera->x;
	particle->vertexWk[3].vtx.y = particle->scale.y / 2 + particle->pos.y + posCamera->y;
	particle->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorParticle(int no, D3DXCOLOR col)
{
	PARTICLE *particle = &particleWk[no];

	// ���ˌ��̐ݒ�
	particle->vertexWk[0].diffuse = col;
	particle->vertexWk[1].diffuse = col;
	particle->vertexWk[2].diffuse = col;
	particle->vertexWk[3].diffuse = col;
}

//=============================================================================
// �p�[�e�B�N�����C���̍쐬
//=============================================================================
void SetParticleLine(D3DXVECTOR3 pos, D3DXVECTOR3 vec, D3DXCOLOR col, float sizeX, float sizeY, int life)
{
	if (sizeX*sizeX * 0.25f < vec.x*vec.x + vec.y*vec.y || sizeY*sizeY * 0.25f < vec.x*vec.x + vec.y*vec.y)
	{
		SetParticleLine(pos, vec / 2.0f, col, sizeX, sizeY, life - 1);
		SetParticleLine(pos - vec / 2.0f, vec / 2.0f, col, sizeX, sizeY, life - 2);
	}
	else
	{
		SetParticle(pos, col, sizeX, sizeY, life);
	}
}

//=============================================================================
// �p�[�e�B�N���̍쐬
//=============================================================================
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float sizeX, float sizeY, int life)
{
	PARTICLE *particle = particleWk;

	for (int i = 0; i < PARTICLE_MAX; i++, particle++)
	{
		if (!particle->use)
		{
			particle->pos = pos;
			particle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			particle->scale = D3DXVECTOR3(sizeX, sizeY, 1.0f);
			particle->vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			particle->col = col;
			particle->life = life;
			particle->use = true;

			// ���_���W�̐ݒ�
			SetVertexParticle(i);

			// �F�̐ݒ�
			SetColorParticle(i, col);

			return;
		}
	}
}

//=============================================================================
// �p�[�e�B�N���擾�֐�
//=============================================================================
PARTICLE *GetParticle(int no)
{
	return(&particleWk[no]);
}