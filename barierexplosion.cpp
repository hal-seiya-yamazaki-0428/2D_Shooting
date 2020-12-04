#include "explosion.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "player.h"
#include "sound.h"

//=====================================================
//グローバル変数
//=====================================================
static EXPLOSION g_BarExp[EXPLOSION_MAX];

//=====================================================
//初期化
//=====================================================
void BarExp_Init(void)
{
	g_BarExp[0].tex = Texture_SetLoadFile("Texture\\UV_bluefire.png", 320, 256);
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		g_BarExp[i].isUse = false;
		g_BarExp[i].pos = D3DXVECTOR2(0, 0);
		g_BarExp[i].frameCount = 0;
	}
}

//=====================================================
//終了
//=====================================================
void BarExp_Uninit(void)
{

}

//=====================================================
//更新
//=====================================================
void BarExp_Update(void)
{
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		if (!g_BarExp[i].isUse)
		{
			continue;
		}
		if (g_BarExp[i].frameCount > 14 * 3)
		{
			g_BarExp[i].isUse = false;
			g_BarExp[i].frameCount = 0;
		}
		g_BarExp[i].frameCount++;
	}
}

//=====================================================
//描画
//=====================================================
void BarExp_Draw(void)
{
	int width = Texture_GetWidth(g_BarExp[0].tex) / 5;
	int height = Texture_GetHeight(g_BarExp[0].tex) / 4;
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		if (!g_BarExp[i].isUse)
		{
			continue;
		}
		int pattern = g_BarExp[i].frameCount / 3;
		//爆発の描画
		Sprite_Draw(g_BarExp[0].tex, g_BarExp[i].pos.x, g_BarExp[i].pos.y, pattern % 5 * width, pattern / 4 * height, width, height);
	}
}

//=====================================================
//爆発の生成
//=====================================================
void BarExp_Create(float x, float y)
{
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		if (!g_BarExp[i].isUse)
		{
			g_BarExp[i].pos.x = x;
			g_BarExp[i].pos.y = y;
			g_BarExp[i].isUse = true;
			PlaySound(SOUND_LABEL_SE_BARI_COL);
			break;
		}
	}
}
