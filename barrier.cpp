#include "barrier.h"
#include "texture.h"
#include "player.h"
#include "sprite.h"
#include "game.h"
#include "sound.h"

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static BARRIER g_Barrier;

//=====================================================
//������
//=====================================================
void Barrier_Init(void)
{
	g_Barrier.isUse = false;
	g_Barrier.Pos = D3DXVECTOR2(0, 0);
	g_Barrier.Tex = Texture_SetLoadFile("Texture\\spr_shield.png", 128, 128);
}
//=====================================================
//�I��
//=====================================================
void Barrier_Uninit(void)
{

}
//=====================================================
//�X�V
//=====================================================
void Barrier_Update(void)
{
	if (g_Barrier.isUse)
	{
		if (Game_GetScene() == SCENE_TUTORIAL)
		{
			return;
		}
		PLAYER *pPlayer = Player_GetObj();
		Player_Damage(GAUGE_EN, 0.3f);
		g_Barrier.Pos.x = pPlayer->pos.x - Texture_GetWidth(g_Barrier.Tex) / 4;
		g_Barrier.Pos.y = pPlayer->pos.y - Texture_GetHeight(g_Barrier.Tex) / 4;
	}
}
//=====================================================
//�`��
//=====================================================
void Barrier_Draw(void)
{
	if (g_Barrier.isUse)
	{
		Sprite_Draw(g_Barrier.Tex, g_Barrier.Pos.x, g_Barrier.Pos.y, 128);
	}
}
//=====================================================
//����
//=====================================================
void Barrier_Create(float x, float y)
{
	if (!g_Barrier.isUse)
	{
		g_Barrier.Pos.x = x - Texture_GetWidth(g_Barrier.Tex) / 4;
		g_Barrier.Pos.y = y - Texture_GetHeight(g_Barrier.Tex) / 4;
		g_Barrier.isUse = true;
		PlaySound(SOUND_LABEL_SE_BARI_CRE);
	}
}
//=====================================================
//����(�T�E���h�Đ�)
//=====================================================
void Barrier_Create(float x, float y, bool sound)
{
	if (!g_Barrier.isUse)
	{
		g_Barrier.Pos.x = x - Texture_GetWidth(g_Barrier.Tex) / 4;
		g_Barrier.Pos.y = y - Texture_GetHeight(g_Barrier.Tex) / 4;
		g_Barrier.isUse = true;
		if (sound)
		{
			PlaySound(SOUND_LABEL_SE_BARI_CRE);
		}
	}
}
//=====================================================
//�t���O�폜
//=====================================================
void Barrier_Destroy(void)
{
	g_Barrier.isUse = false;
}
//=====================================================
//�g�p�������ׂ�
//=====================================================
bool Barrier_isUsed(void)
{
	return g_Barrier.isUse;
}