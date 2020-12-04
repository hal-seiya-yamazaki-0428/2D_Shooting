#include "player.h"
#include "bullet.h"
#include "texture.h"
#include "enemy.h"
#include "enemygenerater.h"
#include "judgement.h"
#include "explosion.h"
#include "number.h"
#include "score.h"
#include "sound.h"
#include "debug.h"
#include "debug_font.h"
#include "UI.h"
#include "barrier.h"
#include "bg.h"
#include "barierexplosion.h"
#include "medium_boss.h"
#include "med_bullet.h"
#include "item.h"
#include "boss.h"
#include "boss_bullet.h"
#include "title.h"
#include "game.h"
#include "fade.h"
#include "tutorial.h"
#include "result.h"
#include "soundgenerater.h"
#include "Efect.h"
#include "hibana.h"
#include "shot_level.h"

//=====================================================
//グローバル変数
//=====================================================
static SCENE g_Scene = SCENE_NONE;
static bool g_TexLoad = true;
static HWND g_hwnd = NULL;

//=====================================================
//初期化
//=====================================================
void Game_Init(HWND hwnd)
{
	g_Scene = SCENE_TITLE;
	g_hwnd = hwnd;
	Title_Init();
	Player_Init();
	Bullet_Init();
	Med_Bullet_Init();
	Boss_Bullet_Init();
	Enemy_Init();
	Medium_Boss_Init();
	Boss_Init();
	EnemyGenerater_Init();
	Explosion_Init();
	Number_Init();
	Score_Init();
	Debug_Init();
	Barrier_Init();
	BG_Init();
	BarExp_Init();
	Item_Init();
	UI_Init();
	Fade_init();
	Tutorial_Init();
	Result_Init();
	EfectInit();
	Hibana_Init();
	ShotLevel_Init();
	if (g_TexLoad)
	{
		if (Texture_Load() > 0)
		{
			MessageBox(hwnd, "テクスチャが読み込めませんでした", "エラー", MB_OK);
			exit(0);
		}
		g_TexLoad = false;
	}
}

//=====================================================
//終了
//=====================================================
void Game_Uninit(void)
{
	Title_Uninit();
	Debug_Uninit();
	Player_Uninit();
	Bullet_Uninit();
	Med_Bullet_Uninit();
	Boss_Bullet_Uninit();
	Enemy_Uninit();
	Medium_Boss_Uninit();
	Boss_Uninit();
	EnemyGenerater_Uninit();
	Explosion_Uninit();
	Barrier_Uninit();
	BG_Uninit();
	BarExp_Uninit();
	Item_Uninit();
	UI_Uninit();
	Fade_Uninit();
	Tutorial_Uninit();
	Result_Uninit();
	EfectUninit();
	Hibana_Uninit();
	ShotLevel_Uninit();
}

//=====================================================
//更新
//=====================================================
void Game_Update(void)
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		Title_Update();
		UI_Updata();
		Fade_Update();
		break;
	case SCENE_TUTORIAL:
		UI_Updata();
		Fade_Update();
		Tutorial_Update();
		Bullet_Update();
		Barrier_Update();
		Explosion_Update();
		break;
	case SCENE_MAIN:
		Fade_Update();
		Explosion_Update();
		Bullet_Update();
		//フェード中であれば操作できない
		if (Fade_isUse())
		{
			break;
		}
		Sound_Generater(SOUND_LABEL_BGM_MAIN);
		BG_Update();
		Item_Update();
		Player_Update();
		Med_Bullet_Update();
		Boss_Bullet_Update();
		Boss_Update();
		EnemyGenerater_Update();
		Enemy_Update();
		Medium_Boss_Update();
		Judgement_Update();
		Score_Update();
		Barrier_Update();
		BarExp_Update();
		UI_Updata();
		EfectUpdate();
		Hibana_Update();
		ShotLevel_Update();
		break;
	case SCENE_GAMECLEAR:
	case SCENE_GAMEOVER:
		BG_Update();
		Item_Update();
		Player_Update();
		Bullet_Update();
		Med_Bullet_Update();
		Boss_Bullet_Update();
		EnemyGenerater_Update();
		Enemy_Update();
		Medium_Boss_Update();
		Boss_Update();
		Judgement_Update();
		Explosion_Update();
		Score_Update();
		Barrier_Update();
		BarExp_Update();
		Result_Update();
		UI_Updata();
		Fade_Update();
		EfectUpdate();
		break;
	default:
		break;
	}
}

//=====================================================
//描画
//=====================================================
void Game_Draw(void)
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		Title_Draw();
		UI_Draw();
		Fade_Draw();
		break;
	case SCENE_TUTORIAL:
		UI_Draw();
		Tutorial_Draw();
		Bullet_Draw();
		Barrier_Draw();
		Explosion_Draw();
		Fade_Draw();
		break;
	case SCENE_MAIN:
		BG_Draw();
		Item_Draw();
		EfectDraw();
		Bullet_Draw();
		Med_Bullet_Draw();
		Boss_Bullet_Draw();
		Player_Draw();
		Enemy_Draw();
		Medium_Boss_Draw();
		Boss_Draw();
		Explosion_Draw();
		Hibana_Draw();
		Barrier_Draw();
		BarExp_Draw();
		UI_Draw();
		Fade_Draw();
		ShotLevel_Draw();
		Score_Draw(0, 1000, 0, 5, false);
		break;
	case SCENE_GAMECLEAR:
	case SCENE_GAMEOVER:
		BG_Draw();
		Item_Draw();
		EfectDraw();
		Bullet_Draw();
		Med_Bullet_Draw();
		Boss_Bullet_Draw();
		Player_Draw();
		Enemy_Draw();
		Medium_Boss_Draw();
		Boss_Draw();
		Explosion_Draw();
		Barrier_Draw();
		BarExp_Draw();
		Result_Draw();
		UI_Draw();
		Fade_Draw();
		break;
	default:
		break;
	}
}

//=====================================================
//シーンの切り替え
//=====================================================
void Game_SceneChange(SCENE scene)
{
	g_Scene = scene;
}

//=====================================================
//シーンの取得
//=====================================================
SCENE Game_GetScene(void)
{
	return g_Scene;
}

//=====================================================
//ウィンドウハンドルの取得
//=====================================================
HWND Game_GetHwnd(void)
{
	return g_hwnd;
}
