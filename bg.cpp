#include "texture.h"
#include "sprite.h"
#include <d3dx9.h>
#include "common.h"
#include "bg.h"
#include "UI.h"

//=====================================================
//グローバル変数
//=====================================================
static int g_Tex;
static D3DXVECTOR2 g_Pos;

//=====================================================
//初期化処理
//=====================================================
void BG_Init(void)
{
	g_Tex = Texture_SetLoadFile("Texture\\bg02.jpg", BG_WIDTH, BG_HEIGHT);
	g_Pos = D3DXVECTOR2(0, 0);
}
//=====================================================
//終了処理
//=====================================================
void BG_Uninit(void)
{

}
//=====================================================
//更新
//=====================================================
void BG_Update(void)
{
	//ゴールするまで背景スクロール
	if (!(UI_GetCond() > 490))
	{
		g_Pos.y++;
	}
	if (g_Pos.y > BG_HEIGHT)
	{
		g_Pos.y = 0;
	}
}
//=====================================================
//描画
//=====================================================
void BG_Draw(void)
{
	Sprite_Draw(g_Tex, (float)BG_START, (float)g_Pos.y, 0, 0, BG_WIDTH, (int)(BG_HEIGHT - g_Pos.y));
	Sprite_Draw(g_Tex, (float)BG_START, (float)(-BG_HEIGHT + g_Pos.y), 0, 0, BG_WIDTH, BG_HEIGHT);
}