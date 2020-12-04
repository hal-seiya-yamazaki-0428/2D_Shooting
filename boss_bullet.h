#ifndef _BOSS_BULLET_H_
#define _BOSS_BULLET_H_

#include "bullet.h"

#define BULLET_MAX (512)	//’e‚ÌÅ‘å”
#define BULLET_ANGLE_LEFT	(120)	//¶‚ÌŠp“x
#define BULLET_ANGLE_RIGHT	(60)	//‰E‚ÌŠp“x

typedef enum
{
	BOSS_BULLET_NONE,		//‚È‚µ
	BOSS_BULLET_NORMAL,		//’P”­c‰¡ŒÅ’è
	BOSS_BULLET_HOMING,		//’e‚ª’Ç”ö‚·‚é
	BOSS_BULLET_FAN,		//îó‚É’e‚ª”­Ë
	BOSS_BULLET_SPIRAL,		//—†ùó
	BOSS_BULLET_INVERSION	//•Ç”½Ë
}BOSS_BULLET_TYPE;

typedef struct
{
	BULLET bullet;
	BOSS_BULLET_TYPE type;	//’e‚Ìí—Ş
	int InvCount;			//”½Ë‚·‚é’e‚Ì”½Ë‚µ‚½‰ñ”
}BOSS_BULLET;

void Boss_Bullet_Init(void);
void Boss_Bullet_Uninit(void);
void Boss_Bullet_Update(void);
void Boss_Bullet_Draw(void);
void Boss_Bullet_Create(BOSS_BULLET_TYPE type, float x, float y);
void Boss_Bullet_Create(BOSS_BULLET_TYPE type, float x, float y, int angle);
void Boss_Bullet_Create(BOSS_BULLET_TYPE type, float px, float py, float ex, float ey);
bool Boss_Bullet_isUsed(int index);
void Boss_Bullet_Destroy(int index);
CIRCLE *Boss_Bullet_GetCollision(int index);
BOSS_BULLET *Boss_Bullet_GetObj(int index);

#endif