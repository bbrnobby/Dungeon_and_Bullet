//=============================================================================
//
// Main���� [main.cpp]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#include "main.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"
#include "pointer.h"
#include "bullet.h"
#include "gun.h"
#include "hand.h"
#include "bg.h"
#include "title.h"
#include "fade.h"
#include "result.h"
#include "score.h"
#include "resultScore.h"
#include "font.h"
#include "dungeon.h"
#include "sound.h"
#include "heart.h"
#include "treasure.h"
#include "drop.h"
#include "particle.h"
#include "minimap.h"
#include <time.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME			_T("AppClass")						// �E�C���h�E�̃N���X��
#define WINDOW_NAME			_T("Dungeon and Bullet ver1.3")		// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

void CheckHit(void);
bool CheckHitBB(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR2 size1, D3DXVECTOR2 size2);
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2);

void InitGame(void);

void UpdateDebugFont(void);

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
LPDIRECT3D9				g_pD3D = NULL;				// Direct3D�I�u�W�F�N�g
LPDIRECT3DDEVICE9		g_pD3DDevice = NULL;		// Device�I�u�W�F�N�g(�`��ɕK�v)

D3DPRESENT_PARAMETERS	d3dpp;

#ifdef _DEBUG
bool					g_bDispDebug = true;		// �f�o�b�O�\��
int						g_nCountFPS;				// FPS�J�E���^
#endif

int						g_nStage = 0;				// �X�e�[�W�ԍ�

LPDIRECTSOUNDBUFFER8	g_pBGM;						// BGM�p�o�b�t�@

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	// ���Ԍv���p
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	
	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW+1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND		hWnd;
	MSG			msg;
	
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindow(CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,																		// �E�B���h�E�̍����W
						CW_USEDEFAULT,																		// �E�B���h�E�̏���W
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME)*2,									// �E�B���h�E����
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME)*2+GetSystemMetrics(SM_CYCAPTION),	// �E�B���h�E�c��
						NULL,
						NULL,
						hInstance,
						NULL);

	// DirectX�̏�����(�E�B���h�E���쐬���Ă���s��)
	if(FAILED(Init(hInstance, hWnd, false)))
	{
		return -1;
	}

	// �t���[���J�E���g������
	timeBeginPeriod(1);	// ����\��ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// �V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime = dwFrameCount = 0;
	
	// �E�C���h�E�̕\��(Init()�̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// ���b�Z�[�W���[�v
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();					// �V�X�e���������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)		// 0.5�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);	// FPS�𑪒�
#endif
				dwFPSLastTime = dwCurrentTime;				// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;							// �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;			// ��������������ۑ�

				Update();								// �X�V����
				Draw();									// �`�揈��
				
				dwFrameCount++;							// �����񐔂̃J�E���g�����Z
			}
		}
	}
	
	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:					// [ESC]�L�[�������ꂽ
			DestroyWindow(hWnd);		// �E�B���h�E��j������悤�w������
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;

	srand((unsigned)time(NULL));

	// Direct3D�I�u�W�F�N�g�̍쐬
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	//ZeroMemory(&d3dpp, sizeof(d3dpp));							// ���[�N���[���N���A
	//d3dpp.BackBufferCount			= 1;						// �o�b�N�o�b�t�@�̐�
	//d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	//d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// �Q�[����ʃT�C�Y(����)
	//d3dpp.BackBufferFormat			= D3DFMT_UNKNOWN;			// �o�b�N�o�b�t�@�̃t�H�[�}�b�g�͌��ݐݒ肳��Ă�����̂��g��
	//d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����
	//d3dpp.Windowed					= bWindow;					// �E�B���h�E���[�h
	//d3dpp.EnableAutoDepthStencil	= TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	//d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	//d3dpp.BackBufferFormat			= d3ddm.Format;				// �J���[���[�h�̎w��

	//if(bWindow)
	//{// �E�B���h�E���[�h
	//	d3dpp.BackBufferFormat           = D3DFMT_UNKNOWN;					// �o�b�N�o�b�t�@
	//	d3dpp.FullScreen_RefreshRateInHz = 0;								// ���t���b�V�����[�g
	//	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
	//}
	//else
	//{// �t���X�N���[�����[�h
	//	d3dpp.BackBufferFormat           = D3DFMT_R5G6B5;					// �o�b�N�o�b�t�@
	//	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
	//	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	//}

	//// �f�o�C�X�̐���
	//// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	//// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	//if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,							// �f�B�X�v���C�A�_�v�^
	//								D3DDEVTYPE_HAL,								// �f�B�X�v���C�^�C�v
	//								hWnd,										// �t�H�[�J�X����E�C���h�E�ւ̃n���h��
	//								D3DCREATE_HARDWARE_VERTEXPROCESSING,		// �f�o�C�X�쐬����̑g�ݍ��킹
	//								&d3dpp,										// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
	//								&g_pD3DDevice)))							// �f�o�C�X�C���^�[�t�F�[�X�ւ̃|�C���^
	//{
	//	// ��L�̐ݒ肪���s������
	//	// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
	//	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
	//									D3DDEVTYPE_HAL, 
	//									hWnd, 
	//									D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
	//									&d3dpp,
	//									&g_pD3DDevice)))
	//	{
	//		// ��L�̐ݒ肪���s������
	//		// �`��ƒ��_������CPU�ōs�Ȃ�
	//		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
	//										D3DDEVTYPE_REF,
	//										hWnd, 
	//										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
	//										&d3dpp,
	//										&g_pD3DDevice)))
	//		{
	//			// ���������s
	//			return E_FAIL;
	//		}
	//	}
	//}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));				// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;						// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = SCREEN_WIDTH;			// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;			// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;			// �o�b�N�o�b�t�@�t�H�[�}�b�g�̓f�B�X�v���C���[�h�ɍ��킹�Ďg��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed = bWindow;						// �E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil = TRUE;			// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;		// �f�v�X�o�b�t�@�Ƃ���16bit���g��

	if (bWindow)
	{// �E�B���h�E���[�h
		d3dpp.FullScreen_RefreshRateInHz = 0;								// ���t���b�V�����[�g
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
	}
	else
	{// �t���X�N���[�����[�h
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	}

	// �f�o�C�X�I�u�W�F�N�g�̐���
	// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &g_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs�Ȃ�
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &g_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs�Ȃ�
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &g_pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}



	// �����_�����O�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// �J�����O���s��Ȃ�
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g�厞�̕�Ԑݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k�����̕�Ԑݒ�

	// �e�N�X�`���X�e�[�W���Z��������
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

