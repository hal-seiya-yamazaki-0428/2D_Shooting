
#include "enemy.h"
#include "texture.h"
#include "sprite.h"
#include "common.h"
#include "collision.h"
#include "debug.h"
#include "input.h"
#include "debug_font.h"
#include "bg.h"
#include "player.h"

//=====================================================
//�}�N����`
//=====================================================
#define ENEMY_SPD (3.0)		//�G�̑���

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static ENEMY g_Enemy[ENEMY_MAX];
static float fsin[360], fcos[360];
static int frame = 0;
static int g_Index = 0;
static int g_SaveHp = 0;

//=====================================================
//����������
//=====================================================
void Enemy_Init(void)
{
	g_Enemy[0].Tex = Texture_SetLoadFile("Texture\\�L����32�~32.png", 320, 320);
	g_Index = 0;
	g_SaveHp = 0;
	for (int i = 0; i < 360; i++)
	{
		fsin[i] = (float)sin(i * D3DX_PI / 180);
		fcos[i] = (float)cos(i * D3DX_PI / 180);
	}
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		g_Enemy[i].isUse = false;
		g_Enemy[i].collision.radius = 28.0f;
		g_Enemy[i].Pos = D3DXVECTOR2(0, 0);
		g_Enemy[i].angle = 90;
		g_Enemy[i].frame = 30;
		g_Enemy[i].hp = ENEMY_DEF_HP;
	}
}

//=====================================================
//�I������
//=====================================================
void Enemy_Uninit(void)
{

}

//=====================================================
//�X�V����
//=====================================================
void Enemy_Update(void)
{
	PLAYER *pPlayer = Player_GetObj();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		//�G���g�p����Ă����ꍇ
		if (!g_Enemy[i].isUse)
		{
			continue;
		}

		//�v���C���[�Ǐ]
		if (g_Enemy[i].type == ENEMY_TYPE_HOMING)
		{
			int angle = Bullet_GetAngle(pPlayer->pos.x, pPlayer->pos.y, g_Enemy[i].Pos.x, g_Enemy[i].Pos.y);
			g_Enemy[i].Pos.x += (FLOAT)(fcos[angle] * ENEMY_SPD);
			g_Enemy[i].Pos.y += ENEMY_SPD;
		}
		//��ʔ����܂Œʏ�~��
		else if (g_Enemy[i].Pos.y < BG_HEIGHT / 3)
		{
			g_Enemy[i].Pos.y += ENEMY_SPD;
		}
		else
		{
			int angle = g_Enemy[i].angle;
			//���ɗ����
			if (g_Enemy[i].type == ENEMY_TYPE_LEFT)
			{
				g_Enemy[i].Pos.x += (FLOAT)(fcos[angle] * ENEMY_SPD);
				g_Enemy[i].Pos.y += (FLOAT)(fsin[angle] * ENEMY_SPD);
			}
			//�E�ɗ����
			else if (g_Enemy[i].type == ENEMY_TYPE_RIGHT)
			{
				g_Enemy[i].Pos.x -= (FLOAT)(fcos[angle] * ENEMY_SPD);
				g_Enemy[i].Pos.y += (FLOAT)(fsin[angle] * ENEMY_SPD);
			}
			//���~
			else
			{
				g_Enemy[i].Pos.y += ENEMY_SPD;
			}
			g_Enemy[i].angle += 2;	//2�x�����炷
			if (g_Enemy[i].angle > 135)
			{
				g_Enemy[i].angle = 135;
			}
		}
		//�����蔻��̍��W�̍X�V
		g_Enemy[i].collision.position.x = g_Enemy[i].Pos.x + (FLOAT)(Texture_GetWidth(g_Enemy[0].Tex) / ENEMY_X_PATTERN / 2);
		g_Enemy[i].collision.position.y = g_Enemy[i].Pos.y + (FLOAT)(Texture_GetHeight(g_Enemy[0].Tex) / ENEMY_Y_PATTERN / 2);

		//��ʊO����
		if (g_Enemy[i].Pos.y + Texture_GetHeight(g_Enemy[0].Tex) / 2 > BG_HEIGHT)
		{
			if (g_Enemy[i].Pos.y > BG_HEIGHT)
			{
				g_Enemy[i].isUse = false;
			}
		}
		//�E�g�̏���
		if (g_Enemy[i].Pos.x + Texture_GetWidth(g_Enemy[0].Tex) / 4 > BG_START + BG_WIDTH)
		{
			if (g_Enemy[i].Pos.x > BG_START + BG_WIDTH)
			{
				g_Enemy[i].isUse = false;
			}
		}
		//���g�̏���
		if (g_Enemy[i].Pos.x < BG_START)
		{
			if (g_Enemy[i].Pos.x + Texture_GetWidth(g_Enemy[0].Tex) / 4 < BG_START)
			{
				g_Enemy[i].isUse = false;
			}
		}

		//�G����ʓ��ɓ����Ă���t���[�����J�E���g
		if (g_Enemy[i].Pos.y + Texture_GetHeight(g_Enemy[i].Tex) > 0)	
		{
			g_Enemy[i].frame++;
		}
		//�e�̔���
		//���t���[�����Ƃɔ���
		if (g_Enemy[i].frame > 70)
		{
			//�^�C�v��
			if (g_Enemy[i].type == ENEMY_TYPE_HOMING)
			{
				Bullet_TypeSelect(pPlayer->pos.x, pPlayer->pos.y, g_Enemy[i].Pos.x, g_Enemy[i].Pos.y, BULLET_FAN);
			}
			else if (g_Enemy[i].type == ENEMY_TYPE_LEFT || ENEMY_TYPE_RIGHT)
			{
				Bullet_TypeSelect(pPlayer->pos.x, pPlayer->pos.y, g_Enemy[i].Pos.x, g_Enemy[i].Pos.y, BULLET_HOMING);
			}
			g_Enemy[i].frame = 0;
		}
		frame++;
	}
}

