#ifndef _UI_H_
#define _UI_H_

typedef enum
{
	GAUGE_HP,	//0
	GAUGE_EN,	//1
	GAUGE_MAX	//2
}GA;

void UI_Init(void);
void UI_Uninit(void);
void UI_Updata(void);
void UI_Draw(void);
void UI_CondCount(void);
float UI_GetCond(void);

#endif