#ifdef _DEBUG
	// �f�o�b�O�\�������̏�����
	InitDebugProc();
#endif

	// ���͏����̏�����
	InitInput(hInstance, hWnd);

	// �T�E���h������
	InitSound(hWnd);

	// �J�����̏�����
	InitCamera();

	// �v���C���[�̏���������
	InitPlayer(0);
	InitGun(0);
	InitHand(0);

	// �o���b�g�̏���������
	InitBullet(0);

	// BG������
	InitBG(0);

	// �^�C�g��������
	InitTitle(0);

	// ���U���g������
	InitResult();

	// �X�R�A������
	InitScore(0);
	InitResultScore(0);

	// �t�H���g������
	InitFont(0);

	// HP������
	InitHeart(0);

	// �_���W����������
	InitDungeon(0);

	// �G�l�~�[�̏���������
	InitEnemy(0);

	// �|�C���^�[�̏���������
	InitPointer(0);

	// �󔠂̏���������
	InitTreasure(0);

	// �h���b�v�̏���������
	InitDrop(0);

	// ��ʑJ�ڂ̏���������
	InitFade();

	// �p�[�e�B�N���̏���������
	InitParticle(0);

	// �~�j�}�b�v�̏���������
	InitMiniMap();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
#ifdef _DEBUG
	// �f�o�b�O�\�������̏I������
	UninitDebugProc();
