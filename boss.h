#ifndef _BOSS_H_
#define _BOSS_H_

#include "enemy.h"
//#include "boss_bullet.h"

typedef enum
{
	BOSS_NONE,		//何もしない
	BOSS_HORIZONTAL,//水平に動く
	BOSS_UMOVE,		//Uの字に動く
	BOSS_WAVE,		//波状に沿って動く
	BOSS_STOP,		//画面真ん中で停止
	BOSS_FIRST_IN	//ボス入場
}BOSS_TYPE;

typedef struct
{
	ENEMY enemy;	//敵の構造体
	BULLET bullet;	//敵の弾の構造体
	BOSS_TYPE type;	//敵の行動種別
	float move;		//移動量
	bool right;		//向き
	int shotFrame;	//何フレーム弾を発射したか
}BOSS;

void Boss_Init(void);
void Boss_Uninit(void);
void Boss_Update(void);
void Boss_Draw(void);
void Boss_Create(float x, float y, int  hp);
bool Boss_isUsed(void);
void Boss_Destroy(void);
CIRCLE *Boss_GetCollision(void);
BOSS *Boss_GetObj(void);
void Boss_Damage(int damage);
int Boss_GetHP(void);
int Boss_GetShotCount(void);

#endif