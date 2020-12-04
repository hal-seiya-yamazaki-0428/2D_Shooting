#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <d3d9.h>
#include <d3dx9.h>
#include "collision.h"
#include "UI.h"
#include "bullet.h"

#define MAX_LEVEL (3)	//���x���̍ő�l

typedef struct 
{
	D3DXVECTOR2 pos;	//���W
	bool isUse;			//�g�p�t���O
	unsigned int tex;	//�e�N�X�`��
	CIRCLE collision;	//�����蔻��
	float HP;			//�̗�
	float EN;			//�G�l���M�[
	float MaxHP = 0;	//hp�l�ۑ�
	float MaxEN = 0;	//en�l�ۑ�
	bool isShot;		//�ˌ������ǂ���
	int shotLevel;		//�ˌ����x��
	int shotExp;		//�ˌ����x���p�o���l
	bool LevelUp;		//���x���A�b�v�������ǂ���
}PLAYER;

void Player_Init(void);
void Player_Uninit(void);
void Player_Update(void);
void Player_Draw(void);
bool Player_isUsed(void);
void Player_Destroy(void);
CIRCLE *Player_GetCollision(void);
PLAYER *Player_GetObj(void);
void Player_Damage(GA gauge, float damage);

#endif