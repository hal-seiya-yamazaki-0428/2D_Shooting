#include "med_bullet.h"
#include "texture.h"
#include "sprite.h"
#include "bg.h"
#include "bullet.h"
#include "debug.h"
#include "medium_boss.h"
#include "player.h"

//=====================================================
//マクロ定義
//=====================================================
#define BULLET_SPD (8.0)	//弾の速さ

//=====================================================
//グローバル変数
//=====================================================
static MED_BULLET g_MedBullet[BULLET_MAX] = {};
static float fsin[360], fcos[360];
static int g_frame = 0;
static float g_Speed = 0;

//=====================================================
//初期化
//=====================================================
void Med_Bullet_Init(void)
{
	g_MedBullet[0].bullet.Tex = Texture_SetLoadFile("Texture\\UV_bullet.png", 230, 272);
	for (int i = 0; i < 360; i++)
	{
		fsin[i] = (float)sin(i * D3DX_PI / 180);
		fcos[i] = (float)cos(i * D3DX_PI / 180);
	}
	for (int i = 0; i < BULLET_MAX; i++)
	{
		g_MedBullet[i].bullet.isUse = false;
		g_MedBullet[i].bullet.collision.radius = 14.0f;
		g_MedBullet[i].bullet.angle = 0;
		g_MedBullet[i].InvCount = 0;
	}
	g_frame = 0;
}

//=====================================================
//終了
//=====================================================
void Med_Bullet_Uninit(void)
{

}

//=====================================================
//更新
//=====================================================
void Med_Bullet_Update(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//敵の弾が使用されていなければ次の要素
		if (!g_MedBullet[i].bullet.isUse)
		{
			continue;
		}

		//弾の種類別で更新
		switch (g_MedBullet[i].type)
		{
			//通常弾
		case MED_BULLET_NORMAL:
			//弾の座標の更新
			g_MedBullet[i].bullet.Pos.x += (FLOAT)(fcos[g_MedBullet[i].bullet.angle % 360] * (BULLET_SPD / 2));
			g_MedBullet[i].bullet.Pos.y -= (FLOAT)(fsin[g_MedBullet[i].bullet.angle % 360] * (BULLET_SPD / 2));
			break;
			//追尾弾
		case MED_BULLET_HOMING:
			//弾の座標の更新
			g_MedBullet[i].bullet.Pos.x += (FLOAT)(fcos[g_MedBullet[i].bullet.angle % 360] * (BULLET_SPD - 2.0));
			g_MedBullet[i].bullet.Pos.y -= (FLOAT)(fsin[g_MedBullet[i].bullet.angle % 360] * (BULLET_SPD - 2.0));
			break;
			//扇状
		case MED_BULLET_FAN:
			//弾の座標の更新
			g_MedBullet[i].bullet.Pos.x += (FLOAT)(fcos[g_MedBullet[i].bullet.angle % 360] * BULLET_SPD / 2);
			g_MedBullet[i].bullet.Pos.y -= (FLOAT)(fsin[g_MedBullet[i].bullet.angle % 360] * BULLET_SPD / 2);
			break;
			//螺旋状
		case MED_BULLET_SPIRAL:
			//弾の座標の更新
			g_MedBullet[i].bullet.Pos.x += (FLOAT)(fcos[g_MedBullet[i].bullet.angle % 360] * BULLET_SPD / 2);
			g_MedBullet[i].bullet.Pos.y -= (FLOAT)(fsin[g_MedBullet[i].bullet.angle % 360] * BULLET_SPD / 2);
			break;
		default:
			break;
		}

		//当たり判定の座標の更新
		g_MedBullet[i].bullet.collision.position.x = g_MedBullet[i].bullet.Pos.x + Texture_GetWidth(g_MedBullet[0].bullet.Tex) / 3 / 3;
		g_MedBullet[i].bullet.collision.position.y = g_MedBullet[i].bullet.Pos.y + Texture_GetHeight(g_MedBullet[0].bullet.Tex) / 2 / 4 + 15.0f;

		//画面外処理
		if (g_MedBullet[i].bullet.Pos.y > BG_HEIGHT)
		{
			g_MedBullet[i].bullet.isUse = false;
		}
		//左
		else if (g_MedBullet[i].bullet.Pos.x < BG_START - (Texture_GetWidth(g_MedBullet[0].bullet.Tex) / 3 / 2 + 10))
		{
			g_MedBullet[i].bullet.isUse = false;
		}
		//右
		else if (g_MedBullet[i].bullet.Pos.x > BG_WIDTH + Texture_GetWidth(g_MedBullet[0].bullet.Tex))
		{
			g_MedBullet[i].bullet.isUse = false;
		}
	}
	g_frame++;
}

