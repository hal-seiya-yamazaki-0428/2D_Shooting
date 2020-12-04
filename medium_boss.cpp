#include "medium_boss.h"
#include "texture.h"
#include "sprite.h"
#include "bg.h" 
#include "debug_font.h" 
#include "debug.h"
#include "med_bullet.h"
#include "player.h"
#include "explosion.h"
#include "score.h"
#include "item.h"

//=====================================================
//�}�N����`
//=====================================================
#define MEDIUM_BOSS_NUM (32)	
#define MEDIUM_BOSS_HP	(50)
#define EXPLOSION_COUNT (10)
#define BULLET_FAN_NUM (8)	//���̒e���������˂��邩 

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static MEDIUM_BOSS g_MedBoss;
static float fsin[360], fcos[360];
static int g_angle = 2;
static int bulletFrame = 0;
static int g_ExpCount = 0;
static int g_ExpPos[EXPLOSION_COUNT * 2] =
{
	20, 60, 40, 10, 100, 30, 100, 18, 42, 120,	//x���W
	20, 50, 100, 10, 80, 20, 64, 114, 46, 90	//y���W
};
static int g_frame = 0;
static int g_ExpFrame = 0;
static MEDIUM_BOSS_TYPE g_NextType = MEDIUM_BOSS_STOP;
static int g_SpiralIndex;
static int g_SpiralFrame = 0;

//=====================================================
//������
//=====================================================
void Medium_Boss_Init(void)
{
	for (int i = 0; i < 360; i++)
	{
		fsin[i] = (float)sin(i * D3DX_PI / 180);
		fcos[i] = (float)cos(i * D3DX_PI / 180);
	}
	g_MedBoss.enemy.Tex = Texture_SetLoadFile("Texture\\���{�X.png", 256 * 3, 192 * 3);
	g_MedBoss.enemy.Pos = D3DXVECTOR2(0, 0);
	g_MedBoss.enemy.collision.position = D3DXVECTOR2(0, 0);
	g_MedBoss.enemy.collision.radius = 96.0f;
	g_MedBoss.enemy.angle = 90;
	g_MedBoss.enemy.isUse = false;
	g_MedBoss.enemy.frame = 0;
	g_MedBoss.enemy.hp = g_MedBoss.enemy.MaxHP = MEDIUM_BOSS_HP;
	g_MedBoss.move = 3.0f;
	g_MedBoss.type = MEDIUM_BOSS_FIRST_IN;
	g_MedBoss.bullet.type = MED_BULLET_NORMAL;
	g_MedBoss.shotFrame = 0;
	bulletFrame = 0;
	g_ExpCount = 0;
	g_frame = 0;
	g_ExpFrame = 0;
	g_SpiralIndex = 0;
	g_SpiralFrame = 0;
	g_angle = 2;
	g_NextType = MEDIUM_BOSS_STOP;
}

//=====================================================
//�I��
//=====================================================
void Medium_Boss_Uninit(void)
{

}

