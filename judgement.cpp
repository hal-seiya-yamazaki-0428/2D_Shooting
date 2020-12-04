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
//プロトタイプ宣言
//=====================================================
static void hitCheck_pBullet_Enemy(void);
static void hitCheck_Player_eBullet(void);
static void hitCheck_pBullet_MedBoss(void);
static void hitCheck_pBullet_Boss(void);
static void hitCheck_Player_Item(void);
static void hitCheck_Player_MedBullet(void);
static void hitCheck_Player_BossBullet(void);

//=====================================================
//更新処理
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
//プレイヤーの弾と敵の当たり判定
//=====================================================
void hitCheck_pBullet_Enemy(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		for (int j = 0; j < ENEMY_MAX; j++)
		{
			//プレイヤーの弾と敵が使用されているか
			if (Bullet_isUsed(i, "Player") && Enemy_isUsed(j))
			{
				CIRCLE *pEnemy = Enemy_GetCollision(j);
				if (isCircle(Bullet_GetCollision(i, "Player"), Enemy_GetCollision(j)))
				{
					Bullet_Destroy(i, "Player");	//弾の削除
					Enemy_Damage(j, 1);
					PlaySound(SOUND_LABEL_SE_BUL_COL);
					//敵の体力が０以下で削除
					if (Enemy_GetHP(j) < 1)
					{
						Enemy_Destroy(j);		//敵の削除
						Explosion_Create(pEnemy->position.x - 20.0f, pEnemy->position.y - 20.0f);	//爆発アニメーション
						Score_Add(10);			//スコア加算
						Item_EnemyCountUp();	//敵撃破数カウント
						Item_Create(pEnemy->position.x, pEnemy->position.y);
					}
					else
					{	//火花描画
						Hibana_Create(pEnemy->position.x - 20.0f, pEnemy->position.y - 20.0f);
					}
				}
			}
		}
	}
}

//=====================================================
//プレイヤーと敵の弾の当たり判定
//=====================================================
void hitCheck_Player_eBullet(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//プレイヤーと敵の弾が使用されているか
		if (Player_isUsed() && Bullet_isUsed(i, "Enemy"))
		{
			if (isCircle(Player_GetCollision(), Bullet_GetCollision(i, "Enemy")))
			{
				BULLET *pBullet = Bullet_GetObj(i, "Enemy");
				if (!Barrier_isUsed())
				{
					//ゴールしてなければ被弾する
					if (UI_GetCond() <= 490)
					{
						Player_Damage(GAUGE_HP, 5);
					}
					Explosion_Create(pBullet->Pos.x, pBullet->Pos.y);	//爆発アニメーション
				}
				else
				{
					BarExp_Create(pBullet->Pos.x, pBullet->Pos.y);
				}
				Bullet_Destroy(i, "Enemy");	//敵の削除
			}
		}
	}
}

//=====================================================
//プレイヤーの弾と中ボスの当たり判定
//=====================================================
void hitCheck_pBullet_MedBoss(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//プレイヤーの弾と敵が使用されているか
		if (Bullet_isUsed(i, "Player") && Medium_Boss_isUsed())
		{
			if (isCircle(Bullet_GetCollision(i, "Player"), Medium_Boss_GetCollision()))
			{
				CIRCLE *pMedBoss = Medium_Boss_GetCollision();
				Hibana_Create(pMedBoss->position.x - 20.0f, pMedBoss->position.y - 20.0f);
				Bullet_Destroy(i, "Player");	//弾の削除
				Medium_Boss_Damage(1);
				PlaySound(SOUND_LABEL_SE_BUL_COL);
			}
		}
	}
}

//=====================================================
//プレイヤーの弾とボスの当たり判定
//=====================================================
void hitCheck_pBullet_Boss(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//プレイヤーの弾と敵が使用されているか
		if (Bullet_isUsed(i, "Player") && Boss_isUsed())
		{
			if (isCircle(Bullet_GetCollision(i, "Player"), Boss_GetCollision()))
			{
				CIRCLE *pBoss = Boss_GetCollision();
				Hibana_Create(pBoss->position.x - 20.0f, pBoss->position.y - 20.0f);
				Bullet_Destroy(i, "Player");	//弾の削除
				Boss_Damage(1);
				PlaySound(SOUND_LABEL_SE_BUL_COL);
			}
		}
	}
}

//=====================================================
//プレイヤーとアイテムの当たり判定
//=====================================================
void hitCheck_Player_Item(void)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		//プレイヤーとアイテムが使用されているかどうか
		if (Player_isUsed() && Item_isUse(i))
		{
			//プレイヤーとアイテムが当たったかどうか
			if (isCircle(Player_GetCollision(), Item_GetCollision(i)))
			{
				PLAYER *pPlayer = Player_GetObj();
				Item_Destroy(i);
				//エネルギー回復
				if (item_GetType(i) == ITEM_TYPE_ENE)
				{
					Player_Damage(GAUGE_EN, -50);
					PlaySound(SOUND_LABEL_SE_ITEM);
					//最大値が超えないように
					if (pPlayer->EN > pPlayer->MaxEN)
					{
						pPlayer->EN = pPlayer->MaxEN;
					}
				}
				//レベルアップ
				else if (item_GetType(i) == ITEM_TYPE_POWER_UP && pPlayer->shotLevel < MAX_LEVEL)
				{
					pPlayer->shotExp += 20;
					//レベルアップ処理
					if (pPlayer->shotExp >= 100 && pPlayer->shotLevel < MAX_LEVEL)
					{
						pPlayer->shotLevel++;
						PlaySound(SOUND_LABEL_SE_LEVEL_UP);
						pPlayer->LevelUp = true;
						//レベルアップしたら経験値をリセット
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
//プレイヤーと中ボスの弾の当たり判定
//=====================================================
void hitCheck_Player_MedBullet(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//プレイヤーと中ボスの弾が使用中かどうか
		if (Player_isUsed() && Med_Bullet_isUsed(i))
		{
			MED_BULLET *pMedBullet = Med_Bullet_GetObj(i);
			//当たったかどうか
			if (isCircle(Player_GetCollision(), Med_Bullet_GetCollision(i)))
			{
				if (!Barrier_isUsed())
				{
					Player_Damage(GAUGE_HP, 5);
					Explosion_Create(pMedBullet->bullet.Pos.x, pMedBullet->bullet.Pos.y);	//爆発アニメーション
				}
				else
				{
					BarExp_Create(pMedBullet->bullet.Pos.x, pMedBullet->bullet.Pos.y);
				}
				Med_Bullet_Destroy(i);	//敵の削除
			}
		}
	}
}

//=====================================================
//プレイヤーとボスの弾の当たり判定
//=====================================================
void hitCheck_Player_BossBullet(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//プレイヤーと中ボスの弾が使用中かどうか
		if (Player_isUsed() && Boss_Bullet_isUsed(i))
		{
			BOSS_BULLET *pBossBullet = Boss_Bullet_GetObj(i);
			//当たったかどうか
			if (isCircle(Player_GetCollision(), Boss_Bullet_GetCollision(i)))
			{
				if (!Barrier_isUsed())
				{
					Player_Damage(GAUGE_HP, 5);
					Explosion_Create(pBossBullet->bullet.Pos.x, pBossBullet->bullet.Pos.y);	//爆発アニメーション
				}
				else
				{
					BarExp_Create(pBossBullet->bullet.Pos.x, pBossBullet->bullet.Pos.y);
				}
				Boss_Bullet_Destroy(i);	//敵の削除
			}
		}
	}
}