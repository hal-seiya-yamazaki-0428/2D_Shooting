#include "collision.h"
#include "bullet.h"
#include "enemy.h"
#include "collision.h"
#include "player.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "debug.h"
#include "player.h"
#include "barrier.h"
#include "barierexplosion.h"
#include "medium_boss.h"
#include "boss.h"
#include "item.h"
#include "med_bullet.h"
#include "boss_bullet.h"
#include "hibana.h"

//=====================================================
//�v���g�^�C�v�錾
//=====================================================
static void hitCheck_pBullet_Enemy(void);
static void hitCheck_Player_eBullet(void);
static void hitCheck_pBullet_MedBoss(void);
static void hitCheck_pBullet_Boss(void);
static void hitCheck_Player_Item(void);
static void hitCheck_Player_MedBullet(void);
static void hitCheck_Player_BossBullet(void);

//=====================================================
//�X�V����
//=====================================================
void Judgement_Update(void)
{
	hitCheck_pBullet_Enemy();
	hitCheck_Player_eBullet();
	hitCheck_pBullet_MedBoss();
	hitCheck_pBullet_Boss();
	hitCheck_Player_Item();
	hitCheck_Player_MedBullet();
	hitCheck_Player_BossBullet();
}

//=====================================================
//�v���C���[�̒e�ƓG�̓����蔻��
//=====================================================
void hitCheck_pBullet_Enemy(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		for (int j = 0; j < ENEMY_MAX; j++)
		{
			//�v���C���[�̒e�ƓG���g�p����Ă��邩
			if (Bullet_isUsed(i, "Player") && Enemy_isUsed(j))
			{
				CIRCLE *pEnemy = Enemy_GetCollision(j);
				if (isCircle(Bullet_GetCollision(i, "Player"), Enemy_GetCollision(j)))
				{
					Bullet_Destroy(i, "Player");	//�e�̍폜
					Enemy_Damage(j, 1);
					PlaySound(SOUND_LABEL_SE_BUL_COL);
					//�G�̗̑͂��O�ȉ��ō폜
					if (Enemy_GetHP(j) < 1)
					{
						Enemy_Destroy(j);		//�G�̍폜
						Explosion_Create(pEnemy->position.x - 20.0f, pEnemy->position.y - 20.0f);	//�����A�j���[�V����
						Score_Add(10);			//�X�R�A���Z
						Item_EnemyCountUp();	//�G���j���J�E���g
						Item_Create(pEnemy->position.x, pEnemy->position.y);
					}
					else
					{	//�Ήԕ`��
						Hibana_Create(pEnemy->position.x - 20.0f, pEnemy->position.y - 20.0f);
					}
				}
			}
		}
	}
}

//=====================================================
//�v���C���[�ƓG�̒e�̓����蔻��
//=====================================================
void hitCheck_Player_eBullet(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//�v���C���[�ƓG�̒e���g�p����Ă��邩
		if (Player_isUsed() && Bullet_isUsed(i, "Enemy"))
		{
			if (isCircle(Player_GetCollision(), Bullet_GetCollision(i, "Enemy")))
			{
				BULLET *pBullet = Bullet_GetObj(i, "Enemy");
				if (!Barrier_isUsed())
				{
					//�S�[�����ĂȂ���Δ�e����
					if (UI_GetCond() <= 490)
					{
						Player_Damage(GAUGE_HP, 5);
					}
					Explosion_Create(pBullet->Pos.x, pBullet->Pos.y);	//�����A�j���[�V����
				}
				else
				{
					BarExp_Create(pBullet->Pos.x, pBullet->Pos.y);
				}
				Bullet_Destroy(i, "Enemy");	//�G�̍폜
			}
		}
	}
}

//=====================================================
//�v���C���[�̒e�ƒ��{�X�̓����蔻��
//=====================================================
void hitCheck_pBullet_MedBoss(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//�v���C���[�̒e�ƓG���g�p����Ă��邩
		if (Bullet_isUsed(i, "Player") && Medium_Boss_isUsed())
		{
			if (isCircle(Bullet_GetCollision(i, "Player"), Medium_Boss_GetCollision()))
			{
				CIRCLE *pMedBoss = Medium_Boss_GetCollision();
				Hibana_Create(pMedBoss->position.x - 20.0f, pMedBoss->position.y - 20.0f);
				Bullet_Destroy(i, "Player");	//�e�̍폜
				Medium_Boss_Damage(1);
				PlaySound(SOUND_LABEL_SE_BUL_COL);
			}
		}
	}
}

