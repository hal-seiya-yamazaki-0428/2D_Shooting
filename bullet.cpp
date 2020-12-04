#include "bullet.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "debug.h"
#include "player.h"
#include "bg.h"
#include "enemy.h"
#include "game.h"
#include "Efect.h"
#include "enemy.h"

//=====================================================
//�}�N����`
//=====================================================
#define BULLET_SPD (8.0)			//�e�̑���

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static BULLET g_PlayerBullet[BULLET_MAX];
static BULLET g_EnemyBullet[BULLET_MAX];
static float fsin[360], fcos[360];

//=====================================================
//����������
//=====================================================
void Bullet_Init(void)
{
	g_PlayerBullet[0].Tex = Texture_SetLoadFile("Texture\\bullet.png", 64, 64);
	g_EnemyBullet[0].Tex = Texture_SetLoadFile("Texture\\UV_bullet.png", 230, 272);
	for (int i = 0; i < 360; i++)
	{
		fsin[i] = (float)sin(i * D3DX_PI / 180);
		fcos[i] = (float)cos(i * D3DX_PI / 180);
	}
	for (int i = 0; i < BULLET_MAX; i++)
	{
		g_PlayerBullet[i].isUse = false;
		g_PlayerBullet[i].collision.radius = 4.0f;
		g_PlayerBullet[i].angle = 0;
		g_EnemyBullet[i].isUse = false;
		g_EnemyBullet[i].collision.radius = 10.0f;
		g_EnemyBullet[i].angle = 0;
	}
}

//=====================================================
//�I������
//=====================================================
void Bullet_Uninit(void)
{

}

//=====================================================
//�X�V����
//=====================================================
void Bullet_Update(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//�v���C���[�̒e���g�p����Ă���Ώ���
		if (g_PlayerBullet[i].isUse)
		{
			//�e�̍��W�̍X�V							
			if (g_PlayerBullet[i].angle == 90)		//90�x
			{
				g_PlayerBullet[i].Pos.y -= BULLET_SPD;
			}
			else
			{
				g_PlayerBullet[i].Pos.x -= (FLOAT)(fcos[g_PlayerBullet[i].angle % 360] * BULLET_SPD);
				g_PlayerBullet[i].Pos.y -= (FLOAT)(fsin[g_PlayerBullet[i].angle % 360] * BULLET_SPD);
			}
			Efect_Create(g_PlayerBullet[i].Pos.x, g_PlayerBullet[i].Pos.y, D3DXCOLOR(1, 1, 1, 0), 5, EFFECT_BULLET);
			//�����蔻��̍��W�̍X�V
			g_PlayerBullet[i].collision.position.x = g_PlayerBullet[i].Pos.x + Texture_GetWidth(g_PlayerBullet[0].Tex) / 2;
			g_PlayerBullet[i].collision.position.y = g_PlayerBullet[i].Pos.y + Texture_GetHeight(g_PlayerBullet[0].Tex) / 2;

			//�`���[�g���A�����̉�ʊO����
			if (Game_GetScene() == SCENE_TUTORIAL && g_PlayerBullet[i].Pos.y < 250.0f)
			{
				g_PlayerBullet[i].isUse = false;
			}
			//��ʊO����
			else
			{
				//��
				if (g_PlayerBullet[i].Pos.y < -20)
				{
					g_PlayerBullet[i].isUse = false;
				}
				//�E
				if (g_PlayerBullet[i].Pos.x > BG_START + BG_WIDTH)
				{
					g_PlayerBullet[i].isUse = false;
				}
				//��
				else if (g_PlayerBullet[i].Pos.x < BG_START - Texture_GetWidth(g_PlayerBullet[0].Tex))
				{
					g_PlayerBullet[i].isUse = false;
				}
			}
		}
		//�G�̒e���g�p����Ă���Ώ���
		if (g_EnemyBullet[i].isUse)
		{
			//�e�̍��W�̍X�V
			g_EnemyBullet[i].Pos.x += (FLOAT)(fcos[g_EnemyBullet[i].angle] * BULLET_SPD / 2);
			g_EnemyBullet[i].Pos.y -= (FLOAT)(fsin[g_EnemyBullet[i].angle] * BULLET_SPD / 2);

			//�����蔻��̍��W�̍X�V
			g_EnemyBullet[i].collision.position.x = g_EnemyBullet[i].Pos.x + Texture_GetWidth(g_EnemyBullet[0].Tex) / 3 / 3 + 10.0f;
			g_EnemyBullet[i].collision.position.y = g_EnemyBullet[i].Pos.y + Texture_GetHeight(g_EnemyBullet[0].Tex) / 2 / 2 / 3;

			//��ʊO����
			if (g_EnemyBullet[i].Pos.y > BG_HEIGHT)
			{
				g_EnemyBullet[i].isUse = false;
			}
			//��
			else if (g_EnemyBullet[i].Pos.x < BG_START - (Texture_GetWidth(g_EnemyBullet[0].Tex) / 3 / 2 + 10))
			{
				g_EnemyBullet[i].isUse = false;
			}
			//�E
			else if (g_EnemyBullet[i].Pos.x > BG_WIDTH + Texture_GetWidth(g_EnemyBullet[0].Tex))
			{
				g_EnemyBullet[i].isUse = false;
			}
		}
	}
}