//=====================================================
//�X�V
//=====================================================
void Medium_Boss_Update(void)
{
	int width = Texture_GetWidth(g_MedBoss.enemy.Tex);
	int height = Texture_GetHeight(g_MedBoss.enemy.Tex);
	PLAYER *pPlayer = Player_GetObj();

	//�{�X���g�p����Ă��Ȃ���ΏI��
	if (!g_MedBoss.enemy.isUse)
	{
		return;
	}
	//���S��
	if (g_MedBoss.enemy.hp < 0)
	{
		if (g_ExpFrame % 20 == 0)
		{
			Explosion_Create(g_MedBoss.enemy.Pos.x + g_ExpPos[g_ExpCount], g_MedBoss.enemy.Pos.y + g_ExpPos[g_ExpCount + 10]);
			g_ExpCount++;
		}
		if (g_ExpCount > EXPLOSION_COUNT)
		{
			g_MedBoss.enemy.isUse = false;
			Score_Add(1000);
			Item_Create(g_MedBoss.enemy.Pos.x, g_MedBoss.enemy.Pos.y, ITEM_TYPE_POWER_UP);
		}
		g_ExpFrame++;
		return;
	}

	//��ʓ�����
	switch (g_MedBoss.type)
	{
		//=====================
		//��ʊO����̓���
		//=====================
	case MEDIUM_BOSS_FIRST_IN:
		//��ʊO����̓o��
		(g_MedBoss.enemy.Pos.y < 0) ? g_MedBoss.enemy.Pos.y += 2 : g_MedBoss.type = g_NextType;
		break;
		//=====================
		//�����ړ�
		//=====================
	case MEDIUM_BOSS_HORIZONTAL:
		g_MedBoss.enemy.Pos.x += g_MedBoss.move;
		if (g_MedBoss.enemy.Pos.y < 0)
		{
			g_MedBoss.enemy.Pos.y += 2;
		}
		//�e�̔���
		if (g_MedBoss.enemy.frame % 14 == 0)
		{
			//�ǔ��e
			Med_Bullet_Create(MED_BULLET_HOMING, pPlayer->pos.x, pPlayer->pos.y, g_MedBoss.enemy.Pos.x + width / 3 / 2 - 25, g_MedBoss.enemy.Pos.y + height / 3 / 2);
			//��萔�������玟�̍s��
			if (g_MedBoss.shotFrame > 10)
			{
				g_MedBoss.type = MEDIUM_BOSS_FIRST_IN;
				g_NextType = MEDIUM_BOSS_WAVE;
				g_MedBoss.shotFrame = 0;
			}
			g_MedBoss.shotFrame++;
		}
		break;
		//=====================
		//�g��ɉ����ē���
		//=====================
	case MEDIUM_BOSS_WAVE:
		//������E������
		if (g_MedBoss.enemy.Pos.x < BG_START)
		{
			g_MedBoss.right = true;
		}
		//�E���獶������
		else if (g_MedBoss.enemy.Pos.x > BG_START + BG_WIDTH - width / 3)
		{
			g_MedBoss.right = false;
		}
		//�E����
		if (g_MedBoss.right)
		{
			g_MedBoss.enemy.Pos.x += 2;
		}
		//������
		else
		{
			g_MedBoss.enemy.Pos.x -= 2;
		}
		g_MedBoss.enemy.Pos.y += (FLOAT)(fcos[g_MedBoss.enemy.angle % 360] * 2);
		g_MedBoss.enemy.angle += 2;

		//�e�̔���
		if (g_MedBoss.enemy.frame % 20 == 0)
		{
			//���������e�̗v�f���擾
			g_SpiralIndex = Med_Bullet_Create_Return(MED_BULLET_NORMAL, g_MedBoss.enemy.Pos.x + width / 3 / 2 - 25, g_MedBoss.enemy.Pos.y + height / 3 / 2, 270);
			//���t���[����������
			if (g_SpiralFrame > 2)
			{
				MED_BULLET *pMedBullet = Med_Bullet_GetObj(g_SpiralIndex);
				for (int i = 0; i < 12; i++)
				{
					Med_Bullet_Create(MED_BULLET_SPIRAL, pMedBullet->bullet.Pos.x, pMedBullet->bullet.Pos.y, 30 * i);
				}
				g_SpiralFrame = 0;
			}
			//��萔�������玟�̍s��
			if (g_MedBoss.shotFrame > 30)
			{
				g_MedBoss.type = MEDIUM_BOSS_FIRST_IN;
				g_NextType = MEDIUM_BOSS_STOP;
				g_MedBoss.shotFrame = 0;
			}
			g_MedBoss.shotFrame++;
			g_SpiralFrame++;
		}
		break;
		//=====================
		//��~
		//=====================
	case MEDIUM_BOSS_STOP:
		//��ʒ����łȂ���Β����܂ňړ�	//440, 540
		if ((g_MedBoss.enemy.Pos.x < 520 - width / 3 / 2 || g_MedBoss.enemy.Pos.x > 540 - width / 3 / 2) || g_MedBoss.enemy.Pos.y > -100)
		{
			//�X�e�[�W�����܂ł̊p�x���v�Z���Ē����Ɍ�����
			int angle = Bullet_GetAngle(530.0f - width / 3 / 2, -100.0f, g_MedBoss.enemy.Pos.x, g_MedBoss.enemy.Pos.y) % 360;
			g_MedBoss.enemy.Pos.x += (FLOAT)(fcos[angle] * 2);
			g_MedBoss.enemy.Pos.y -= (FLOAT)(fsin[angle] * 4);
		}
		//��~���ł���Βe�̔���
		else if (g_MedBoss.enemy.frame % 12 == 0)
		{
			//��
			for (int angle = 0; angle < BULLET_FAN_NUM; angle++)
			{
				Med_Bullet_Create(MED_BULLET_FAN, g_MedBoss.enemy.Pos.x + width / 3 / 2 - 25, g_MedBoss.enemy.Pos.y + height / 3 / 2,
					((180 + g_angle) + angle * (180 / BULLET_FAN_NUM)) % 180 + 180);
			}
			//��萔�������玟�̍s��
			if (g_MedBoss.shotFrame > 10)
			{
				g_MedBoss.type = MEDIUM_BOSS_FIRST_IN;
				g_NextType = MEDIUM_BOSS_HORIZONTAL;
				g_MedBoss.shotFrame = 0;
			}
			g_MedBoss.shotFrame++;
		}
		break;
	default:
		break;
	}

	//�����ؑ�
	if (g_MedBoss.enemy.Pos.x < BG_START || g_MedBoss.enemy.Pos.x > BG_START + BG_WIDTH - width / 3)
	{
		g_MedBoss.move *= -1;
	}
	g_angle++;
	g_angle %= 180;
	g_MedBoss.enemy.frame++;

	//�����蔻��̍X�V
	g_MedBoss.enemy.collision.position.x = g_MedBoss.enemy.Pos.x + width / 3 / 2;
	g_MedBoss.enemy.collision.position.y = g_MedBoss.enemy.Pos.y + height / 2 / 2;
}

