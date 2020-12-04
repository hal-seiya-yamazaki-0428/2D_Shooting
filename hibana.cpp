#include "hibana.h"
#include "texture.h"
#include "sprite.h"
#include "enemy.h"
#include "sound.h"

//=====================================================
//�}�N����`
//=====================================================

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static HIBANA g_Hibana[HIBANA_MAX];

//=====================================================
//������
//=====================================================
void Hibana_Init(void)
{
	g_Hibana[0].tex = Texture_SetLoadFile("Texture\\hibana.png", 259, 194);
	for (int i = 0; i < HIBANA_MAX; i++)
	{
		g_Hibana[i].isUse = false;
		g_Hibana[i].pos = D3DXVECTOR2(0, 0);
		g_Hibana[i].frameCount = 0;
	}
}
//=====================================================
//�I������
//=====================================================
void Hibana_Uninit(void)
{

}

//=====================================================
//�X�V����
//=====================================================
void Hibana_Update(void)
{
	for (int i = 0; i < HIBANA_MAX; i++)
	{
		if (!g_Hibana[i].isUse)
		{
			continue;
		}
		if (g_Hibana[i].frameCount > 8 * 2)
		{
			g_Hibana[i].isUse = false;
			g_Hibana[i].frameCount = 0;
		}
		g_Hibana[i].frameCount++;
	}
}

//=====================================================
//�`�揈��
//=====================================================
void Hibana_Draw(void)
{
	int width = Texture_GetWidth(g_Hibana[0].tex) / 4;
	int height = Texture_GetHeight(g_Hibana[0].tex) / 3;
	for (int i = 0; i < HIBANA_MAX; i++)
	{
		if (!g_Hibana[i].isUse)
		{
			continue;
		}
		int pattern = g_Hibana[i].frameCount / 2;
		//�����̕`��
		Sprite_Draw(g_Hibana[0].tex, g_Hibana[i].pos.x, g_Hibana[i].pos.y, pattern % 4 * width, pattern % 4 / 2 * height, width, height);
	}
}

//=====================================================
//��������
//=====================================================
void Hibana_Create(float x, float y)
{
	for (int i = 0; i < HIBANA_MAX; i++)
	{
		if (!g_Hibana[i].isUse)
		{
			g_Hibana[i].pos.x = x;
			g_Hibana[i].pos.y = y;
			g_Hibana[i].isUse = true;
			break;
		}
	}
}

//=====================================================
//�g�p�����ǂ���
//=====================================================
bool Hibana_isUse(int index)
{
	return g_Hibana[index].isUse;
}

//=====================================================
//�폜
//=====================================================
void Hibana_Destory(int index)
{
	g_Hibana[index].isUse = false;
}