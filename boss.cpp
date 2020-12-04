
#include "boss.h"
#include "texture.h"
#include "sprite.h"
#include "bg.h" 
#include "debug_font.h" 
#include "debug.h"
#include "boss_bullet.h"
#include "player.h"
#include "explosion.h"
#include "score.h"

//=====================================================
//マクロ定義
//=====================================================
#define BOSS_NUM (32)	
#define BOSS_HP	(100)
#define EXPLOSION_COUNT (10)
#define BULLET_FAN_NUM (8)	//扇状の弾を何発発射するか 

//=====================================================
//グローバル変数
//=====================================================
static BOSS g_Boss;
static float fsin[360], fcos[360];
static int bulletFrame = 0;
static int g_ExpCount = 0;
static int g_ExpPos[EXPLOSION_COUNT * 2] =
{
	20, 80, 50, 30, 140, 30, 180, 38, 72, 150,	//x座標
	20, 50, 100, 10, 80, 20, 64, 114, 46, 90	//y座標
};
static int g_ExpFrame = 0;
static int g_angle = 0;
static int g_HomingAngle = 0;
static int g_shotCount = 0;
static int g_generater = 0;
static BOSS_TYPE g_NextType = BOSS_STOP;

//=====================================================
//初期化
//=====================================================
void Boss_Init(void)
{
	for (int i = 0; i < 360; i++)
	{
		fsin[i] = (float)sin(i * D3DX_PI / 180);
		fcos[i] = (float)cos(i * D3DX_PI / 180);
	}
	g_Boss.enemy.Tex = Texture_SetLoadFile("Texture\\ボス.png", 288 * 3, 192 * 3);
	g_Boss.enemy.Pos = D3DXVECTOR2(0, 0);
	g_Boss.enemy.collision.position = D3DXVECTOR2(0, 0);
	g_Boss.enemy.collision.radius = 96.0f;
	g_Boss.enemy.angle = 90;
	g_Boss.enemy.isUse = false;
	g_Boss.enemy.frame = 1;
	g_Boss.enemy.hp = g_Boss.enemy.MaxHP = BOSS_HP;
	g_Boss.move = 3.0f;
	g_Boss.type = BOSS_FIRST_IN;
	g_Boss.shotFrame = 0;
	bulletFrame = 0;
	g_ExpCount = 0;
	g_ExpFrame = 0;
	g_angle = 0;
	g_HomingAngle = 0;
	g_shotCount = 0;
	g_generater = 0;
	g_NextType = BOSS_STOP;
}

//=====================================================
//終了
//=====================================================
void Boss_Uninit(void)
{

}

