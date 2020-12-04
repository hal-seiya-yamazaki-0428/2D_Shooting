#include "sound.h"

//=====================================================
//ƒOƒ[ƒoƒ‹•Ï”
//=====================================================
static bool g_play = true;

//=====================================================
//BGMÄ¶
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
