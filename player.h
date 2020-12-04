#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <d3d9.h>
#include <d3dx9.h>
#include "collision.h"
#include "UI.h"
#include "bullet.h"

#define MAX_LEVEL (3)	//レベルの最大値

typedef struct 
{
	D3DXVECTOR2 pos;	//座標
	bool isUse;			//使用フラグ
	unsigned int tex;	//テクスチャ
	CIRCLE collision;	//当たり判定
	float HP;			//体力
	float EN;			//エネルギー
	float MaxHP = 0;	//hp値保存
	float MaxEN = 0;	//en値保存
	bool isShot;		//射撃中かどうか
	int shotLevel;		//射撃レベル
	int shotExp;		//射撃レベル用経験値
	bool LevelUp;		//レベルアップしたかどうか
}PLAYER;

void Player_Init(void);
void Player_Uninit(void);
void Player_Update(void);
void Player_Draw(void);
bool Player_isUsed(void);
void Player_Destroy(void);
CIRCLE *Player_GetCollision(void);
PLAYER *Player_GetObj(void);
void Player_Damage(GA gauge, float damage);

#endif