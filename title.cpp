//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#include "main.h"
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TITLE_BG		_T("data/TEXTURE/TITLE/title_bg.png")			// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_BG_MONO	_T("data/TEXTURE/TITLE/title_bg_mono.png")		// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_BG_CHROMA	_T("data/TEXTURE/TITLE/title_bg_chroma.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_CHROMA	_T("data/TEXTURE/TITLE/title_logo_chroma.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_TEXT		_T("data/TEXTURE/TITLE/title_text.png")			// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_VERSION	_T("data/TEXTURE/TITLE/title_version.png")		// 読み込むテクスチャファイル名
#define TEXTURE_TITLE_HOLE		_T("data/TEXTURE/TITLE/title_bullethole.png")	// 読み込むテクスチャファイル名

const char *TEXTURE_TITLE_LOGO[] =
{
	"data/TEXTURE/TITLE/title_logo_00.png",
	"data/TEXTURE/TITLE/title_logo_01.png",
	"data/TEXTURE/TITLE/title_logo_02.png",
	"data/TEXTURE/TITLE/title_logo_03.png",
	"data/TEXTURE/TITLE/title_logo_04.png",
	"data/TEXTURE/TITLE/title_logo_05.png",
	"data/TEXTURE/TITLE/title_logo_06.png",
	"data/TEXTURE/TITLE/title_logo_07.png",
	"data/TEXTURE/TITLE/title_logo_08.png",
	"data/TEXTURE/TITLE/title_logo_09.png",
	"data/TEXTURE/TITLE/title_logo_10.png",
	"data/TEXTURE/TITLE/title_logo_11.png",
	"data/TEXTURE/TITLE/title_logo_12.png",
	"data/TEXTURE/TITLE/title_logo_13.png",
};

