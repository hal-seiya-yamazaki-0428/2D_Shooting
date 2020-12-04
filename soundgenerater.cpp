#include "sound.h"

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static bool g_play = true;

//=====================================================
//BGM�Đ�
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
