#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <d3d9.h>
#include <d3dx9.h>
#include "collision.h"
#include "bullet.h"

#define ENEMY_MAX (32)		//�G�̍ő吔
#define ENEMY_DEF_HP (3)	//�W���̗�
#define ENEMY_X_PATTERN (5)	//�G�f�ނ̉��p�^�[����
#define ENEMY_Y_PATTERN (5)	//�G�f�ނ̏c�p�^�[����


typedef enum
{
	ENEMY_TYPE_NONE,	//�Ȃ�
	ENEMY_TYPE_LEFT,	//��
	ENEMY_TYPE_RIGHT,	//�E
	ENEMY_TYPE_HOMING	//�ǔ�
}ENEMY_TYPE;

typedef struct
{
	D3DXVECTOR2 Pos;			//���W
	bool isUse;					//�g�p�����ǂ���	//1:�g�p 0:���g�p
	unsigned int Tex;			//�e�N�X�`��
	CIRCLE collision;			//�����蔻��̍\����
	int angle;					//�p�x
	ENEMY_TYPE type;			//�G�̍s�����
	//BULLET bullet;				//�e�̍\����
	int frame;					//�t���[����
	int hp;						//�̗�
	int MaxHP;					//�ő�̗�
}ENEMY;

void Enemy_Init(void);
void Enemy_Uninit(void);
void Enemy_Update(void);
void Enemy_Draw(void);
void Enemy_Create(ENEMY_TYPE type, float x, float y, int  hp);
bool Enemy_isUsed(int index);
void Enemy_Destroy(int index);
CIRCLE *Enemy_GetCollision(int index);
ENEMY *Enemy_GetObj(int index);
void Enemy_Damage(int id, int damage);
int Enemy_GetHP(int id);
int Enemy_IndexAlgorithm(void);

#endif