//=====================================================
//�`�揈��
//=====================================================
void Enemy_Draw(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		//�G���g�p����Ă���Ε`��
		if (!g_Enemy[i].isUse)
		{
			continue;
		}
		if (g_Enemy[i].type != ENEMY_TYPE_NONE && g_Enemy[i].type != ENEMY_TYPE_HOMING)
		{
			Sprite_Draw(g_Enemy[0].Tex, g_Enemy[i].Pos.x, g_Enemy[i].Pos.y, Texture_GetWidth(g_Enemy[0].Tex) * 3 / ENEMY_X_PATTERN + 2, 0 + 2, Texture_GetWidth(g_Enemy[0].Tex) / ENEMY_X_PATTERN - 4, Texture_GetHeight(g_Enemy[0].Tex) / ENEMY_Y_PATTERN - 4);
		}
		else
		{
			Sprite_Upside_Draw(g_Enemy[0].Tex, g_Enemy[i].Pos.x, g_Enemy[i].Pos.y, Texture_GetWidth(g_Enemy[0].Tex) * 2 / ENEMY_X_PATTERN + 2, Texture_GetHeight(g_Enemy[0].Tex) * 2 / ENEMY_Y_PATTERN + 2, Texture_GetWidth(g_Enemy[0].Tex) / ENEMY_X_PATTERN - 4, Texture_GetHeight(g_Enemy[0].Tex) / ENEMY_Y_PATTERN - 4);
		}
	}
}

//=====================================================
//�G�쐬����
//=====================================================
void Enemy_Create(ENEMY_TYPE type, float x, float y, int hp)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		//�G���g�p����Ă��Ȃ���ΐ���
		if (!g_Enemy[i].isUse)
		{
			g_Enemy[i].isUse = true;
			g_Enemy[i].Pos = D3DXVECTOR2(x, y);
			g_Enemy[i].collision.position = D3DXVECTOR2(x, y);
			g_Enemy[i].type = type;
			g_Enemy[i].hp = hp;
			g_Enemy[i].frame = 0;
			g_Enemy[i].angle = 90;
			break;
		}
	}
}

//=====================================================
//�G�̃t���O�擾
//=====================================================
bool Enemy_isUsed(int index)
{
	return g_Enemy[index].isUse;
}

//=====================================================
//�e�̃t���O�폜
//=====================================================
void Enemy_Destroy(int index)
{
	g_Enemy[index].isUse = false;
}

//=====================================================
//�����蔻��̍\���̎擾
//=====================================================
CIRCLE *Enemy_GetCollision(int index)
{
	return &g_Enemy[index].collision;
}

//=====================================================
//�G�̍\���̎擾
//=====================================================
ENEMY *Enemy_GetObj(int index)
{
	return &g_Enemy[index];
}

//=====================================================
//�_���[�W����
//=====================================================
void Enemy_Damage(int id, int damage)
{
	g_Enemy[id].hp -= damage;
}

//=====================================================
//���݂�HP�擾
//=====================================================
int Enemy_GetHP(int id)
{
	return g_Enemy[id].hp;
}

//=====================================================
//��ʓ��̈�ԑ̗͂��������G�̗v�f���擾
//=====================================================
int Enemy_IndexAlgorithm(void)
{
	g_SaveHp = 0;

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (!g_Enemy[i].isUse)
		{
			return g_Index;
		}
		//�̗͔�r
		if (g_SaveHp < g_Enemy[i].hp)
		{
			g_SaveHp = g_Enemy[i].hp;
			g_Index = i;
		}
	}
	return -1;
}