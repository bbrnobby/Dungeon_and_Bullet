//=============================================================================
//
// �������� [font.h]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#ifndef _FONT_H_
#define _FONT_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_GAME_FONT		_T("data/TEXTURE/font001.png")	// �T���v���p�摜
#define TEXTURE_FONT_SIZE				(64)	// �e�N�X�`���T�C�Y

#define TEXTURE_PATTERN_FONT_DIVIDE_X	(35)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_FONT_DIVIDE_Y	(6)		// �A�j���p�^�[���̃e�N�X�`�����������iY)

#define FONT_BUFFER						(128)	// �\���ł��镶����

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct	// �t�H���g�\����
{
	bool			use;						// true:�g�p  false:���g�p
	D3DXVECTOR3		pos;						// �ʒu
	int				size;						// �傫��
	int				index;						// �����C���f�b�N�X

	int				duration;					// �A�j���[�V�����J�E���g

	LPDIRECT3DTEXTURE9	Texture;				// �e�N�X�`�����
	VERTEX_2D		vertexWk[NUM_VERTEX];		// ���_���i�[���[�N
} FONT;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitFont(int type);
void UninitFont(void);
void UpdateFont(void);
void DrawFont(void);

void SetString(char *string, float posX, float posY, int size, int interval);
void RefreshFont(void);

#endif
