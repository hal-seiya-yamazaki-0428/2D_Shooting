#ifndef _HIBANA_H_
#define _HIBANA_H_

#include <d3d9.h>
#include <d3dx9.h>

#define HIBANA_MAX (32)	//�ΉԂ̍ő吔

typedef struct
{
	D3DXVECTOR2 pos;
	bool isUse;
	unsigned int tex;
	int frameCount;
}HIBANA;

void Hibana_Init(void);
void Hibana_Uninit(void);
void Hibana_Update(void);
void Hibana_Draw(void);
void Hibana_Create(float x, float y);
bool Hibana_isUse(int index);
void Hibana_Destory(int index);

#endif