#include "explosion.h"
#include "texture.h"
#include "sprite.h"
#include "enemy.h"
#include "sound.h"
#include "fade.h"

//=====================================================
//マクロ定義
//=====================================================

//=====================================================
//グローバル変数
//=====================================================
static EXPLOSION g_Explosion[EXPLOSION_MAX];

//=====================================================
//初期化
//=====================================================
void Explosion_Init(void)
{
	g_Explosion[0].tex = Texture_SetLoadFile("Texture\\exp04.png", 568, 296);
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		g_Explosion[i].isUse = false;
		g_Explosion[i].pos = D3DXVECTOR2(0, 0);
		g_Explosion[i].frameCount = 0;
	}
}
//=====================================================
//終了処理
//=====================================================
void Explosion_Uninit(void)
{

}

//=====================================================
//更新処理
//=====================================================
void Explosion_Update(void)
{
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		if (!g_Explosion[i].isUse)
		{
			continue;
		}
		if (g_Explosion[i].frameCount > 9 * 5)
		{
			g_Explosion[i].isUse = false;
			g_Explosion[i].frameCount = 0;
		}
		g_Explosion[i].frameCount++;
	}
}

//=====================================================
//描画処理
//=====================================================
void Explosion_Draw(void)
{
	int width = Texture_GetWidth(g_Explosion[0].tex) / 10;
	int height = Texture_GetHeight(g_Explosion[0].tex) / 5;

	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		if (!g_Explosion[i].isUse)
		{
			continue;
		}
		int pattern = g_Explosion[i].frameCount / 5;
		//爆発の描画
		Sprite_Draw(g_Explosion[0].tex, g_Explosion[i].pos.x, g_Explosion[i].pos.y, pattern % 10 * width, 0 * height, width, height);
	}
}

//=====================================================
//爆発生成
//=====================================================
void Explosion_Create(float x, float y)
{
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		if (!g_Explosion[i].isUse)
		{
			g_Explosion[i].pos.x = x;
			g_Explosion[i].pos.y = y;
			g_Explosion[i].isUse = true;
			PlaySound(SOUND_LABEL_SE_EXPLOSION);
			break;
		}
	}
}

//=====================================================
//爆発生成(サウンド切り替え)
//=====================================================
void Explosion_Create(float x, float y, bool sound)
{
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		if (!g_Explosion[i].isUse)
		{
			g_Explosion[i].pos.x = x;
			g_Explosion[i].pos.y = y;
			g_Explosion[i].isUse = true;
			if (sound)
			{
				PlaySound(SOUND_LABEL_SE_EXPLOSION);
			}
			break;
		}
	}
}

//=====================================================
//使用中かどうか
//=====================================================
bool Explosion_isUse(int index)
{
	return g_Explosion[index].isUse;
}

//=====================================================
//削除
//=====================================================
void Explosion_Destory(int index)
{
	g_Explosion[index].isUse = false;
}