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
//マクロ定義
//=====================================================
#define TEXTURE_MAX (16)	//テクスチャの数
#define INCRE_NUM (4)		//点滅する感覚

//=====================================================
//グローバル変数
//=====================================================
static unsigned int g_Tex[TEXTURE_MAX];
static int g_alpha = INCRE_NUM;
static int g_incre = INCRE_NUM;
static XINPUT_STATE g_State;
static bool g_Trigger = false;

//=====================================================
//初期化
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
//終了
//=====================================================
void Title_Uninit()
{

}

//=====================================================
//更新
//=====================================================
void Title_Update()
{
	Sound_Generater(SOUND_LABEL_BGM_TITLE);
	//press k button 点滅
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

	//キーボード更新
	Keyboard_Update();
	//キー入力
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
//描画
//=====================================================
void Title_Draw()
{
	//タイトル表示
	Sprite_Draw(g_Tex[0], (float)(BG_START * 1.5), 50.0f);

	//press k button
	Sprite_Draw(g_Tex[1], (float)(BG_START * 1.3), (float)(BG_HEIGHT - 200), 0, 0, 512, 64, g_alpha);
}