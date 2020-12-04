#include "enemy.h"
#include "medium_boss.h"
#include "boss.h"
#include "UI.h"
#include "bg.h"
#include "game.h"
#include "sound.h"
#include "soundgenerater.h"

//=====================================================
//グローバル変数
//=====================================================
static int frameCounter = 100;
static int generateCounter = 0;
static bool g_MedSpawn = false;
static bool g_BossSpawn = false;
static int spawnCount = 50;

//=====================================================
//初期化処理
//=====================================================
void EnemyGenerater_Init(void)
{
	frameCounter = 100;
	generateCounter = 0;
	spawnCount = 50;
	g_MedSpawn = false;
	g_BossSpawn = false;
}

//=====================================================
//終了処理
//=====================================================
void EnemyGenerater_Uninit(void)
{

}

//=====================================================
//更新処理
//=====================================================
void EnemyGenerater_Update(void)
{
	//ゴールしたら生成しない
	if (UI_GetCond() > 490 || Game_GetScene() != SCENE_MAIN && Game_GetScene() != SCENE_TUTORIAL)
	{
		return;
	}
	//一定数フレームごとに出現
	if (frameCounter % spawnCount == 0)
	{
		switch (generateCounter % 3)
		{
		case 0:
			Enemy_Create(ENEMY_TYPE_LEFT, (float)(frameCounter % 500 + BG_START), -64.0f, ENEMY_DEF_HP);
			break;
		case 1:
			Enemy_Create(ENEMY_TYPE_HOMING, (float)((frameCounter * 2 / 5) % 500 + BG_START), -64.0f, ENEMY_DEF_HP);
			break;
		case 2:
			Enemy_Create(ENEMY_TYPE_RIGHT, (float)((frameCounter * 4 / 7) % 500 + BG_START), -64.0f, ENEMY_DEF_HP);
			break;
		default:
			break;
		}
		generateCounter++;
	}

	//中ボス
	if (UI_GetCond() > 250 && !g_MedSpawn)	//250
	{
		Medium_Boss_Create(300.0f, -256.0f, 100);
		g_MedSpawn = true;
		spawnCount = 100;
	}

	//ボス
	else if (UI_GetCond() > 480 && !g_BossSpawn)	//480
	{
		Boss_Create((BG_START + BG_WIDTH) / 2, -256.0f, 200);
		StopSound(SOUND_LABEL_BGM_MAIN);
		Sound_Reset();
		Sound_Generater(SOUND_LABEL_BGM_BOSS);
		g_BossSpawn = true;
		spawnCount = 100;
	}

	//ボスが出現していないときの雑魚スポーン感覚
	if(!Medium_Boss_isUsed() && !Boss_isUsed())
	{
		spawnCount = 50;
	}

	frameCounter++;
}