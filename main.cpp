//=============================================================================
//
// Main処理 [main.cpp]
// Author : GP11A243 06 楳田健斗
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
// マクロ定義
//*****************************************************************************
#define CLASS_NAME			_T("AppClass")						// ウインドウのクラス名
#define WINDOW_NAME			_T("Dungeon and Bullet ver1.3")		// ウインドウのキャプション名

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
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
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9				g_pD3D = NULL;				// Direct3Dオブジェクト
LPDIRECT3DDEVICE9		g_pD3DDevice = NULL;		// Deviceオブジェクト(描画に必要)

D3DPRESENT_PARAMETERS	d3dpp;

#ifdef _DEBUG
bool					g_bDispDebug = true;		// デバッグ表示
int						g_nCountFPS;				// FPSカウンタ
#endif

int						g_nStage = 0;				// ステージ番号

LPDIRECTSOUNDBUFFER8	g_pBGM;						// BGM用バッファ

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	// 時間計測用
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
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,																		// ウィンドウの左座標
						CW_USEDEFAULT,																		// ウィンドウの上座標
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME)*2,									// ウィンドウ横幅
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME)*2+GetSystemMetrics(SM_CYCAPTION),	// ウィンドウ縦幅
						NULL,
						NULL,
						hInstance,
						NULL);

	// DirectXの初期化(ウィンドウを作成してから行う)
	if(FAILED(Init(hInstance, hWnd, false)))
	{
		return -1;
	}

	// フレームカウント初期化
	timeBeginPeriod(1);	// 分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;
	
	// ウインドウの表示(Init()の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// メッセージループ
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();					// システム時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)		// 0.5秒ごとに実行
			{
#ifdef _DEBUG
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);	// FPSを測定
#endif
				dwFPSLastTime = dwCurrentTime;				// FPSを測定した時刻を保存
				dwFrameCount = 0;							// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;			// 処理した時刻を保存

				Update();								// 更新処理
				Draw();									// 描画処理
				
				dwFrameCount++;							// 処理回数のカウントを加算
			}
		}
	}
	
	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
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
		case VK_ESCAPE:					// [ESC]キーが押された
			DestroyWindow(hWnd);		// ウィンドウを破棄するよう指示する
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;

	srand((unsigned)time(NULL));

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//// デバイスのプレゼンテーションパラメータの設定
	//ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	//d3dpp.BackBufferCount			= 1;						// バックバッファの数
	//d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	//d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	//d3dpp.BackBufferFormat			= D3DFMT_UNKNOWN;			// バックバッファのフォーマットは現在設定されているものを使う
	//d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	//d3dpp.Windowed					= bWindow;					// ウィンドウモード
	//d3dpp.EnableAutoDepthStencil	= TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	//d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// デプスバッファとして16bitを使う
	//d3dpp.BackBufferFormat			= d3ddm.Format;				// カラーモードの指定

	//if(bWindow)
	//{// ウィンドウモード
	//	d3dpp.BackBufferFormat           = D3DFMT_UNKNOWN;					// バックバッファ
	//	d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
	//	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	//}
	//else
	//{// フルスクリーンモード
	//	d3dpp.BackBufferFormat           = D3DFMT_R5G6B5;					// バックバッファ
	//	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
	//	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	//}

	//// デバイスの生成
	//// ディスプレイアダプタを表すためのデバイスを作成
	//// 描画と頂点処理をハードウェアで行なう
	//if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,							// ディスプレイアダプタ
	//								D3DDEVTYPE_HAL,								// ディスプレイタイプ
	//								hWnd,										// フォーカスするウインドウへのハンドル
	//								D3DCREATE_HARDWARE_VERTEXPROCESSING,		// デバイス作成制御の組み合わせ
	//								&d3dpp,										// デバイスのプレゼンテーションパラメータ
	//								&g_pD3DDevice)))							// デバイスインターフェースへのポインタ
	//{
	//	// 上記の設定が失敗したら
	//	// 描画をハードウェアで行い、頂点処理はCPUで行なう
	//	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
	//									D3DDEVTYPE_HAL, 
	//									hWnd, 
	//									D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
	//									&d3dpp,
	//									&g_pD3DDevice)))
	//	{
	//		// 上記の設定が失敗したら
	//		// 描画と頂点処理をCPUで行なう
	//		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
	//										D3DDEVTYPE_REF,
	//										hWnd, 
	//										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
	//										&d3dpp,
	//										&g_pD3DDevice)))
	//		{
	//			// 初期化失敗
	//			return E_FAIL;
	//		}
	//	}
	//}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));				// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;						// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;			// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;			// バックバッファフォーマットはディスプレイモードに合わせて使う
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.Windowed = bWindow;						// ウィンドウモード
	d3dpp.EnableAutoDepthStencil = TRUE;			// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;		// デプスバッファとして16bitを使う

	if (bWindow)
	{// ウィンドウモード
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{// フルスクリーンモード
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスオブジェクトの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &g_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}



	// レンダリングステートパラメータの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// カリングを行わない
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大時の補間設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小時の補間設定

	// テクスチャステージ加算合成処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

#ifdef _DEBUG
	// デバッグ表示処理の初期化
	InitDebugProc();
#endif

	// 入力処理の初期化
	InitInput(hInstance, hWnd);

	// サウンド初期化
	InitSound(hWnd);

	// カメラの初期化
	InitCamera();

	// プレイヤーの初期化処理
	InitPlayer(0);
	InitGun(0);
	InitHand(0);

	// バレットの初期化処理
	InitBullet(0);

	// BG初期化
	InitBG(0);

	// タイトル初期化
	InitTitle(0);

	// リザルト初期化
	InitResult();

	// スコア初期化
	InitScore(0);
	InitResultScore(0);

	// フォント初期化
	InitFont(0);

	// HP初期化
	InitHeart(0);

	// ダンジョン初期化
	InitDungeon(0);

	// エネミーの初期化処理
	InitEnemy(0);

	// ポインターの初期化処理
	InitPointer(0);

	// 宝箱の初期化処理
	InitTreasure(0);

	// ドロップの初期化処理
	InitDrop(0);

	// 画面遷移の初期化処理
	InitFade();

	// パーティクルの初期化処理
	InitParticle(0);

	// ミニマップの初期化処理
	InitMiniMap();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
#ifdef _DEBUG
	// デバッグ表示処理の終了処理
	UninitDebugProc();
#endif

	// 入力処理の終了処理
	UninitInput();

	// プレイヤーの終了処理
	UninitPlayer();
	UninitGun();
	UninitHand();

	// エネミーの終了処理
	UninitEnemy();

	// ポインターの終了処理
	UninitPointer();

	// ポインターの終了処理
	UninitPointer();

	// バレットの終了処理
	UninitBullet();

	// 宝箱の終了処理
	UninitTreasure();

	// ドロップの終了処理
	UninitDrop();

	// BG終了処理
	UninitBG();

	// ダンジョン終了処理
	UninitDungeon();

	// タイトル終了処理
	UninitTitle();

	// リザルト終了処理
	UninitResult();
	
	// スコア終了処理
	UninitScore();

	// フォント終了処理
	UninitFont();

	// パーティクル終了処理
	UninitParticle();

	// ミニマップ終了処理
	UninitMiniMap();

	// 画面遷移終了処理
	UninitFade();	

	if (g_pD3DDevice != NULL)
	{	// デバイスの開放
		g_pD3DDevice->Release();
	}

	if (g_pD3D != NULL)
	{	// Direct3Dオブジェクトの開放
		g_pD3D->Release();
	}

	if (g_pBGM != NULL)
	{	// サウンドの解放
		g_pBGM->Release();
		g_pBGM = NULL;
	}
	UninitSound();

}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F1))
	{// デバッグ表示ON/OFF
		g_bDispDebug = g_bDispDebug ? false : true;
	}

	UpdateDebugFont();
