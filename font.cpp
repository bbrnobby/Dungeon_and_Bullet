//=============================================================================
//
// 文字処理 [font.cpp]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#include "font.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexFont(int no);
void SetTextureFont(int no);
void SetVertexFont(int no);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureFont = NULL;					// テクスチャへのポリゴン

FONT					fontWk[FONT_BUFFER];						// フォント構造体

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitFont(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FONT *font = fontWk;					// フォントのポインターを初期化

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
			TEXTURE_GAME_FONT,				// ファイルの名前
			&g_pD3DTextureFont);			// 読み込むメモリのポインタ
	}

	// フォントの初期化処理
	for (int i = 0; i < FONT_BUFFER; i++, font++)
	{
		font->use = false;										// 使用
		font->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 座標データを初期化
		font->size = TEXTURE_FONT_SIZE;
		font->index = 0;

		font->duration = 0;										// アニメカウントを初期化

		font->Texture = g_pD3DTextureFont;						// テクスチャ情報
		MakeVertexFont(i);										// 頂点情報の作成
	}

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitFont(void)
{
	if (g_pD3DTextureFont != NULL)
	{	// テクスチャの開放
		g_pD3DTextureFont->Release();
		g_pD3DTextureFont = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateFont(void)
{
	FONT *font = fontWk;

	for (int i = 0; i < FONT_BUFFER; i++, font++)
	{
		if (font->use)			// 使用している状態なら更新する
		{
			if (font->duration > 0) font->duration--;
			else if (font->duration == 0)
			{
				font->use = false;
			}
			SetVertexFont(i);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawFont(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FONT *font = fontWk;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < FONT_BUFFER; i++, font++)
	{
		if (font->use)					// 使用している状態なら描画する
		{
			// テクスチャの設定
			pDevice->SetTexture(0, font->Texture);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, font->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexFont( int no )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FONT *font = &fontWk[no];

	// 頂点座標の設定
	SetVertexFont(no);

	// rhwの設定
	font->vertexWk[0].rhw =
	font->vertexWk[1].rhw =
	font->vertexWk[2].rhw =
	font->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	font->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	font->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	font->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	font->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	font->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	font->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_FONT_DIVIDE_X, 0.0f);
	font->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_FONT_DIVIDE_Y);
	font->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_FONT_DIVIDE_X, 1.0f / TEXTURE_PATTERN_FONT_DIVIDE_Y);


	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureFont(int no)
{
	FONT *font = &fontWk[no];

	int x,y;
	x = (TEXTURE_PATTERN_FONT_DIVIDE_X - 1) - (font->index / TEXTURE_PATTERN_FONT_DIVIDE_Y);
	y = font->index % TEXTURE_PATTERN_FONT_DIVIDE_Y;

	// テクスチャ座標の設定
	font->vertexWk[0].tex = D3DXVECTOR2(x * (1.0f / TEXTURE_PATTERN_FONT_DIVIDE_X), y * (1.0f / TEXTURE_PATTERN_FONT_DIVIDE_Y));
	font->vertexWk[1].tex = D3DXVECTOR2((x + 1) * (1.0f / TEXTURE_PATTERN_FONT_DIVIDE_X), y * (1.0f / TEXTURE_PATTERN_FONT_DIVIDE_Y));
	font->vertexWk[2].tex = D3DXVECTOR2(x * (1.0f / TEXTURE_PATTERN_FONT_DIVIDE_X), (y + 1) * (1.0f / TEXTURE_PATTERN_FONT_DIVIDE_Y));
	font->vertexWk[3].tex = D3DXVECTOR2((x + 1) * (1.0f / TEXTURE_PATTERN_FONT_DIVIDE_X), (y + 1) * (1.0f / TEXTURE_PATTERN_FONT_DIVIDE_Y));
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexFont(int no)
{
	FONT *font = &fontWk[no];
	D3DXVECTOR3 *subPosCamera = GetSubPosCamera();

	// 頂点座標の設定
	font->vertexWk[0].vtx.x = font->pos.x;
	font->vertexWk[0].vtx.y = font->pos.y;
	font->vertexWk[0].vtx.z = 0.0f;
	font->vertexWk[0].vtx += *subPosCamera;

	font->vertexWk[1].vtx.x = font->pos.x + font->size * 0.75f;
	font->vertexWk[1].vtx.y = font->pos.y;
	font->vertexWk[1].vtx.z = 0.0f;
	font->vertexWk[1].vtx += *subPosCamera;

	font->vertexWk[2].vtx.x = font->pos.x;
	font->vertexWk[2].vtx.y = font->pos.y + font->size;
	font->vertexWk[2].vtx.z = 0.0f;
	font->vertexWk[2].vtx += *subPosCamera;

	font->vertexWk[3].vtx.x = font->pos.x + font->size * 0.75f;
	font->vertexWk[3].vtx.y = font->pos.y + font->size;
	font->vertexWk[3].vtx.z = 0.0f;
	font->vertexWk[3].vtx += *subPosCamera;
}

void SetString(char *string, float posX, float posY, int size, int interval)
{
	char *buf;
	int length, index;

	for (length = 0, buf = string; *buf != '\0'; length++, buf++)
	{
		if (*buf == -125 || *buf == -126 || *buf == -127) buf++;
	}

	for (index = 0, buf = string; *buf != '\0'; index = 0, buf++)
	{
		if (*buf == -125)
		{
			buf++;
			// 「ァ」～「ミ」
			if (*buf >= 64 && *buf <= 126)
			{
				index = *buf + 20;
			}
			// 「ム」～「ヴ」
			else if (*buf >= -128 && *buf <= -108)
			{
				index = *buf + 275;
			}
		}
		else if (*buf == -126)
		{
			buf++;
			// 「ぁ」～「ん」
			if (*buf >= -97 && *buf <= -15)
			{
				index = *buf + 97;
			}
			// 「０」～「９」
			else if (*buf >= 79 && *buf <= 88)
			{
				index = *buf + 89;
			}
		}
		else if (*buf == -127)
		{
			buf++;
			// 「　」
			if (*buf == 64)
			{
				index = 83;
			}
			// 「＋」
			else if (*buf == 123)
			{
				index = 207;
			}
			// 「ー」・「―」・「－」
			else if (*buf == 91 || *buf == 92 || *buf == 124)
			{
				index = 206;
			}
			// 「×」・「＊」
			else if (*buf == 126 || *buf == -106)
			{
				index = 208;
			}
			// 「÷」・「／」
			else if (*buf == -128 || *buf == 94)
			{
				index = 209;
			}
			// 「！」
			else if (*buf == 73)
			{
				index = 178;
			}
			// 「？」
			else if (*buf == 72)
			{
				index = 179;
			}
		}
		// 「0」～「9」
		else if (*buf >= 48 && *buf <= 57)
		{
			index = *buf + 120;
		}
		// 「A」～「Z」
		else if (*buf >= 65 && *buf <= 90)
		{
			index = *buf + 115;
		}
		// 「a」～「z」
		else if (*buf >= 97 && *buf <= 122)
		{
			index = *buf + 83;
		}
		// 「 」
		else if (*buf == 32)
		{
			index = 83;
		}
		// 「+」
		else if (*buf == 43)
		{
			index = 207;
		}
		// 「-」
		else if (*buf == 45)
		{
			index = 206;
		}
		// 「*」
		else if (*buf == 42)
		{
			index = 208;
		}
		// 「/」
		else if (*buf == 47)
		{
			index = 209;
		}
		// 「!」
		else if (*buf == 33)
		{
			index = 178;
		}
		// 「?」
		else if (*buf == 63)
		{
			index = 179;
		}

		if (index != 0)
		{
			FONT *font = fontWk;
			for (int i = 0; i < FONT_BUFFER; i++, font++)
			{
				if (!font->use)
				{
					font->use = true;
					font->pos.x = posX + size * (-length * 0.5f)* 0.75f;
					font->pos.y = posY - size * 0.5f;
					font->size = size;
					font->index = index;

					font->duration = interval;

					SetTextureFont(i);
					SetVertexFont(i);
					break;
				}
			}
		}
		posX += size * 0.75f;
	}
}

void RefreshFont(void)
{
	FONT *font = fontWk;

	for (int i = 0; i < FONT_BUFFER; i++, font++)
	{
		if (font->use && font->duration > 0)
		{
			font->use = false;
			font->pos.x = 0.0f;
			font->pos.y = 0.0f;
			font->size = TEXTURE_FONT_SIZE;
			font->index = 0;

			font->duration = 0;	

			SetTextureFont(i);
			SetVertexFont(i);
		}
	}
}