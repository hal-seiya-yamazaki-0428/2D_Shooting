#include "item.h"
#include "texture.h"
#include "sprite.h"
#include "bg.h"
#include "debug.h"

//=====================================================
//グローバル変数
//=====================================================
static ITEM g_item[ITEM_MAX];
static int g_Frame = 0;
static int g_EnemyCount = 0;

//=====================================================
//初期化
//=====================================================
void Item_Init(void)
{
	g_item[0].tex = Texture_SetLoadFile("Texture\\item.png", 60, 39);
	g_item[1].tex = Texture_SetLoadFile("Texture\\shoot_mate.png", 453, 340);
	g_Frame = 0;
	g_EnemyCount = 0;
	for (int i = 0; i < ITEM_MAX; i++)
	{
		g_item[i].pos = D3DXVECTOR2(0, 0);
		g_item[i].collision.position = D3DXVECTOR2(0, 0);
		g_item[i].collision.radius = 20.0f;
		g_item[i].isUse = false;
		g_item[i].type = ITEM_TYPE_ENE;
	}
}

//=====================================================
//終了
//=====================================================
void Item_Uninit(void)
{

}

//=====================================================
//更新
//=====================================================
void Item_Update(void)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (g_item[i].isUse)
		{
			//アイテムの移動
			g_item[i].pos.y += 2.0f;

			//当たり判定の更新;
			g_item[i].collision.position.x = g_item[i].pos.x + Texture_GetWidth(g_item[0].tex) / 2;
			g_item[i].collision.position.y = g_item[i].pos.y + Texture_GetHeight(g_item[0].tex) / 2;

			//画面外処理
			if (g_item[i].pos.y > BG_HEIGHT)
			{
				g_item[i].isUse = false;
			}
		}
	}
	g_Frame++;
}

//=====================================================
//描画
//=====================================================
void Item_Draw(void)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (g_item[i].isUse)
		{
			//エネルギー回復
			if (g_item[i].type == ITEM_TYPE_ENE)
			{
				Sprite_Draw(g_item[0].tex, g_item[i].pos.x, g_item[i].pos.y);
			}
			//レベルアップ用アイテム
			else
			{
				Sprite_Draw(g_item[1].tex, g_item[i].pos.x, g_item[i].pos.y, 312, 0 / 4, 47, 47);
			}
		}
	}
}

//=====================================================
//生成
//=====================================================
void Item_Create(float x, float y)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (!g_item[i].isUse)
		{
			//撃破数ごとにタイプ変更
			if (g_EnemyCount % 6 == 0)
			{
				g_item[i].type = ITEM_TYPE_ENE;
			}
			else if (g_EnemyCount % 6 == 3)
			{
				g_item[i].type = ITEM_TYPE_POWER_UP;
			}
			else
			{
				return;
			}
			g_item[i].pos = D3DXVECTOR2(x, y);
			g_item[i].collision.position = D3DXVECTOR2(x, y);
			g_item[i].isUse = true;
			break;
		}
	}
}

//=====================================================
//生成(タイプ指定)
//=====================================================
void Item_Create(float x, float y, ITEM_TYPE type)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (!g_item[i].isUse)
		{
			g_item[i].pos = D3DXVECTOR2(x, y);
			g_item[i].collision.position = D3DXVECTOR2(x, y);
			g_item[i].isUse = true;
			g_item[i].type = type;
			break;
		}
	}
}

//=====================================================
//使用中かどうか
//=====================================================
bool Item_isUse(int index)
{
	return g_item[index].isUse;
}

//=====================================================
//削除
//=====================================================
void Item_Destroy(int index)
{
	g_item[index].isUse = false;
}

//=====================================================
//当たり判定の構造体
//=====================================================
CIRCLE *Item_GetCollision(int index)
{
	return &g_item[index].collision;
}

//=====================================================
//アイテムのタイプ取得
//=====================================================
ITEM_TYPE item_GetType(int index)
{
	return g_item[index].type;
}

//=====================================================
//敵撃破数カウント
//=====================================================
void Item_EnemyCountUp(void)
{
	g_EnemyCount++;
}