//=====================================================
//�`��
//=====================================================
void Medium_Boss_Draw(void)
{
	if (g_MedBoss.enemy.isUse)
	{
		int width = Texture_GetWidth(g_MedBoss.enemy.Tex);
		int height = Texture_GetHeight(g_MedBoss.enemy.Tex);
		Sprite_Draw(g_MedBoss.enemy.Tex, g_MedBoss.enemy.Pos.x, g_MedBoss.enemy.Pos.y, width * 3 / 4, 0, width / 4, height / 3 - 2);
	}
}

//=====================================================
//����
//=====================================================
void Medium_Boss_Create(float x, float y, int  hp)
{
	if (!g_MedBoss.enemy.isUse)
	{
		g_MedBoss.enemy.isUse = true;
		g_MedBoss.enemy.angle = 0;
		g_MedBoss.enemy.collision.position = D3DXVECTOR2(x, y);
		g_MedBoss.enemy.Pos = D3DXVECTOR2(x, y);
		g_MedBoss.enemy.frame = 0;
		g_MedBoss.enemy.hp = hp;
		g_MedBoss.right = true;
		g_MedBoss.bullet.type = MED_BULLET_HOMING;
	}
}

//=====================================================
//�g�p�����ǂ���
//=====================================================
bool Medium_Boss_isUsed(void)
{
	return g_MedBoss.enemy.isUse;
}

//=====================================================
//�폜
//=====================================================
void Medium_Boss_Destroy(void)
{
	g_MedBoss.enemy.isUse = false;
}

//=====================================================
//�����蔻��̍\����
//=====================================================
CIRCLE *Medium_Boss_GetCollision(void)
{
	return &g_MedBoss.enemy.collision;
}

//=====================================================
//�_���[�W�t�^
//=====================================================
void Medium_Boss_Damage(int damage)
{
	g_MedBoss.enemy.hp -= damage;
}

//=====================================================
//HP�擾
//=====================================================
int Medium_Boss_GetHP(void)
{
	return g_MedBoss.enemy.hp;
}