#include "texture.h"
#include "sprite.h"
#include "game.h"
#include "fade.h"
#include "input.h"
#include "sound.h"
#include "soundgenerater.h"
#include <Xinput.h>

#pragma comment(lib, "Xinput.lib")

//=====================================================
//�}�N����`
//=====================================================
#define TEXTURE_MAX (16)

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static unsigned int g_Tex[TEXTURE_MAX];
static int g_alpha = 0;
static int g_frame = 0;
static XINPUT_STATE g_State;
static bool g_Trigger;

//=====================================================
//������
//=====================================================
void Result_Init()
{
	g_Tex[0] = Texture_SetLoadFile("Texture\\font.png", 512, 512);
	g_alpha = 0;
	g_frame = 0;
	g_Trigger = false;
}

//=====================================================
//�I��
//=====================================================
void Result_Uninit()
{

}

//=====================================================
//�X�V
//=====================================================
void Result_Update()
{
	//�������̍��w�i
	Fade_Half();
	g_alpha += 2;

	//�A���t�@�̑���
	if (g_alpha > 255)
	{
		g_alpha = 255;
	}
	g_frame++;
	if (g_frame < 200)
	{
		return;
	}
	XInputGetState(0, &g_State);
	//�L�[�{�[�h�X�V
	Keyboard_Update();
	//�L�[����
	if (Keyboard_IsTrigger(DIK_K) || Keyboard_IsTrigger(DIK_SPACE) || g_State.Gamepad.wButtons & XINPUT_GAMEPAD_A && g_Trigger)
	{
		Sound_Reset();
		StopSound();
		Fade_Destroy();
		Game_SceneChange(SCENE_TITLE);
		Game_Uninit();
		Game_Init(Game_GetHwnd());
		g_Trigger = false;
	}
	else if (!(g_State.Gamepad.wButtons & XINPUT_GAMEPAD_A))
	{
		g_Trigger = true;
	}
}

//=====================================================
//�`��
//=====================================================
void Result_Draw()
{
	//�Q�[���I�[�o�[�\��
	if (Game_GetScene() == SCENE_GAMEOVER)
	{
		Sprite_Draw(g_Tex[0], 300.0f, 250.0f, 0, 128, 512, 64, g_alpha);
	}
	//�Q�[���N���A�\��
	else if (Game_GetScene() == SCENE_GAMECLEAR)
	{
		Sprite_Draw(g_Tex[0], 300.0f, 250.0f, 0, 192, 512, 64, g_alpha);
	}
}