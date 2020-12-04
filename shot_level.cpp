#include "mydirectx.h"
#include "texture.h"
#include "sprite.h"
#include "player.h"
#include "common.h"

//====================================================
//�}�N����`
//====================================================
#define TEXTURE_MAX (4)

//====================================================
//�\���̐錾
//====================================================
typedef struct
{
	D3DXVECTOR4 position;	//���_���W
	D3DCOLOR color;			//���_�̐F���
	D3DXVECTOR2 UV;			//uv���W(texcoord)
}Vertex2d;
#define FVF_VERTEX2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)	//x,y,z,RHW�������Ē��_�f�[�^��\��

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static D3DXVECTOR2 g_Pos;
static D3DCOLOR g_Color[4];
static unsigned int g_Tex[TEXTURE_MAX];
static int g_LevelFrame = 0;

//=====================================================
//������
//=====================================================
void ShotLevel_Init(void)
{
	g_Pos = D3DXVECTOR2(SCREEN_WIDTH - 200.0f, 200.0f);
	g_Tex[0] = Texture_SetLoadFile("Texture\\font2.png", 256, 256);
	g_Tex[1] = Texture_SetLoadFile("Texture\\number.tga", 320, 32);
	g_LevelFrame = 0;
	for (int i = 0; i < 4; i++)
	{
		g_Color[i] = D3DCOLOR_RGBA(255, 255, 255, 255);
	}
}

//=====================================================
//�I��
//=====================================================
void ShotLevel_Uninit(void)
{

}

//=====================================================
//�X�V
//=====================================================
void ShotLevel_Update(void)
{
	PLAYER *pPlayer = Player_GetObj();

	//���x���A�b�v�����t���[���o�߂�����LEVELUP�̕`������Ȃ�
	if (pPlayer->LevelUp && g_LevelFrame < 120)
	{
		g_LevelFrame++;
	}
	else
	{
		pPlayer->LevelUp = false;
		g_LevelFrame = 0;
	}

	//���x���}�b�N�X���̓��F���o
	if (pPlayer->shotLevel >= MAX_LEVEL)
	{
		g_Color[0] = D3DCOLOR_RGBA(255, 0, 0, 255);
		g_Color[1] = D3DCOLOR_RGBA(128, 255, 0, 255);
		g_Color[2] = D3DCOLOR_RGBA(0, 128, 255, 255);
		g_Color[3] = D3DCOLOR_RGBA(255, 0, 128, 255);
	}
}

//=====================================================
//�`��
//=====================================================
void ShotLevel_Draw(void)
{
	int width = Texture_GetWidth(g_Tex[0]);		//�uSHOTLEVEL�v�̍���
	int height = Texture_GetHeight(g_Tex[0]);	//�uSHOTLEVEL�v�̕�
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *pPlayer = Player_GetObj();

	//���x���Q�[�W�̒��_�f�[�^
	Vertex2d v[32] =
	{
		{ D3DXVECTOR4(g_Pos.x, g_Pos.y, 0.0f, 1.0f),g_Color[0], D3DXVECTOR2(0, 0) },
	{ D3DXVECTOR4(g_Pos.x + (182 * pPlayer->shotExp / 100), g_Pos.y, 0.0f, 1.0f),g_Color[1], D3DXVECTOR2(1.0f, 0) },
	{ D3DXVECTOR4(g_Pos.x, g_Pos.y + 32 - 0.5f, 0.0f, 1.0f), g_Color[2], D3DXVECTOR2(0.0f, 1.0f) },
	{ D3DXVECTOR4(g_Pos.x + (182 * pPlayer->shotExp / 100), g_Pos.y + 32, 0.0f, 1.0f), g_Color[3], D3DXVECTOR2(1.0f,1.0f) },
	};
	pDevice->SetFVF(FVF_VERTEX2D);			//�f�o�C�X�ɒ��_�f�[�^��n��
	pDevice->SetTexture(0, NULL);			//�e�N�X�`�����f�o�C�X�ɓn��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));

	Sprite_Draw(g_Tex[0], SCREEN_WIDTH - 265.0f, 175.0f, 0, height / 2, width, height / 10);	//SHOTLEVEL
	//���x���`��
	if (pPlayer->shotLevel != MAX_LEVEL)
	{
		Sprite_Draw(g_Tex[1], SCREEN_WIDTH - 50.0f, 170.0f, pPlayer->shotLevel * 32, 0, 32, 32);//���݂̃��x��
	}
	//MAX�`��
	else
	{
		Sprite_Draw(g_Tex[0], SCREEN_WIDTH - 160.0f, 170.0f, 0, height / 2 + height / 10, width, height / 10);	//MAX���x��
	}

	//LEVELUP�`��
	if (pPlayer->LevelUp)
	{
		Sprite_Draw(g_Tex[0], pPlayer->pos.x - 100.0f, pPlayer->pos.y - 10.0f, 0, height / 2 + height / 10 * 2 + 10, width, height / 10);
	}
}