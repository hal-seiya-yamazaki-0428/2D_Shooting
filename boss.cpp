
#include "boss.h"
#include "texture.h"
#include "sprite.h"
#include "bg.h" 
#include "debug_font.h" 
#include "debug.h"
#include "boss_bullet.h"
#include "player.h"
#include "explosion.h"
#include "score.h"

//=====================================================
//�}�N����`
//=====================================================
#define BOSS_NUM (32)	
#define BOSS_HP	(100)
#define EXPLOSION_COUNT (10)
#define BULLET_FAN_NUM (8)	//���̒e���������˂��邩 

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static BOSS g_Boss;
static float fsin[360], fcos[360];
static int bulletFrame = 0;
static int g_ExpCount = 0;
static int g_ExpPos[EXPLOSION_COUNT * 2] =
{
	20, 80, 50, 30, 140, 30, 180, 38, 72, 150,	//x���W
	20, 50, 100, 10, 80, 20, 64, 114, 46, 90	//y���W
};
static int g_ExpFrame = 0;
static int g_angle = 0;
static int g_HomingAngle = 0;
static int g_shotCount = 0;
static int g_generater = 0;
static BOSS_TYPE g_NextType = BOSS_STOP;

//=====================================================
//������
//=====================================================
void Boss_Init(void)
{
	for (int i = 0; i < 360; i++)
	{
		fsin[i] = (float)sin(i * D3DX_PI / 180);
		fcos[i] = (float)cos(i * D3DX_PI / 180);
	}
	g_Boss.enemy.Tex = Texture_SetLoadFile("Texture\\�{�X.png", 288 * 3, 192 * 3);
	g_Boss.enemy.Pos = D3DXVECTOR2(0, 0);
	g_Boss.enemy.collision.position = D3DXVECTOR2(0, 0);
	g_Boss.enemy.collision.radius = 96.0f;
	g_Boss.enemy.angle = 90;
	g_Boss.enemy.isUse = false;
	g_Boss.enemy.frame = 1;
	g_Boss.enemy.hp = g_Boss.enemy.MaxHP = BOSS_HP;
	g_Boss.move = 3.0f;
	g_Boss.type = BOSS_FIRST_IN;
	g_Boss.shotFrame = 0;
	bulletFrame = 0;
	g_ExpCount = 0;
	g_ExpFrame = 0;
	g_angle = 0;
	g_HomingAngle = 0;
	g_shotCount = 0;
	g_generater = 0;
	g_NextType = BOSS_STOP;
}

//=====================================================
//�I��
//=====================================================
void Boss_Uninit(void)
{

}

//=====================================================
//�X�V
//=====================================================
void Boss_Update(void)
{
	int width = Texture_GetWidth(g_Boss.enemy.Tex);
	int height = Texture_GetHeight(g_Boss.enemy.Tex);
	PLAYER *pPlayer = Player_GetObj();

	//�{�X���g�p����Ă��Ȃ���ΏI��
	if (!g_Boss.enemy.isUse)
	{
		return;
	}
	//���S��
	if (g_Boss.enemy.hp < 0)
	{
		if (g_ExpFrame % 20 == 0)
		{
			Explosion_Create(g_Boss.enemy.Pos.x + g_ExpPos[g_ExpCount], g_Boss.enemy.Pos.y + g_ExpPos[g_ExpCount + 10]);
			g_ExpCount++;
		}
		if (g_ExpCount > EXPLOSION_COUNT)
		{
			g_Boss.enemy.isUse = false;
			Score_Add(1000);
		}
		g_ExpFrame++;
		return;
	}

	//��ʓ�����
	switch (g_Boss.type)
	{
		//=====================
		//��ʊO����̓���
		//=====================
	case BOSS_FIRST_IN:
		//��ʊO����̓o��
		(g_Boss.enemy.Pos.y < 0) ? g_Boss.enemy.Pos.y += 2 : g_Boss.type = g_NextType;
		break;
		//=====================
		//�����ړ�
		//=====================
	case BOSS_HORIZONTAL:
		g_Boss.enemy.Pos.x += g_Boss.move;
		if (g_Boss.enemy.Pos.y < 0)
		{
			g_Boss.enemy.Pos.y += 2;
		}
		//�e�̔���
		if (g_Boss.enemy.frame % 16 == 0)
		{
			//�ǔ��e
			for (int angle = 0; angle < 3; angle++)
			{
				Boss_Bullet_Create(BOSS_BULLET_HOMING, pPlayer->pos.x - 150 + angle * 150, pPlayer->pos.y, g_Boss.enemy.Pos.x + width / 3 / 2 - 25, g_Boss.enemy.Pos.y + height / 3 / 2);
			}
			//��萔�������玟�̍s��
			if (g_Boss.shotFrame > 10)
			{
				g_Boss.type = BOSS_FIRST_IN;
				g_NextType = BOSS_WAVE;
				g_Boss.shotFrame = 0;
			}
			g_Boss.shotFrame++;
		}
		break;
		//=====================
		//�g��ɉ����ē���
		//=====================
	case BOSS_WAVE:
		//������E������
		if (g_Boss.enemy.Pos.x < BG_START)
		{
			g_Boss.right = true;
		}
		//�E���獶������
		else if (g_Boss.enemy.Pos.x > BG_START + BG_WIDTH - width / 3)
		{
			g_Boss.right = false;
		}
		//�E����
		if (g_Boss.right)
		{
			g_Boss.enemy.Pos.x += 2;
		}
		//������
		else
		{
			g_Boss.enemy.Pos.x -= 2;
		}
		g_Boss.enemy.Pos.y += (FLOAT)(fcos[g_Boss.enemy.angle % 360] * 2);
		g_Boss.enemy.angle += 2;

		//�e�̔���
		if (g_Boss.enemy.frame % 20 == 0)
		{
			Boss_Bullet_Create(BOSS_BULLET_INVERSION, g_Boss.enemy.Pos.x + width / 3 / 2 - 25, g_Boss.enemy.Pos.y + height / 3 / 2, g_angle + 180);
			//��萔�������玟�̍s��
			if (g_Boss.shotFrame > 30)
			{
				g_Boss.type = BOSS_FIRST_IN;
				g_NextType = BOSS_STOP;
				g_Boss.shotFrame = 0;
			}
			g_Boss.shotFrame++;
		}
		break;
		//=====================
		//��~
		//=====================
	case BOSS_STOP:
		//��ʒ����łȂ���Β����܂ňړ�	//440, 540
		if ((g_Boss.enemy.Pos.x < 520 - width / 3 / 2 || g_Boss.enemy.Pos.x > 540 - width / 3 / 2) || g_Boss.enemy.Pos.y > -100)
		{
			//�X�e�[�W�����܂ł̊p�x���v�Z���Ē����Ɍ�����
			int angle = Bullet_GetAngle(530.0f - width / 3 / 2, -100.0f, g_Boss.enemy.Pos.x, g_Boss.enemy.Pos.y) % 360;
			g_Boss.enemy.Pos.x += (FLOAT)(fcos[angle] * 2);
			g_Boss.enemy.Pos.y -= (FLOAT)(fsin[angle] * 4);
		}
		//��~���ł���Βe�̔���
		else if (g_Boss.enemy.frame % 12 == 0)
		{
			//��
			for (int angle = 0; angle < BULLET_FAN_NUM; angle++)
			{
				Boss_Bullet_Create(BOSS_BULLET_FAN, g_Boss.enemy.Pos.x + width / 3 / 2 - 25, g_Boss.enemy.Pos.y + height / 3 / 2, 
					((180 + g_angle) + angle * (180 / BULLET_FAN_NUM)) % 180 + 180);
			}
			//��萔�������玟�̍s��
			if (g_Boss.shotFrame > 10)
			{
				g_Boss.type = BOSS_FIRST_IN;
				g_NextType = BOSS_HORIZONTAL;
				g_Boss.shotFrame = 0;
			}
			g_Boss.shotFrame++;
		}
		break;
	default:
		break;
	}

	//�����ؑ�
	if (g_Boss.enemy.Pos.x < BG_START || g_Boss.enemy.Pos.x > BG_START + BG_WIDTH - width / 3)
	{
		g_Boss.move *= -1;
	}
	g_angle++;
	g_angle %= 180;
	g_Boss.enemy.frame++;

	//�����蔻��̍X�V
	g_Boss.enemy.collision.position.x = g_Boss.enemy.Pos.x + width / 3 / 2;
	g_Boss.enemy.collision.position.y = g_Boss.enemy.Pos.y + height / 2 / 2;
}

