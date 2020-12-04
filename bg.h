#ifndef _BG_H_
#define _BG_H_

#include "common.h"

#define BG_WIDTH (SCREEN_WIDTH / 2 + 100)	//�w�i�̕�
#define BG_HEIGHT (SCREEN_HEIGHT - 48)	//�w�i�̍���
#define BG_START (SCREEN_WIDTH / 5)	//�w�i�̕`��J�n�ʒu

void BG_Init(void);
void BG_Uninit(void);
void BG_Update(void);
void BG_Draw(void);

#endif