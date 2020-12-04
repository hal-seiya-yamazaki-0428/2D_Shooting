#ifndef _BARRIER_H_
#define _BARRIER_H_

#include <d3dx9.h>
#include "collision.h"

typedef struct
{
	D3DXVECTOR2 Pos;
	bool isUse;
	unsigned int Tex;
}BARRIER;

void Barrier_Init(void);
void Barrier_Uninit(void);
void Barrier_Update(void);
void Barrier_Draw(void);
void Barrier_Create(float x, float y);
void Barrier_Create(float x, float y, bool sound);
void Barrier_Destroy(void);
bool Barrier_isUsed(void);

#endif