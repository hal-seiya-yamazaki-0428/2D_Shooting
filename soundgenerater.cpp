#include "sound.h"

//=====================================================
//グローバル変数
//=====================================================
static bool g_play = true;

//=====================================================
//BGM再生
//=====================================================
void Sound_Generater(SOUND_LABEL label)
{
	if (g_play)
	{
		PlaySound(label);
		g_play = false;
	}
}

void Sound_Reset(void)
{
	g_play = true;
}
