#ifndef _BOSS_BULLET_H_
#define _BOSS_BULLET_H_

#include "bullet.h"

#define BULLET_MAX (512)	//�e�̍ő吔
#define BULLET_ANGLE_LEFT	(120)	//���̊p�x
#define BULLET_ANGLE_RIGHT	(60)	//�E�̊p�x

typedef enum
{
	BOSS_BULLET_NONE,		//�Ȃ�
	BOSS_BULLET_NORMAL,		//�P���c���Œ�
	BOSS_BULLET_HOMING,		//�e���ǔ�����
	BOSS_BULLET_FAN,		//���ɒe������
	BOSS_BULLET_SPIRAL,		//������
	BOSS_BULLET_INVERSION	//�ǔ���
}BOSS_BULLET_TYPE;

typedef struct
{
	BULLET bullet;
	BOSS_BULLET_TYPE type;	//�e�̎��
	int InvCount;			//���˂���e�̔��˂�����
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