//=====================================================
//�`��
//=====================================================
void Boss_Draw(void)
{
	if (g_Boss.enemy.isUse)
	{
		int width = Texture_GetWidth(g_Boss.enemy.Tex);
		int height = Texture_GetHeight(g_Boss.enemy.Tex);
		Sprite_Draw(g_Boss.enemy.Tex, g_Boss.enemy.Pos.x, g_Boss.enemy.Pos.y, width * 2 / 3, 2, width / 3, height / 2 - 6);
	}
}

//=====================================================
//����
//=====================================================
void Boss_Create(float x, float y, int  hp)
{
	if (!g_Boss.enemy.isUse)
	{
		g_Boss.enemy.isUse = true;
		g_Boss.enemy.angle = 0;
		g_Boss.enemy.collision.position = D3DXVECTOR2(x, y);
		g_Boss.enemy.Pos = D3DXVECTOR2(x, y);
		g_Boss.enemy.frame = 1;
		g_Boss.enemy.hp = hp;
		g_Boss.right = true;
	}
}

//=====================================================
//�g�p�����ǂ���
//=====================================================
bool Boss_isUsed(void)
{
	return g_Boss.enemy.isUse;
}

//=====================================================
//�폜
//=====================================================
void Boss_Destroy(void)
{
	g_Boss.enemy.isUse = false;
}

//=====================================================
//�����蔻��̍\����
//=====================================================
CIRCLE *Boss_GetCollision(void)
{
	return &g_Boss.enemy.collision;
}

//=====================================================
//�{�X�̍\����
//=====================================================
BOSS *Boss_GetObj(void)
{
	return &g_Boss;
}

//=====================================================
//�_���[�W�t�^
//=====================================================
void Boss_Damage(int damage)
{
	g_Boss.enemy.hp -= damage;
}

//=====================================================
//HP�擾
//=====================================================
int Boss_GetHP(void)
{
	return g_Boss.enemy.hp;
}

//=====================================================
//���˂����e�̐��擾
//=====================================================
int Boss_GetShotCount(void)
{
	return g_shotCount;
}

//	//=====================
//	//U�̎��ړ�
//	//=====================
//case BOSS_UMOVE:
//	g_Boss.enemy.Pos.x -= (FLOAT)(fcos[g_Boss.enemy.angle % 360] * 3);
//	g_Boss.enemy.Pos.y += (FLOAT)(fsin[g_Boss.enemy.angle % 360] * 6);
//	g_Boss.enemy.angle += 2;
//	//�e�̔���
//	if (g_Boss.enemy.frame % g_Boss.shotInter == 0)
//	{
//	}
//	break;