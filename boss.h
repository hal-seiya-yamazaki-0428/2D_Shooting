#ifndef _BOSS_H_
#define _BOSS_H_

#include "enemy.h"
//#include "boss_bullet.h"

typedef enum
{
	BOSS_NONE,		//�������Ȃ�
	BOSS_HORIZONTAL,//�����ɓ���
	BOSS_UMOVE,		//U�̎��ɓ���
	BOSS_WAVE,		//�g��ɉ����ē���
	BOSS_STOP,		//��ʐ^�񒆂Œ�~
	BOSS_FIRST_IN	//�{�X����
}BOSS_TYPE;

typedef struct
{
	ENEMY enemy;	//�G�̍\����
	BULLET bullet;	//�G�̒e�̍\����
	BOSS_TYPE type;	//�G�̍s�����
	float move;		//�ړ���
	bool right;		//����
	int shotFrame;	//���t���[���e�𔭎˂�����
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