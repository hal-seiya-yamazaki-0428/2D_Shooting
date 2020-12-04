#ifndef _ITEM_H_
#define _ITEM_H_

#include "collision.h"
#include <d3dx9.h>

//=====================================================
//�}�N����`
//=====================================================
#define ITEM_MAX (32)	//�A�C�e���̍ő吔

typedef enum
{
	ITEM_TYPE_ENE,		//�G�l���M�[��
	ITEM_TYPE_POWER_UP	//�V���b�g���x���A�b�v
}ITEM_TYPE;

typedef struct
{
	D3DXVECTOR2 pos;
	CIRCLE collision;
	bool isUse;
	unsigned int tex;
	ITEM_TYPE type;
}ITEM;

void Item_Init(void);
void Item_Uninit(void);
void Item_Update(void);
void Item_Draw(void);
void Item_Create(float x, float y);
void Item_Create(float x, float y, ITEM_TYPE type);
bool Item_isUse(int index);
void Item_Destroy(int index);
CIRCLE *Item_GetCollision(int index);
ITEM_TYPE item_GetType(int index);
void Item_EnemyCountUp(void);

#endif 