//=====================================================
//描画
//=====================================================
void Med_Bullet_Draw(void)
{
	int Width = Texture_GetWidth(g_MedBullet[0].bullet.Tex);
	int Height = Texture_GetHeight(g_MedBullet[0].bullet.Tex);

	for (int i = 0; i < BULLET_MAX; i++)
	{
		//エネミーの弾が使用されていれば描画
		if (g_MedBullet[i].bullet.isUse)
		{
			Sprite_Draw(g_MedBullet[0].bullet.Tex, g_MedBullet[i].bullet.Pos.x, g_MedBullet[i].bullet.Pos.y, Width / 3, Height * 3 / 4 - 10, Width / 3 / 3, Height / 2 / 4, 0, 0, 0, 1.0f);
		}
	}
}

//=====================================================
//生成
//=====================================================
void Med_Bullet_Create(MED_BULLET_TYPE type, float x, float y)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//弾が使用されていなかったら設定
		if (!g_MedBullet[i].bullet.isUse)
		{
			g_MedBullet[i].bullet.isUse = true;
			g_MedBullet[i].bullet.Pos = D3DXVECTOR2(x, y);
			g_MedBullet[i].bullet.collision.position = D3DXVECTOR2(x, y);
			g_MedBullet[i].type = type;
			break;
		}
	}
}

//=====================================================
//生成(位置取得)
//=====================================================
void Med_Bullet_Create(MED_BULLET_TYPE type, float px, float py, float ex, float ey)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//弾が使用されていなかったら設定
		if (!g_MedBullet[i].bullet.isUse)
		{
			g_MedBullet[i].bullet.isUse = true;
			g_MedBullet[i].bullet.Pos = D3DXVECTOR2(ex, ey);
			g_MedBullet[i].bullet.collision.position = D3DXVECTOR2(ex, ey);
			g_MedBullet[i].bullet.angle = Bullet_GetAngle(px, py, ex, ey);
			g_MedBullet[i].type = type;
			break;
		}
	}
}

//=====================================================
//生成(角度指定)
//=====================================================
void Med_Bullet_Create(MED_BULLET_TYPE type, float x, float y, int angle)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//弾が使用されていなかったら設定
		if (!g_MedBullet[i].bullet.isUse)
		{
			g_MedBullet[i].bullet.isUse = true;
			g_MedBullet[i].bullet.Pos = D3DXVECTOR2(x, y);
			g_MedBullet[i].bullet.collision.position = D3DXVECTOR2(x, y);
			g_MedBullet[i].bullet.angle = angle;
			g_MedBullet[i].type = type;
			break;
		}
	}
}

//=====================================================
//生成(角度指定)(要素返値)
//=====================================================
int Med_Bullet_Create_Return(MED_BULLET_TYPE type, float x, float y, int angle)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//弾が使用されていなかったら設定
		if (!g_MedBullet[i].bullet.isUse)
		{
			g_MedBullet[i].bullet.isUse = true;
			g_MedBullet[i].bullet.Pos = D3DXVECTOR2(x, y);
			g_MedBullet[i].bullet.collision.position = D3DXVECTOR2(x, y);
			g_MedBullet[i].bullet.angle = angle;
			g_MedBullet[i].type = type;
			return i;
		}
	}
	return -1;
}

//=====================================================
//使用中かどうか
//=====================================================
bool Med_Bullet_isUsed(int index)
{
	return g_MedBullet[index].bullet.isUse;
}

//=====================================================
//削除
//=====================================================
void Med_Bullet_Destroy(int index)
{
	g_MedBullet[index].bullet.isUse = false;
}

//=====================================================
//当たり判定の構造体取得
//=====================================================
CIRCLE *Med_Bullet_GetCollision(int index)
{
	return &g_MedBullet[index].bullet.collision;
}

//=====================================================
//ボスの弾の構造体取得
//=====================================================
MED_BULLET *Med_Bullet_GetObj(int index)
{
	return &g_MedBullet[index];
}