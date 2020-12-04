#include "texture.h"
#include "sprite.h"
#include <d3dx9.h>
#include "input.h"
#include "common.h"
#include "bullet.h"
#include "collision.h"
#include "player.h"
#include "debug.h"
#include "barrier.h"
#include "bg.h"
#include "game.h"
#include "UI.h"
#include "sound.h"
#include "Efect.h"
#include "enemy.h"
#include <Xinput.h>

#pragma comment(lib, "Xinput.lib")

//=====================================================
//�}�N����`
//=====================================================
#define PLAYER_SPD (5.0)		//�v���C���[�̑��x
#define MOVE_NUM (1.0)			//�ړ���
#define PLAYER_COLLISION (8.0f)	//�����蔻��

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static PLAYER g_Player;
static int g_ShotFrame = 0;
static XINPUT_STATE g_State;
static bool g_Trigger;

//=====================================================
//����������
//=====================================================
void Player_Init(void)
{
	g_Player.tex = Texture_SetLoadFile("Texture\\player.png", 192, 128);
	g_Player.collision.radius = PLAYER_COLLISION;
	g_Player.pos = D3DXVECTOR2(450.0f, 500.0f);
	g_Player.isUse = true;
	g_Player.HP = 150;
	g_Player.EN = 150;
	g_Player.MaxHP = g_Player.HP;
	g_Player.MaxEN = g_Player.EN;
	g_Player.isShot = false;
	g_Player.shotLevel = 1;
	g_Player.shotExp = 0;
	g_Player.LevelUp = false;
	g_ShotFrame = 0;
	g_Trigger = false;
}

//=====================================================
//�I������
//=====================================================
void Player_Uninit(void)
{

}

