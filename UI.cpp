#include "sprite.h"
#include "texture.h"
#include <d3dx9.h>
#include "UI.h"
#include "player.h"
#include "bg.h"
#include "game.h"
#include "medium_boss.h"
#include "boss.h"

//=====================================================
//�}�N����`
//=====================================================
#define TEXTURE_MAX (16)	//�e�N�X�`���̐�

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static unsigned int g_Tex[TEXTURE_MAX];
static float g_conductor;
static D3DXVECTOR2 g_pos;

//=====================================================
//������
//=====================================================
void UI_Init(void)
{
	g_Tex[0] = Texture_SetLoadFile("Texture\\HP_Gauge_bg.png", 200, 32);		//�Q�[�W���̍��w�i
	g_Tex[1] = Texture_SetLoadFile("Texture\\HP_Gauge_01_green.png", 200, 32);	//hp�Q�[�W(��)
	g_Tex[2] = Texture_SetLoadFile("Texture\\HP_Gauge_01_frame.png", 200, 32);	//�Q�[�W�g
	g_Tex[3] = Texture_SetLoadFile("Texture\\HP_Gauge_01_blue.png", 200, 32);	//en�Q�[�W(��)
	g_Tex[4] = Texture_SetLoadFile("Texture\\�n�[�g.png", 50, 50);				//hp�p�̃A�C�R��
	g_Tex[5] = Texture_SetLoadFile("Texture\\�G�l���M�[.png", 60, 39);			//en�p�̃A�C�R��
	g_Tex[6] = Texture_SetLoadFile("Texture\\black.jpeg", BG_START, 744);		//���w�i
	g_Tex[7] = Texture_SetLoadFile("Texture\\start.png", 71, 22);				//�X�^�[�g�A�C�R��
	g_Tex[8] = Texture_SetLoadFile("Texture\\clear.png", 67, 22);				//�S�[���A�C�R��
	g_Tex[9] = Texture_SetLoadFile("Texture\\laser.png", 512, 650);				//����
	g_Tex[10] = Texture_SetLoadFile("Texture\\p_red.png", 54, 54);				//�v���C���[�A�C�R��
	g_Tex[11] = Texture_SetLoadFile("Texture\\operation.png", 136, 203);		//�������
	g_conductor = 0;
	g_pos = D3DXVECTOR2((FLOAT)(BG_START - Texture_GetWidth(g_Tex[7]) - 30.0f + 10.0f) - 3.0f, (FLOAT)(BG_HEIGHT - Texture_GetHeight(g_Tex[7]) - 25.0f));
}
//=====================================================
//�I��
//=====================================================
void UI_Uninit(void)
{

}
//=====================================================
//�X�V
//=====================================================
void UI_Updata(void)
{
	//�}�b�v�̐i�s
	if (!Medium_Boss_isUsed() && !Boss_isUsed() && Game_GetScene() == SCENE_MAIN)
	{
		UI_CondCount();
	}
	//�S�[��
	if (g_conductor > 500)
	{
		g_conductor = 500;
		Game_SceneChange(SCENE_GAMECLEAR);
	}
}
//=====================================================
//�`��
//=====================================================
void UI_Draw(void)
{
	PLAYER *pPlayer = Player_GetObj();

	//���g
	Sprite_Draw(g_Tex[6], 0, 0);					//��
	Sprite_Draw(g_Tex[6], BG_START + BG_WIDTH, 0, 0, 0, 1024, 1024);	//�E
	for (int i = 0; i < 3; i++)
	{
		Sprite_Draw(g_Tex[6], (float)(BG_START + i * BG_START), (float)BG_HEIGHT);	//��
	}

	//���C����ʂłȂ���Ε\�����Ȃ�
	if (Game_GetScene() != SCENE_MAIN)
	{
		return;
	}

	//hp�Q�[�W�̕\��
	Sprite_Draw(g_Tex[0], 300, BG_HEIGHT + 10);									//�w�i
	Sprite_Draw(g_Tex[1], 300, BG_HEIGHT + 10, 0, 0, (int)(Texture_GetWidth(g_Tex[1]) * pPlayer->HP / pPlayer->MaxHP), 32);		//�΃Q�[�W
	Sprite_Draw(g_Tex[2], 300, BG_HEIGHT + 10);									//�t���[��

	//en�Q�[�W�̕\��
	Sprite_Draw(g_Tex[0], 580, BG_HEIGHT + 10);									//�w�i
	Sprite_Draw(g_Tex[3], 580, BG_HEIGHT + 10, 0, 0, (int)(Texture_GetWidth(g_Tex[1]) * pPlayer->EN / pPlayer->MaxEN), 32);		//�Q�[�W
	Sprite_Draw(g_Tex[2], 580, BG_HEIGHT + 10);									//�t���[��

	//hp�Aen�̃A�C�R���\��
	Sprite_Draw(g_Tex[4], 250, BG_HEIGHT);
	Sprite_Draw(g_Tex[5], 530, BG_HEIGHT + 5);

	//�����\��
	int width = Texture_GetWidth(g_Tex[9]);
	int height = Texture_GetHeight(g_Tex[9]);
	Sprite_Draw(g_Tex[9], (float)(BG_START - Texture_GetWidth(g_Tex[7]) - 30.0f), (float)((BG_HEIGHT - height) + 100),
		width * 5 / 7, 100, width / 7, height - 100);
	//�X�^�[�g�\��
	Sprite_Draw(g_Tex[7], (float)(BG_START - Texture_GetWidth(g_Tex[7]) - 30.0f), (float)(BG_HEIGHT - Texture_GetHeight(g_Tex[7])));
	//�S�[���\��
	Sprite_Draw(g_Tex[8], (float)(BG_START - Texture_GetWidth(g_Tex[8]) - 30.0f), (float)(Texture_GetHeight(g_Tex[8])));
	//�n�}��̃v���C���[�A�C�R��
	Sprite_Draw(g_Tex[10], g_pos.x, g_pos.y - g_conductor);
	//�������
	Sprite_Draw(g_Tex[11], (float)(BG_START + BG_WIDTH), (float)(BG_HEIGHT - Texture_GetHeight(g_Tex[11])));
}

//=====================================================
//�v���C���[�A�C�R���̐i�s
//=====================================================
void UI_CondCount(void)
{
	g_conductor += 0.1f;
}

//=====================================================
//�v���C���[�A�C�R���̏ꏊ
//=====================================================
float UI_GetCond(void)
{
	return g_conductor;
}