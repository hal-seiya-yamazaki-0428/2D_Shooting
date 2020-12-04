#include "texture.h"
#include "sprite.h"
#include <d3dx9.h>
#include "input.h"
#include "common.h"
#include "bullet.h"
#include "collision.h"
#include "player.h"
#include "debug.h"
#include "barrier.h"
#include "bg.h"
#include "game.h"
#include "UI.h"
#include "sound.h"
#include "Efect.h"
#include "enemy.h"
#include <Xinput.h>

#pragma comment(lib, "Xinput.lib")

//=====================================================
//マクロ定義
//=====================================================
#define PLAYER_SPD (5.0)		//プレイヤーの速度
#define MOVE_NUM (1.0)			//移動量
#define PLAYER_COLLISION (8.0f)	//当たり判定

//=====================================================
//グローバル変数
//=====================================================
static PLAYER g_Player;
static int g_ShotFrame = 0;
static XINPUT_STATE g_State;
static bool g_Trigger;

//=====================================================
//初期化処理
//=====================================================
void Player_Init(void)
{
	g_Player.tex = Texture_SetLoadFile("Texture\\player.png", 192, 128);
	g_Player.collision.radius = PLAYER_COLLISION;
	g_Player.pos = D3DXVECTOR2(450.0f, 500.0f);
	g_Player.isUse = true;
	g_Player.HP = 150;
	g_Player.EN = 150;
	g_Player.MaxHP = g_Player.HP;
	g_Player.MaxEN = g_Player.EN;
	g_Player.isShot = false;
	g_Player.shotLevel = 1;
	g_Player.shotExp = 0;
	g_Player.LevelUp = false;
	g_ShotFrame = 0;
	g_Trigger = false;
}

//=====================================================
//終了処理
//=====================================================
void Player_Uninit(void)
{

}