//=====================================================
//�`�揈��
//=====================================================
void Bullet_Draw(void)
{
	int eWidth = Texture_GetWidth(g_EnemyBullet[0].Tex);
	int eHeight = Texture_GetHeight(g_EnemyBullet[0].Tex);

	for (int i = 0; i < BULLET_MAX; i++)
	{
		//�v���C���[�̒e���g�p����Ă���Ε`��
		if (g_PlayerBullet[i].isUse)
		{
			//�ʏ�e
			if (g_PlayerBullet[i].type != BULLET_HOMING)
			{
				//�E�΂߂ɌX�����e
				if (g_PlayerBullet[i].angle == BULLET_ANGLE_LEFT)
				{
					Sprite_Draw(g_PlayerBullet[0].Tex, g_PlayerBullet[i].Pos.x, g_PlayerBullet[i].Pos.y, 0, 0, Texture_GetWidth(g_PlayerBullet[0].Tex), Texture_GetHeight(g_PlayerBullet[0].Tex), BULLET_ANGLE_RIGHT, (float)(Texture_GetWidth(g_PlayerBullet[0].Tex) / 2), (float)(Texture_GetHeight(g_PlayerBullet[0].Tex) / 2), 0);
				}
				else if (g_PlayerBullet[i].angle == 90)
				{
					Sprite_Draw(g_PlayerBullet[0].Tex, g_PlayerBullet[i].Pos.x, g_PlayerBullet[i].Pos.y);
				}
				//���΂߂ɌX�����e
				else if (g_PlayerBullet[i].angle == BULLET_ANGLE_RIGHT)
				{
					Sprite_Draw(g_PlayerBullet[0].Tex, g_PlayerBullet[i].Pos.x, g_PlayerBullet[i].Pos.y, 0, 0, Texture_GetWidth(g_PlayerBullet[0].Tex), Texture_GetHeight(g_PlayerBullet[0].Tex), -BULLET_ANGLE_RIGHT, (float)(Texture_GetWidth(g_PlayerBullet[0].Tex) / 2), (float)(Texture_GetHeight(g_PlayerBullet[0].Tex) / 2), 0);
				}
			}
			//�ǔ��e
			else
			{
				Sprite_Draw(g_EnemyBullet[0].Tex, g_PlayerBullet[i].Pos.x, g_PlayerBullet[i].Pos.y, eWidth / 2, eHeight / 1.5f, 16.0f, 16.0f, 0, 0, 0, 1.0f);
			}
		}
		//�G�l�~�[�̒e���g�p����Ă���Ε`��
		if (g_EnemyBullet[i].isUse)
		{
			Sprite_Draw(g_EnemyBullet[0].Tex, g_EnemyBullet[i].Pos.x, g_EnemyBullet[i].Pos.y, 0, eHeight / 4 - 10, eWidth / 3 / 3, eHeight / 2 / 2 / 3, 0, 0, 0, 1.0f);
		}
	}
}

