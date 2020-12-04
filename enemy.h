#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <d3d9.h>
#include <d3dx9.h>
#include "collision.h"
#include "bullet.h"

#define ENEMY_MAX (32)		//敵の最大数
#define ENEMY_DEF_HP (3)	//標準体力
#define ENEMY_X_PATTERN (5)	//敵素材の横パターン数
#define ENEMY_Y_PATTERN (5)	//敵素材の縦パターン数


typedef enum
{
	ENEMY_TYPE_NONE,	//なし
	ENEMY_TYPE_LEFT,	//左
	ENEMY_TYPE_RIGHT,	//右
	ENEMY_TYPE_HOMING	//追尾
}ENEMY_TYPE;

typedef struct
{
	D3DXVECTOR2 Pos;			//座標
	bool isUse;					//使用中かどうか	//1:使用 0:未使用
	unsigned int Tex;			//テクスチャ
	CIRCLE collision;			//当たり判定の構造体
	int angle;					//角度
	ENEMY_TYPE type;			//敵の行動種類
	//BULLET bullet;				//弾の構造体
	int frame;					//フレーム数
	int hp;						//体力
	int MaxHP;					//最大体力
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