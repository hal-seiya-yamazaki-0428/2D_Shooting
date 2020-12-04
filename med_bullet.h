#ifndef _MED_BULLET_H_
#define _MED_BULLET_H_

#include "bullet.h"

#define BULLET_MAX (512)	//�e�̍ő吔
#define BULLET_ANGLE_LEFT	(120)	//���̊p�x
#define BULLET_ANGLE_RIGHT	(60)	//�E�̊p�x

typedef enum
{
	MED_BULLET_NONE,		//�Ȃ�
	MED_BULLET_NORMAL,		//�P���c���Œ�
	MED_BULLET_HOMING,		//�e���ǔ�����
	MED_BULLET_FAN,			//���ɒe������
	MED_BULLET_SPIRAL		//������
}MED_BULLET_TYPE;

typedef struct
{
	BULLET bullet;
	MED_BULLET_TYPE type;	//�e�̎��
	int InvCount;			//���˂���e�̔��˂�����
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