//=====================================================
//�e�쐬����
//=====================================================
void Bullet_Create(BULLET *bullet, float x, float y)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//�e���g�p����Ă��Ȃ�������ݒ�
		if (!(bullet + i)->isUse)
		{
			(bullet + i)->isUse = true;
			(bullet + i)->Pos = D3DXVECTOR2(x, y);
			(bullet + i)->collision.position = D3DXVECTOR2(x, y);
			break;
		}
	}
}

//=====================================================
//�e�쐬����(�^�C�v�w��)
//=====================================================
void Bullet_Create(BULLET *bullet, float x, float y, BULLET_TYPE type)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//�e���g�p����Ă��Ȃ�������ݒ�
		if (!(bullet + i)->isUse)
		{
			(bullet + i)->isUse = true;
			(bullet + i)->Pos = D3DXVECTOR2(x, y);
			(bullet + i)->collision.position = D3DXVECTOR2(x, y);
			(bullet + i)->type = type;
			break;
		}
	}
}

//=====================================================
//�e�쐬����(�p�x�w��)
//=====================================================
void Bullet_Create(BULLET *bullet, float x, float y, int angle)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//�e���g�p����Ă��Ȃ�������ݒ�
		if (!(bullet + i)->isUse)
		{
			(bullet + i)->isUse = true;
			(bullet + i)->Pos = D3DXVECTOR2(x, y);
			(bullet + i)->collision.position = D3DXVECTOR2(x, y);
			(bullet + i)->angle = angle;
			break;
		}
	}
}

//=====================================================
//�e�쐬����(�p�x�w��)(�^�C�v�w��)
//=====================================================
void Bullet_Create(BULLET *bullet, float x, float y, int angle, BULLET_TYPE type)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//�e���g�p����Ă��Ȃ�������ݒ�
		if (!(bullet + i)->isUse)
		{
			(bullet + i)->isUse = true;
			(bullet + i)->Pos = D3DXVECTOR2(x, y);
			(bullet + i)->collision.position = D3DXVECTOR2(x, y);
			(bullet + i)->angle = angle;
			(bullet + i)->type = type;
			break;
		}
	}
}

//=====================================================
//�e�̃t���O�擾
//=====================================================
bool Bullet_isUsed(int index, const char *obj)
{
	return (strcmp(obj, "Player")) ? g_EnemyBullet[index].isUse : g_PlayerBullet[index].isUse;
}

//=====================================================
//�e�̃t���O�폜
//=====================================================
void Bullet_Destroy(int index, const char *obj)
{
	(strcmp(obj, "Player")) ? g_EnemyBullet[index].isUse = false : g_PlayerBullet[index].isUse = false;
}

//=====================================================
//�e�̍\���̎擾
//=====================================================
CIRCLE *Bullet_GetCollision(int index, const char *obj)
{
	return (strcmp(obj, "Player")) ? &g_EnemyBullet[index].collision : &g_PlayerBullet[index].collision;
}

//=====================================================
//�e�̍\���̎擾
//=====================================================
BULLET *Bullet_GetObj(int index, const char *obj)
{
	return (strcmp(obj, "Player")) ? &g_EnemyBullet[index] : &g_PlayerBullet[index];
}

//=====================================================
//�G�ƃv���C���[�̊Ԃ̊p�x�����߂�
//=====================================================
int Bullet_GetAngle(float px, float py, float ex, float ey)
{
	return (int)(atan2(-double(py - ey), double(px - ex)) / D3DX_PI * 180 + 360) % 360;
}

//=====================================================
//�e�̎�ޕʂŔ���
//=====================================================
void Bullet_TypeSelect(float px, float py, float ex, float ey, BULLET_TYPE type)
{
	switch (type)
	{
	case BULLET_HOMING:	//�ǔ��e
		Bullet_Create(g_EnemyBullet, ex, ey, Bullet_GetAngle(px, py, ex, ey));
		break;
	case BULLET_FAN:	//��`�e
		for (int i = 0; i < 6; i++)
		{
			Bullet_Create(g_EnemyBullet, ex, ey, 180 + i * 30);
		}
		break;
	case BULLET_NORMAL:	//������e
		Bullet_Create(g_EnemyBullet, ex, ey);
		break;
	default:
		break;
	}
}