//=====================================================
//�X�V����
//=====================================================
void Player_Update(void)
{
	//�{�X���j��
	if (UI_GetCond() > 490)
	{
		//�����őO�ɐi��
		g_Player.pos.y -= 2.5f;
		if (g_Player.pos.y < 0)
		{
			Player_Destroy();
		}
		return;
	}
	if (Game_GetScene() != SCENE_MAIN && Game_GetScene() != SCENE_TUTORIAL)
	{
		return;
	}
	D3DXVECTOR2 dir(0.0f, 0.0f);

	XInputGetState(0, &g_State);

	//�L�[�{�[�h�X�V
	Keyboard_Update();
	//�L�[����
	if (Keyboard_IsPress(DIK_A))	//��
	{
		dir.x -= MOVE_NUM;
		//�v���C���[�Ǐ]�G�t�F�N�g
		if (Barrier_isUsed())
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(1, 1, 1, 0), 30, EFFECT_PLAYER);
		}
		else
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(0, 0, 1, 0), 30, EFFECT_PLAYER);
		}
	}
	else if (Keyboard_IsPress(DIK_D))	//�E
	{
		dir.x += MOVE_NUM;
		//�v���C���[�Ǐ]�G�t�F�N�g
		if (Barrier_isUsed())
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(1, 1, 1, 0), 30, EFFECT_PLAYER);
		}
		else
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(0, 0, 1, 0), 30, EFFECT_PLAYER);
		}
	}
	//�R���g���[���[ x��
	else if (g_State.Gamepad.sThumbLX < -8000 || g_State.Gamepad.sThumbLX > 8000)
	{
		dir.x += (FLOAT)(MOVE_NUM * g_State.Gamepad.sThumbLX / 32787);
		//�v���C���[�Ǐ]�G�t�F�N�g
		if (Barrier_isUsed())
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(1, 1, 1, 0), 30, EFFECT_PLAYER);
		}
		else
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(0, 0, 1, 0), 30, EFFECT_PLAYER);
		}
	}
	if (Keyboard_IsPress(DIK_W))	//��
	{
		dir.y -= MOVE_NUM;
		//�v���C���[�Ǐ]�G�t�F�N�g
		if (Barrier_isUsed())
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(1, 1, 1, 0), 30, EFFECT_PLAYER);
		}
		else
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(0, 0, 1, 0), 30, EFFECT_PLAYER);
		}
	}
	else if (Keyboard_IsPress(DIK_S))	//��
	{
		dir.y += MOVE_NUM;
		//�v���C���[�Ǐ]�G�t�F�N�g
		if (Barrier_isUsed())
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(1, 1, 1, 0), 30, EFFECT_PLAYER);
		}
		else
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(0, 0, 1, 0), 30, EFFECT_PLAYER);
		}
	}
	//�R���g���[���[ y��
	else if (g_State.Gamepad.sThumbLY < -8000 || g_State.Gamepad.sThumbLY > 8000)
	{
		dir.y -= (FLOAT)(MOVE_NUM * g_State.Gamepad.sThumbLY / 32787);
		//�v���C���[�Ǐ]�G�t�F�N�g
		if (Barrier_isUsed())
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(1, 1, 1, 0), 30, EFFECT_PLAYER);
		}
		else
		{
			Efect_Create(g_Player.pos.x, g_Player.pos.y, D3DXCOLOR(0, 0, 1, 0), 30, EFFECT_PLAYER);
		}
	}

	D3DXVec2Normalize(&dir, &dir);	//�x�N�g���̐��K��������֐�

	D3DXVECTOR2 vecSpeed = dir * PLAYER_SPD;
	g_Player.pos += vecSpeed;

	//��ʊO����
	if (g_Player.pos.x < BG_START)		//��
	{
		g_Player.pos.x = BG_START;
	}
	if (g_Player.pos.x > BG_START + BG_WIDTH - Texture_GetWidth(g_Player.tex) / 4 - 12)		//�E
	{
		g_Player.pos.x = (float)(BG_START + BG_WIDTH - Texture_GetWidth(g_Player.tex) / 4 - 12);
	}
	if (g_Player.pos.y < 40)			//��
	{
		g_Player.pos.y = 40;
	}
	if (g_Player.pos.y > BG_HEIGHT - Texture_GetHeight(g_Player.tex) / 2)	//��
	{
		g_Player.pos.y = (FLOAT)(BG_HEIGHT - Texture_GetHeight(g_Player.tex) / 2);
	}

	//�����蔻����W�̏�����
	g_Player.collision.position.x = g_Player.pos.x + Texture_GetWidth(g_Player.tex) / 3 / 2;
	g_Player.collision.position.y = g_Player.pos.y + Texture_GetHeight(g_Player.tex) / 2 / 2;

	//�e�̔���
	if ((Keyboard_IsPress(DIK_K) || g_State.Gamepad.bLeftTrigger > 128 || g_State.Gamepad.bRightTrigger > 128) && g_Player.EN > 0)
	{
		if (g_ShotFrame > 10)
		{
			switch (g_Player.shotLevel)
			{
				//1��
			case 1:
				Bullet_Create(Bullet_GetObj(0, "Player"), g_Player.pos.x, g_Player.pos.y, 90);
				Player_Damage(GAUGE_EN, 2.5f);	//�G�l���M�[����
				break;
				//2��
			case 2:
				for (int i = 0; i < 2; i++)
				{
					Bullet_Create(Bullet_GetObj(0, "Player"), g_Player.pos.x - 25.0f + 50.0f * i, g_Player.pos.y, 90);
				}
				Player_Damage(GAUGE_EN, 3.5f);	//�G�l���M�[����
				break;
				//4��
			case 3:
				//�O2��
				for (int i = 0; i < 2; i++)
				{
					Bullet_Create(Bullet_GetObj(0, "Player"), g_Player.pos.x - 25.0f + 50.0f * i, g_Player.pos.y, 90);
				}
				//�T�C�h2��
				for (int i = 0; i < 2; i++)
				{
					Bullet_Create(Bullet_GetObj(0, "Player"), g_Player.pos.x, g_Player.pos.y, BULLET_ANGLE_RIGHT + i * 60);
				}
				Player_Damage(GAUGE_EN, 4.5f);	//�G�l���M�[����
				break;
			default:
				break;
			}
			g_Player.isShot = true;
			g_ShotFrame = 0;
		}
	}
	else
	{
		g_Player.isShot = false;
	}

	//�o���A�̐���
	if ((Keyboard_IsTrigger(DIK_L) || g_State.Gamepad.wButtons & XINPUT_GAMEPAD_A || g_State.Gamepad.wButtons & XINPUT_GAMEPAD_X) && !Barrier_isUsed() && g_Player.EN > 0 && g_Trigger)
	{
		Barrier_Create(g_Player.pos.x, g_Player.pos.y);
		g_Trigger = false;
	}

	else if (!(g_State.Gamepad.wButtons & XINPUT_GAMEPAD_A || g_State.Gamepad.wButtons & XINPUT_GAMEPAD_X))
	{
		g_Trigger = true;
	}

	if ((Keyboard_IsTrigger(DIK_L) || g_State.Gamepad.wButtons & XINPUT_GAMEPAD_A || g_State.Gamepad.wButtons & XINPUT_GAMEPAD_X) && Barrier_isUsed() && g_Trigger || g_Player.EN < 0)
	{
		Barrier_Destroy();
		g_Trigger = false;
	}


	//�o���A�������v���C���[�̓����蔻�葝��
	if (Barrier_isUsed())
	{
		g_Player.collision.radius = 64.0f;
	}
	else
	{
		g_Player.collision.radius = PLAYER_COLLISION;
	}

	//�G�l���M�[�̉�
	if (!Barrier_isUsed() && !g_Player.isShot)
	{
		g_Player.EN += 0.2f;
	}

	//�G�l���M�[���ő�l�ȏ�ɂȂ�Ȃ�����
	if (g_Player.EN > g_Player.MaxEN)
	{
		g_Player.EN = g_Player.MaxEN;
	}

	//�v���C���[��HP��1�����ɂȂ�����폜
	if (g_Player.HP < 1)
	{
		Player_Destroy();
		Game_SceneChange(SCENE_GAMEOVER);
	}

	g_ShotFrame++;
}

//=====================================================
//�`�揈��
//=====================================================
void Player_Draw(void)
{
	if (g_Player.isUse)
	{
		Sprite_Draw(g_Player.tex, g_Player.pos.x, g_Player.pos.y, 0, 0, 64, 64);
	}
}

//=====================================================
//�v���C���[�̃t���O�擾
//=====================================================
bool Player_isUsed(void)
{
	return g_Player.isUse;
}

//=====================================================
//�v���C���[�̃t���O�폜
//=====================================================
void Player_Destroy(void)
{
	g_Player.isUse = false;
}

//=====================================================
//�����蔻��̍\���̎擾
//=====================================================
CIRCLE *Player_GetCollision(void)
{
	return &g_Player.collision;
}

//=====================================================
//�v���C���[�̍\���̎擾
//=====================================================
PLAYER *Player_GetObj(void)
{
	return &g_Player;
}

//=====================================================
//�_���[�W����
//=====================================================
void Player_Damage(GA gauge, float damage)
{
	switch (gauge)
	{
	case GAUGE_HP:
		if (g_Player.HP <= g_Player.MaxHP)
		{
			g_Player.HP -= damage;
		}
		break;
	case GAUGE_EN:
		if (g_Player.EN <= g_Player.MaxEN)
		{
			g_Player.EN -= damage;
		}
		break;
	default:
		break;
	}
}