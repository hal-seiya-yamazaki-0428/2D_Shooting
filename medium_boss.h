#ifndef _MEDIUM_BOSS_H_
#define _MEDIUM_BOSS_H_

#include "enemy.h"
#include "med_bullet.h"

typedef enum
{
	MEDIUM_BOSS_NONE,		//�������Ȃ�
	MEDIUM_BOSS_HORIZONTAL,	//�����ɓ���
	MEDIUM_BOSS_UMOVE,		//U�̎��ɓ���
	MEDIUM_BOSS_WAVE,		//�g��ɉ����ē���
	MEDIUM_BOSS_STOP,		//��ʐ^�񒆂Œ�~
	MEDIUM_BOSS_FIRST_IN	//�{�X����
}MEDIUM_BOSS_TYPE;

typedef struct
{
	ENEMY enemy;			//�G�̍\����
	MED_BULLET bullet;		//�G�̒e�̍\����
	MEDIUM_BOSS_TYPE type;	//�G�̍s�����
	float move;				//�ړ���
	bool right;				//����
	int shotFrame;			//���t���[���e�𔭎˂�����
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