#endif

	// ���͏����̏I������
	UninitInput();

	// �v���C���[�̏I������
	UninitPlayer();
	UninitGun();
	UninitHand();

	// �G�l�~�[�̏I������
	UninitEnemy();

	// �|�C���^�[�̏I������
	UninitPointer();

	// �|�C���^�[�̏I������
	UninitPointer();

	// �o���b�g�̏I������
	UninitBullet();

	// �󔠂̏I������
	UninitTreasure();

	// �h���b�v�̏I������
	UninitDrop();

	// BG�I������
	UninitBG();

	// �_���W�����I������
	UninitDungeon();

	// �^�C�g���I������
	UninitTitle();

	// ���U���g�I������
	UninitResult();
	
	// �X�R�A�I������
	UninitScore();

	// �t�H���g�I������
	UninitFont();

	// �p�[�e�B�N���I������
	UninitParticle();

	// �~�j�}�b�v�I������
	UninitMiniMap();

	// ��ʑJ�ڏI������
	UninitFade();	

	if (g_pD3DDevice != NULL)
	{	// �f�o�C�X�̊J��
		g_pD3DDevice->Release();
	}

	if (g_pD3D != NULL)
	{	// Direct3D�I�u�W�F�N�g�̊J��
		g_pD3D->Release();
	}

	if (g_pBGM != NULL)
	{	// �T�E���h�̉��
		g_pBGM->Release();
		g_pBGM = NULL;
	}
	UninitSound();

}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F1))
	{// �f�o�b�O�\��ON/OFF
		g_bDispDebug = g_bDispDebug ? false : true;
	}

	UpdateDebugFont();
#endif

	// ���͂̍X�V����
	UpdateInput();
	
	// ��ʑJ��
	switch (g_nStage)
	{
	case STAGE_TITLE:
		UpdateTitle();
		UpdateFont();
		break;

	case STAGE_GAME:
		// �J�����̍X�V����
		UpdateCamera();

		// �v���C���[�̍X�V����
		UpdatePlayer();
		UpdateGun();
		UpdateHand();

		// �G�l�~�[�̍X�V����
		UpdateEnemy();

		// �|�C���^�[�̍X�V����
		UpdatePointer();

		// �o���b�g�̍X�V����
		UpdateBullet();

		// �󔠂̍X�V����
		UpdateTreasure();

		// �h���b�v�̍X�V����
		UpdateDrop();

		// BG�̍X�V����
		UpdateBG();

		// �_���W�����̍X�V����
		UpdateDungeon();

		// �X�R�A�̍X�V����
		UpdateScore();

		// �t�H���g�̍X�V����
		UpdateFont();

		// HP�̍X�V����
		UpdateHeart();

		// �p�[�e�B�N���̍X�V����
		UpdateParticle();

		// �~�j�}�b�v�̍X�V����
		UpdateMiniMap();
		
		// �����蔻��
		CheckHit();

		// ���Z�b�g
		if (GetKeyboardTrigger(DIK_Q))
		{
			SetFade(FADE_W_OUT, STAGE_REFRESH);
		}
		break;

	case STAGE_GAME_END:
		SetStage(STAGE_RESULT);
		break;
	
	case STAGE_RESULT:
		UpdateResultScore();
		UpdateResult();
		UpdateFont();
		break;
		
	case STAGE_RESET:
		InitGame();				// �Q�[���̍ď���������
		SetString("-�~�b�V����-", SCREEN_CENTER_X, SCREEN_CENTER_Y - TEXTURE_FONT_SIZE * 2, TEXTURE_FONT_SIZE * 2, MESSAGE_INTERVAL);
		SetString("����������@������", SCREEN_CENTER_X, SCREEN_CENTER_Y + TEXTURE_FONT_SIZE, TEXTURE_FONT_SIZE, MESSAGE_INTERVAL);
		SetString("������傭", HEART_POS_X + TEXTURE_HEART_SIZE_X * MAX_HP / 1.9f, HEART_POS_Y - TEXTURE_FONT_SIZE * 0.5, TEXTURE_FONT_SIZE, -1);
		//SetString("�X�R�A", SCORE_POS_X + SCORE_DIGIT * TEXTURE_SCORE_SIZE_X / 2.0f, SCORE_POS_Y - TEXTURE_FONT_SIZE / 2.0f, TEXTURE_FONT_SIZE, -1);
		//SetString("SELECT�ŃT�C�Y�ւ񂱂�", MINIMAP_POS_X + MINIMAP_SIZE_SMALL * MAP_WIDTH, MINIMAP_POS_Y - TEXTURE_FONT_SIZE * 0.5f, TEXTURE_FONT_SIZE * 0.5, -1);
		SetStage(STAGE_GAME);
		break;

	case STAGE_REFRESH:
		if (IsPlaying(g_pBGM))
		{
			StopSound(g_pBGM);
		}
		InitTitle(1);
		InitFont(1);
		SetStage(STAGE_TITLE);
		break;
	}

	// ��ʑJ�ڂ̍X�V
	UpdateFade();
}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		// ��ʑJ��
		switch (g_nStage)
		{
		case STAGE_TITLE:
			DrawTitle();
			DrawFont();
			break;

		case STAGE_GAME:
		case STAGE_GAME_END:
		case STAGE_RESET:
		case STAGE_REFRESH:
			// BG�̕`�揈��
			DrawBG();

			// �}�b�v�̕`�揈��
			DrawDungeon();

			// �o���b�g�̕`�揈��
			DrawBullet();

			// �p�[�e�B�N���̕`�揈��
			DrawParticle();

			// �G�l�~�[�̕`�揈��
			DrawEnemy();

			// �|�C���^�[�̕`�揈��
			DrawPointer();

			// �󔠂̕`�揈��
			DrawTreasure();

			// �h���b�v�̕`�揈��
			DrawDrop();

			// �v���C���[�̕`�揈��(�e�Ǝ�̕`�揇���l������)
			switch (GetPlayer()->dir)
			{
			case DIR_RIGHT:
				DrawPlayer();
				DrawHand();
				DrawGun();
				break;
			case DIR_DOWN:
				DrawPlayer();
				DrawGun();
				DrawHand();
				break;
			case DIR_LEFT:
				DrawGun();
				DrawPlayer();
				DrawHand();
				break;
			case DIR_UP:
				DrawGun();
				DrawHand();
				DrawPlayer();
				break;
			}

			// �X�R�A�̕`�揈��
			DrawScore();

			// �t�H���g�̕`�揈��
			DrawFont();

			// HP�̕`�揈��
			DrawHeart();

			// �~�j�}�b�v�̕`�揈��
			DrawMiniMap();

			break;

		case STAGE_RESULT:
			// ���U���g�̕`�揈��
			DrawResult();
			// �X�R�A�̕`�揈��
			DrawResultScore();
			break;
		}

		// ��ʑJ�ڂ̕`�揈��
		DrawFade();

