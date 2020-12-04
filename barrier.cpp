#include "barrier.h"
#include "texture.h"
#include "player.h"
#include "sprite.h"
#include "game.h"
#include "sound.h"

//=====================================================
//グローバル変数
//=====================================================
static BARRIER g_Barrier;

//=====================================================
//初期化
//=====================================================
void Barrier_Init(void)
{
	g_Barrier.isUse = false;
	g_Barrier.Pos = D3DXVECTOR2(0, 0);
	g_Barrier.Tex = Texture_SetLoadFile("Texture\\spr_shield.png", 128, 128);
}
//=====================================================
//終了
//=====================================================
void Barrier_Uninit(void)
{

}
//=====================================================
//更新
//=====================================================
void Barrier_Update(void)
{
	if (g_Barrier.isUse)
	{
		if (Game_GetScene() == SCENE_TUTORIAL)
		{
			return;
		}
		PLAYER *pPlayer = Player_GetObj();
		Player_Damage(GAUGE_EN, 0.3f);
		g_Barrier.Pos.x = pPlayer->pos.x - Texture_GetWidth(g_Barrier.Tex) / 4;
		g_Barrier.Pos.y = pPlayer->pos.y - Texture_GetHeight(g_Barrier.Tex) / 4;
	}
}
//=====================================================
//描画
//=====================================================
void Barrier_Draw(void)
{
	if (g_Barrier.isUse)
	{
		Sprite_Draw(g_Barrier.Tex, g_Barrier.Pos.x, g_Barrier.Pos.y, 128);
	}
}
//=====================================================
//生成
//=====================================================
void Barrier_Create(float x, float y)
{
	if (!g_Barrier.isUse)
	{
		g_Barrier.Pos.x = x - Texture_GetWidth(g_Barrier.Tex) / 4;
		g_Barrier.Pos.y = y - Texture_GetHeight(g_Barrier.Tex) / 4;
		g_Barrier.isUse = true;
		PlaySound(SOUND_LABEL_SE_BARI_CRE);
	}
}
//=====================================================
//生成(サウンド再生)
//=====================================================
void Barrier_Create(float x, float y, bool sound)
{
	if (!g_Barrier.isUse)
	{
		g_Barrier.Pos.x = x - Texture_GetWidth(g_Barrier.Tex) / 4;
		g_Barrier.Pos.y = y - Texture_GetHeight(g_Barrier.Tex) / 4;
		g_Barrier.isUse = true;
		if (sound)
		{
			PlaySound(SOUND_LABEL_SE_BARI_CRE);
		}
	}
}
//=====================================================
//フラグ削除
//=====================================================
void Barrier_Destroy(void)
{
	g_Barrier.isUse = false;
}
//=====================================================
//使用中か調べる
//=====================================================
bool Barrier_isUsed(void)
{
	return g_Barrier.isUse;
}