#endif

	// 入力の更新処理
	UpdateInput();
	
	// 画面遷移
	switch (g_nStage)
	{
	case STAGE_TITLE:
		UpdateTitle();
		UpdateFont();
		break;

	case STAGE_GAME:
		// カメラの更新処理
		UpdateCamera();

		// プレイヤーの更新処理
		UpdatePlayer();
		UpdateGun();
		UpdateHand();

		// エネミーの更新処理
		UpdateEnemy();

		// ポインターの更新処理
		UpdatePointer();

		// バレットの更新処理
		UpdateBullet();

		// 宝箱の更新処理
		UpdateTreasure();

		// ドロップの更新処理
		UpdateDrop();

		// BGの更新処理
		UpdateBG();

		// ダンジョンの更新処理
		UpdateDungeon();

		// スコアの更新処理
		UpdateScore();

		// フォントの更新処理
		UpdateFont();

		// HPの更新処理
		UpdateHeart();

		// パーティクルの更新処理
		UpdateParticle();

		// ミニマップの更新処理
		UpdateMiniMap();
		
		// 当たり判定
		CheckHit();

		// リセット
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
		InitGame();				// ゲームの再初期化処理
		SetString("-ミッション-", SCREEN_CENTER_X, SCREEN_CENTER_Y - TEXTURE_FONT_SIZE * 2, TEXTURE_FONT_SIZE * 2, MESSAGE_INTERVAL);
		SetString("かいだんを　さがせ", SCREEN_CENTER_X, SCREEN_CENTER_Y + TEXTURE_FONT_SIZE, TEXTURE_FONT_SIZE, MESSAGE_INTERVAL);
		SetString("たいりょく", HEART_POS_X + TEXTURE_HEART_SIZE_X * MAX_HP / 1.9f, HEART_POS_Y - TEXTURE_FONT_SIZE * 0.5, TEXTURE_FONT_SIZE, -1);
		//SetString("スコア", SCORE_POS_X + SCORE_DIGIT * TEXTURE_SCORE_SIZE_X / 2.0f, SCORE_POS_Y - TEXTURE_FONT_SIZE / 2.0f, TEXTURE_FONT_SIZE, -1);
		//SetString("SELECTでサイズへんこう", MINIMAP_POS_X + MINIMAP_SIZE_SMALL * MAP_WIDTH, MINIMAP_POS_Y - TEXTURE_FONT_SIZE * 0.5f, TEXTURE_FONT_SIZE * 0.5, -1);
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

	// 画面遷移の更新
	UpdateFade();
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		// 画面遷移
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
			// BGの描画処理
			DrawBG();

			// マップの描画処理
			DrawDungeon();

			// バレットの描画処理
			DrawBullet();

			// パーティクルの描画処理
			DrawParticle();

			// エネミーの描画処理
			DrawEnemy();

			// ポインターの描画処理
			DrawPointer();

			// 宝箱の描画処理
			DrawTreasure();

			// ドロップの描画処理
			DrawDrop();

			// プレイヤーの描画処理(銃と手の描画順を考慮する)
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

			// スコアの描画処理
			DrawScore();

			// フォントの描画処理
			DrawFont();

			// HPの描画処理
			DrawHeart();

			// ミニマップの描画処理
			DrawMiniMap();

			break;

		case STAGE_RESULT:
			// リザルトの描画処理
			DrawResult();
			// スコアの描画処理
			DrawResultScore();
			break;
		}

		// 画面遷移の描画処理
		DrawFade();

