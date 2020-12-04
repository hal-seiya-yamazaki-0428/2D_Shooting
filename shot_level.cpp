#include "mydirectx.h"
#include "texture.h"
#include "sprite.h"
#include "player.h"
#include "common.h"

//====================================================
//マクロ定義
//====================================================
#define TEXTURE_MAX (4)

//====================================================
//構造体宣言
//====================================================
typedef struct
{
	D3DXVECTOR4 position;	//頂点座標
	D3DCOLOR color;			//頂点の色情報
	D3DXVECTOR2 UV;			//uv座標(texcoord)
}Vertex2d;
#define FVF_VERTEX2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)	//x,y,z,RHWをもって頂点データを表す

//=====================================================
//グローバル変数
//=====================================================
static D3DXVECTOR2 g_Pos;
static D3DCOLOR g_Color[4];
static unsigned int g_Tex[TEXTURE_MAX];
static int g_LevelFrame = 0;

//=====================================================
//初期化
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
//終了
//=====================================================
void ShotLevel_Uninit(void)
{

}

//=====================================================
//更新
//=====================================================
void ShotLevel_Update(void)
{
	PLAYER *pPlayer = Player_GetObj();

	//レベルアップ中一定フレーム経過したらLEVELUPの描画をしない
	if (pPlayer->LevelUp && g_LevelFrame < 120)
	{
		g_LevelFrame++;
	}
	else
	{
		pPlayer->LevelUp = false;
		g_LevelFrame = 0;
	}

	//レベルマックス時の虹色演出
	if (pPlayer->shotLevel >= MAX_LEVEL)
	{
		g_Color[0] = D3DCOLOR_RGBA(255, 0, 0, 255);
		g_Color[1] = D3DCOLOR_RGBA(128, 255, 0, 255);
		g_Color[2] = D3DCOLOR_RGBA(0, 128, 255, 255);
		g_Color[3] = D3DCOLOR_RGBA(255, 0, 128, 255);
	}
}

//=====================================================
//描画
//=====================================================
void ShotLevel_Draw(void)
{
	int width = Texture_GetWidth(g_Tex[0]);		//「SHOTLEVEL」の高さ
	int height = Texture_GetHeight(g_Tex[0]);	//「SHOTLEVEL」の幅
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *pPlayer = Player_GetObj();

	//レベルゲージの頂点データ
	Vertex2d v[32] =
	{
		{ D3DXVECTOR4(g_Pos.x, g_Pos.y, 0.0f, 1.0f),g_Color[0], D3DXVECTOR2(0, 0) },
	{ D3DXVECTOR4(g_Pos.x + (182 * pPlayer->shotExp / 100), g_Pos.y, 0.0f, 1.0f),g_Color[1], D3DXVECTOR2(1.0f, 0) },
	{ D3DXVECTOR4(g_Pos.x, g_Pos.y + 32 - 0.5f, 0.0f, 1.0f), g_Color[2], D3DXVECTOR2(0.0f, 1.0f) },
	{ D3DXVECTOR4(g_Pos.x + (182 * pPlayer->shotExp / 100), g_Pos.y + 32, 0.0f, 1.0f), g_Color[3], D3DXVECTOR2(1.0f,1.0f) },
	};
	pDevice->SetFVF(FVF_VERTEX2D);			//デバイスに頂点データを渡す
	pDevice->SetTexture(0, NULL);			//テクスチャをデバイスに渡す
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));

	Sprite_Draw(g_Tex[0], SCREEN_WIDTH - 265.0f, 175.0f, 0, height / 2, width, height / 10);	//SHOTLEVEL
	//レベル描画
	if (pPlayer->shotLevel != MAX_LEVEL)
	{
		Sprite_Draw(g_Tex[1], SCREEN_WIDTH - 50.0f, 170.0f, pPlayer->shotLevel * 32, 0, 32, 32);//現在のレベル
	}
	//MAX描画
	else
	{
		Sprite_Draw(g_Tex[0], SCREEN_WIDTH - 160.0f, 170.0f, 0, height / 2 + height / 10, width, height / 10);	//MAXレベル
	}

	//LEVELUP描画
	if (pPlayer->LevelUp)
	{
		Sprite_Draw(g_Tex[0], pPlayer->pos.x - 100.0f, pPlayer->pos.y - 10.0f, 0, height / 2 + height / 10 * 2 + 10, width, height / 10);
	}
}