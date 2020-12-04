#ifndef _MED_BULLET_H_
#define _MED_BULLET_H_

#include "bullet.h"

#define BULLET_MAX (512)	//’e‚ÌÅ‘å”
#define BULLET_ANGLE_LEFT	(120)	//¶‚ÌŠp“x
#define BULLET_ANGLE_RIGHT	(60)	//‰E‚ÌŠp“x

typedef enum
{
	MED_BULLET_NONE,		//‚È‚µ
	MED_BULLET_NORMAL,		//’P”­c‰¡ŒÅ’è
	MED_BULLET_HOMING,		//’e‚ª’Ç”ö‚·‚é
	MED_BULLET_FAN,			//îó‚É’e‚ª”­Ë
	MED_BULLET_SPIRAL		//—†ùó
}MED_BULLET_TYPE;

typedef struct
{
	BULLET bullet;
	MED_BULLET_TYPE type;	//’e‚Ìí—Ş
	int InvCount;			//”½Ë‚·‚é’e‚Ì”½Ë‚µ‚½‰ñ”
}MED_BULLET;

void Med_Bullet_Init(void);
void Med_Bullet_Uninit(void);
void Med_Bullet_Update(void);
void Med_Bullet_Draw(void);
void Med_Bullet_Create(MED_BULLET_TYPE type, float x, float y);
void Med_Bullet_Create(MED_BULLET_TYPE type, float px, float py, float ex, float ey);
void Med_Bullet_Create(MED_BULLET_TYPE type, float x, float y, int angle);
int Med_Bullet_Create_Return(MED_BULLET_TYPE type, float x, float y, int angle);
bool Med_Bullet_isUsed(int index);
void Med_Bullet_Destroy(int index);
CIRCLE *Med_Bullet_GetCollision(int index);
MED_BULLET *Med_Bullet_GetObj(int index);

#endif
