//=============================================================================
//
// ���U���g��ʏ��� [result.h]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RESULT			_T("data/TEXTURE/bg001.jpg")			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RESULT_LOGO		_T("data/TEXTURE/result_logo.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_RESULT_IMAGE	_T("data/TEXTURE/kaizoku_takara.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	RESULT_POS_X			(0)			// ���U���g�w�i�̕\���ʒu
#define	RESULT_POS_Y			(0)			// ���U���g�w�i�̕\���ʒu
#define	RESULT_SIZE_X			(1080)		// ���U���g�w�i�̕�
#define	RESULT_SIZE_Y			(720)		// ���U���g�w�i�̍���

#define	RESULTLOGO_POS_X		(300)		// ���U���g���S�̕\���ʒu
#define	RESULTLOGO_POS_Y		(100)		// ���U���g���S�̕\���ʒu
#define	RESULTLOGO_SIZE_X		(480)		// ���U���g���S�̕�
#define	RESULTLOGO_SIZE_Y		(80)		// ���U���g���S�̍���

#define	RESULTIMAGE_POS_X		(380)		// ���U���g�摜�̕\���ʒu
#define	RESULTIMAGE_POS_Y		(350)		// ���U���g�摜�̕\���ʒu
#define	RESULTIMAGE_SIZE_X		(320)		// ���U���g�摜�̕�
#define	RESULTIMAGE_SIZE_Y		(320)		// ���U���g�摜�̍���

#define RESULTSCORE_POS_X		(380)		// ���U���g�X�R�A�̕\���ʒu
#define RESULTSCORE_POS_Y		(240)		// ���U���g�X�R�A�̕\���ʒu
#define RESULTSCORE_SIZE_X		(64)		// ���U���g�X�R�A�̕������T�C�Y
#define RESULTSCORE_SIZE_Y		(128)		// ���U���g�X�R�A�̕����c�T�C�Y
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

#endif
