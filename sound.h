//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <Windows.h>

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_SE_SHOT,		// �e���ˉ�
	SOUND_LABEL_SE_EXPLOSION,	// ������
	SOUND_LABEL_SE_BARI_CRE,	// �o���A����
	SOUND_LABEL_SE_BARI_COL,	// �o���A�Փ�
	SOUND_LABEL_SE_BUL_COL,		// �e�Փ�
	SOUND_LABEL_SE_DECISION,	// ���艹
	SOUND_LABEL_SE_ITEM,		// �A�C�e���l��
	SOUND_LABEL_SE_POWER_UP,	// �p���[�A�b�v�l����
	SOUND_LABEL_SE_LEVEL_UP,	// ���x���A�b�v
	SOUND_LABEL_BGM_TITLE,		// �^�C�g��BGM
	SOUND_LABEL_BGM_MAIN,		// �Q�[�����C��BGM
	SOUND_LABEL_BGM_BOSS,		// �{�XBGM
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