//=====================================================
//�v���C���[�̒e�ƃ{�X�̓����蔻��
//=====================================================
void hitCheck_pBullet_Boss(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//�v���C���[�̒e�ƓG���g�p����Ă��邩
		if (Bullet_isUsed(i, "Player") && Boss_isUsed())
		{
			if (isCircle(Bullet_GetCollision(i, "Player"), Boss_GetCollision()))
			{
				CIRCLE *pBoss = Boss_GetCollision();
				Hibana_Create(pBoss->position.x - 20.0f, pBoss->position.y - 20.0f);
				Bullet_Destroy(i, "Player");	//�e�̍폜
				Boss_Damage(1);
				PlaySound(SOUND_LABEL_SE_BUL_COL);
			}
		}
	}
}

//=====================================================
//�v���C���[�ƃA�C�e���̓����蔻��
//=====================================================
void hitCheck_Player_Item(void)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		//�v���C���[�ƃA�C�e�����g�p����Ă��邩�ǂ���
		if (Player_isUsed() && Item_isUse(i))
		{
			//�v���C���[�ƃA�C�e���������������ǂ���
			if (isCircle(Player_GetCollision(), Item_GetCollision(i)))
			{
				PLAYER *pPlayer = Player_GetObj();
				Item_Destroy(i);
				//�G�l���M�[��
				if (item_GetType(i) == ITEM_TYPE_ENE)
				{
					Player_Damage(GAUGE_EN, -50);
					PlaySound(SOUND_LABEL_SE_ITEM);
					//�ő�l�������Ȃ��悤��
					if (pPlayer->EN > pPlayer->MaxEN)
					{
						pPlayer->EN = pPlayer->MaxEN;
					}
				}
				//���x���A�b�v
				else if (item_GetType(i) == ITEM_TYPE_POWER_UP && pPlayer->shotLevel < MAX_LEVEL)
				{
					pPlayer->shotExp += 20;
					//���x���A�b�v����
					if (pPlayer->shotExp >= 100 && pPlayer->shotLevel < MAX_LEVEL)
					{
						pPlayer->shotLevel++;
						PlaySound(SOUND_LABEL_SE_LEVEL_UP);
						pPlayer->LevelUp = true;
						//���x���A�b�v������o���l�����Z�b�g
						if (pPlayer->shotLevel != MAX_LEVEL)
						{
							pPlayer->shotExp = 0;
						}
					}
					else
					{
						PlaySound(SOUND_LABEL_SE_POWER_UP);
					}
				}
			}
		}
	}
}

//=====================================================
//�v���C���[�ƒ��{�X�̒e�̓����蔻��
//=====================================================
void hitCheck_Player_MedBullet(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//�v���C���[�ƒ��{�X�̒e���g�p�����ǂ���
		if (Player_isUsed() && Med_Bullet_isUsed(i))
		{
			MED_BULLET *pMedBullet = Med_Bullet_GetObj(i);
			//�����������ǂ���
			if (isCircle(Player_GetCollision(), Med_Bullet_GetCollision(i)))
			{
				if (!Barrier_isUsed())
				{
					Player_Damage(GAUGE_HP, 5);
					Explosion_Create(pMedBullet->bullet.Pos.x, pMedBullet->bullet.Pos.y);	//�����A�j���[�V����
				}
				else
				{
					BarExp_Create(pMedBullet->bullet.Pos.x, pMedBullet->bullet.Pos.y);
				}
				Med_Bullet_Destroy(i);	//�G�̍폜
			}
		}
	}
}

//=====================================================
//�v���C���[�ƃ{�X�̒e�̓����蔻��
//=====================================================
void hitCheck_Player_BossBullet(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//�v���C���[�ƒ��{�X�̒e���g�p�����ǂ���
		if (Player_isUsed() && Boss_Bullet_isUsed(i))
		{
			BOSS_BULLET *pBossBullet = Boss_Bullet_GetObj(i);
			//�����������ǂ���
			if (isCircle(Player_GetCollision(), Boss_Bullet_GetCollision(i)))
			{
				if (!Barrier_isUsed())
				{
					Player_Damage(GAUGE_HP, 5);
					Explosion_Create(pBossBullet->bullet.Pos.x, pBossBullet->bullet.Pos.y);	//�����A�j���[�V����
				}
				else
				{
					BarExp_Create(pBossBullet->bullet.Pos.x, pBossBullet->bullet.Pos.y);
				}
				Boss_Bullet_Destroy(i);	//�G�̍폜
			}
		}
	}
}