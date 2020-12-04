#include "texture.h"
#include "sprite.h"
#include <d3dx9.h>
#include "common.h"
#include "bg.h"
#include "UI.h"

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static int g_Tex;
static D3DXVECTOR2 g_Pos;

//=====================================================
//����������
//=====================================================
void BG_Init(void)
{
	g_Tex = Texture_SetLoadFile("Texture\\bg02.jpg", BG_WIDTH, BG_HEIGHT);
	g_Pos = D3DXVECTOR2(0, 0);
}
//=====================================================
//�I������
//=====================================================
void BG_Uninit(void)
{

}
//=====================================================
//�X�V
//=====================================================
void BG_Update(void)
{
	//�S�[������܂Ŕw�i�X�N���[��
	if (!(UI_GetCond() > 490))
	{
		g_Pos.y++;
	}
	if (g_Pos.y > BG_HEIGHT)
	{
		g_Pos.y = 0;
	}
}
//=====================================================
//�`��
//=====================================================
void BG_Draw(void)
{
	Sprite_Draw(g_Tex, (float)BG_START, (float)g_Pos.y, 0, 0, BG_WIDTH, (int)(BG_HEIGHT - g_Pos.y));
	Sprite_Draw(g_Tex, (float)BG_START, (float)(-BG_HEIGHT + g_Pos.y), 0, 0, BG_WIDTH, BG_HEIGHT);
}