#ifdef _DEBUG
		// �f�o�b�O�\�������̕`��
		if (g_bDispDebug)
		{
			DrawDebugProc();
		}
#endif
		
		// Direct3D�ɂ��`��̏I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	if (g_pD3DDevice->Present(NULL, NULL, NULL, NULL) == D3DERR_DEVICELOST)
	{
		if (g_pD3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
#ifdef _DEBUG
			GetProc()->OnLostDevice();
#endif
			g_pD3DDevice->Reset(&d3dpp);

			// �����_�����O�X�e�[�g�p�����[�^�̐ݒ�
			g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// �J�����O���s��Ȃ�
			g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
			g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
			g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
			g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

																					// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
			g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
			g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
			g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g�厞�̕�Ԑݒ�
			g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k�����̕�Ԑݒ�

																					// �e�N�X�`���X�e�[�W���Z��������
			g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
			g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
			g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

#ifdef _DEBUG
			GetProc()->OnResetDevice();
#endif
		}
	}
}

//=============================================================================
// �f�o�C�X�擾�֐�
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return(g_pD3DDevice);
}


#ifdef _DEBUG
//=============================================================================
// �f�o�b�O�\������
//=============================================================================
void UpdateDebugFont(void)
{
	PLAYER *player = GetPlayer();
	D3DXVECTOR3 *posCamera = GetCameraPos();
	D3DXVECTOR3 *vecCamera = GetCameraVec();
	PARTICLE *particle = GetParticle(0);

	// �e�L�X�g�`��
	PrintDebugProc("FPS:%d\n", g_nCountFPS);
	PrintDebugProc("PX:%f  PY:%f  MX:%d  MY:%d\n", player->pos.x, player->pos.y, (int)(player->pos.x / TEXTURE_DUNGEON_SIZE_X), (int)(player->pos.y / TEXTURE_DUNGEON_SIZE_Y));
	PrintDebugProc("CPX:%f  CPY:%f\n", posCamera->x, posCamera->y);
	PrintDebugProc("CVX:%f  CVY:%f\n", vecCamera->x, vecCamera->y);
	PrintDebugProc("GUNTYPE = %d\n", player->gunType);

	int id = GetRoomIDByPos(player->pos.x, player->pos.y);
	if (id != -1)
	{
		ROOM *room = GetRoom(id);

		PrintDebugProc("-------------------\n");
		PrintDebugProc("Room%d\n", id);
		PrintDebugProc("X:%d~%d  Y:%d~%d\n", room->baseX + 1, room->baseX + room->width - 2, room->baseY + 1, room->baseY + room->height - 2);
		PrintDebugProc("Enemy:%d  Spawn:%d\n", room->cntEnemy, room->spawnEnemy);
	}
}
#endif

