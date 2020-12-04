#include "texture.h"
#include "sprite.h"
#include "game.h"
#include "bg.h"
#include "debug_font.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "soundgenerater.h"
#include <Xinput.h>

#pragma comment(lib, "Xinput.lib")

//=====================================================
//�}�N����`
//=====================================================
#define TEXTURE_MAX (16)	//�e�N�X�`���̐�
#define INCRE_NUM (4)		//�_�ł��銴�o

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static unsigned int g_Tex[TEXTURE_MAX];
static int g_alpha = INCRE_NUM;
static int g_incre = INCRE_NUM;
static XINPUT_STATE g_State;
static bool g_Trigger = false;

//=====================================================
//������
//=====================================================
void Title_Init()
{
	g_Tex[0] = Texture_SetLoadFile("Texture\\shooting_title.png", 897 / 2, 505 / 2);
	g_Tex[1] = Texture_SetLoadFile("Texture\\font.png", 512, 512);
	g_alpha = INCRE_NUM;
	g_incre = INCRE_NUM;
	g_Trigger = false;
}

//=====================================================
//�I��
//=====================================================
void Title_Uninit()
{

}

//=====================================================
//�X�V
//=====================================================
void Title_Update()
{
	Sound_Generater(SOUND_LABEL_BGM_TITLE);
	//press k button �_��
	if (g_alpha > 255 - INCRE_NUM)
	{
		g_incre *= -1;
	}
	else if (g_alpha < INCRE_NUM)
	{
		g_incre *= -1;
	}
	g_alpha += g_incre;

	XInputGetState(0, &g_State);

	//�L�[�{�[�h�X�V
	Keyboard_Update();
	//�L�[����
	if (Keyboard_IsTrigger(DIK_K) || g_State.Gamepad.wButtons & XINPUT_GAMEPAD_A && g_Trigger)
	{
		PlaySound(SOUND_LABEL_SE_DECISION);
		Fade_InOut(SCENE_TUTORIAL);
		g_Trigger = false;
	}
	else if(!(g_State.Gamepad.wButtons & XINPUT_GAMEPAD_A))
	{
		g_Trigger = true;
	}
}

//=====================================================
//�`��
//=====================================================
void Title_Draw()
{
	//�^�C�g���\��
	Sprite_Draw(g_Tex[0], (float)(BG_START * 1.5), 50.0f);

	//press k button
	Sprite_Draw(g_Tex[1], (float)(BG_START * 1.3), (float)(BG_HEIGHT - 200), 0, 0, 512, 64, g_alpha);
}