#include "bullet.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "debug.h"
#include "player.h"
#include "bg.h"
#include "enemy.h"
#include "game.h"
#include "Efect.h"
#include "enemy.h"

//=====================================================
//マクロ定義
//=====================================================
#define BULLET_SPD (8.0)			//弾の速さ

//=====================================================
//グローバル変数
//=====================================================
static BULLET g_PlayerBullet[BULLET_MAX];
static BULLET g_EnemyBullet[BULLET_MAX];
static float fsin[360], fcos[360];

//=====================================================
//初期化処理
//=====================================================
void Bullet_Init(void)
{
	g_PlayerBullet[0].Tex = Texture_SetLoadFile("Texture\\bullet.png", 64, 64);
	g_EnemyBullet[0].Tex = Texture_SetLoadFile("Texture\\UV_bullet.png", 230, 272);
	for (int i = 0; i < 360; i++)
	{
		fsin[i] = (float)sin(i * D3DX_PI / 180);
		fcos[i] = (float)cos(i * D3DX_PI / 180);
	}
	for (int i = 0; i < BULLET_MAX; i++)
	{
		g_PlayerBullet[i].isUse = false;
		g_PlayerBullet[i].collision.radius = 4.0f;
		g_PlayerBullet[i].angle = 0;
		g_EnemyBullet[i].isUse = false;
		g_EnemyBullet[i].collision.radius = 10.0f;
		g_EnemyBullet[i].angle = 0;
	}
}

//=====================================================
//終了処理
//=====================================================
void Bullet_Uninit(void)
{

}

//=====================================================
//更新処理
//=====================================================
void Bullet_Update(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//プレイヤーの弾が使用されていれば処理
		if (g_PlayerBullet[i].isUse)
		{
			//弾の座標の更新							
			if (g_PlayerBullet[i].angle == 90)		//90度
			{
				g_PlayerBullet[i].Pos.y -= BULLET_SPD;
			}
			else
			{
				g_PlayerBullet[i].Pos.x -= (FLOAT)(fcos[g_PlayerBullet[i].angle % 360] * BULLET_SPD);
				g_PlayerBullet[i].Pos.y -= (FLOAT)(fsin[g_PlayerBullet[i].angle % 360] * BULLET_SPD);
			}
			Efect_Create(g_PlayerBullet[i].Pos.x, g_PlayerBullet[i].Pos.y, D3DXCOLOR(1, 1, 1, 0), 5, EFFECT_BULLET);
			//当たり判定の座標の更新
			g_PlayerBullet[i].collision.position.x = g_PlayerBullet[i].Pos.x + Texture_GetWidth(g_PlayerBullet[0].Tex) / 2;
			g_PlayerBullet[i].collision.position.y = g_PlayerBullet[i].Pos.y + Texture_GetHeight(g_PlayerBullet[0].Tex) / 2;

			//チュートリアル時の画面外処理
			if (Game_GetScene() == SCENE_TUTORIAL && g_PlayerBullet[i].Pos.y < 250.0f)
			{
				g_PlayerBullet[i].isUse = false;
			}
			//画面外処理
			else
			{
				//上
				if (g_PlayerBullet[i].Pos.y < -20)
				{
					g_PlayerBullet[i].isUse = false;
				}
				//右
				if (g_PlayerBullet[i].Pos.x > BG_START + BG_WIDTH)
				{
					g_PlayerBullet[i].isUse = false;
				}
				//左
				else if (g_PlayerBullet[i].Pos.x < BG_START - Texture_GetWidth(g_PlayerBullet[0].Tex))
				{
					g_PlayerBullet[i].isUse = false;
				}
			}
		}
		//敵の弾が使用されていれば処理
		if (g_EnemyBullet[i].isUse)
		{
			//弾の座標の更新
			g_EnemyBullet[i].Pos.x += (FLOAT)(fcos[g_EnemyBullet[i].angle] * BULLET_SPD / 2);
			g_EnemyBullet[i].Pos.y -= (FLOAT)(fsin[g_EnemyBullet[i].angle] * BULLET_SPD / 2);

			//当たり判定の座標の更新
			g_EnemyBullet[i].collision.position.x = g_EnemyBullet[i].Pos.x + Texture_GetWidth(g_EnemyBullet[0].Tex) / 3 / 3 + 10.0f;
			g_EnemyBullet[i].collision.position.y = g_EnemyBullet[i].Pos.y + Texture_GetHeight(g_EnemyBullet[0].Tex) / 2 / 2 / 3;

			//画面外処理
			if (g_EnemyBullet[i].Pos.y > BG_HEIGHT)
			{
				g_EnemyBullet[i].isUse = false;
			}
			//左
			else if (g_EnemyBullet[i].Pos.x < BG_START - (Texture_GetWidth(g_EnemyBullet[0].Tex) / 3 / 2 + 10))
			{
				g_EnemyBullet[i].isUse = false;
			}
			//右
			else if (g_EnemyBullet[i].Pos.x > BG_WIDTH + Texture_GetWidth(g_EnemyBullet[0].Tex))
			{
				g_EnemyBullet[i].isUse = false;
			}
		}
	}
}