//=============================================================================
// ��ʑJ��
//=============================================================================
void SetStage( int stage )
{
	//if( state < 0 || state >= STATE_MAX ) return;

	g_nStage = stage;
}

//=============================================================================
// �����蔻�菈��
//=============================================================================
void CheckHit(void)
{
	PLAYER *player;
	ENEMY *enemy, *enemy2;
	TREASURE *treasure;
	BULLET *bullet;
	DROP *drop;
	int i, j;

	// �G�Ƒ���L����(BB)
	for (i = 0, enemy = GetEnemy(0), player = GetPlayer(); i < ENEMY_MAX; i++, enemy++)
	{
		if (player->use == false || enemy->use == false || enemy->state == ENEMY_SPAWN || enemy->state == ENEMY_DESPAWN || enemy->state == ENEMY_DEAD || enemy->state == ENEMY_DEFAULT_STATE) continue;

		if (CheckHitBB(player->pos, enemy->pos, D3DXVECTOR2(TEXTURE_PLAYER_SIZE_X, TEXTURE_PLAYER_SIZE_Y), D3DXVECTOR2(TEXTURE_ENEMY_SIZE_X, TEXTURE_ENEMY_SIZE_Y)))
		{
			// �Փˏ���
			KnockBackEnemy(enemy, ENEMY_KNOCKBACK_SPEED, atan2(enemy->pos.y - player->pos.y, enemy->pos.x - player->pos.x));
			KnockBackPlayer(player, atan2(player->pos.y - enemy->pos.y, player->pos.x - enemy->pos.x));
		}
	}

	// �󔠂Ƒ���L����(BC)
	for (i = 0, treasure = GetTreasure(0), player = GetPlayer(); i < TREASURE_MAX; i++, treasure++)
	{
		if (player->use == false || treasure->use == false || treasure->state == BOX_SPAWNED || treasure->state == BOX_EMPTY) continue;

		if (CheckHitBC(treasure->pos, player->pos, TEXTURE_TREASURE_SIZE / 2, TEXTURE_PLAYER_SIZE_X))
		{
			if (treasure->state == BOX_WAITING)
			{
				treasure->state = BOX_OPENED;
			}
			// �Փˏ���
			KnockBackPlayer(player, atan2(player->pos.y - treasure->pos.y, player->pos.x - treasure->pos.x));
		}
	}

	// �󔠂ƒe(BC)
	for (i = 0, treasure = GetTreasure(0); i < TREASURE_MAX; i++, treasure++)
	{
		if (treasure->use == false || treasure->state == BOX_SPAWNED) continue;

		for (j = 0, bullet = GetBullet(0); j < BULLET_MAX; j++, bullet++)
		{
			if (bullet->use == false || bullet->type == ENEMY_BULLET) continue;

			if (CheckHitBC(treasure->pos, bullet->pos, TEXTURE_TREASURE_SIZE / 2, TEXTURE_PLAYER_SIZE_X))
			{
				bullet->use = false;
				if (treasure->state == BOX_WAITING)
				{
					treasure->state = BOX_OPENED;
				}
			}
		}
	}

	// �h���b�v�Ƒ���L����(BC)
	for (i = 0, drop = GetDrop(0), player = GetPlayer(); i < MAX_DROP; i++, drop++)
	{
		if (player->use == false || drop->use == false || drop->height > 1.0f) continue;

		if (CheckHitBC(drop->pos, player->pos, 0, TEXTURE_PLAYER_SIZE_X))
		{
			PickDrop(drop);
		}
	}

	// �G���m(BC)
	for (i = 0, enemy = GetEnemy(0); i < ENEMY_MAX - 1; i++, enemy++)
	{
		if (enemy->use == false || enemy->state == ENEMY_SPAWN || enemy->state == ENEMY_DESPAWN || enemy->state == ENEMY_DEFAULT_STATE) continue;

		for (j = i + 1, enemy2 = GetEnemy(i + 1); j < ENEMY_MAX; j++, enemy2++)
		{
			if (enemy2->use == false || enemy2->state == ENEMY_SPAWN || enemy2->state == ENEMY_DESPAWN || enemy->state == ENEMY_DEFAULT_STATE) continue;

			if (CheckHitBC(enemy->pos, enemy2->pos, TEXTURE_ENEMY_SIZE_X, TEXTURE_ENEMY_SIZE_X))
			{
				// �Փˏ���
				KnockBackEnemy(enemy, ENEMY_AVOID_SPEED, atan2(enemy->pos.y - enemy2->pos.y, enemy->pos.x - enemy2->pos.x));
				KnockBackEnemy(enemy2, ENEMY_AVOID_SPEED, atan2(enemy2->pos.y - enemy->pos.y, enemy2->pos.x - enemy->pos.x));
			}
		}
	}

	// �G�ƓG�̎���(BC)
	for (i = 0, enemy = GetEnemy(0); i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->state != ENEMY_DEAD) continue;

		for (j = 0, enemy2 = GetEnemy(0); j < ENEMY_MAX; j++, enemy2++)
		{
			if (enemy2->use == false || enemy2->state == ENEMY_SPAWN || enemy2->state == ENEMY_DESPAWN || enemy->state == ENEMY_DEFAULT_STATE || enemy == enemy2) continue;

			if (CheckHitBC(enemy->pos, enemy2->pos, TEXTURE_ENEMY_SIZE_X, TEXTURE_ENEMY_SIZE_X))
			{
				// �Փˏ���
				KnockBackEnemy(enemy, ENEMY_KNOCKBACK_SPEED, atan2(enemy->pos.y - enemy2->pos.y, enemy->pos.x - enemy2->pos.x));
				KnockBackEnemy(enemy2, ENEMY_KNOCKBACK_SPEED, atan2(enemy2->pos.y - enemy->pos.y, enemy2->pos.x - enemy->pos.x));
				// �G��HP��������
				enemy2->hp -= 1;
				if (enemy2->hp <= 0)
				{
					DeleteEnemy(enemy2);
				}
				else
				{
					SetState(enemy2, ENEMY_DAMAGE);
				}
			}
		}
	}

	// �e�ƓG(BC)
	for (i = 0, bullet = GetBullet(0); i < BULLET_MAX; i++, bullet++)
	{
		if (bullet->use == false) continue;

		if (bullet->type != ENEMY_BULLET)
		{
			for (j = 0, enemy = GetEnemy(0); j < ENEMY_MAX; j++, enemy++)
			{
				if (enemy->use == false || enemy->state == ENEMY_SPAWN || enemy->state == ENEMY_DESPAWN || enemy->state == ENEMY_DEAD) continue;

				if (CheckHitBC(bullet->pos, enemy->pos, bullet->Radius, TEXTURE_ENEMY_SIZE_X))
				{
					bullet->use = false;	// �e�̏��ŏ������s��

					if (bullet->type == PLAYER_BULLET_SHOTGUN)
					{
						KnockBackEnemy(enemy, ENEMY_KNOCKBACK_SPEED, atan2(bullet->vec.y, bullet->vec.x));
					}
					else
					{
						KnockBackEnemy(enemy, ENEMY_KNOCKBACK_SPEED, atan2(enemy->pos.y - bullet->pos.y, enemy->pos.x - bullet->pos.x));
					}
					// �G��HP��������
					enemy->hp--;
					if (enemy->hp <= 0)
					{
						DeleteEnemy(enemy);
					}
					else
					{
						SetState(enemy, ENEMY_DAMAGE);
					}
				}
			}
		}

		// �{�X�ƒe(BC)

		// �����ƓG�̒e(BC)
		if (bullet->type != PLAYER_BULLET_PISTOL)
		{
			player = GetPlayer();
			if (player->use == false || player->invincible) continue;

			if (CheckHitBC(bullet->pos, player->pos, bullet->Radius, TEXTURE_PLAYER_SIZE_X))
			{
				bullet->use = false;	// �e�̏��ŏ������s��

				// ������HP��������
				player->hp--;
				SetInvincible(player);
				float rotZ = atan2(player->pos.y - bullet->pos.y, player->pos.x - bullet->pos.x);
				KnockBackPlayer(player, rotZ);
				if (player->hp <= 0)
				{
					player->movable = false;
					SetFade(FADE_W_OUT, STAGE_GAME_END);
				}
			}

		}
	}

	// �K�i
	if (GetMapByPos(player->pos.x, player->pos.y) == MAP_EXIT && GetFade() != FADE_B_OUT)
	{
		SetFade(FADE_B_OUT, STAGE_GAME_END);
	}
	else if (GetMapByPos(player->pos.x, player->pos.y) != MAP_EXIT && GetFade() == FADE_B_OUT)
	{
		SetFade(FADE_IN, STAGE_GAME);
	}
}


