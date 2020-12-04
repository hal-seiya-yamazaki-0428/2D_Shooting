#ifndef _MEDIUM_BOSS_H_
#define _MEDIUM_BOSS_H_

#include "enemy.h"
#include "med_bullet.h"

typedef enum
{
	MEDIUM_BOSS_NONE,		//何もしない
	MEDIUM_BOSS_HORIZONTAL,	//水平に動く
	MEDIUM_BOSS_UMOVE,		//Uの字に動く
	MEDIUM_BOSS_WAVE,		//波状に沿って動く
	MEDIUM_BOSS_STOP,		//画面真ん中で停止
	MEDIUM_BOSS_FIRST_IN	//ボス入場
}MEDIUM_BOSS_TYPE;

typedef struct
{
	ENEMY enemy;			//敵の構造体
	MED_BULLET bullet;		//敵の弾の構造体
	MEDIUM_BOSS_TYPE type;	//敵の行動種別
	float move;				//移動量
	bool right;				//向き
	int shotFrame;			//何フレーム弾を発射したか
}MEDIUM_BOSS;

void Medium_Boss_Init(void);
void Medium_Boss_Uninit(void);
void Medium_Boss_Update(void);
void Medium_Boss_Draw(void);
void Medium_Boss_Create(float x, float y, int  hp);
bool Medium_Boss_isUsed(void);
void Medium_Boss_Destroy(void);
CIRCLE *Medium_Boss_GetCollision(void);
void Medium_Boss_Damage(int damage);
int Medium_Boss_GetHP(void);

#endif