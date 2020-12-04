#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include <d3d9.h>
#include <d3dx9.h>

#define EXPLOSION_MAX (32)	//îöî≠ÇÃç≈ëÂêî

typedef struct
{
	D3DXVECTOR2 pos;
	bool isUse;
	unsigned int tex;
	int frameCount;
}EXPLOSION;

void Explosion_Init(void);
void Explosion_Uninit(void);
void Explosion_Update(void);
void Explosion_Draw(void);
void Explosion_Create(float x, float y);
void Explosion_Create(float x, float y, bool sound);
bool Explosion_isUse(int index);
void Explosion_Destory(int index);

#endif