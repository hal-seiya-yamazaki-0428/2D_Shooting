#include "texture.h"
#include "sprite.h"
#include "game.h"
#include "input.h"
#include "number.h"
#include "bg.h"
#include <d3dx9.h>
#include "bullet.h"
#include "barrier.h"
#include "enemy.h"
#include "explosion.h"
#include "fade.h"
#include "sound.h"
#include "soundgenerater.h"
#include <Xinput.h>

#pragma comment(lib, "Xinput.lib")

//=====================================================
//�}�N����`
//=====================================================
#define TEXTURE_MAX (16)
#define FRAME_SHOT (100)
#define FRAME_BARRIER (300)

//=====================================================
//�񋓌^
//=====================================================
typedef enum
{
	NONE,
	w,
	a,
	s,
	d
}WASD;

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static unsigned int g_Tex[TEXTURE_MAX];
static int g_Pages = 1;
static D3DXVECTOR2 g_pos;
static int g_frame = 0;
static WASD wasd = a;
static float g_gauge = 100.0f;
static int g_alpha = 255;
static XINPUT_STATE g_State;
static bool g_Trigger;

//=====================================================
//������
//=====================================================
void Tutorial_Init()
{
	g_Tex[0] = Texture_SetLoadFile("Texture\\tutorial.png", 1024, 520);
	g_Tex[1] = Texture_SetLoadFile("Texture\\player.png", 192, 128);
	g_Tex[2] = Texture_SetLoadFile("Texture\\HP_Gauge_bg.png", 200, 32);		//�Q�[�W���̍��w�i
	g_Tex[3] = Texture_SetLoadFile("Texture\\HP_Gauge_01_blue.png", 200, 32);	//en�Q�[�W(��)
	g_Tex[4] = Texture_SetLoadFile("Texture\\HP_Gauge_01_frame.png", 200, 32);	//�Q�[�W�g
	g_Tex[5] = Texture_SetLoadFile("Texture\\�L����32�~32.png", 320, 320);
	g_Tex[6] = Texture_SetLoadFile("Texture\\item.png", 60, 39);
	g_Tex[7] = Texture_SetLoadFile("Texture\\font.png", 512, 512);
	g_Pages = 1;
	g_pos = D3DXVECTOR2(0, 0);
	g_frame = 1;
	wasd = a;
	g_gauge = 100.0f;
	g_alpha = 255;
	g_Trigger = false;
}

//=====================================================
//�I��
//=====================================================
void Tutorial_Uninit()
{

}

