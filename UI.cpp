#include "sprite.h"
#include "texture.h"
#include <d3dx9.h>
#include "UI.h"
#include "player.h"
#include "bg.h"
#include "game.h"
#include "medium_boss.h"
#include "boss.h"

//=====================================================
//マクロ定義
//=====================================================
#define TEXTURE_MAX (16)	//テクスチャの数

//=====================================================
//グローバル変数
//=====================================================
static unsigned int g_Tex[TEXTURE_MAX];
static float g_conductor;
static D3DXVECTOR2 g_pos;

//=====================================================
//初期化
//=====================================================
void UI_Init(void)
{
	g_Tex[0] = Texture_SetLoadFile("Texture\\HP_Gauge_bg.png", 200, 32);		//ゲージ後ろの黒背景
	g_Tex[1] = Texture_SetLoadFile("Texture\\HP_Gauge_01_green.png", 200, 32);	//hpゲージ(緑)
	g_Tex[2] = Texture_SetLoadFile("Texture\\HP_Gauge_01_frame.png", 200, 32);	//ゲージ枠
	g_Tex[3] = Texture_SetLoadFile("Texture\\HP_Gauge_01_blue.png", 200, 32);	//enゲージ(青)
	g_Tex[4] = Texture_SetLoadFile("Texture\\ハート.png", 50, 50);				//hp用のアイコン
	g_Tex[5] = Texture_SetLoadFile("Texture\\エネルギー.png", 60, 39);			//en用のアイコン
	g_Tex[6] = Texture_SetLoadFile("Texture\\black.jpeg", BG_START, 744);		//黒背景
	g_Tex[7] = Texture_SetLoadFile("Texture\\start.png", 71, 22);				//スタートアイコン
	g_Tex[8] = Texture_SetLoadFile("Texture\\clear.png", 67, 22);				//ゴールアイコン
	g_Tex[9] = Texture_SetLoadFile("Texture\\laser.png", 512, 650);				//導線
	g_Tex[10] = Texture_SetLoadFile("Texture\\p_red.png", 54, 54);				//プレイヤーアイコン
	g_Tex[11] = Texture_SetLoadFile("Texture\\operation.png", 136, 203);		//操作説明
	g_conductor = 0;
	g_pos = D3DXVECTOR2((FLOAT)(BG_START - Texture_GetWidth(g_Tex[7]) - 30.0f + 10.0f) - 3.0f, (FLOAT)(BG_HEIGHT - Texture_GetHeight(g_Tex[7]) - 25.0f));
}
//=====================================================
//終了
//=====================================================
void UI_Uninit(void)
{

}
//=====================================================
//更新
//=====================================================
void UI_Updata(void)
{
	//マップの進行
	if (!Medium_Boss_isUsed() && !Boss_isUsed() && Game_GetScene() == SCENE_MAIN)
	{
		UI_CondCount();
	}
	//ゴール
	if (g_conductor > 500)
	{
		g_conductor = 500;
		Game_SceneChange(SCENE_GAMECLEAR);
	}
}
//=====================================================
//描画
//=====================================================
void UI_Draw(void)
{
	PLAYER *pPlayer = Player_GetObj();

	//黒枠
	Sprite_Draw(g_Tex[6], 0, 0);					//左
	Sprite_Draw(g_Tex[6], BG_START + BG_WIDTH, 0, 0, 0, 1024, 1024);	//右
	for (int i = 0; i < 3; i++)
	{
		Sprite_Draw(g_Tex[6], (float)(BG_START + i * BG_START), (float)BG_HEIGHT);	//下
	}

	//メイン画面でなければ表示しない
	if (Game_GetScene() != SCENE_MAIN)
	{
		return;
	}

	//hpゲージの表示
	Sprite_Draw(g_Tex[0], 300, BG_HEIGHT + 10);									//背景
	Sprite_Draw(g_Tex[1], 300, BG_HEIGHT + 10, 0, 0, (int)(Texture_GetWidth(g_Tex[1]) * pPlayer->HP / pPlayer->MaxHP), 32);		//緑ゲージ
	Sprite_Draw(g_Tex[2], 300, BG_HEIGHT + 10);									//フレーム

	//enゲージの表示
	Sprite_Draw(g_Tex[0], 580, BG_HEIGHT + 10);									//背景
	Sprite_Draw(g_Tex[3], 580, BG_HEIGHT + 10, 0, 0, (int)(Texture_GetWidth(g_Tex[1]) * pPlayer->EN / pPlayer->MaxEN), 32);		//青ゲージ
	Sprite_Draw(g_Tex[2], 580, BG_HEIGHT + 10);									//フレーム

	//hp、enのアイコン表示
	Sprite_Draw(g_Tex[4], 250, BG_HEIGHT);
	Sprite_Draw(g_Tex[5], 530, BG_HEIGHT + 5);

	//導線表示
	int width = Texture_GetWidth(g_Tex[9]);
	int height = Texture_GetHeight(g_Tex[9]);
	Sprite_Draw(g_Tex[9], (float)(BG_START - Texture_GetWidth(g_Tex[7]) - 30.0f), (float)((BG_HEIGHT - height) + 100),
		width * 5 / 7, 100, width / 7, height - 100);
	//スタート表示
	Sprite_Draw(g_Tex[7], (float)(BG_START - Texture_GetWidth(g_Tex[7]) - 30.0f), (float)(BG_HEIGHT - Texture_GetHeight(g_Tex[7])));
	//ゴール表示
	Sprite_Draw(g_Tex[8], (float)(BG_START - Texture_GetWidth(g_Tex[8]) - 30.0f), (float)(Texture_GetHeight(g_Tex[8])));
	//地図上のプレイヤーアイコン
	Sprite_Draw(g_Tex[10], g_pos.x, g_pos.y - g_conductor);
	//操作説明
	Sprite_Draw(g_Tex[11], (float)(BG_START + BG_WIDTH), (float)(BG_HEIGHT - Texture_GetHeight(g_Tex[11])));
}

//=====================================================
//プレイヤーアイコンの進行
//=====================================================
void UI_CondCount(void)
{
	g_conductor += 0.1f;
}

//=====================================================
//プレイヤーアイコンの場所
//=====================================================
float UI_GetCond(void)
{
	return g_conductor;
}