#ifdef _DEBUG
		// デバッグ表示処理の描画
		if (g_bDispDebug)
		{
			DrawDebugProc();
		}
#endif
		
		// Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	if (g_pD3DDevice->Present(NULL, NULL, NULL, NULL) == D3DERR_DEVICELOST)
	{
		if (g_pD3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
#ifdef _DEBUG
			GetProc()->OnLostDevice();
#endif
			g_pD3DDevice->Reset(&d3dpp);

			// レンダリングステートパラメータの設定
			g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// カリングを行わない
			g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
			g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
			g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
			g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

																					// サンプラーステートパラメータの設定
			g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
			g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定
			g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大時の補間設定
			g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小時の補間設定

																					// テクスチャステージ加算合成処理
			g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
			g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
			g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

#ifdef _DEBUG
			GetProc()->OnResetDevice();
#endif
		}
	}
}

//=============================================================================
// デバイス取得関数
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return(g_pD3DDevice);
}


#ifdef _DEBUG
//=============================================================================
// デバッグ表示処理
//=============================================================================
void UpdateDebugFont(void)
{
	PLAYER *player = GetPlayer();
	D3DXVECTOR3 *posCamera = GetCameraPos();
	D3DXVECTOR3 *vecCamera = GetCameraVec();
	PARTICLE *particle = GetParticle(0);

	// テキスト描画
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
// 画面遷移
//=============================================================================
void SetStage( int stage )
{
	//if( state < 0 || state >= STATE_MAX ) return;

	g_nStage = stage;
}

//=============================================================================
// 当たり判定処理
//=============================================================================
void CheckHit(void)
{
	PLAYER *player;
	ENEMY *enemy, *enemy2;
	TREASURE *treasure;
	BULLET *bullet;
	DROP *drop;
	int i, j;

	// 敵と操作キャラ(BB)
	for (i = 0, enemy = GetEnemy(0), player = GetPlayer(); i < ENEMY_MAX; i++, enemy++)
	{
		if (player->use == false || enemy->use == false || enemy->state == ENEMY_SPAWN || enemy->state == ENEMY_DESPAWN || enemy->state == ENEMY_DEAD || enemy->state == ENEMY_DEFAULT_STATE) continue;

		if (CheckHitBB(player->pos, enemy->pos, D3DXVECTOR2(TEXTURE_PLAYER_SIZE_X, TEXTURE_PLAYER_SIZE_Y), D3DXVECTOR2(TEXTURE_ENEMY_SIZE_X, TEXTURE_ENEMY_SIZE_Y)))
		{
			// 衝突処理
			KnockBackEnemy(enemy, ENEMY_KNOCKBACK_SPEED, atan2(enemy->pos.y - player->pos.y, enemy->pos.x - player->pos.x));
			KnockBackPlayer(player, atan2(player->pos.y - enemy->pos.y, player->pos.x - enemy->pos.x));
		}
	}

	// 宝箱と操作キャラ(BC)
	for (i = 0, treasure = GetTreasure(0), player = GetPlayer(); i < TREASURE_MAX; i++, treasure++)
	{
		if (player->use == false || treasure->use == false || treasure->state == BOX_SPAWNED || treasure->state == BOX_EMPTY) continue;

		if (CheckHitBC(treasure->pos, player->pos, TEXTURE_TREASURE_SIZE / 2, TEXTURE_PLAYER_SIZE_X))
		{
			if (treasure->state == BOX_WAITING)
			{
				treasure->state = BOX_OPENED;
			}
			// 衝突処理
			KnockBackPlayer(player, atan2(player->pos.y - treasure->pos.y, player->pos.x - treasure->pos.x));
		}
	}

	// 宝箱と弾(BC)
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

	// ドロップと操作キャラ(BC)
	for (i = 0, drop = GetDrop(0), player = GetPlayer(); i < MAX_DROP; i++, drop++)
	{
		if (player->use == false || drop->use == false || drop->height > 1.0f) continue;

		if (CheckHitBC(drop->pos, player->pos, 0, TEXTURE_PLAYER_SIZE_X))
		{
			PickDrop(drop);
		}
	}

	// 敵同士(BC)
	for (i = 0, enemy = GetEnemy(0); i < ENEMY_MAX - 1; i++, enemy++)
	{
		if (enemy->use == false || enemy->state == ENEMY_SPAWN || enemy->state == ENEMY_DESPAWN || enemy->state == ENEMY_DEFAULT_STATE) continue;

		for (j = i + 1, enemy2 = GetEnemy(i + 1); j < ENEMY_MAX; j++, enemy2++)
		{
			if (enemy2->use == false || enemy2->state == ENEMY_SPAWN || enemy2->state == ENEMY_DESPAWN || enemy->state == ENEMY_DEFAULT_STATE) continue;

			if (CheckHitBC(enemy->pos, enemy2->pos, TEXTURE_ENEMY_SIZE_X, TEXTURE_ENEMY_SIZE_X))
			{
				// 衝突処理
				KnockBackEnemy(enemy, ENEMY_AVOID_SPEED, atan2(enemy->pos.y - enemy2->pos.y, enemy->pos.x - enemy2->pos.x));
				KnockBackEnemy(enemy2, ENEMY_AVOID_SPEED, atan2(enemy2->pos.y - enemy->pos.y, enemy2->pos.x - enemy->pos.x));
			}
		}
	}

	// 敵と敵の死体(BC)
	for (i = 0, enemy = GetEnemy(0); i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->state != ENEMY_DEAD) continue;

		for (j = 0, enemy2 = GetEnemy(0); j < ENEMY_MAX; j++, enemy2++)
		{
			if (enemy2->use == false || enemy2->state == ENEMY_SPAWN || enemy2->state == ENEMY_DESPAWN || enemy->state == ENEMY_DEFAULT_STATE || enemy == enemy2) continue;

			if (CheckHitBC(enemy->pos, enemy2->pos, TEXTURE_ENEMY_SIZE_X, TEXTURE_ENEMY_SIZE_X))
			{
				// 衝突処理
				KnockBackEnemy(enemy, ENEMY_KNOCKBACK_SPEED, atan2(enemy->pos.y - enemy2->pos.y, enemy->pos.x - enemy2->pos.x));
				KnockBackEnemy(enemy2, ENEMY_KNOCKBACK_SPEED, atan2(enemy2->pos.y - enemy->pos.y, enemy2->pos.x - enemy->pos.x));
				// 敵のHP減少処理
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

	// 弾と敵(BC)
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
					bullet->use = false;	// 弾の消滅処理を行い

					if (bullet->type == PLAYER_BULLET_SHOTGUN)
					{
						KnockBackEnemy(enemy, ENEMY_KNOCKBACK_SPEED, atan2(bullet->vec.y, bullet->vec.x));
					}
					else
					{
						KnockBackEnemy(enemy, ENEMY_KNOCKBACK_SPEED, atan2(enemy->pos.y - bullet->pos.y, enemy->pos.x - bullet->pos.x));
					}
					// 敵のHP減少処理
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

		// ボスと弾(BC)

		// 自分と敵の弾(BC)
		if (bullet->type != PLAYER_BULLET_PISTOL)
		{
			player = GetPlayer();
			if (player->use == false || player->invincible) continue;

			if (CheckHitBC(bullet->pos, player->pos, bullet->Radius, TEXTURE_PLAYER_SIZE_X))
			{
				bullet->use = false;	// 弾の消滅処理を行い

				// 自分のHP減少処理
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

	// 階段
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
// BBによる当たり判定処理
// 回転は考慮しない
// 戻り値：当たってたらtrue
//=============================================================================
bool CheckHitBB( D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR2 size1, D3DXVECTOR2 size2 )
{
	size1 /= 2.0f;	// 半サイズにする
	size2 /= 2.0f;	// 同上

	if( ((pos2.x + size2.x) > (pos1.x - size1.x)) && ((pos1.x + size1.x) > (pos2.x - size2.x)) &&
		((pos2.y + size2.y) > (pos1.y - size1.y)) && ((pos1.y + size1.y) > (pos2.y - size2.y)) )
	{
		return true;
	}

	return false;
}

//=============================================================================
// BCによる当たり判定処理
// サイズは半径
// 戻り値：当たってたらtrue
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
// ゲームの再初期化処理処理
// 戻り値：無し
//=============================================================================
void InitGame(void)
{
	InitCamera();		// カメラの再初期化
	InitPlayer(1);		// プレイヤーの再初期化
	InitGun(1);			// 銃の再初期化
	InitBullet(1);		// バレットの再初期化
	InitBG(1);			// BGの再初期化
	InitScore(1);		// スコアの再初期化
	InitResultScore(1);	// リザルトスコアの再初期化
	InitFont(1);		// フォントの再初期化
	InitDungeon(1);		// ダンジョンの再初期化
	InitEnemy(1);		// エネミーの再初期化
	InitPointer(1);		// ポインターの再初期化
	InitTreasure(1);	// 宝箱の再初期化
	InitDrop(1);		// ドロップの再初期化
	InitParticle(1);	// パーティクルの再初期化
	InitMiniMap();		// ミニマップの再初期化

	// サウンドのロードと再生
	g_pBGM = LoadSound(BGM_MAZE);
	PlaySound(g_pBGM, E_DS8_FLAG_LOOP);
}

