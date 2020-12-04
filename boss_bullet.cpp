#include "boss_bullet.h"
#include "texture.h"
#include "sprite.h"
#include "bg.h"
#include "bullet.h"
#include "debug.h"
#include "boss.h"
#include "player.h"

//=====================================================
//マクロ定義
//=====================================================
#define BULLET_SPD (8.0)	//弾の速さ

//=====================================================
//グローバル変数
//=====================================================F
static BOSS_BULLET g_BossBullet[BULLET_MAX] = {};
static float fsin[360], fcos[360];
static int g_frame = 0;
static float g_Speed = 0;

//=====================================================
//初期化
//=====================================================
void Boss_Bullet_Init(void)
{
	g_BossBullet[0].bullet.Tex = Texture_SetLoadFile("Texture\\UV_bullet.png", 230, 272);
	for (int i = 0; i < 360; i++)
	{
		fsin[i] = (float)sin(i * D3DX_PI / 180);
		fcos[i] = (float)cos(i * D3DX_PI / 180);
	}
	for (int i = 0; i < BULLET_MAX; i++)
	{
		g_BossBullet[i].bullet.isUse = false;
		g_BossBullet[i].bullet.collision.radius = 14.0f;
		g_BossBullet[i].bullet.angle = 0;
		g_BossBullet[i].InvCount = 0;
	}
	g_frame = 0;
}

//=====================================================
//終了
//=====================================================
void Boss_Bullet_Uninit(void)
{

}

//=====================================================
//更新
//=====================================================
void Boss_Bullet_Update(void)
{
	int Height = Texture_GetHeight(g_BossBullet[0].bullet.Tex);

	for (int i = 0; i < BULLET_MAX; i++)
	{
		//敵の弾が使用されていなければ次の要素
		if (!g_BossBullet[i].bullet.isUse)
		{
			continue;
		}

		//弾の種類別で更新
		switch (g_BossBullet[i].type)
		{
			//通常弾
		case BOSS_BULLET_NORMAL:
			//弾の座標の更新
			g_BossBullet[i].bullet.Pos.x += (FLOAT)(fcos[g_BossBullet[i].bullet.angle] * (BULLET_SPD / 2));
			g_BossBullet[i].bullet.Pos.y -= (FLOAT)(fsin[g_BossBullet[i].bullet.angle] * (BULLET_SPD / 2));
			break;
			//追尾弾
		case BOSS_BULLET_HOMING:
			//弾の座標の更新
			g_BossBullet[i].bullet.Pos.x += (FLOAT)(fcos[g_BossBullet[i].bullet.angle] * (BULLET_SPD - 2.0));
			g_BossBullet[i].bullet.Pos.y -= (FLOAT)(fsin[g_BossBullet[i].bullet.angle] * (BULLET_SPD - 2.0));
			break;
			//扇状
		case BOSS_BULLET_FAN:
			//弾の座標の更新
			g_BossBullet[i].bullet.Pos.x += (FLOAT)(fcos[g_BossBullet[i].bullet.angle] * BULLET_SPD / 2);
			g_BossBullet[i].bullet.Pos.y -= (FLOAT)(fsin[g_BossBullet[i].bullet.angle] * BULLET_SPD / 2);
			break;
			//螺旋状
		case BOSS_BULLET_SPIRAL:
			//弾の座標の更新
			g_BossBullet[i].bullet.Pos.x += (FLOAT)(fcos[g_BossBullet[i].bullet.angle] * BULLET_SPD / 2);
			g_BossBullet[i].bullet.Pos.y -= (FLOAT)(fsin[g_BossBullet[i].bullet.angle] * BULLET_SPD / 2);
			break;
			//壁反射
		case BOSS_BULLET_INVERSION:
			//反射処理
			if (g_BossBullet[i].bullet.Pos.y > BG_HEIGHT - Height / 2 / 4)
			{
				g_BossBullet[i].bullet.angle = (g_BossBullet[i].bullet.angle + 180) % 360;
				g_BossBullet[i].InvCount++;
			}
			//左
			else if (g_BossBullet[i].bullet.Pos.x < BG_START)
			{
				g_BossBullet[i].bullet.angle = (g_BossBullet[i].bullet.angle + 90) % 360;
				g_BossBullet[i].InvCount++;
			}
			//右
			else if (g_BossBullet[i].bullet.Pos.x > BG_WIDTH + BG_START - 50)
			{
				g_BossBullet[i].bullet.angle = (g_BossBullet[i].bullet.angle + 90) % 360;
				g_BossBullet[i].InvCount++;
			}

			//弾の座標の更新
			g_BossBullet[i].bullet.Pos.x += (FLOAT)(fcos[g_BossBullet[i].bullet.angle % 360] * BULLET_SPD / 2);
			g_BossBullet[i].bullet.Pos.y -= (FLOAT)(fsin[g_BossBullet[i].bullet.angle % 360] * BULLET_SPD / 2);

			//壁に一定の数あたったらタイプ変更
			if (g_BossBullet[i].InvCount > 1)
			{
				g_BossBullet[i].type = BOSS_BULLET_NORMAL;
				g_BossBullet[i].InvCount = 0;
			}
			break;
		default:
			break;
		}

		//当たり判定の座標の更新
		g_BossBullet[i].bullet.collision.position.x = g_BossBullet[i].bullet.Pos.x + Texture_GetWidth(g_BossBullet[0].bullet.Tex) / 3 / 3;
		g_BossBullet[i].bullet.collision.position.y = g_BossBullet[i].bullet.Pos.y + Texture_GetHeight(g_BossBullet[0].bullet.Tex) / 2 / 4 + 15.0f;

		//画面外処理
		if (g_BossBullet[i].bullet.Pos.y > BG_HEIGHT)
		{
			g_BossBullet[i].bullet.isUse = false;
		}
		//左
		else if (g_BossBullet[i].bullet.Pos.x < BG_START - (Texture_GetWidth(g_BossBullet[0].bullet.Tex) / 3 / 2 + 10))
		{
			g_BossBullet[i].bullet.isUse = false;
		}
		//右
		else if (g_BossBullet[i].bullet.Pos.x > BG_WIDTH + Texture_GetWidth(g_BossBullet[0].bullet.Tex))
		{
			g_BossBullet[i].bullet.isUse = false;
		}
	}
	g_frame++;
}

