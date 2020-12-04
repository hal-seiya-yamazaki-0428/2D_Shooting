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
//マクロ定義
//=====================================================
#define TEXTURE_MAX (16)

//=====================================================
//グローバル変数
//=====================================================
static unsigned int g_Tex[TEXTURE_MAX];
static int g_alpha = 0;
static int g_frame = 0;
static XINPUT_STATE g_State;
static bool g_Trigger;

//=====================================================
//初期化
//=====================================================
void Result_Init()
{
	g_Tex[0] = Texture_SetLoadFile("Texture\\font.png", 512, 512);
	g_alpha = 0;
	g_frame = 0;
	g_Trigger = false;
}

//=====================================================
//終了
//=====================================================
void Result_Uninit()
{

}

//=====================================================
//更新
//=====================================================
void Result_Update()
{
	//半透明の黒背景
	Fade_Half();
	g_alpha += 2;

	//アルファの増減
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
	//キーボード更新
	Keyboard_Update();
	//キー入力
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
//描画
//=====================================================
void Result_Draw()
{
	//ゲームオーバー表示
	if (Game_GetScene() == SCENE_GAMEOVER)
	{
		Sprite_Draw(g_Tex[0], 300.0f, 250.0f, 0, 128, 512, 64, g_alpha);
	}
	//ゲームクリア表示
	else if (Game_GetScene() == SCENE_GAMECLEAR)
	{
		Sprite_Draw(g_Tex[0], 300.0f, 250.0f, 0, 192, 512, 64, g_alpha);
	}
}