//=====================================================
//�X�V
//=====================================================
void Tutorial_Update()
{
	XInputGetState(0, &g_State);

	//�L�[�{�[�h�X�V
	Keyboard_Update();
	//���̃y�[�W
	if ((Keyboard_IsTrigger(DIK_L) || g_State.Gamepad.bRightTrigger > 128) && g_Pages != 2)
	{
		g_Pages = 2;
		Barrier_Destroy();
		PlaySound(SOUND_LABEL_SE_DECISION);
	}
	//�O�̃y�[�W
	else if ((Keyboard_IsTrigger(DIK_K) || g_State.Gamepad.bLeftTrigger > 128) && g_Pages != 1)
	{
		g_Pages = 1;
		PlaySound(SOUND_LABEL_SE_DECISION);
	}

	//�ړ�
	switch (wasd)
	{
	case w:
		g_pos.y -= 1.0f;
		break;
	case a:
		g_pos.x -= 1.0f;
		break;
	case s:
		g_pos.y += 1.0f;
		break;
	case d:
		g_pos.x += 1.0f;
		break;
	default:
		break;
	}
	//�����ؑ�
	if (wasd == a && g_frame % FRAME_SHOT == 0)
	{
		wasd = s;
	}
	else if (wasd == s && g_frame % FRAME_SHOT == 0)
	{
		wasd = d;
	}
	else if (wasd == d && g_frame % FRAME_SHOT == 0)
	{
		wasd = w;
	}
	else if (wasd == w && g_frame % FRAME_SHOT == 0)
	{
		wasd = a;
	}

	//�e�̔���
	if (g_Pages == 1)
	{
		if (g_frame % 30 == 0)
		{
			for (int i = 0; i < 3; i++)
			{
				Bullet_Create(Bullet_GetObj(0, "Player"), BG_START + 140.0f, 330.0f, BULLET_ANGLE_RIGHT + i * 30);
			}
		}
		//�o���A�̐���
		if (g_frame % FRAME_BARRIER == 0)
		{
			Barrier_Destroy();
		}
		else if (g_frame % FRAME_BARRIER == FRAME_BARRIER / 2)
		{
			Barrier_Create(BG_START + 140.0f, 345.0f, false);
		}
	}

	//�G�l���M�[�̐���
	else if (g_Pages == 2)
	{
		//�Q�[�W�̑�������
		if (g_frame % 400 < 200)
		{
			g_gauge -= 0.5f;
			if (g_gauge < 0)
			{
				g_gauge = 0;
			}
		}
		else
		{
			g_gauge += 0.5f;
			if (g_gauge > 100)
			{
				g_gauge = 100;
			}
		}
		//�A�C�e���̐���
		if (!Explosion_isUse(0) && g_frame % 100 == 0)
		{
			Explosion_Create(BG_START + 150.0f, 300.0f, false);
		}
		//�Q�[���{��
		if (Keyboard_IsTrigger(DIK_SPACE) || g_State.Gamepad.wButtons & XINPUT_GAMEPAD_A && g_Trigger)
		{
			PlaySound(SOUND_LABEL_SE_DECISION);
			StopSound(SOUND_LABEL_BGM_TITLE);
			Sound_Reset();
			Fade_InOut(SCENE_MAIN);
			Explosion_Destory(0);
			//�`���[�g���A�����Ŏg�p�����e�̍폜
			for (int i = 0; i < BULLET_MAX; i++)
			{
				if (Bullet_isUsed(i, "Player"))
				{
					Bullet_Destroy(i, "Player");
				}
			}
			g_Trigger = false;
		}
		else if(!(g_State.Gamepad.wButtons & XINPUT_GAMEPAD_A))
		{
			g_Trigger = true;
		}
	}
	g_frame++;
}

//=====================================================
//�`��
//=====================================================
void Tutorial_Draw()
{
	//1�y�[�W��
	if (g_Pages == 1)
	{
		//�`���[�g���A��
		Sprite_Draw(g_Tex[0], BG_START, 0, 0, 0, 512, 460, 0, 0, 0, 0.3f);
		//�v���C���[
		Sprite_Draw(g_Tex[1], BG_START + 500.0f + g_pos.x, 110.0f + g_pos.y, 0, 0, 64, 64, 0, 0, 0, -0.25f);//�E��
		Sprite_Draw(g_Tex[1], BG_START + 150.0f, 350.0f, 0, 0, 64, 64, 0, 0, 0, -0.25f);//����
	}
	//2�y�[�W��
	else if (g_Pages == 2)
	{
		//�`���[�g���A��
		Sprite_Draw(g_Tex[0], BG_START, 0, 512, 0, 512, 460, 0, 0, 0, 0.3f);
		//�G�l���M�[
		Sprite_Draw(g_Tex[2], BG_START + 400, 150);	//���w�i
		Sprite_Draw(g_Tex[3], BG_START + 400, 150, 0, 0, (int)(Texture_GetWidth(g_Tex[2]) * g_gauge / 100), 32);
		Sprite_Draw(g_Tex[4], BG_START + 400, 150);	//�Q�[�W�g
		//�G
		if (Explosion_isUse(0))
		{
			Sprite_Draw(g_Tex[5], BG_START + 150.0f, 300.0f, Texture_GetWidth(g_Tex[5]) * 3 / ENEMY_X_PATTERN + 2, 0 + 2, Texture_GetWidth(g_Tex[5]) / ENEMY_X_PATTERN - 4, Texture_GetHeight(g_Tex[5]) / ENEMY_Y_PATTERN - 4);
		}
		else
		{
			Sprite_Draw(g_Tex[6], BG_START + 150.0f, 300.0f);
		}
		//�X�y�[�X�ł͂��߂�
		Sprite_Draw(g_Tex[7], (float)(BG_START * 1.3 + 10), (float)(BG_HEIGHT - 160), 0, 64, 512, 64, g_alpha);
	}
	//�y�[�W��
	Number_Draw(g_Pages, 500.0f, 510.0f);	//���q
	Number_Draw(2, 550.0f, 510.0f);			//����
}