//=====================================================
//描画処理
//=====================================================
void Bullet_Draw(void)
{
	int eWidth = Texture_GetWidth(g_EnemyBullet[0].Tex);
	int eHeight = Texture_GetHeight(g_EnemyBullet[0].Tex);

	for (int i = 0; i < BULLET_MAX; i++)
	{
		//プレイヤーの弾が使用されていれば描画
		if (g_PlayerBullet[i].isUse)
		{
			//通常弾
			if (g_PlayerBullet[i].type != BULLET_HOMING)
			{
				//右斜めに傾いた弾
				if (g_PlayerBullet[i].angle == BULLET_ANGLE_LEFT)
				{
					Sprite_Draw(g_PlayerBullet[0].Tex, g_PlayerBullet[i].Pos.x, g_PlayerBullet[i].Pos.y, 0, 0, Texture_GetWidth(g_PlayerBullet[0].Tex), Texture_GetHeight(g_PlayerBullet[0].Tex), BULLET_ANGLE_RIGHT, (float)(Texture_GetWidth(g_PlayerBullet[0].Tex) / 2), (float)(Texture_GetHeight(g_PlayerBullet[0].Tex) / 2), 0);
				}
				else if (g_PlayerBullet[i].angle == 90)
				{
					Sprite_Draw(g_PlayerBullet[0].Tex, g_PlayerBullet[i].Pos.x, g_PlayerBullet[i].Pos.y);
				}
				//左斜めに傾いた弾
				else if (g_PlayerBullet[i].angle == BULLET_ANGLE_RIGHT)
				{
					Sprite_Draw(g_PlayerBullet[0].Tex, g_PlayerBullet[i].Pos.x, g_PlayerBullet[i].Pos.y, 0, 0, Texture_GetWidth(g_PlayerBullet[0].Tex), Texture_GetHeight(g_PlayerBullet[0].Tex), -BULLET_ANGLE_RIGHT, (float)(Texture_GetWidth(g_PlayerBullet[0].Tex) / 2), (float)(Texture_GetHeight(g_PlayerBullet[0].Tex) / 2), 0);
				}
			}
			//追尾弾
			else
			{
				Sprite_Draw(g_EnemyBullet[0].Tex, g_PlayerBullet[i].Pos.x, g_PlayerBullet[i].Pos.y, eWidth / 2, eHeight / 1.5f, 16.0f, 16.0f, 0, 0, 0, 1.0f);
			}
		}
		//エネミーの弾が使用されていれば描画
		if (g_EnemyBullet[i].isUse)
		{
			Sprite_Draw(g_EnemyBullet[0].Tex, g_EnemyBullet[i].Pos.x, g_EnemyBullet[i].Pos.y, 0, eHeight / 4 - 10, eWidth / 3 / 3, eHeight / 2 / 2 / 3, 0, 0, 0, 1.0f);
		}
	}
}

