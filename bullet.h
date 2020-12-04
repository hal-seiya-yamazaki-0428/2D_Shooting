#ifndef _BULLET_H_
#define _BULLET_H_

#include <d3d9.h>
#include <d3dx9.h>
#include "collision.h"

#define BULLET_MAX (512)	//弾の最大数
#define BULLET_ANGLE_LEFT	(120)	//左の角度
#define BULLET_ANGLE_RIGHT	(60)	//右の角度

typedef enum
{
	BULLET_NONE,	//なし
	BULLET_NORMAL,	//単発縦横固定
	BULLET_HOMING,	//弾が追尾する
	BULLET_FAN		//扇状に弾が発射
}BULLET_TYPE;

typedef struct
{
	D3DXVECTOR2 Pos;	//弾の座標
	bool isUse;			//使用中かどうか	1:使用 0:未使用
	unsigned int Tex;	//テクスチャ
	CIRCLE collision;	//当たり判定の構造体
	int angle;			//角度
	int shotCount;		//発射した弾数
	BULLET_TYPE type;	//弾の種類
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