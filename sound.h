//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <Windows.h>

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_SE_SHOT,		// 弾発射音
	SOUND_LABEL_SE_EXPLOSION,	// 爆発音
	SOUND_LABEL_SE_BARI_CRE,	// バリア生成
	SOUND_LABEL_SE_BARI_COL,	// バリア衝突
	SOUND_LABEL_SE_BUL_COL,		// 弾衝突
	SOUND_LABEL_SE_DECISION,	// 決定音
	SOUND_LABEL_SE_ITEM,		// アイテム獲得
	SOUND_LABEL_SE_POWER_UP,	// パワーアップ獲得時
	SOUND_LABEL_SE_LEVEL_UP,	// レベルアップ
	SOUND_LABEL_BGM_TITLE,		// タイトルBGM
	SOUND_LABEL_BGM_MAIN,		// ゲームメインBGM
	SOUND_LABEL_BGM_BOSS,		// ボスBGM
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