//=====================================================
//弾作成処理
//=====================================================
void Bullet_Create(BULLET *bullet, float x, float y)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//弾が使用されていなかったら設定
		if (!(bullet + i)->isUse)
		{
			(bullet + i)->isUse = true;
			(bullet + i)->Pos = D3DXVECTOR2(x, y);
			(bullet + i)->collision.position = D3DXVECTOR2(x, y);
			break;
		}
	}
}

//=====================================================
//弾作成処理(タイプ指定)
//=====================================================
void Bullet_Create(BULLET *bullet, float x, float y, BULLET_TYPE type)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//弾が使用されていなかったら設定
		if (!(bullet + i)->isUse)
		{
			(bullet + i)->isUse = true;
			(bullet + i)->Pos = D3DXVECTOR2(x, y);
			(bullet + i)->collision.position = D3DXVECTOR2(x, y);
			(bullet + i)->type = type;
			break;
		}
	}
}

//=====================================================
//弾作成処理(角度指定)
//=====================================================
void Bullet_Create(BULLET *bullet, float x, float y, int angle)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//弾が使用されていなかったら設定
		if (!(bullet + i)->isUse)
		{
			(bullet + i)->isUse = true;
			(bullet + i)->Pos = D3DXVECTOR2(x, y);
			(bullet + i)->collision.position = D3DXVECTOR2(x, y);
			(bullet + i)->angle = angle;
			break;
		}
	}
}

//=====================================================
//弾作成処理(角度指定)(タイプ指定)
//=====================================================
void Bullet_Create(BULLET *bullet, float x, float y, int angle, BULLET_TYPE type)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//弾が使用されていなかったら設定
		if (!(bullet + i)->isUse)
		{
			(bullet + i)->isUse = true;
			(bullet + i)->Pos = D3DXVECTOR2(x, y);
			(bullet + i)->collision.position = D3DXVECTOR2(x, y);
			(bullet + i)->angle = angle;
			(bullet + i)->type = type;
			break;
		}
	}
}

//=====================================================
//弾のフラグ取得
//=====================================================
bool Bullet_isUsed(int index, const char *obj)
{
	return (strcmp(obj, "Player")) ? g_EnemyBullet[index].isUse : g_PlayerBullet[index].isUse;
}

//=====================================================
//弾のフラグ削除
//=====================================================
void Bullet_Destroy(int index, const char *obj)
{
	(strcmp(obj, "Player")) ? g_EnemyBullet[index].isUse = false : g_PlayerBullet[index].isUse = false;
}

//=====================================================
//弾の構造体取得
//=====================================================
CIRCLE *Bullet_GetCollision(int index, const char *obj)
{
	return (strcmp(obj, "Player")) ? &g_EnemyBullet[index].collision : &g_PlayerBullet[index].collision;
}

//=====================================================
//弾の構造体取得
//=====================================================
BULLET *Bullet_GetObj(int index, const char *obj)
{
	return (strcmp(obj, "Player")) ? &g_EnemyBullet[index] : &g_PlayerBullet[index];
}

//=====================================================
//敵とプレイヤーの間の角度を求める
//=====================================================
int Bullet_GetAngle(float px, float py, float ex, float ey)
{
	return (int)(atan2(-double(py - ey), double(px - ex)) / D3DX_PI * 180 + 360) % 360;
}

//=====================================================
//弾の種類別で発射
//=====================================================
void Bullet_TypeSelect(float px, float py, float ex, float ey, BULLET_TYPE type)
{
	switch (type)
	{
	case BULLET_HOMING:	//追尾弾
		Bullet_Create(g_EnemyBullet, ex, ey, Bullet_GetAngle(px, py, ex, ey));
		break;
	case BULLET_FAN:	//扇形弾
		for (int i = 0; i < 6; i++)
		{
			Bullet_Create(g_EnemyBullet, ex, ey, 180 + i * 30);
		}
		break;
	case BULLET_NORMAL:	//一方向弾
		Bullet_Create(g_EnemyBullet, ex, ey);
		break;
	default:
		break;
	}
}