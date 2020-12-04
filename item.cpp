#include "item.h"
#include "texture.h"
#include "sprite.h"
#include "bg.h"
#include "debug.h"

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static ITEM g_item[ITEM_MAX];
static int g_Frame = 0;
static int g_EnemyCount = 0;

//=====================================================
//������
//=====================================================
void Item_Init(void)
{
	g_item[0].tex = Texture_SetLoadFile("Texture\\item.png", 60, 39);
	g_item[1].tex = Texture_SetLoadFile("Texture\\shoot_mate.png", 453, 340);
	g_Frame = 0;
	g_EnemyCount = 0;
	for (int i = 0; i < ITEM_MAX; i++)
	{
		g_item[i].pos = D3DXVECTOR2(0, 0);
		g_item[i].collision.position = D3DXVECTOR2(0, 0);
		g_item[i].collision.radius = 20.0f;
		g_item[i].isUse = false;
		g_item[i].type = ITEM_TYPE_ENE;
	}
}

//=====================================================
//�I��
//=====================================================
void Item_Uninit(void)
{

}

//=====================================================
//�X�V
//=====================================================
void Item_Update(void)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (g_item[i].isUse)
		{
			//�A�C�e���̈ړ�
			g_item[i].pos.y += 2.0f;

			//�����蔻��̍X�V;
			g_item[i].collision.position.x = g_item[i].pos.x + Texture_GetWidth(g_item[0].tex) / 2;
			g_item[i].collision.position.y = g_item[i].pos.y + Texture_GetHeight(g_item[0].tex) / 2;

			//��ʊO����
			if (g_item[i].pos.y > BG_HEIGHT)
			{
				g_item[i].isUse = false;
			}
		}
	}
	g_Frame++;
}

//=====================================================
//�`��
//=====================================================
void Item_Draw(void)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (g_item[i].isUse)
		{
			//�G�l���M�[��
			if (g_item[i].type == ITEM_TYPE_ENE)
			{
				Sprite_Draw(g_item[0].tex, g_item[i].pos.x, g_item[i].pos.y);
			}
			//���x���A�b�v�p�A�C�e��
			else
			{
				Sprite_Draw(g_item[1].tex, g_item[i].pos.x, g_item[i].pos.y, 312, 0 / 4, 47, 47);
			}
		}
	}
}

//=====================================================
//����
//=====================================================
void Item_Create(float x, float y)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (!g_item[i].isUse)
		{
			//���j�����ƂɃ^�C�v�ύX
			if (g_EnemyCount % 6 == 0)
			{
				g_item[i].type = ITEM_TYPE_ENE;
			}
			else if (g_EnemyCount % 6 == 3)
			{
				g_item[i].type = ITEM_TYPE_POWER_UP;
			}
			else
			{
				return;
			}
			g_item[i].pos = D3DXVECTOR2(x, y);
			g_item[i].collision.position = D3DXVECTOR2(x, y);
			g_item[i].isUse = true;
			break;
		}
	}
}

//=====================================================
//����(�^�C�v�w��)
//=====================================================
void Item_Create(float x, float y, ITEM_TYPE type)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (!g_item[i].isUse)
		{
			g_item[i].pos = D3DXVECTOR2(x, y);
			g_item[i].collision.position = D3DXVECTOR2(x, y);
			g_item[i].isUse = true;
			g_item[i].type = type;
			break;
		}
	}
}

//=====================================================
//�g�p�����ǂ���
//=====================================================
bool Item_isUse(int index)
{
	return g_item[index].isUse;
}

//=====================================================
//�폜
//=====================================================
void Item_Destroy(int index)
{
	g_item[index].isUse = false;
}

//=====================================================
//�����蔻��̍\����
//=====================================================
CIRCLE *Item_GetCollision(int index)
{
	return &g_item[index].collision;
}

//=====================================================
//�A�C�e���̃^�C�v�擾
//=====================================================
ITEM_TYPE item_GetType(int index)
{
	return g_item[index].type;
}

//=====================================================
//�G���j���J�E���g
//=====================================================
void Item_EnemyCountUp(void)
{
	g_EnemyCount++;
}