//=============================================================================
// BB�ɂ�铖���蔻�菈��
// ��]�͍l�����Ȃ�
// �߂�l�F�������Ă���true
//=============================================================================
bool CheckHitBB( D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR2 size1, D3DXVECTOR2 size2 )
{
	size1 /= 2.0f;	// ���T�C�Y�ɂ���
	size2 /= 2.0f;	// ����

	if( ((pos2.x + size2.x) > (pos1.x - size1.x)) && ((pos1.x + size1.x) > (pos2.x - size2.x)) &&
		((pos2.y + size2.y) > (pos1.y - size1.y)) && ((pos1.y + size1.y) > (pos2.y - size2.y)) )
	{
		return true;
	}

	return false;
}

//=============================================================================
// BC�ɂ�铖���蔻�菈��
// �T�C�Y�͔��a
// �߂�l�F�������Ă���true
//=============================================================================
bool CheckHitBC( D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2 )
{
	D3DXVECTOR3 temp = pos1 - pos2;
	float len = D3DXVec3LengthSq(&temp);

	if (len <= ((size1 + size2) * (size1 + size2)))
	{
		return true;
	}

	return false;
}


//=============================================================================
// �Q�[���̍ď�������������
// �߂�l�F����
//=============================================================================
void InitGame(void)
{
	InitCamera();		// �J�����̍ď�����
	InitPlayer(1);		// �v���C���[�̍ď�����
	InitGun(1);			// �e�̍ď�����
	InitBullet(1);		// �o���b�g�̍ď�����
	InitBG(1);			// BG�̍ď�����
	InitScore(1);		// �X�R�A�̍ď�����
	InitResultScore(1);	// ���U���g�X�R�A�̍ď�����
	InitFont(1);		// �t�H���g�̍ď�����
	InitDungeon(1);		// �_���W�����̍ď�����
	InitEnemy(1);		// �G�l�~�[�̍ď�����
	InitPointer(1);		// �|�C���^�[�̍ď�����
	InitTreasure(1);	// �󔠂̍ď�����
	InitDrop(1);		// �h���b�v�̍ď�����
	InitParticle(1);	// �p�[�e�B�N���̍ď�����
	InitMiniMap();		// �~�j�}�b�v�̍ď�����

	// �T�E���h�̃��[�h�ƍĐ�
	g_pBGM = LoadSound(BGM_MAZE);
	PlaySound(g_pBGM, E_DS8_FLAG_LOOP);
}

