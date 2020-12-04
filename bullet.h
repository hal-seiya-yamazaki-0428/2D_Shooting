#ifndef _BULLET_H_
#define _BULLET_H_

#include <d3d9.h>
#include <d3dx9.h>
#include "collision.h"

#define BULLET_MAX (512)	//�e�̍ő吔
#define BULLET_ANGLE_LEFT	(120)	//���̊p�x
#define BULLET_ANGLE_RIGHT	(60)	//�E�̊p�x

typedef enum
{
	BULLET_NONE,	//�Ȃ�
	BULLET_NORMAL,	//�P���c���Œ�
	BULLET_HOMING,	//�e���ǔ�����
	BULLET_FAN		//���ɒe������
}BULLET_TYPE;

typedef struct
{
	D3DXVECTOR2 Pos;	//�e�̍��W
	bool isUse;			//�g�p�����ǂ���	1:�g�p 0:���g�p
	unsigned int Tex;	//�e�N�X�`��
	CIRCLE collision;	//�����蔻��̍\����
	int angle;			//�p�x
	int shotCount;		//���˂����e��
	BULLET_TYPE type;	//�e�̎��
}BULLET;

void Bullet_Init(void);
void Bullet_Uninit(void);
void Bullet_Update(void);
void Bullet_Draw(void);
void Bullet_Create(BULLET *bullet, float x, float y);
void Bullet_Create(BULLET *bullet, float x, float y, BULLET_TYPE type);
void Bullet_Create(BULLET *bullet, float x, float y, int angle);
void Bullet_Create(BULLET *bullet, float x, float y, int angle, BULLET_TYPE type);
bool Bullet_isUsed(int index, const char *obj);
void Bullet_Destroy(int index, const char *obj);
CIRCLE *Bullet_GetCollision(int index, const char *obj);
BULLET *Bullet_GetObj(int index, const char *obj);
int Bullet_GetAngle(float px, float py, float ex, float ey);
void Bullet_TypeSelect(float px, float py, float ex, float ey, BULLET_TYPE type);

#endif