//=====================================================
//更新
//=====================================================
void Boss_Update(void)
{
	int width = Texture_GetWidth(g_Boss.enemy.Tex);
	int height = Texture_GetHeight(g_Boss.enemy.Tex);
	PLAYER *pPlayer = Player_GetObj();

	//ボスが使用されていなければ終了
	if (!g_Boss.enemy.isUse)
	{
		return;
	}
	//死亡時
	if (g_Boss.enemy.hp < 0)
	{
		if (g_ExpFrame % 20 == 0)
		{
			Explosion_Create(g_Boss.enemy.Pos.x + g_ExpPos[g_ExpCount], g_Boss.enemy.Pos.y + g_ExpPos[g_ExpCount + 10]);
			g_ExpCount++;
		}
		if (g_ExpCount > EXPLOSION_COUNT)
		{
			g_Boss.enemy.isUse = false;
			Score_Add(1000);
		}
		g_ExpFrame++;
		return;
	}

	//画面内処理
	switch (g_Boss.type)
	{
		//=====================
		//画面外からの入場
		//=====================
	case BOSS_FIRST_IN:
		//画面外からの登場
		(g_Boss.enemy.Pos.y < 0) ? g_Boss.enemy.Pos.y += 2 : g_Boss.type = g_NextType;
		break;
		//=====================
		//水平移動
		//=====================
	case BOSS_HORIZONTAL:
		g_Boss.enemy.Pos.x += g_Boss.move;
		if (g_Boss.enemy.Pos.y < 0)
		{
			g_Boss.enemy.Pos.y += 2;
		}
		//弾の発射
		if (g_Boss.enemy.frame % 16 == 0)
		{
			//追尾弾
			for (int angle = 0; angle < 3; angle++)
			{
				Boss_Bullet_Create(BOSS_BULLET_HOMING, pPlayer->pos.x - 150 + angle * 150, pPlayer->pos.y, g_Boss.enemy.Pos.x + width / 3 / 2 - 25, g_Boss.enemy.Pos.y + height / 3 / 2);
			}
			//一定数たったら次の行動
			if (g_Boss.shotFrame > 10)
			{
				g_Boss.type = BOSS_FIRST_IN;
				g_NextType = BOSS_WAVE;
				g_Boss.shotFrame = 0;
			}
			g_Boss.shotFrame++;
		}
		break;
		//=====================
		//波状に沿って動く
		//=====================
	case BOSS_WAVE:
		//左から右方向へ
		if (g_Boss.enemy.Pos.x < BG_START)
		{
			g_Boss.right = true;
		}
		//右から左方向へ
		else if (g_Boss.enemy.Pos.x > BG_START + BG_WIDTH - width / 3)
		{
			g_Boss.right = false;
		}
		//右方向
		if (g_Boss.right)
		{
			g_Boss.enemy.Pos.x += 2;
		}
		//左方向
		else
		{
			g_Boss.enemy.Pos.x -= 2;
		}
		g_Boss.enemy.Pos.y += (FLOAT)(fcos[g_Boss.enemy.angle % 360] * 2);
		g_Boss.enemy.angle += 2;

		//弾の発射
		if (g_Boss.enemy.frame % 20 == 0)
		{
			Boss_Bullet_Create(BOSS_BULLET_INVERSION, g_Boss.enemy.Pos.x + width / 3 / 2 - 25, g_Boss.enemy.Pos.y + height / 3 / 2, g_angle + 180);
			//一定数たったら次の行動
			if (g_Boss.shotFrame > 30)
			{
				g_Boss.type = BOSS_FIRST_IN;
				g_NextType = BOSS_STOP;
				g_Boss.shotFrame = 0;
			}
			g_Boss.shotFrame++;
		}
		break;
		//=====================
		//停止
		//=====================
	case BOSS_STOP:
		//画面中央でなければ中央まで移動	//440, 540
		if ((g_Boss.enemy.Pos.x < 520 - width / 3 / 2 || g_Boss.enemy.Pos.x > 540 - width / 3 / 2) || g_Boss.enemy.Pos.y > -100)
		{
			//ステージ中央までの角度を計算して中央に向かう
			int angle = Bullet_GetAngle(530.0f - width / 3 / 2, -100.0f, g_Boss.enemy.Pos.x, g_Boss.enemy.Pos.y) % 360;
			g_Boss.enemy.Pos.x += (FLOAT)(fcos[angle] * 2);
			g_Boss.enemy.Pos.y -= (FLOAT)(fsin[angle] * 4);
		}
		//停止中であれば弾の発射
		else if (g_Boss.enemy.frame % 12 == 0)
		{
			//扇
			for (int angle = 0; angle < BULLET_FAN_NUM; angle++)
			{
				Boss_Bullet_Create(BOSS_BULLET_FAN, g_Boss.enemy.Pos.x + width / 3 / 2 - 25, g_Boss.enemy.Pos.y + height / 3 / 2, 
					((180 + g_angle) + angle * (180 / BULLET_FAN_NUM)) % 180 + 180);
			}
			//一定数たったら次の行動
			if (g_Boss.shotFrame > 10)
			{
				g_Boss.type = BOSS_FIRST_IN;
				g_NextType = BOSS_HORIZONTAL;
				g_Boss.shotFrame = 0;
			}
			g_Boss.shotFrame++;
		}
		break;
	default:
		break;
	}

	//方向切替
	if (g_Boss.enemy.Pos.x < BG_START || g_Boss.enemy.Pos.x > BG_START + BG_WIDTH - width / 3)
	{
		g_Boss.move *= -1;
	}
	g_angle++;
	g_angle %= 180;
	g_Boss.enemy.frame++;

	//当たり判定の更新
	g_Boss.enemy.collision.position.x = g_Boss.enemy.Pos.x + width / 3 / 2;
	g_Boss.enemy.collision.position.y = g_Boss.enemy.Pos.y + height / 2 / 2;
}

//=====================================================
//描画
//=====================================================
void Boss_Draw(void)
{
	if (g_Boss.enemy.isUse)
	{
		int width = Texture_GetWidth(g_Boss.enemy.Tex);
		int height = Texture_GetHeight(g_Boss.enemy.Tex);
		Sprite_Draw(g_Boss.enemy.Tex, g_Boss.enemy.Pos.x, g_Boss.enemy.Pos.y, width * 2 / 3, 2, width / 3, height / 2 - 6);
	}
}

//=====================================================
//生成
//=====================================================
void Boss_Create(float x, float y, int  hp)
{
	if (!g_Boss.enemy.isUse)
	{
		g_Boss.enemy.isUse = true;
		g_Boss.enemy.angle = 0;
		g_Boss.enemy.collision.position = D3DXVECTOR2(x, y);
		g_Boss.enemy.Pos = D3DXVECTOR2(x, y);
		g_Boss.enemy.frame = 1;
		g_Boss.enemy.hp = hp;
		g_Boss.right = true;
	}
}

//=====================================================
//使用中かどうか
//=====================================================
bool Boss_isUsed(void)
{
	return g_Boss.enemy.isUse;
}

//=====================================================
//削除
//=====================================================
void Boss_Destroy(void)
{
	g_Boss.enemy.isUse = false;
}

//=====================================================
//当たり判定の構造体
//=====================================================
CIRCLE *Boss_GetCollision(void)
{
	return &g_Boss.enemy.collision;
}

//=====================================================
//ボスの構造体
//=====================================================
BOSS *Boss_GetObj(void)
{
	return &g_Boss;
}

//=====================================================
//ダメージ付与
//=====================================================
void Boss_Damage(int damage)
{
	g_Boss.enemy.hp -= damage;
}

//=====================================================
//HP取得
//=====================================================
int Boss_GetHP(void)
{
	return g_Boss.enemy.hp;
}

//=====================================================
//発射した弾の数取得
//=====================================================
int Boss_GetShotCount(void)
{
	return g_shotCount;
}

//	//=====================
//	//Uの字移動
//	//=====================
//case BOSS_UMOVE:
//	g_Boss.enemy.Pos.x -= (FLOAT)(fcos[g_Boss.enemy.angle % 360] * 3);
//	g_Boss.enemy.Pos.y += (FLOAT)(fsin[g_Boss.enemy.angle % 360] * 6);
//	g_Boss.enemy.angle += 2;
//	//弾の発射
//	if (g_Boss.enemy.frame % g_Boss.shotInter == 0)
//	{
//	}
//	break;