//=====================================================
//更新処理
//=====================================================
void Player_Update(void)
{
	//ボス撃破時
	if (UI_GetCond() > 490)
	{
		//自動で前に進む
		g_Player.pos.y -= 2.5f;
		if (g_Player.pos.y < 0)
		{
			Player_Destroy();
		}
		return;
	}
	if (Game_GetScene() != SCENE_MAIN && Game_GetScene() != SCENE_TUTORIAL)
	{
		return;
	}
	D3DXVECTOR2 dir(0.0f, 0.0f);

	XInputGetState(0, &g_State);

	//キーボード更新
	Keyboard_Update();
	//キー入力
	if (Keyboard_IsPress(DIK_A))	//左
	{
		dir.x -= MOVE_NUM;
		//プレイヤー追従エフェクト
		if (Barrier_isUsed())
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(1, 1, 1, 0), 30, EFFECT_PLAYER);
		}
		else
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(0, 0, 1, 0), 30, EFFECT_PLAYER);
		}
	}
	else if (Keyboard_IsPress(DIK_D))	//右
	{
		dir.x += MOVE_NUM;
		//プレイヤー追従エフェクト
		if (Barrier_isUsed())
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(1, 1, 1, 0), 30, EFFECT_PLAYER);
		}
		else
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(0, 0, 1, 0), 30, EFFECT_PLAYER);
		}
	}
	//コントローラー x軸
	else if (g_State.Gamepad.sThumbLX < -8000 || g_State.Gamepad.sThumbLX > 8000)
	{
		dir.x += (FLOAT)(MOVE_NUM * g_State.Gamepad.sThumbLX / 32787);
		//プレイヤー追従エフェクト
		if (Barrier_isUsed())
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(1, 1, 1, 0), 30, EFFECT_PLAYER);
		}
		else
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(0, 0, 1, 0), 30, EFFECT_PLAYER);
		}
	}
	if (Keyboard_IsPress(DIK_W))	//上
	{
		dir.y -= MOVE_NUM;
		//プレイヤー追従エフェクト
		if (Barrier_isUsed())
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(1, 1, 1, 0), 30, EFFECT_PLAYER);
		}
		else
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(0, 0, 1, 0), 30, EFFECT_PLAYER);
		}
	}
	else if (Keyboard_IsPress(DIK_S))	//下
	{
		dir.y += MOVE_NUM;
		//プレイヤー追従エフェクト
		if (Barrier_isUsed())
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(1, 1, 1, 0), 30, EFFECT_PLAYER);
		}
		else
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(0, 0, 1, 0), 30, EFFECT_PLAYER);
		}
	}
	//コントローラー y軸
	else if (g_State.Gamepad.sThumbLY < -8000 || g_State.Gamepad.sThumbLY > 8000)
	{
		dir.y -= (FLOAT)(MOVE_NUM * g_State.Gamepad.sThumbLY / 32787);
		//プレイヤー追従エフェクト
		if (Barrier_isUsed())
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(1, 1, 1, 0), 30, EFFECT_PLAYER);
		}
		else
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(0, 0, 1, 0), 30, EFFECT_PLAYER);
		}
	}

	D3DXVec2Normalize(&dir, &dir);	//ベクトルの正規化をする関数

	D3DXVECTOR2 vecSpeed = dir * PLAYER_SPD;
	g_Player.pos += vecSpeed;

	//画面外処理
	if (g_Player.pos.x < BG_START)		//左
	{
		g_Player.pos.x = BG_START;
	}
	if (g_Player.pos.x > BG_START + BG_WIDTH - Texture_GetWidth(g_Player.tex) / 4 - 12)		//右
	{
		g_Player.pos.x = (float)(BG_START + BG_WIDTH - Texture_GetWidth(g_Player.tex) / 4 - 12);
	}
	if (g_Player.pos.y < 40)			//上
	{
		g_Player.pos.y = 40;
	}
	if (g_Player.pos.y > BG_HEIGHT - Texture_GetHeight(g_Player.tex) / 2)	//下
	{
		g_Player.pos.y = (FLOAT)(BG_HEIGHT - Texture_GetHeight(g_Player.tex) / 2);
	}

	//当たり判定座標の初期化
	g_Player.collision.position.x = g_Player.pos.x + Texture_GetWidth(g_Player.tex) / 3 / 2;
	g_Player.collision.position.y = g_Player.pos.y + Texture_GetHeight(g_Player.tex) / 2 / 2;

	//弾の発射
	if ((Keyboard_IsPress(DIK_K) || g_State.Gamepad.bLeftTrigger > 128 || g_State.Gamepad.bRightTrigger > 128) && g_Player.EN > 0)
	{
		if (g_ShotFrame > 10)
		{
			switch (g_Player.shotLevel)
			{
				//1発
			case 1:
				Bullet_Create(Bullet_GetObj(0, "Player"), g_Player.pos.x, g_Player.pos.y, 90);
				Player_Damage(GAUGE_EN, 2.5f);	//エネルギー消費
				break;
				//2発
			case 2:
				for (int i = 0; i < 2; i++)
				{
					Bullet_Create(Bullet_GetObj(0, "Player"), g_Player.pos.x - 25.0f + 50.0f * i, g_Player.pos.y, 90);
				}
				Player_Damage(GAUGE_EN, 3.5f);	//エネルギー消費
				break;
				//4発
			case 3:
				//前2発
				for (int i = 0; i < 2; i++)
				{
					Bullet_Create(Bullet_GetObj(0, "Player"), g_Player.pos.x - 25.0f + 50.0f * i, g_Player.pos.y, 90);
				}
				//サイド2発
				for (int i = 0; i < 2; i++)
				{
					Bullet_Create(Bullet_GetObj(0, "Player"), g_Player.pos.x, g_Player.pos.y, BULLET_ANGLE_RIGHT + i * 60);
				}
				Player_Damage(GAUGE_EN, 4.5f);	//エネルギー消費
				break;
			default:
				break;
			}
			g_Player.isShot = true;
			g_ShotFrame = 0;
		}
	}
	else
	{
		g_Player.isShot = false;
	}

	//バリアの生成
	if ((Keyboard_IsTrigger(DIK_L) || g_State.Gamepad.wButtons & XINPUT_GAMEPAD_A || g_State.Gamepad.wButtons & XINPUT_GAMEPAD_X) && !Barrier_isUsed() && g_Player.EN > 0 && g_Trigger)
	{
		Barrier_Create(g_Player.pos.x, g_Player.pos.y);
		g_Trigger = false;
	}

	else if (!(g_State.Gamepad.wButtons & XINPUT_GAMEPAD_A || g_State.Gamepad.wButtons & XINPUT_GAMEPAD_X))
	{
		g_Trigger = true;
	}

	if ((Keyboard_IsTrigger(DIK_L) || g_State.Gamepad.wButtons & XINPUT_GAMEPAD_A || g_State.Gamepad.wButtons & XINPUT_GAMEPAD_X) && Barrier_isUsed() && g_Trigger || g_Player.EN < 0)
	{
		Barrier_Destroy();
		g_Trigger = false;
	}


	//バリア生成時プレイヤーの当たり判定増減
	if (Barrier_isUsed())
	{
		g_Player.collision.radius = 64.0f;
	}
	else
	{
		g_Player.collision.radius = PLAYER_COLLISION;
	}

	//エネルギーの回復
	if (!Barrier_isUsed() && !g_Player.isShot)
	{
		g_Player.EN += 0.2f;
	}

	//エネルギーが最大値以上にならない処理
	if (g_Player.EN > g_Player.MaxEN)
	{
		g_Player.EN = g_Player.MaxEN;
	}

	//プレイヤーのHPが1未満になったら削除
	if (g_Player.HP < 1)
	{
		Player_Destroy();
		Game_SceneChange(SCENE_GAMEOVER);
	}

	g_ShotFrame++;
}

//=====================================================
//描画処理
//=====================================================
void Player_Draw(void)
{
	if (g_Player.isUse)
	{
		Sprite_Draw(g_Player.tex, g_Player.pos.x, g_Player.pos.y, 0, 0, 64, 64);
	}
}

//=====================================================
//プレイヤーのフラグ取得
//=====================================================
bool Player_isUsed(void)
{
	return g_Player.isUse;
}

//=====================================================
//プレイヤーのフラグ削除
//=====================================================
void Player_Destroy(void)
{
	g_Player.isUse = false;
}

//=====================================================
//当たり判定の構造体取得
//=====================================================
CIRCLE *Player_GetCollision(void)
{
	return &g_Player.collision;
}

//=====================================================
//プレイヤーの構造体取得
//=====================================================
PLAYER *Player_GetObj(void)
{
	return &g_Player;
}

//=====================================================
//ダメージ処理
//=====================================================
void Player_Damage(GA gauge, float damage)
{
	switch (gauge)
	{
	case GAUGE_HP:
		if (g_Player.HP <= g_Player.MaxHP)
		{
			g_Player.HP -= damage;
		}
		break;
	case GAUGE_EN:
		if (g_Player.EN <= g_Player.MaxEN)
		{
			g_Player.EN -= damage;
		}
		break;
	default:
		break;
	}
}