#define LOGO_MAX				(14)		// ロゴの文字数
#define TITLE_INTERVAL_LOGO		(4)			// 文字の出現間隔
#define TITLE_INTERVAL			(100)		// タイトルアウトのインターバル
#define POS_HOLE_X				(192)		// 弾痕のX座標
#define POS_HOLE_Y				(256)		// 弾痕のY座標

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTitle(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureTitleBG = NULL;			// テクスチャへのポインタ
LPDIRECT3DTEXTURE9	g_pD3DTextureTitleBGMono = NULL;		// テクスチャへのポインタ
LPDIRECT3DTEXTURE9	g_pD3DTextureTitleBGChroma = NULL;		// テクスチャへのポインタ
LPDIRECT3DTEXTURE9	g_pD3DTextureTitleHole = NULL;			// テクスチャへのポインタ
LPDIRECT3DTEXTURE9	g_pD3DTextureTitleChroma = NULL;		// テクスチャへのポインタ
LPDIRECT3DTEXTURE9	g_pD3DTextureTitleText = NULL;			// テクスチャへのポインタ
LPDIRECT3DTEXTURE9	g_pD3DTextureTitleVersion = NULL;		// テクスチャへのポインタ
LPDIRECT3DTEXTURE9	g_pD3DTextureTitleLogo[LOGO_MAX] = {};	// テクスチャへのポインタ

VERTEX_2D		g_vertexWkTitleBG[NUM_VERTEX];				// 頂点情報格納ワーク
VERTEX_2D		g_vertexWkTitleBGMono[NUM_VERTEX];			// 頂点情報格納ワーク
VERTEX_2D		g_vertexWkTitleBGChroma[NUM_VERTEX];		// 頂点情報格納ワーク
VERTEX_2D		g_vertexWkTitleHole[NUM_VERTEX];			// 頂点情報格納ワーク
VERTEX_2D		g_vertexWkTitleChroma[NUM_VERTEX];			// 頂点情報格納ワーク
VERTEX_2D		g_vertexWkTitleText[NUM_VERTEX];			// 頂点情報格納ワーク
VERTEX_2D		g_vertexWkTitleVersion[NUM_VERTEX];			// 頂点情報格納ワーク
VERTEX_2D		g_vertexWkTitleLogo[NUM_VERTEX * LOGO_MAX];	// 頂点情報格納ワーク

D3DXVECTOR3		g_posTitleLogo[LOGO_MAX];					// ロゴの位置
int				g_stageState;								// タイトルの状態
int				g_interval;									// インターバル
int				g_staLogo;									// ロゴ番号(開始)
int				g_endLogo;									// ロゴ番号(終了)
float			g_rotTitle;										// 回転(揺らぎに使用)

LPDIRECTSOUNDBUFFER8	g_pTitleBGM;						// BGM用バッファ
LPDIRECTSOUNDBUFFER8	g_pTitleSE_Shot;					// SE用バッファ
LPDIRECTSOUNDBUFFER8	g_pTitleSE_Wind[LOGO_MAX];			// SE用バッファ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_TITLE_BG,					// ファイルの名前
			&g_pD3DTextureTitleBG);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_TITLE_BG_MONO,				// ファイルの名前
			&g_pD3DTextureTitleBGMono);			// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_TITLE_BG_CHROMA,			// ファイルの名前
			&g_pD3DTextureTitleBGChroma);		// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_TITLE_HOLE,					// ファイルの名前
			&g_pD3DTextureTitleHole);			// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_TITLE_CHROMA,				// ファイルの名前
			&g_pD3DTextureTitleChroma);			// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_TITLE_TEXT,					// ファイルの名前
			&g_pD3DTextureTitleText);			// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			TEXTURE_TITLE_VERSION,				// ファイルの名前
			&g_pD3DTextureTitleVersion);		// 読み込むメモリー

		for (int i = 0; i < LOGO_MAX; i++)
		{
			D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
				TEXTURE_TITLE_LOGO[i],			// ファイルの名前
				&g_pD3DTextureTitleLogo[i]);	// 読み込むメモリー
		}
	}

	// サウンドのロード
	g_pTitleBGM = LoadSound(BGM_TITLE);
	g_pTitleSE_Shot = LoadSound(SE_BOMB);

	for (int i = 0; i < LOGO_MAX; i++)
	{
		// サウンドのロード
		g_pTitleSE_Wind[i] = LoadSound(SE_WIND);

		g_posTitleLogo[i] = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	}

	// 頂点情報の作成
	MakeVertexTitle();

	g_stageState = TITLE_IN;
	g_interval = 10;
	g_staLogo = g_endLogo = 0;
	g_rotTitle = 0.0f;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	if (g_pD3DTextureTitleBG != NULL)
	{	// テクスチャの開放
		g_pD3DTextureTitleBG->Release();
		g_pD3DTextureTitleBG = NULL;
	}

	if (g_pD3DTextureTitleBGMono != NULL)
	{	// テクスチャの開放
		g_pD3DTextureTitleBGMono->Release();
		g_pD3DTextureTitleBGMono = NULL;
	}

	if (g_pD3DTextureTitleBGChroma != NULL)
	{	// テクスチャの開放
		g_pD3DTextureTitleBGChroma->Release();
		g_pD3DTextureTitleBGChroma = NULL;
	}

	if (g_pD3DTextureTitleChroma != NULL)
	{	// テクスチャの開放
		g_pD3DTextureTitleChroma->Release();
		g_pD3DTextureTitleChroma = NULL;
	}

	if (g_pD3DTextureTitleText != NULL)
	{	// テクスチャの開放
		g_pD3DTextureTitleText->Release();
		g_pD3DTextureTitleText = NULL;
	}

	if (g_pD3DTextureTitleVersion != NULL)
	{	// テクスチャの開放
		g_pD3DTextureTitleVersion->Release();
		g_pD3DTextureTitleVersion = NULL;
	}

	for (int i = 0; i < LOGO_MAX; i++)
	{
		if (g_pD3DTextureTitleLogo[i] != NULL)
		{	// テクスチャの開放
			g_pD3DTextureTitleLogo[i]->Release();
			g_pD3DTextureTitleLogo[i] = NULL;
		}
	}

	if (g_pTitleBGM != NULL)
	{	// サウンドの開放
		g_pTitleBGM->Release();
		g_pTitleBGM = NULL;
	}

	if (g_pTitleSE_Shot != NULL)
	{// サウンドの開放
		g_pTitleSE_Shot->Release();
		g_pTitleSE_Shot = NULL;
	}
	for (int i = 0; i < LOGO_MAX; i++)
	{
		if (g_pTitleSE_Wind[i] != NULL)
		{// サウンドの開放
			g_pTitleSE_Wind[i]->Release();
			g_pTitleSE_Wind[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
	if (IsAnyKeyTriggered() || IsAnyButtonTriggered(0)) 
	{
		switch (g_stageState)
		{
		case TITLE_IN:
			// サウンドの再生
			PlaySound(g_pTitleBGM, E_DS8_FLAG_LOOP);
			
			for (int i = 0; i < LOGO_MAX; i++)
			{
				g_posTitleLogo[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				// 頂点座標の設定
				g_vertexWkTitleLogo[0 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7), 32 + 96 * (i / 7), 0.0f);
				g_vertexWkTitleLogo[1 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7) + 256, 32 + 96 * (i / 7), 0.0f);
				g_vertexWkTitleLogo[2 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7), 32 + 96 * (i / 7) + 256, 0.0f);
				g_vertexWkTitleLogo[3 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7) + 256, 32 + 96 * (i / 7) + 256, 0.0f);

				// 反射光の設定
				g_vertexWkTitleVersion[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				g_vertexWkTitleVersion[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				g_vertexWkTitleVersion[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				g_vertexWkTitleVersion[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			}
			g_interval = 255;

			// 反射光の設定
			g_vertexWkTitleBG[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, g_interval);
			g_vertexWkTitleBG[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, g_interval);
			g_vertexWkTitleBG[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, g_interval);
			g_vertexWkTitleBG[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, g_interval);

			// 反射光の設定
			g_vertexWkTitleHole[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 - g_interval);
			g_vertexWkTitleHole[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 - g_interval);
			g_vertexWkTitleHole[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 - g_interval);
			g_vertexWkTitleHole[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 - g_interval);

			g_stageState++;
			break;
		case TITLE_WAIT:
			g_stageState++;
			g_interval = 0;
			SetFade(FADE_W_OUT, STAGE_RESET);
			break;
		case TITLE_OUT:
			break;
		}
	}
	
	switch (g_stageState)
	{
	case TITLE_IN:
		g_interval--;
		if (g_interval <= 0 && g_endLogo < LOGO_MAX)
		{
			// サウンドの再生
			PlaySound(g_pTitleSE_Wind[g_endLogo], E_DS8_FLAG_NONE);

			g_interval = TITLE_INTERVAL_LOGO;
			g_endLogo++;
		}
		for (int i = g_staLogo; i < g_endLogo; i++)
		{
			g_posTitleLogo[i].x = g_posTitleLogo[i].x * TITLE_INTERVAL_LOGO / (TITLE_INTERVAL_LOGO + 1);

			if (g_posTitleLogo[i].x <= 0.1f)
			{
				g_posTitleLogo[i].x = 0.0f;
				g_staLogo++;
			}
			// 頂点座標の設定
			g_vertexWkTitleLogo[0 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7), 32 + 96 * (i / 7), 0.0f);
			g_vertexWkTitleLogo[1 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7) + 256, 32 + 96 * (i / 7), 0.0f);
			g_vertexWkTitleLogo[2 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7), 32 + 96 * (i / 7) + 256, 0.0f);
			g_vertexWkTitleLogo[3 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7) + 256, 32 + 96 * (i / 7) + 256, 0.0f);
		}
		if (g_staLogo == LOGO_MAX)
		{
			if (g_rotTitle == 0.0f)
			{
				// サウンドの再生
				PlaySound(g_pTitleSE_Shot, E_DS8_FLAG_NONE);

				// 反射光の設定
				g_vertexWkTitleHole[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				g_vertexWkTitleHole[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				g_vertexWkTitleHole[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				g_vertexWkTitleHole[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			}

			g_rotTitle += D3DX_PI * 0.75;
			if (g_rotTitle < D3DX_PI * 50)
			{
				for (int i = 0; i < LOGO_MAX; i++)
				{
					g_posTitleLogo[i].x = (D3DX_PI * 50 - g_rotTitle) * cosf(g_rotTitle) * 0.25;
					g_posTitleLogo[i].y = (D3DX_PI * 50 - g_rotTitle) * sinf(g_rotTitle) * 0.5;

					// 頂点座標の設定
					g_vertexWkTitleLogo[0 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7), 32 + 96 * (i / 7), 0.0f);
					g_vertexWkTitleLogo[1 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7) + 256, 32 + 96 * (i / 7), 0.0f);
					g_vertexWkTitleLogo[2 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7), 32 + 96 * (i / 7) + 256, 0.0f);
					g_vertexWkTitleLogo[3 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7) + 256, 32 + 96 * (i / 7) + 256, 0.0f);
				}
			}
			else
			{
				// サウンドの再生
				PlaySound(g_pTitleBGM, E_DS8_FLAG_LOOP);

				for (int i = 0; i < LOGO_MAX; i++)
				{
					g_posTitleLogo[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				
					// 頂点座標の設定
					g_vertexWkTitleLogo[0 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7), 32 + 96 * (i / 7), 0.0f);
					g_vertexWkTitleLogo[1 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7) + 256, 32 + 96 * (i / 7), 0.0f);
					g_vertexWkTitleLogo[2 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7), 32 + 96 * (i / 7) + 256, 0.0f);
					g_vertexWkTitleLogo[3 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7) + 256, 32 + 96 * (i / 7) + 256, 0.0f);
				}

				// 反射光の設定
				g_vertexWkTitleVersion[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				g_vertexWkTitleVersion[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				g_vertexWkTitleVersion[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				g_vertexWkTitleVersion[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

				g_interval = 0;
				g_stageState++;
			}
		}
		break;
	case TITLE_WAIT:
		if (g_interval < 255)
		{
			g_interval ++;

			// 反射光の設定
			g_vertexWkTitleBG[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, g_interval);
			g_vertexWkTitleBG[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, g_interval);
			g_vertexWkTitleBG[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, g_interval);
			g_vertexWkTitleBG[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, g_interval);

			// 反射光の設定
			g_vertexWkTitleHole[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 - g_interval);
			g_vertexWkTitleHole[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 - g_interval);
			g_vertexWkTitleHole[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 - g_interval);
			g_vertexWkTitleHole[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 - g_interval);
		}
		g_rotTitle += 0.02f;
		if (g_rotTitle > D3DX_PI * 2) g_rotTitle -= D3DX_PI * 2;

		// 反射光の設定
		g_vertexWkTitleText[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 * (int)(1 + cosf(g_rotTitle * 2)));
		g_vertexWkTitleText[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 * (int)(1 + cosf(g_rotTitle * 2)));
		g_vertexWkTitleText[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 * (int)(1 + cosf(g_rotTitle * 2)));
		g_vertexWkTitleText[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 * (int)(1 + cosf(g_rotTitle * 2)));

		// 反射光の設定
		g_vertexWkTitleChroma[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, (int)(255 * (1 - cosf(g_rotTitle)) / 2.0f));
		g_vertexWkTitleChroma[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, (int)(255 * (1 - cosf(g_rotTitle)) / 2.0f));
		g_vertexWkTitleChroma[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, (int)(255 * (1 - cosf(g_rotTitle)) / 2.0f));
		g_vertexWkTitleChroma[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, (int)(255 * (1 - cosf(g_rotTitle)) / 2.0f));

		// 反射光の設定
		g_vertexWkTitleBGChroma[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, (int)(g_interval * (1 - cosf(g_rotTitle)) / 2.0f));
		g_vertexWkTitleBGChroma[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, (int)(g_interval * (1 - cosf(g_rotTitle)) / 2.0f));
		g_vertexWkTitleBGChroma[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, (int)(g_interval * (1 - cosf(g_rotTitle)) / 2.0f));
		g_vertexWkTitleBGChroma[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, (int)(g_interval * (1 - cosf(g_rotTitle)) / 2.0f));
		break;
	case TITLE_OUT:
		if (g_interval == 50)
		{
			// サウンドの停止
			StopSound(g_pTitleBGM);
		}
		else
		{
			g_interval++;
		}
		break;
	}
	
	PrintDebugProc("TITLE_STATE:%d\n", g_stageState);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureTitleBG);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleBG, sizeof(VERTEX_2D));


	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureTitleBGMono);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleBGMono, sizeof(VERTEX_2D));
	
	
	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureTitleBGChroma);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleBGChroma, sizeof(VERTEX_2D));


	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureTitleHole);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleHole, sizeof(VERTEX_2D));


	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureTitleChroma);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleChroma, sizeof(VERTEX_2D));


	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureTitleText);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleText, sizeof(VERTEX_2D));


	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureTitleVersion);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleVersion, sizeof(VERTEX_2D));
	
	for (int i = 0; i < LOGO_MAX; i++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureTitleLogo[i]);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &g_vertexWkTitleLogo[i * NUM_VERTEX], sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTitle(void)
{
	// 頂点座標の設定
	g_vertexWkTitleBG[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_vertexWkTitleBG[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	g_vertexWkTitleBG[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	g_vertexWkTitleBG[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkTitleBG[0].rhw =
	g_vertexWkTitleBG[1].rhw =
	g_vertexWkTitleBG[2].rhw =
	g_vertexWkTitleBG[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkTitleBG[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleBG[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleBG[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleBG[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

	// テクスチャ座標の設定
	g_vertexWkTitleBG[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitleBG[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitleBG[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitleBG[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// 頂点座標の設定
	g_vertexWkTitleBGMono[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_vertexWkTitleBGMono[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	g_vertexWkTitleBGMono[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	g_vertexWkTitleBGMono[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkTitleBGMono[0].rhw =
	g_vertexWkTitleBGMono[1].rhw =
	g_vertexWkTitleBGMono[2].rhw =
	g_vertexWkTitleBGMono[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkTitleBGMono[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleBGMono[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleBGMono[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleBGMono[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

	// テクスチャ座標の設定
	g_vertexWkTitleBGMono[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitleBGMono[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitleBGMono[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitleBGMono[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// 頂点座標の設定
	g_vertexWkTitleBGChroma[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_vertexWkTitleBGChroma[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	g_vertexWkTitleBGChroma[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	g_vertexWkTitleBGChroma[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkTitleBGChroma[0].rhw =
	g_vertexWkTitleBGChroma[1].rhw =
	g_vertexWkTitleBGChroma[2].rhw =
	g_vertexWkTitleBGChroma[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkTitleBGChroma[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleBGChroma[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleBGChroma[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleBGChroma[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

	// テクスチャ座標の設定
	g_vertexWkTitleBGChroma[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitleBGChroma[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitleBGChroma[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitleBGChroma[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// 頂点座標の設定
	g_vertexWkTitleHole[0].vtx = D3DXVECTOR3(POS_HOLE_X - 256, POS_HOLE_Y - 256, 0.0f);
	g_vertexWkTitleHole[1].vtx = D3DXVECTOR3(POS_HOLE_X + 256, POS_HOLE_Y - 256, 0.0f);
	g_vertexWkTitleHole[2].vtx = D3DXVECTOR3(POS_HOLE_X - 256, POS_HOLE_Y + 256, 0.0f);
	g_vertexWkTitleHole[3].vtx = D3DXVECTOR3(POS_HOLE_X + 256, POS_HOLE_Y + 256, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkTitleHole[0].rhw =
	g_vertexWkTitleHole[1].rhw =
	g_vertexWkTitleHole[2].rhw =
	g_vertexWkTitleHole[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkTitleHole[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleHole[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleHole[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleHole[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

	// テクスチャ座標の設定
	g_vertexWkTitleHole[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitleHole[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitleHole[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitleHole[3].tex = D3DXVECTOR2(1.0f, 1.0f);



	// 頂点座標の設定
	g_vertexWkTitleChroma[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_vertexWkTitleChroma[1].vtx = D3DXVECTOR3(1024, 0.0f, 0.0f);
	g_vertexWkTitleChroma[2].vtx = D3DXVECTOR3(0.0f, 512, 0.0f);
	g_vertexWkTitleChroma[3].vtx = D3DXVECTOR3(1024, 512, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkTitleChroma[0].rhw =
	g_vertexWkTitleChroma[1].rhw =
	g_vertexWkTitleChroma[2].rhw =
	g_vertexWkTitleChroma[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkTitleChroma[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleChroma[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleChroma[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleChroma[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

	// テクスチャ座標の設定
	g_vertexWkTitleChroma[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitleChroma[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitleChroma[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitleChroma[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// 頂点座標の設定
	g_vertexWkTitleText[0].vtx = D3DXVECTOR3(SCREEN_CENTER_X - 256, SCREEN_HEIGHT - 224, 0.0f);
	g_vertexWkTitleText[1].vtx = D3DXVECTOR3(SCREEN_CENTER_X + 256, SCREEN_HEIGHT - 224, 0.0f);
	g_vertexWkTitleText[2].vtx = D3DXVECTOR3(SCREEN_CENTER_X - 256, SCREEN_HEIGHT - 128, 0.0f);
	g_vertexWkTitleText[3].vtx = D3DXVECTOR3(SCREEN_CENTER_X + 256, SCREEN_HEIGHT - 128, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkTitleText[0].rhw =
	g_vertexWkTitleText[1].rhw =
	g_vertexWkTitleText[2].rhw =
	g_vertexWkTitleText[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkTitleText[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleText[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleText[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleText[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

	// テクスチャ座標の設定
	g_vertexWkTitleText[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitleText[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitleText[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitleText[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// 頂点座標の設定
	g_vertexWkTitleVersion[0].vtx = D3DXVECTOR3(SCREEN_WIDTH - 160, SCREEN_HEIGHT - 64, 0.0f);
	g_vertexWkTitleVersion[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 64, 0.0f);
	g_vertexWkTitleVersion[2].vtx = D3DXVECTOR3(SCREEN_WIDTH - 160, SCREEN_HEIGHT, 0.0f);
	g_vertexWkTitleVersion[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkTitleVersion[0].rhw =
	g_vertexWkTitleVersion[1].rhw =
	g_vertexWkTitleVersion[2].rhw =
	g_vertexWkTitleVersion[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkTitleVersion[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleVersion[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleVersion[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	g_vertexWkTitleVersion[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

	// テクスチャ座標の設定
	g_vertexWkTitleVersion[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitleVersion[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitleVersion[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitleVersion[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	for (int i = 0; i < LOGO_MAX; i++)
	{
		// 頂点座標の設定
		g_vertexWkTitleLogo[0 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7), 32 + 96 * (i / 7), 0.0f);
		g_vertexWkTitleLogo[1 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7) + 256, 32 + 96 * (i / 7), 0.0f);
		g_vertexWkTitleLogo[2 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7), 32 + 96 * (i / 7) + 256, 0.0f);
		g_vertexWkTitleLogo[3 + i * NUM_VERTEX].vtx = g_posTitleLogo[i] + D3DXVECTOR3(192 * (i / 7) + (128 - 32 * (i / 7)) * (i % 7) + 256, 32 + 96 * (i / 7) + 256, 0.0f);

		// テクスチャのパースペクティブコレクト用
		g_vertexWkTitleLogo[0 + i * NUM_VERTEX].rhw =
		g_vertexWkTitleLogo[1 + i * NUM_VERTEX].rhw =
		g_vertexWkTitleLogo[2 + i * NUM_VERTEX].rhw =
		g_vertexWkTitleLogo[3 + i * NUM_VERTEX].rhw = 1.0f;

		// 反射光の設定
		g_vertexWkTitleLogo[0 + i * NUM_VERTEX].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleLogo[1 + i * NUM_VERTEX].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleLogo[2 + i * NUM_VERTEX].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleLogo[3 + i * NUM_VERTEX].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		g_vertexWkTitleLogo[0 + i * NUM_VERTEX].tex = D3DXVECTOR2(0.0f, 0.0f);
		g_vertexWkTitleLogo[1 + i * NUM_VERTEX].tex = D3DXVECTOR2(1.0f, 0.0f);
		g_vertexWkTitleLogo[2 + i * NUM_VERTEX].tex = D3DXVECTOR2(0.0f, 1.0f);
		g_vertexWkTitleLogo[3 + i * NUM_VERTEX].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	return S_OK;
}

