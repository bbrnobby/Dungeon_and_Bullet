//=============================================================================
//
// リザルト画面処理 [result.h]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RESULT			_T("data/TEXTURE/bg001.jpg")			// 読み込むテクスチャファイル名
#define	TEXTURE_RESULT_LOGO		_T("data/TEXTURE/result_logo.png")		// 読み込むテクスチャファイル名
#define TEXTURE_RESULT_IMAGE	_T("data/TEXTURE/kaizoku_takara.png")	// 読み込むテクスチャファイル名
#define	RESULT_POS_X			(0)			// リザルト背景の表示位置
#define	RESULT_POS_Y			(0)			// リザルト背景の表示位置
#define	RESULT_SIZE_X			(1080)		// リザルト背景の幅
#define	RESULT_SIZE_Y			(720)		// リザルト背景の高さ

#define	RESULTLOGO_POS_X		(300)		// リザルトロゴの表示位置
#define	RESULTLOGO_POS_Y		(100)		// リザルトロゴの表示位置
#define	RESULTLOGO_SIZE_X		(480)		// リザルトロゴの幅
#define	RESULTLOGO_SIZE_Y		(80)		// リザルトロゴの高さ

#define	RESULTIMAGE_POS_X		(380)		// リザルト画像の表示位置
#define	RESULTIMAGE_POS_Y		(350)		// リザルト画像の表示位置
#define	RESULTIMAGE_SIZE_X		(320)		// リザルト画像の幅
#define	RESULTIMAGE_SIZE_Y		(320)		// リザルト画像の高さ

#define RESULTSCORE_POS_X		(380)		// リザルトスコアの表示位置
#define RESULTSCORE_POS_Y		(240)		// リザルトスコアの表示位置
#define RESULTSCORE_SIZE_X		(64)		// リザルトスコアの文字横サイズ
#define RESULTSCORE_SIZE_Y		(128)		// リザルトスコアの文字縦サイズ
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

#endif