//=====================================================
//描画
//=====================================================
void Boss_Bullet_Draw(void)
{
	int Width = Texture_GetWidth(g_BossBullet[0].bullet.Tex);
	int Height = Texture_GetHeight(g_BossBullet[0].bullet.Tex);

	for (int i = 0; i < BULLET_MAX; i++)
	{
		//エネミーの弾が使用されていれば描画
		if (g_BossBullet[i].bullet.isUse)
		{
			//反射弾
			if (g_BossBullet[i].type == BOSS_BULLET_INVERSION || g_BossBullet[i].type == BOSS_BULLET_NORMAL)
			{
				Sprite_Draw(g_BossBullet[0].bullet.Tex, g_BossBullet[i].bullet.Pos.x, g_BossBullet[i].bullet.Pos.y, Width / 3, Height * 3 / 4 - 10, Width / 3 / 3, Height / 2 / 4, 0, 0, 0, 1.0f, D3DXCOLOR(255, 128, 0, 255));
			}
			//それ以外
			else
			{
				Sprite_Draw(g_BossBullet[0].bullet.Tex, g_BossBullet[i].bullet.Pos.x, g_BossBullet[i].bullet.Pos.y, Width / 3, Height * 3 / 4 - 10, Width / 3 / 3, Height / 2 / 4, 0, 0, 0, 1.0f);
			}
		}
	}
}

//=====================================================
//生成
//=====================================================
void Boss_Bullet_Create(BOSS_BULLET_TYPE type, float x, float y)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//弾が使用されていなかったら設定
		if (!g_BossBullet[i].bullet.isUse)
		{
			g_BossBullet[i].bullet.isUse = true;
			g_BossBullet[i].bullet.Pos = D3DXVECTOR2(x, y);
			g_BossBullet[i].bullet.collision.position = D3DXVECTOR2(x, y);
			g_BossBullet[i].type = type;
			break;
		}
	}
}

//=====================================================
//生成
//=====================================================
void Boss_Bullet_Create(BOSS_BULLET_TYPE type, float px, float py, float ex, float ey)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//弾が使用されていなかったら設定
		if (!g_BossBullet[i].bullet.isUse)
		{
			g_BossBullet[i].bullet.isUse = true;
			g_BossBullet[i].bullet.Pos = D3DXVECTOR2(ex, ey);
			g_BossBullet[i].bullet.collision.position = D3DXVECTOR2(ex, ey);
			g_BossBullet[i].bullet.angle = Bullet_GetAngle(px, py, ex, ey);
			g_BossBullet[i].type = type;
			break;
		}
	}
}

//=====================================================
//生成(角度指定)
//=====================================================
void Boss_Bullet_Create(BOSS_BULLET_TYPE type, float x, float y, int angle)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//弾が使用されていなかったら設定
		if (!g_BossBullet[i].bullet.isUse)
		{
			g_BossBullet[i].bullet.isUse = true;
			g_BossBullet[i].bullet.Pos = D3DXVECTOR2(x, y);
			g_BossBullet[i].bullet.collision.position = D3DXVECTOR2(x, y);
			g_BossBullet[i].bullet.angle = angle;
			g_BossBullet[i].type = type;
			break;
		}
	}
}

//=====================================================
//使用中かどうか
//=====================================================
bool Boss_Bullet_isUsed(int index)
{
	return g_BossBullet[index].bullet.isUse;
}

//=====================================================
//削除
//=====================================================
void Boss_Bullet_Destroy(int index)
{
	g_BossBullet[index].bullet.isUse = false;
}

//=====================================================
//当たり判定の構造体取得
//=====================================================
CIRCLE *Boss_Bullet_GetCollision(int index)
{
	return &g_BossBullet[index].bullet.collision;
}

//=====================================================
//ボスの弾の構造体取得
//=====================================================
BOSS_BULLET *Boss_Bullet_GetObj(int index)
{
	return &g_BossBullet[index];
}