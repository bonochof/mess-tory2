/*
*	夏休み課題「♀tory 改」
*
*	平成26年9月28日		Ver. 1.00
*							完成
*	平成26年10月15日	Ver. 1.10
*							大幅修正
*	平成26年10月19日	Ver. 1.11
*							修正(対戦の当たり判定, ダメージ音)
*	平成26年10月21日	Ver. 1.12
*							修正(マップ情報, 耐久状態時ダメージ, op・ed文章/背景, オープニングステージチェンジ時の針)
*							追加(状態異常アイコン, 不利アイテム獲得音, アイテム効果発動音)
*	平成26年10月24日	Ver. 1.13
*							修正(学校PCでオープニング・エンディングをスキップできない問題, エンディング後キー反応しない問題, ♂残りライフで負の数を表示する問題)
*	平成27年8月6日		Ver. 2.00
*							コード大幅改変
*							マップ調整
*	平成27年8月8日		Ver. 2.01
*							修正(入力を一括方式にした, 初期ジャンプ力増加)
*							追加(イントロスキップ, ステージ0スキップ, いつでも無敵化)
*
*		J1318	高田亮介
*
*＜プログラム参考＞
*	ＤＸライブラリ置き場　サンプルプログラム：
*		http://homepage2.nifty.com/natupaji/DxLib/dxprogram.html#N3
*
*
*＜音楽＞
*	魔王魂：
*		http://maoudamashii.jokersounds.com
*
*＜背景＞
*	http://www5f.biglobe.ne.jp/~psychephoto/rpg/buttle_01.html
*	http://tyrano.jp/usage/tutorial/title
*	http://rune123.jugem.cc/?cid=6
*
*＜その他画像＞
*	http://www.slot-pioneer.co.jp/product/new_oasis/pop_download.html
*/


/* 【include】 */
#include "DxLib.h"	//Dxライブラリ
#include<stdio.h>	//セーブ用( ファイル関係 )

/* 【マクロ定義】 */
#define MENUX 230
#define MENUY_OVER 185
#define MENUY_UNDER 369
#define MENUSPACE 46
#define START 0
#define CONTINUE 1
#define BATTLE 2
#define EXPLANATION 3
#define END 4
#define WINDOW_SIZE_X 640	//画面サイズ(横)
#define WINDOW_SIZE_Y 480	//画面サイズ(縦)
#define CHARA_SIZE 64		//キャラクターサイズ
#define MAP_SIZE 32			//＜参考＞マップチップ1つのドットサイズ
#define STAGE_WIDTH 60		//＜参考＞ステージ幅
#define STAGE_BATTLEWIDTH 20
#define STAGE_HEIGHT 15		//＜参考＞ステージ縦長さ
#define MAX_FLOOR 30		//最高階層
#define MAX_BATTLEFLOOR 3	//
#define MOVEY_MAX 32		//最高速度
#define NO_ENTRY -1
#define EMPTY 0
#define BRICK 1
#define NEEDLE 6
#define SAVE 7
#define GOAL 8
#define FREEZE 9
#define BLOCK 10
#define POISON 11
#define Erosion 12
#define G 13
#define C_NEEDLE 14
#define BROKEN_FLOOR 16
#define STAR 17
#define BSTAR 18
#define LIFE 19
#define ATTACK 20
#define INV 21
#define E 22
#define B 23
#define J 24
#define D 25
#define TIMER 26
#define BROKEN_LIFE 27
#define BROKEN_ATTACK 28
#define L_R 29
#define S_DOWN 30
#define J_LIMIT 31
#define PI 3.14159265359

/* 【構造体型】 */
struct S_Data
{
	int datanum;
	char name[11];
	int stage, x, y, mapchipx;
};

struct B_Data
{
	int x, y;
};

struct grData
{
	int intro_messonly, intro_messtory, intro_back0, intro_back1, intro_back2;
	int main_back, menu_back, stage1_back, stage2_back, stage3_back, stage4_back, stage5_back, stage6_back, stage7_back, stage8_back, stage9_back, stage10_back;
	int opening, fukidashi, gameover;
	int battleview1, battleview2, battleview3, battlestage1, battlestage2, battlestage3;
	int exp1, exp2, exp3, exp4, exp5;
	int mess[16], oss[8], enemy[16], damage[16], battlemess1[16], battlemess2[16], battledamage1[16], battledamage2[16], attack[8], mapchip[16], item[16], state[4];
};

struct seData
{
	int select, enter, notenter, exit;
	int save, clear, gameover, damage, attack, flash, star, bigstar, medicine, badmedicine, effect;
	int battleend;
	int expnext;
};

/* 【グローバル変数】 */
struct S_Data mess, data1, data2, data3;
struct B_Data b_mess1, b_mess2;
struct grData grp = {0};
struct seData se = {0};

int mode;
int movex, movex_b1, movex_b2;
int movey, movey_b1, movey_b2;
int movey0 = -45;
int g, g_b1, g_b2;
int life, life_b1, life_b2;
int point;
int power = 1;
int energy = 100;
int energy_b1 = 100;
int energy_b2 = 100;
int battlestage;
int ossx, ossy;
int osslife;
int enemy1exist, enemy2exist;
int enemy1life, enemy2life;
int enemy1x, enemy1y;
int enemy2x, enemy2y;
int attackrightx, attackleftx;
int attackrightx_b1, attackleftx_b1;
int attackrightx_b2, attackleftx_b2;
int isMovingRight = 0;
int isMovingRight_b1 = 0;
int isMovingRight_b2 = 0;
int isMovingLeft = 0;
int isMovingLeft_b1 = 0;
int isMovingLeft_b2 = 0;
int isJumping = 0;
int isJumping_b1 = 0;
int isJumping_b2 = 0;
int isAttack = 0;
int isAttack_b1 = 0;
int isAttack_b2 = 0;
int isDamaging = 0;
int isDamaging_b1 = 0;
int isDamaging_b2 = 0;
int InvTime = 0;
int isProtecting = 0;
int isReversing = 0;
int isJ_Restricting = 0;
int osssay;
int ossmove1 = 0;
int ossmove2 = 0;
int enemy1direction, enemy2direction;
int ossdirection;
int stagebackx = 0;
int Save = 0;
int StageClear = 0;
int Damage = 0;
int BattleEnd = 0;
int exitFlag = 0;
int maxfloor;
int skipflag = 0;

char key[256];

int changesizemap[ STAGE_HEIGHT * MAP_SIZE ][ STAGE_WIDTH * MAP_SIZE ];
int changesizemap_b[ STAGE_HEIGHT * MAP_SIZE ][ STAGE_BATTLEWIDTH * MAP_SIZE ];

/*	
*			 1:レンガ
*			 2:土(中)
*			 3:土(左上)
*			 4:土(中上)
*			 5:土(右上)
*			 6:トゲ
*			 7:SAVE
*			 8:GOAL
*			 9:氷
*			10:■
*			11:毒
*			12:侵蝕レンガ
*			13:G
*			14:∧
*			15:フロア
*			16:壊れフロア
*			17:スター
*			18:ビッグスター
*			19:ライフ
*			20:アタック
*			21:無敵
*			22:エネルギー弾使用可薬
*			23:耐久薬
*			24:ジャンプアップ
*			25:ダッシュアップ
*			26:タイマー
*			27:ひびハート
*			28:ひびアタック
*			29:左右反転
*			30:スピードダウン
*			31:ジャンプ不可
*/


/* 【グローバル変数】ステージ */
int map[ MAX_FLOOR ][ STAGE_HEIGHT ][ STAGE_WIDTH ] = { 0 };
int stage[ STAGE_HEIGHT ][ STAGE_WIDTH ] = { 0 };
int battlemap[ MAX_BATTLEFLOOR ][ STAGE_HEIGHT ][ STAGE_BATTLEWIDTH ];

/* 【関数プロトタイプ】 */
void InitGrp();
void InitSe();
void LoadMap();
void Input();
void Intro();
int Menu();
int Story_SavedataSelect( int );
void Story_Opening();
void Story_AfterOpening();
void Story_Ending();
void Story_StageInput();
void Story_Init( int );
void Story_InitAfterDamage();
void Story_MapRenew();
void Story_ChangeEmpty( int );
int Story_CheckMapHitRight();
int Story_CheckMapHitLeft();
int Story_CheckMapHitOver( int );
int Story_CheckMapHitUnder( int );
void Story_Draw();
void Story_Flash( int );
void Story_EscapeOss();
void Story_DamageMove();
void Story_Hit();
void Story_EnemyHit();
void Story_Move();
void Story_EnemyMove();
void Story_OssMove();
void Story_Save();
void Story_GameOver();
void Story( int );
int Battle_StageSelect();
void Battle_Init();
void Battle_MapRenew();
int Battle_CheckMapHitRight( int, int );
int Battle_CheckMapHitLeft( int, int );
int Battle_CheckMapHitOver( int, int, int );
int Battle_CheckMapHitUnder( int, int, int );
void Battle_Move();
void Battle_Draw();
void Battle_DamageMove( int );
void Battle_Hit();
void Battle();
void Explanation();

/* 【メイン関数】 */
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	SetGraphMode( 640 , 480 , 16 );			//画面モードのセット
	SetOutApplicationLogValidFlag(false);	//Log.txtの出力を抑制
	SetMainWindowText( "♀tory 改" );		//タイトル変更
	ChangeWindowMode( TRUE ) ;				//ウィンドウモードに変更
	if( DxLib_Init() == -1 )				//DXライブラリ初期化処理
	{
		return -1;							//エラーが起きたら直ちに終了
	}

	InitGrp();
	InitSe();
	LoadMap();

	Intro();

	while( !(exitFlag) )
	{
		SetFontSize( 50 );

		mode = Menu();

		if( mode == START )
		{
			mess.datanum = Story_SavedataSelect( 0 );

			if( skipflag )
			{
				mess.stage = 1;
			}
			else
			{
				mess.stage = 0;
			}

			Story( 0 );
		}
		else if( mode == CONTINUE )
		{
			mess.datanum = Story_SavedataSelect( 1 );

			if( mess.datanum == 1 )
				mess = data1;
			else if( mess.datanum == 2 )
				mess = data2;
			else if( mess.datanum == 3 )
				mess = data3;

			if( mess.datanum )
				Story( 1 );
		}
		else if( mode == BATTLE )
		{
			Battle();
		}
		else if( mode == EXPLANATION )
		{
			Explanation();
		}
		else if( mode == END )
		{
			break;
		}
	}

	DxLib_End();	//DXライブラリ使用の終了処理
	return 0;		//ソフトの終了
}

/* 【関数】画像読み込み */
void InitGrp()
{
	SetDrawScreen( DX_SCREEN_BACK );		//裏画面設定
	ChangeFont( "ARハイカラＰＯＰ体H" );	//フォント設定
	SetTransColor( 0, 255, 0 );				//透過色設定
	SetBackgroundColor( 255, 255, 255 );	//背景色設定
	SetKeyInputStringColor( GetColor(50,50,50), GetColor(0,0,0), GetColor(200,200,200), GetColor(0,0,0), GetColor(50,50,50),
		GetColor(200,200,200), GetColor(50,50,50), GetColor(0,0,0), GetColor(0,0,0), GetColor(0,0,0), GetColor(0,0,0), GetColor(255,255,255) );

	grp.intro_messonly = LoadGraph( "./Mess-tory_data/Graphic/♀.png" );
	grp.intro_messtory = LoadGraph( "./Mess-tory_data/Graphic/♀tory.png" );
	grp.intro_back0 = LoadGraph( "./Mess-tory_data/Graphic/intro00.png" );
	grp.intro_back1 = LoadGraph( "./Mess-tory_data/Graphic/intro01.png" );
	grp.intro_back2 = LoadGraph( "./Mess-tory_data/Graphic/intro02.png" );
	grp.main_back = LoadGraph( "./Mess-tory_data/Graphic/back.png" );
	grp.menu_back = LoadGraph( "./Mess-tory_data/Graphic/menuback.png" );
	grp.opening = LoadGraph( "./Mess-tory_data/Graphic/opening.png" );
	grp.fukidashi = LoadGraph( "./Mess-tory_data/Graphic/fukidashi.png" );
	grp.stage1_back = LoadGraph( "./Mess-tory_data/Graphic/stage01back.png" );
	grp.stage2_back = LoadGraph( "./Mess-tory_data/Graphic/stage02back.png" );
	grp.stage3_back = LoadGraph( "./Mess-tory_data/Graphic/stage03back.png" );
	grp.stage4_back = LoadGraph( "./Mess-tory_data/Graphic/stage04back.png" );
	grp.stage5_back = LoadGraph( "./Mess-tory_data/Graphic/stage05back.png" );
	grp.stage6_back = LoadGraph( "./Mess-tory_data/Graphic/stage06back.png" );
	grp.stage7_back = LoadGraph( "./Mess-tory_data/Graphic/stage07back.png" );
	grp.stage8_back = LoadGraph( "./Mess-tory_data/Graphic/stage08back.png" );
	grp.stage9_back = LoadGraph( "./Mess-tory_data/Graphic/stage09back.png" );
	grp.stage10_back = LoadGraph( "./Mess-tory_data/Graphic/stage10back.png" );
	grp.gameover = LoadGraph( "./Mess-tory_data/Graphic/gameover.png" );
	grp.battleview1 = LoadGraph( "./Mess-tory_data/Graphic/bstageview1.png" );
	grp.battleview2 = LoadGraph( "./Mess-tory_data/Graphic/bstageview2.png" );
	grp.battleview3 = LoadGraph( "./Mess-tory_data/Graphic/bstageview3.png" );
	grp.battlestage1 = LoadGraph( "./Mess-tory_data/Graphic/bstage01back.png" );
	grp.battlestage2 = LoadGraph( "./Mess-tory_data/Graphic/bstage02back.png" );
	grp.battlestage3 = LoadGraph( "./Mess-tory_data/Graphic/bstage03back.png" );
	grp.exp1 = LoadGraph( "./Mess-tory_data/Graphic/exppage01.png" );
	grp.exp2 = LoadGraph( "./Mess-tory_data/Graphic/exppage02.png" );
	grp.exp3 = LoadGraph( "./Mess-tory_data/Graphic/exppage03.png" );
	grp.exp4 = LoadGraph( "./Mess-tory_data/Graphic/exppage04.png" );
	grp.exp5 = LoadGraph( "./Mess-tory_data/Graphic/exppage05.png" );
	LoadDivGraph( "./Mess-tory_data/Graphic/mess.png", 16, 4, 4, 100, 100, grp.mess  );
	LoadDivGraph( "./Mess-tory_data/Graphic/oss.png", 8, 4, 2, 100, 100, grp.oss );
	LoadDivGraph( "./Mess-tory_data/Graphic/enemy.png", 16, 4, 4, 32, 32, grp.enemy );
	LoadDivGraph( "./Mess-tory_data/Graphic/messdamage.png", 16, 4, 4, 100, 100, grp.damage );
	LoadDivGraph( "./Mess-tory_data/Graphic/b_mess1.png", 16, 4, 4, 100, 100, grp.battlemess1 );
	LoadDivGraph( "./Mess-tory_data/Graphic/b_mess2.png", 16, 4, 4, 100, 100, grp.battlemess2 );
	LoadDivGraph( "./Mess-tory_data/Graphic/b_mess1damage.png", 16, 4, 4, 100, 100, grp.battledamage1 );
	LoadDivGraph( "./Mess-tory_data/Graphic/b_mess2damage.png", 16, 4, 4, 100, 100, grp.battledamage2 );
	LoadDivGraph( "./Mess-tory_data/Graphic/attack.png", 8, 4, 2, 30, 30, grp.attack );
	LoadDivGraph( "./Mess-tory_data/Graphic/mapchip.png", 16, 4, 4, 32, 32, grp.mapchip );
	LoadDivGraph( "./Mess-tory_data/Graphic/item.png", 16, 4, 4, 32, 32, grp.item );
	LoadDivGraph( "./Mess-tory_data/Graphic/state.png", 4, 4, 1, 100, 100, grp.state );
}

/* 【関数】音読み込み */
void InitSe()
{
	se.select = LoadSoundMem( "./Mess-tory_data/Sound/selectsound.wav" );
	se.enter = LoadSoundMem( "./Mess-tory_data/Sound/entersound.wav" );
	se.notenter = LoadSoundMem( "./Mess-tory_data/Sound/notentersound.wav" );
	se.exit = LoadSoundMem( "./Mess-tory_data/Sound/exitsound.wav" );
	se.save = LoadSoundMem( "./Mess-tory_data/Sound/savesound.wav" );
	se.clear = LoadSoundMem( "./Mess-tory_data/Sound/clearsound.wav" );
	se.gameover = LoadSoundMem( "./Mess-tory_data/Sound/gameoversound.wav" );
	se.damage = LoadSoundMem( "./Mess-tory_data/Sound/damagesound.wav" );
	se.attack = LoadSoundMem( "./Mess-tory_data/Sound/attacksound.wav" );
	se.flash = LoadSoundMem( "./Mess-tory_data/Sound/flashsound.wav" );
	se.star = LoadSoundMem( "./Mess-tory_data/Sound/getstarsound.wav" );
	se.bigstar = LoadSoundMem( "./Mess-tory_data/Sound/getbstarsound.wav" );
	se.medicine = LoadSoundMem( "./Mess-tory_data/Sound/getmedsound.wav" );
	se.badmedicine = LoadSoundMem( "./Mess-tory_data/Sound/getbmedsound.wav" );
	se.effect = LoadSoundMem( "./Mess-tory_data/Sound/effectsound.wav" );
	se.battleend = LoadSoundMem( "./Mess-tory_data/Sound/battleendsound.wav" );
	se.expnext = LoadSoundMem( "./Mess-tory_data/Sound/nextpagesound.wav" );
}

/* 【関数】マップ読み込み */
void LoadMap()
{
	int i, j, k;
	FILE *fp1, *fp2;

	fp1 = fopen( "./Mess-tory_data/map.txt", "r" );
	fp2 = fopen( "./Mess-tory_data/battlemap.txt", "r" );
	if( fp1 == NULL || fp2 == NULL )
	{
		DxLib_End();
		exitFlag = 1;
	}

	fscanf( fp1, "%d", &maxfloor );

	for( i = 0; i < maxfloor + 1; i++ )
	{
		for( j = 0; j < STAGE_HEIGHT; j++ )
		{
			for( k = 0; k < STAGE_WIDTH; k++ )
			{
				fscanf( fp1, "%d", &map[i][j][k] );
			}
		}
	}

	for( i = 0; i < MAX_BATTLEFLOOR; i++ )
	{
		for( j = 0; j < STAGE_HEIGHT; j++ )
		{
			for( k = 0; k < STAGE_BATTLEWIDTH; k++ )
			{
				fscanf( fp2, "%d", &battlemap[i][j][k] );
			}
		}
	}

	fclose( fp1 );
	fclose( fp2 );
}

/* 【関数】入力 */
void Input()
{
	GetHitKeyStateAll( key );
}

/* 【関数】イントロ */
void Intro()
{
	int c;
	int i;
	int j = 0;

	for( c = 0; c <= 255; c++ )
	{
		if( ProcessMessage() == -1 )
			DxLib_End();

		SetDrawBright( c, c, c );
		DrawExtendGraph( 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.intro_back0, FALSE );
		ScreenFlip();
		ClearDrawScreen();
	}
	WaitTimer( 1000 );

	PlaySoundFile( "./Mess-tory_data/Sound/introsound1.wav", DX_PLAYTYPE_BACK );
	DrawExtendGraph( 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.intro_back0, FALSE );
	DrawExtendGraph( 94, 90, 304, 397, grp.intro_messonly, TRUE );
	ScreenFlip();
	WaitTimer( 1000 );
	ClearDrawScreen();

	for( i = 0; i <= 150; i++ )
	{
		if( ProcessMessage() == -1 )
			DxLib_End();

		Input();

		if( key[ KEY_INPUT_ESCAPE ] )
		{
			return;
		}

		if( i % 3 == 0 )
			j += 2;
		DrawExtendGraph( 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.intro_back0, FALSE );
		DrawExtendGraph( 94, 90 + i, 304 - j, 397, grp.intro_messonly, TRUE );
		ScreenFlip();
		ClearDrawScreen();
	}
	WaitTimer( 1000 );

	PlaySoundFile( "./Mess-tory_data/Sound/introsound2.wav", DX_PLAYTYPE_BACK );
	DrawExtendGraph( 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.intro_back0, FALSE );
	DrawExtendGraph( 94, 240, 390, 240 + 156, grp.intro_messtory, TRUE );
	ScreenFlip();
	WaitTimer( 1000 );
	ClearDrawScreen();

	for( i = 240; i > 5; i-- )
	{
		if( ProcessMessage() == -1 )
			DxLib_End();

		Input();

		if( key[ KEY_INPUT_ESCAPE ] )
		{
			return;
		}

		DrawExtendGraph( 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.intro_back0, FALSE );
		DrawExtendGraph( 94, i, 390, i + 156, grp.intro_messtory, TRUE );
		ScreenFlip();
		ClearDrawScreen();
	}
	WaitTimer( 1000 );

	PlaySoundFile( "./Mess-tory_data/Sound/introsound3.wav", DX_PLAYTYPE_BACK );
	DrawExtendGraph( 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.intro_back1, FALSE );
	ScreenFlip();
	WaitTimer( 1000 );
	ClearDrawScreen();

	PlaySoundFile( "./Mess-tory_data/Sound/introsound4.wav", DX_PLAYTYPE_BACK );
	DrawExtendGraph( 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.intro_back2, FALSE );
	ScreenFlip();
	WaitTimer( 1000 );
	ClearDrawScreen();
}

/* 【関数】メニュー */
int Menu()
{
	int menucursor_x = MENUX;
	int menucursor_y = MENUY_OVER;
	int out = 0;
	int menunum = START;

	PlayMusic( "./Mess-tory_data/Sound/menubgm.mp3", DX_PLAYTYPE_LOOP );
	while(1)
	{
		if( ProcessMessage() == -1 )	//メッセージ処理
			DxLib_End();				//DXライブラリ使用の終了処理

		Input();

		if( key[ KEY_INPUT_SPACE ] || key[ KEY_INPUT_RETURN ] )
		{
			menucursor_x += 10;
			out = 1;
		}
		if( key[ KEY_INPUT_S ] )
		{
			skipflag = 1;
			out = 1;
			menunum = START;
		}
		if( key[ KEY_INPUT_UP ] )
		{
			PlaySoundMem( se.select, DX_PLAYTYPE_BACK );
			menucursor_y -= MENUSPACE;
			menunum--;
		}
		if( key[ KEY_INPUT_DOWN ] )
		{
			PlaySoundMem( se.select, DX_PLAYTYPE_BACK );
			menucursor_y += MENUSPACE;
			menunum++;
		}

		if( menucursor_y >= MENUY_UNDER + MENUSPACE )
		{
			menucursor_y = MENUY_OVER;
			menunum = START;
		}
		else if( menucursor_y <= MENUY_OVER - MENUSPACE )
		{
			menucursor_y = MENUY_UNDER;
			menunum = END;
		}

		DrawExtendGraph( 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.menu_back, FALSE );
		DrawString( menucursor_x, menucursor_y, "♀", GetColor(255,0,0) );

		ScreenFlip();
		ClearDrawScreen();

		if( out )
		{
			PlaySoundMem( se.enter, DX_PLAYTYPE_NORMAL );
			StopMusic();
			return menunum;
		}

		WaitKey();
	}
}

/* 【関数】ストーリー：セーブデータ選択 */
int Story_SavedataSelect( int Continue )
{
	int selecty = 1;
	int select1x = 0;
	int select2x = 0;
	int select3x = 0;

	FILE *fp1, *fp2, *fp3;

	fp1 = fopen( "./Mess-tory_data/savedata1.dat", "rb" );
	fp2 = fopen( "./Mess-tory_data/savedata2.dat", "rb" );
	fp3 = fopen( "./Mess-tory_data/savedata3.dat", "rb" );

	if( fp1 == NULL || fp2 == NULL || fp3 == NULL )
		return 0;

	fread( &data1, sizeof(data1), 1, fp1 );
	fread( &data2, sizeof(data2), 1, fp2 );
	fread( &data3, sizeof(data3), 1, fp3 );

	while(1)
	{
		if( ProcessMessage() == -1 )
			DxLib_End();

		Input();

		if( key[ KEY_INPUT_SPACE ] || key[ KEY_INPUT_RETURN ] )
		{
			if( Continue && selecty == 1 && data1.stage == 0 ||
				Continue && selecty == 2 && data2.stage == 0 ||
				Continue && selecty == 3 && data3.stage == 0 )
				PlaySoundMem( se.notenter, DX_PLAYTYPE_NORMAL );
			else
			{
				PlaySoundMem( se.enter, DX_PLAYTYPE_NORMAL );
				fclose( fp1 );
				fclose( fp2 );
				fclose( fp3 );
				return selecty;
			}
		}
		else if( Continue && key[ KEY_INPUT_ESCAPE ] )
		{
			PlaySoundMem( se.exit, DX_PLAYTYPE_NORMAL );
			fclose( fp1 );
			fclose( fp2 );
			fclose( fp3 );
			return 0;
		}
		else if( key[ KEY_INPUT_UP ] )
		{
			PlaySoundMem( se.select, DX_PLAYTYPE_BACK );
			selecty--;
			if( selecty < 1 )
				selecty = 3;
		}
		else if( key[ KEY_INPUT_DOWN ] )
		{
			PlaySoundMem( se.select, DX_PLAYTYPE_BACK );
			selecty++;
			if( selecty > 3 )
				selecty = 1;
		}

		switch( selecty )
		{
		case 1:
			select1x = 30;
			select2x = 0;
			select3x = 0;
			break;
		case 2:
			select1x = 0;
			select2x = 30;
			select3x = 0;
			break;
		case 3:
			select1x = 0;
			select2x = 0;
			select3x = 30;
			break;
		}

		DrawGraph( 0, 0, grp.main_back, FALSE );

		DrawString( select1x, WINDOW_SIZE_Y / 10, "データ１", GetColor(0,0,0) );
		if( data1.stage == 0 )
			DrawString( select1x + 50, WINDOW_SIZE_Y / 10 + 50, "セーブデータ無し", GetColor(0,0,0) );
		else
			DrawFormatString( select1x + 50, WINDOW_SIZE_Y / 10 + 50, GetColor(0,0,0), "%s,ステージ：%d", data1.name, data1.stage );

		DrawString( select2x, WINDOW_SIZE_Y / 10 + 120, "データ２", GetColor(0,0,0) );
		if( data2.stage == 0 )
			DrawString( select2x + 50, WINDOW_SIZE_Y / 10 + 170, "セーブデータ無し", GetColor(0,0,0) );
		else
			DrawFormatString( select2x + 50, WINDOW_SIZE_Y / 10 + 170, GetColor(0,0,0), "%s,ステージ：%d", data2.name, data2.stage );

		DrawString( select3x, WINDOW_SIZE_Y / 10 + 240, "データ３", GetColor(0,0,0) );
		if( data3.stage == 0 )
			DrawString( select3x + 50, WINDOW_SIZE_Y / 10 + 290, "セーブデータ無し", GetColor(0,0,0) );
		else
			DrawFormatString( select3x + 50, WINDOW_SIZE_Y / 10 + 290, GetColor(0,0,0), "%s,ステージ：%d", data3.name, data3.stage );

		ScreenFlip();
		WaitKey();
		ClearDrawScreen();
	}
}

/* 【関数】ストーリー：オープニング */
void Story_Opening()
{
	int nameokay = 0;
	int over_y;
	int under_y = 1;

	SetDrawScreen( DX_SCREEN_FRONT );

	do{
		DrawGraph( 0, 0, grp.main_back, FALSE );

		DrawString( WINDOW_SIZE_X / 10, WINDOW_SIZE_Y / 3, "名前を入力してください。", GetColor(0,0,0) );
		DrawString( WINDOW_SIZE_X / 10, WINDOW_SIZE_Y / 3 + 20, "＞", GetColor(0,0,0) );
		KeyInputString( WINDOW_SIZE_X / 10 + 20, WINDOW_SIZE_Y / 3 + 20, 10, mess.name, FALSE );
		DrawFormatString( WINDOW_SIZE_X / 10 + 20, WINDOW_SIZE_Y / 3 + 20, GetColor(0,0,0), "%s", mess.name );
		DrawFormatString( WINDOW_SIZE_X / 10, WINDOW_SIZE_Y / 3 + 60, GetColor(0,0,0), "%s でよろしいですか？", mess.name );
		DrawString( WINDOW_SIZE_X / 10, WINDOW_SIZE_Y / 3 + 80, "＞YES(SpaceKey or EnterKey) / NO(AnyOtherKey)", GetColor(0,0,0) );

		WaitKey();

		if( CheckHitKey( KEY_INPUT_SPACE ) || CheckHitKey( KEY_INPUT_RETURN ) )
			nameokay = 1;
		else
			nameokay = 0;

		ClearDrawScreen();
	}while( nameokay == 0 );

	SetDrawScreen( DX_SCREEN_BACK );

	PlayMusic( "./Mess-tory_data/Sound/openingbgm.mp3", DX_PLAYTYPE_LOOP );
	for( over_y = WINDOW_SIZE_Y; under_y > 0; over_y-- )
	{
		if( ProcessMessage() == -1 )
			DxLib_End();

		Input();

		if( key[ KEY_INPUT_ESCAPE ] )
		{
			WaitTimer( 500 );
			break;
		}

		under_y = over_y + 20 * 42;

		DrawGraph( 0, 0, grp.main_back, FALSE );

		DrawFormatString( 10, over_y, GetColor(0,0,0), "%sよ。よく来てくれた。", mess.name );
		DrawString( 10, over_y + 20 * 2, "春休みの大(?)冒険\"♀tory\"。", GetColor(0,0,0) );
		DrawString( 10, over_y + 20 * 3, "この物語はその続きである。", GetColor(0,0,0) );
		DrawString( 10, over_y + 20 * 4, "まずはこれまでのあらすじを確認しておこう。", GetColor(0,0,0) );
		DrawString( 10, over_y + 20 * 6, "元々\"♀tory\"はグラフィカルで美しい世界だった。", GetColor(0,0,0) );
		DrawString( 10, over_y + 20 * 7, "主人公である\"♀\"も手足を自由に動かせたのだった。", GetColor(0,0,0) );
		DrawString( 10, over_y + 20 * 9, "しかしある時、悪者が世界を\"CUI(CharacterUserInterface)\"に、", GetColor(0,0,0) );
		DrawString( 10, over_y + 20 *10, "つまり文字だけの世界へと変えてしまったのだ。", GetColor(0,0,0) );
		DrawString( 10, over_y + 20 *12, "世界を戻す方法はただひとつ", GetColor(0,0,0) );
		DrawString( 10, over_y + 20 *13, "\"G\"を手に入れることだった。", GetColor(0,0,0) );
		DrawString( 10, over_y + 20 *14, "\"G\"は現実世界の安泰を祈るために人々が祀っていたものだったが", GetColor(0,0,0) );
		DrawString( 10, over_y + 20 *15, "悪者はそれを奪ってしまったのだ。", GetColor(0,0,0) );
		DrawString( 10, over_y + 20 *17, "そして主人公は世界を戻すべく", GetColor(0,0,0) );
		DrawString( 10, over_y + 20 *18, "悪者である\"♂\"から\"G\"を取り戻すのだった。", GetColor(0,0,0) );
		DrawExtendGraph( 10, over_y + 20 * 20, WINDOW_SIZE_X - 10, over_y + 20 * 40, grp.opening, FALSE );
		DrawString( 10, over_y + 20 *41, "そしてここから新しい物語が始まる・・・。", GetColor(200,0,0) );

		DrawBox( WINDOW_SIZE_X - 140, WINDOW_SIZE_Y - 40, WINDOW_SIZE_X - 3, WINDOW_SIZE_Y - 10, GetColor(200,200,200), TRUE );
		DrawString( WINDOW_SIZE_X - 135, WINDOW_SIZE_Y - 35, "スキップ:Esc", GetColor(0,100,0) );

		ScreenFlip();
		ClearDrawScreen();
	}

	StopMusic();
}

/* 【関数】ストーリー：オープニング後半 */
void Story_AfterOpening()
{
	int over_y;
	int under_y = 1;

	PlayMusic( "./Mess-tory_data/Sound/openingbgm.mp3", DX_PLAYTYPE_LOOP );
	for( over_y = WINDOW_SIZE_Y; under_y > 0; over_y-- )
	{
		if( ProcessMessage() == -1 )
			DxLib_End();

		Input();

		if( key[ KEY_INPUT_ESCAPE ] )
		{
			WaitTimer( 500 );
			return;
		}

		under_y = over_y + 20 * 7;

		DrawGraph( 0, 0, grp.main_back, FALSE );

		DrawString( 10, over_y + 20 * 1, "こうして世界は元に戻った。", GetColor(0,0,0) );
		DrawString( 10, over_y + 20 * 2, "しかし\"♂\"は逃げ出してしまった。", GetColor(0,0,0) );
		DrawString( 10, over_y + 20 * 3, "\"♂\"を倒さなければ再び文字の世界へ変えられてしまうだろう。", GetColor(0,0,0) );
		DrawFormatString( 10, over_y + 20 * 4, GetColor(0,0,0), "%s。あなたが\"♂\"を倒すのだ。", mess.name );
		DrawString( 10, over_y + 20 * 6, "世界の平和のために・・・。", GetColor(200,0,0) );

		DrawBox( WINDOW_SIZE_X - 140, WINDOW_SIZE_Y - 40, WINDOW_SIZE_X - 3, WINDOW_SIZE_Y - 10, GetColor(200,200,200), TRUE );
		DrawString( WINDOW_SIZE_X - 135, WINDOW_SIZE_Y - 35, "スキップ:Esc", GetColor(0,100,0) );

		ScreenFlip();
		ClearDrawScreen();
	}

	StopMusic();
}

/* 【関数】ストーリー：エンディング */
void Story_Ending()
{
	int over_y;
	int under_y = 1;

	PlayMusic( "./Mess-tory_data/Sound/endingbgm.mp3", DX_PLAYTYPE_LOOP );
	for( over_y = WINDOW_SIZE_Y; under_y > 0; over_y-- )
	{
		if( ProcessMessage() == -1 )
			DxLib_End();

		Input();

		if( key[ KEY_INPUT_ESCAPE ] )
		{
			WaitTimer( 500 );
			break;
		}

		under_y = over_y + 20 * 9;

		DrawGraph( 0, 0, grp.main_back, FALSE );

		DrawString( 10, over_y + 20 * 1, "おめでとう！！！", GetColor(0,0,0) );
		DrawString( 10, over_y + 20 * 2, "君は見事\"♂\"を倒してくれた！", GetColor(0,0,0) );
		DrawString( 10, over_y + 20 * 3, "これでもう\"♂\"は悪いことに懲りただろう。", GetColor(0,0,0) );
		DrawFormatString( 10, over_y + 20 * 4, GetColor(0,0,0), "%s。あなたが世界を救ってくれたのだ。", mess.name );
		DrawString( 10, over_y + 20 * 6, "世界を救ってくれて、", GetColor(200,0,0) );
		DrawString( 10, over_y + 20 * 7, "最後まで遊んでくれて、", GetColor(200,0,0) );
		DrawString( 10, over_y + 20 * 8, "どうもありがとう☆彡", GetColor(210,0,0) );

		DrawBox( WINDOW_SIZE_X - 140, WINDOW_SIZE_Y - 40, WINDOW_SIZE_X - 3, WINDOW_SIZE_Y - 10, GetColor(200,200,200), TRUE );
		DrawString( WINDOW_SIZE_X - 135, WINDOW_SIZE_Y - 35, "スキップ:Esc", GetColor(0,100,0) );

		ScreenFlip();
		ClearDrawScreen();
	}
	StopMusic();

	PlaySoundMem( se.clear, DX_PLAYTYPE_BACK );

	point += (life * 100);
	do{
		if( ProcessMessage() == -1 )
			DxLib_End();

		DrawGraph( 0, 0, grp.main_back, FALSE );

		DrawBox( 200, 200, 400, 280, GetColor(200,200,200), TRUE );

		DrawFormatString( 210, 210, GetColor(0,0,0), "%sのスコア", mess.name );
		DrawFormatString( 230, 250, GetColor(0,200,0), "%dポイント！", point );

		ScreenFlip();
		ClearDrawScreen();
	}while( CheckHitKey( KEY_INPUT_RETURN ) != 1 && CheckHitKey( KEY_INPUT_SPACE ) != 1 );

	WaitTimer( 500 );
}

/* 【関数】ストーリー：ステージ情報取得 */
void Story_StageInput()
{
	int i, j;

	for( i = 0; i < STAGE_HEIGHT; i++ )
	{
		for( j = 0; j < STAGE_WIDTH; j++ )
		{
			stage[ i ][ j ] = map[ mess.stage ][ i ][ j ];
		}
	}
}

/* 【関数】ストーリー：初期化 */
void Story_Init( int Continue )
{
	switch( mess.stage )
	{
	case 0:
		PlayMusic( "./Mess-tory_data/Sound/♀torybgm.mp3", DX_PLAYTYPE_LOOP );
		mess.x = 15 * MAP_SIZE;
		mess.y = 1 * MAP_SIZE;
		mess.mapchipx = 0;

		ossx = 5 * MAP_SIZE;
		ossy = 7 * MAP_SIZE;

		enemy1exist = 0;

		enemy2exist = 0;
		break;
	case 1:
		PlayMusic( "./Mess-tory_data/Sound/stage01bgm.mp3", DX_PLAYTYPE_LOOP );
		stagebackx = 0;

		enemy1exist = 1;
		enemy1life = 3;
		enemy1x = 50 * MAP_SIZE;
		enemy1y = 12 * MAP_SIZE;

		enemy2exist = 0;

		if( !(Continue) )
		{
			mess.x = 8 * MAP_SIZE;
			mess.y = 1 * MAP_SIZE;
			mess.mapchipx = 0;
		}
		break;
	case 2:
		PlayMusic( "./Mess-tory_data/Sound/stage02bgm.mp3", DX_PLAYTYPE_LOOP );
		stagebackx = 0;

		enemy1exist = 1;
		enemy1life = 3;
		enemy1x = 22 * MAP_SIZE;
		enemy1y = 12 * MAP_SIZE;

		enemy2exist = 0;

		if( !(Continue) )
		{
			mess.x = 1 * MAP_SIZE;
			mess.y = 11 * MAP_SIZE;
			mess.mapchipx = 0;
		}
		break;
	case 3:
		PlayMusic( "./Mess-tory_data/Sound/stage03bgm.mp3", DX_PLAYTYPE_LOOP );
		stagebackx = 0;

		enemy1exist = 0;

		enemy2exist = 1;
		enemy2life = 10;
		enemy2x = 51 * MAP_SIZE;
		enemy2y = 13 * MAP_SIZE;

		if( !(Continue) )
		{
			mess.x = 1 * MAP_SIZE;
			mess.y = 10 * MAP_SIZE;
			mess.mapchipx = 0;
		}
		break;
	case 4:
		PlayMusic( "./Mess-tory_data/Sound/stage04bgm.mp3", DX_PLAYTYPE_LOOP );
		stagebackx = 0;

		enemy1exist = 0;

		enemy2exist = 1;
		enemy2life = 10;
		enemy2x = 54 * MAP_SIZE;
		enemy2y = 13 * MAP_SIZE;

		if( !(Continue) )
		{
			mess.x = 1 * MAP_SIZE;
			mess.y = 9 * MAP_SIZE;
			mess.mapchipx = 0;
		}
		break;
	case 5:
		PlayMusic( "./Mess-tory_data/Sound/stage05bgm.mp3", DX_PLAYTYPE_LOOP );
		stagebackx = 0;

		enemy1exist = 1;
		enemy1life = 3;
		enemy1x = 20 * MAP_SIZE;
		enemy1y = 13 * MAP_SIZE;

		enemy2exist = 1;
		enemy2life = 10;
		enemy2x = 54 * MAP_SIZE;
		enemy2y = 13 * MAP_SIZE;

		ossx = 57 * MAP_SIZE;
		ossy = 2 * MAP_SIZE;
		osssay = 1;
		ossmove1 = 0;
		ossmove2 = 0;

		if( !(Continue) )
		{
			mess.x = 1 * MAP_SIZE;
			mess.y = 12 * MAP_SIZE;
			mess.mapchipx = 0;
		}
		break;
	case 6:
		PlayMusic( "./Mess-tory_data/Sound/stage06bgm.mp3", DX_PLAYTYPE_LOOP );
		stagebackx = 0;

		enemy1exist = 0;

		enemy2exist = 0;

		if( !(Continue) )
		{
			mess.x = 1 * MAP_SIZE;
			mess.y = 12 * MAP_SIZE;
			mess.mapchipx = 0;
		}
		break;
	case 7:
		PlayMusic( "./Mess-tory_data/Sound/stage07bgm.mp3", DX_PLAYTYPE_LOOP );
		stagebackx = 0;

		enemy1exist = 1;
		enemy1life = 3;
		enemy1x = 55 * MAP_SIZE;
		enemy1y = 13 * MAP_SIZE;

		enemy2exist = 0;

		if( !(Continue) )
		{
			mess.x = 1 * MAP_SIZE;
			mess.y = 12 * MAP_SIZE;
			mess.mapchipx = 0;
		}
		break;
	case 8:
		PlayMusic( "./Mess-tory_data/Sound/stage08bgm.mp3", DX_PLAYTYPE_LOOP );
		stagebackx = 0;

		enemy1exist = 0;

		enemy2exist = 1;
		enemy2life = 10;
		enemy2x = 55 * MAP_SIZE;
		enemy2y = 13 * MAP_SIZE;

		if( !(Continue) )
		{
			mess.x = 1 * MAP_SIZE;
			mess.y = 12 * MAP_SIZE;
			mess.mapchipx = 0;
		}
		break;
	case 9:
		PlayMusic( "./Mess-tory_data/Sound/stage09bgm.mp3", DX_PLAYTYPE_LOOP );
		stagebackx = 0;

		enemy1exist = 1;
		enemy1life =  3;
		enemy1x = 55 * MAP_SIZE;
		enemy1y = 13 * MAP_SIZE;

		enemy2exist = 1;
		enemy2life = 10;
		enemy2x = 45 * MAP_SIZE;
		enemy2y = 13 * MAP_SIZE;

		if( !(Continue) )
		{
			mess.x = 1 * MAP_SIZE;
			mess.y = 10 * MAP_SIZE;
			mess.mapchipx = 0;
		}
		break;
	case 10:
		PlayMusic( "./Mess-tory_data/Sound/stage10bgm.mp3", DX_PLAYTYPE_LOOP );
		stagebackx = 0;

		enemy1exist = 0;

		enemy2exist = 0;

		ossx = 50 * MAP_SIZE;
		ossy = 8 * MAP_SIZE;
		ossdirection = 1;

		if( !(Continue) )
		{
			mess.x = 1 * MAP_SIZE;
			mess.y = 9 * MAP_SIZE;
			mess.mapchipx = 0;
		}
		break;
	}

	isJumping = 0;
	enemy1direction = 1;
	enemy2direction = 1;
	energy = 100;
}

/* 【関数】ストーリー：ダメージ後初期化 */
void Story_InitAfterDamage()
{
	FILE *fp;

	/* 読み込み */
	if( mess.datanum == 1 )
		fp = fopen( "./Mess-tory_data/savedata1.dat", "rb" );
	else if( mess.datanum == 2 )
		fp = fopen( "./Mess-tory_data/savedata2.dat", "rb" );
	else if( mess.datanum == 3 )
		fp = fopen( "./Mess-tory_data/savedata3.dat", "rb" );

	if( fp == NULL )
		return;

	fread( &mess, sizeof(mess), 1, fp );

	fclose( fp );
}

/* 【関数】ストーリー：拡大マップ更新 */
void Story_MapRenew()
{
	int i, j;

	for( i = 0; i < STAGE_HEIGHT * MAP_SIZE; i++ )
	{
		for( j = 0; j < STAGE_WIDTH * MAP_SIZE; j++ )
			changesizemap[ i ][ j ] = stage[ i / MAP_SIZE ][ j / MAP_SIZE ];
	}
}

/* 【関数】ストーリー：モノを消去 */
void Story_ChangeEmpty( int What )
{
	stage[ ( mess.y + CHARA_SIZE / 2 ) / MAP_SIZE ][ ( mess.x + CHARA_SIZE / 2 - mess.mapchipx ) / MAP_SIZE ]  = EMPTY;
	stage[ ( mess.y + CHARA_SIZE / 2 - 1 ) / MAP_SIZE ][ ( mess.x + CHARA_SIZE / 2 - mess.mapchipx ) / MAP_SIZE ]  = EMPTY;
}

/* 【関数】ストーリー：マップ当たり判定・右 */
int Story_CheckMapHitRight()
{
	int i;
	int isMapHit = 0;

	for( i = mess.y; i < mess.y + CHARA_SIZE; i++ )
	{
		if( changesizemap[ i ][ mess.x + CHARA_SIZE - mess.mapchipx - 1 ] != EMPTY &&
			changesizemap[ i ][ mess.x + CHARA_SIZE - mess.mapchipx - 1 ] != NEEDLE &&
			changesizemap[ i ][ mess.x + CHARA_SIZE - mess.mapchipx - 1 ] != SAVE &&
			changesizemap[ i ][ mess.x + CHARA_SIZE - mess.mapchipx - 1 ] != GOAL &&
			changesizemap[ i ][ mess.x + CHARA_SIZE - mess.mapchipx - 1 ] != G &&
			changesizemap[ i ][ mess.x + CHARA_SIZE - mess.mapchipx - 1 ] != C_NEEDLE &&
			changesizemap[ i ][ mess.x + CHARA_SIZE - mess.mapchipx - 1 ] != BROKEN_FLOOR &&
			changesizemap[ i ][ mess.x + CHARA_SIZE - mess.mapchipx - 1 ] != STAR &&
			changesizemap[ i ][ mess.x + CHARA_SIZE - mess.mapchipx - 1 ] != BSTAR &&
			changesizemap[ i ][ mess.x + CHARA_SIZE - mess.mapchipx - 1 ] != LIFE &&
			changesizemap[ i ][ mess.x + CHARA_SIZE - mess.mapchipx - 1 ] != ATTACK &&
			changesizemap[ i ][ mess.x + CHARA_SIZE - mess.mapchipx - 1 ] != INV &&
			changesizemap[ i ][ mess.x + CHARA_SIZE - mess.mapchipx - 1 ] != E &&
			changesizemap[ i ][ mess.x + CHARA_SIZE - mess.mapchipx - 1 ] != B &&
			changesizemap[ i ][ mess.x + CHARA_SIZE - mess.mapchipx - 1 ] != J &&
			changesizemap[ i ][ mess.x + CHARA_SIZE - mess.mapchipx - 1 ] != D &&
			changesizemap[ i ][ mess.x + CHARA_SIZE - mess.mapchipx - 1 ] != TIMER &&
			changesizemap[ i ][ mess.x + CHARA_SIZE - mess.mapchipx - 1 ] != BROKEN_LIFE &&
			changesizemap[ i ][ mess.x + CHARA_SIZE - mess.mapchipx - 1 ] != BROKEN_ATTACK &&
			changesizemap[ i ][ mess.x + CHARA_SIZE - mess.mapchipx - 1 ] != L_R &&
			changesizemap[ i ][ mess.x + CHARA_SIZE - mess.mapchipx - 1 ] != S_DOWN &&
			changesizemap[ i ][ mess.x + CHARA_SIZE - mess.mapchipx - 1 ] != J_LIMIT )
			isMapHit = 1;
	}

	return isMapHit;
}

/* 【関数】ストーリー：マップ当たり判定・左 */
int Story_CheckMapHitLeft()
{
	int i;
	int isMapHit = 0;

	for( i = mess.y; i < mess.y + CHARA_SIZE; i++ )
	{
		if( changesizemap[ i ][ mess.x - mess.mapchipx + 1 ] != EMPTY &&
			changesizemap[ i ][ mess.x - mess.mapchipx + 1 ] != NEEDLE &&
			changesizemap[ i ][ mess.x - mess.mapchipx + 1 ] != SAVE &&
			changesizemap[ i ][ mess.x - mess.mapchipx + 1 ] != GOAL &&
			changesizemap[ i ][ mess.x - mess.mapchipx + 1 ] != G &&
			changesizemap[ i ][ mess.x - mess.mapchipx + 1 ] != C_NEEDLE &&
			changesizemap[ i ][ mess.x - mess.mapchipx + 1 ] != BROKEN_FLOOR &&
			changesizemap[ i ][ mess.x - mess.mapchipx + 1 ] != STAR &&
			changesizemap[ i ][ mess.x - mess.mapchipx + 1 ] != BSTAR &&
			changesizemap[ i ][ mess.x - mess.mapchipx + 1 ] != LIFE &&
			changesizemap[ i ][ mess.x - mess.mapchipx + 1 ] != ATTACK &&
			changesizemap[ i ][ mess.x - mess.mapchipx + 1 ] != INV &&
			changesizemap[ i ][ mess.x - mess.mapchipx + 1 ] != E &&
			changesizemap[ i ][ mess.x - mess.mapchipx + 1 ] != B &&
			changesizemap[ i ][ mess.x - mess.mapchipx + 1 ] != J &&
			changesizemap[ i ][ mess.x - mess.mapchipx + 1 ] != D &&
			changesizemap[ i ][ mess.x - mess.mapchipx + 1 ] != TIMER &&
			changesizemap[ i ][ mess.x - mess.mapchipx + 1 ] != BROKEN_LIFE &&
			changesizemap[ i ][ mess.x - mess.mapchipx + 1 ] != BROKEN_ATTACK &&
			changesizemap[ i ][ mess.x - mess.mapchipx + 1 ] != L_R &&
			changesizemap[ i ][ mess.x - mess.mapchipx + 1 ] != S_DOWN &&
			changesizemap[ i ][ mess.x - mess.mapchipx + 1 ] != J_LIMIT )
			isMapHit = 1;
	}

	return isMapHit;
}

/* 【関数】ストーリー：マップ当たり判定・上 */
int Story_CheckMapHitOver( int look )
{
	int isMapHit = 0;


	if( changesizemap[ mess.y + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != EMPTY &&
		changesizemap[ mess.y + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != NEEDLE &&
		changesizemap[ mess.y + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != SAVE &&
		changesizemap[ mess.y + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != GOAL &&
		changesizemap[ mess.y + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != G &&
		changesizemap[ mess.y + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != C_NEEDLE &&
		changesizemap[ mess.y + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != BROKEN_FLOOR &&
		changesizemap[ mess.y + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != STAR &&
		changesizemap[ mess.y + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != BSTAR &&
		changesizemap[ mess.y + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != LIFE &&
		changesizemap[ mess.y + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != ATTACK &&
		changesizemap[ mess.y + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != INV &&
		changesizemap[ mess.y + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != E &&
		changesizemap[ mess.y + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != B &&
		changesizemap[ mess.y + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != J &&
		changesizemap[ mess.y + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != D &&
		changesizemap[ mess.y + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != TIMER &&
		changesizemap[ mess.y + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != BROKEN_LIFE &&
		changesizemap[ mess.y + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != BROKEN_ATTACK &&
		changesizemap[ mess.y + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != L_R &&
		changesizemap[ mess.y + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != S_DOWN &&
		changesizemap[ mess.y + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != J_LIMIT )
		isMapHit = 1;

	return isMapHit;
}

/* 【関数】ストーリー：マップ当たり判定・下 */
int Story_CheckMapHitUnder( int look )
{
	int isMapHit = 0;

	if( changesizemap[ mess.y + CHARA_SIZE + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != EMPTY &&
		changesizemap[ mess.y + CHARA_SIZE + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != NEEDLE &&
		changesizemap[ mess.y + CHARA_SIZE + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != SAVE &&
		changesizemap[ mess.y + CHARA_SIZE + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != GOAL &&
		changesizemap[ mess.y + CHARA_SIZE + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != G &&
		changesizemap[ mess.y + CHARA_SIZE + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != C_NEEDLE &&
		changesizemap[ mess.y + CHARA_SIZE + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != BROKEN_FLOOR &&
		changesizemap[ mess.y + CHARA_SIZE + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != STAR &&
		changesizemap[ mess.y + CHARA_SIZE + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != BSTAR &&
		changesizemap[ mess.y + CHARA_SIZE + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != LIFE &&
		changesizemap[ mess.y + CHARA_SIZE + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != ATTACK &&
		changesizemap[ mess.y + CHARA_SIZE + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != INV &&
		changesizemap[ mess.y + CHARA_SIZE + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != E &&
		changesizemap[ mess.y + CHARA_SIZE + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != B &&
		changesizemap[ mess.y + CHARA_SIZE + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != J &&
		changesizemap[ mess.y + CHARA_SIZE + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != D &&
		changesizemap[ mess.y + CHARA_SIZE + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != TIMER &&
		changesizemap[ mess.y + CHARA_SIZE + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != BROKEN_LIFE &&
		changesizemap[ mess.y + CHARA_SIZE + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != BROKEN_ATTACK &&
		changesizemap[ mess.y + CHARA_SIZE + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != L_R &&
		changesizemap[ mess.y + CHARA_SIZE + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != S_DOWN &&
		changesizemap[ mess.y + CHARA_SIZE + look ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] != J_LIMIT )
		isMapHit = 1;

	return isMapHit;
}

/* 【関数】ストーリー：表示 */
void Story_Draw()
{
	int i, j;
	static int movecount = 0;
	static int ossmovecount = 0;
	static int attackmoverightcount = 0;
	static int attackmoveleftcount = 7;
	static int enemy1movecount = 0;
	static int enemy2movecount = 0;

	stagebackx = stagebackx % WINDOW_SIZE_X;

	if( mess.stage == 1 )
	{
		DrawExtendGraph( stagebackx, 0, stagebackx + WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.stage1_back, FALSE );
		DrawExtendGraph( stagebackx + WINDOW_SIZE_X, 0, stagebackx + WINDOW_SIZE_X * 2, WINDOW_SIZE_Y, grp.stage1_back, FALSE );
		DrawExtendGraph( stagebackx - WINDOW_SIZE_X, 0, stagebackx, WINDOW_SIZE_Y, grp.stage1_back, FALSE );
	}
	else if( mess.stage == 2 )
	{
		DrawExtendGraph( stagebackx, 0, stagebackx + WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.stage2_back, FALSE );
		DrawExtendGraph( stagebackx + WINDOW_SIZE_X, 0, stagebackx + WINDOW_SIZE_X * 2, WINDOW_SIZE_Y, grp.stage2_back, FALSE );
		DrawExtendGraph( stagebackx - WINDOW_SIZE_X, 0, stagebackx, WINDOW_SIZE_Y, grp.stage2_back, FALSE );
	}
	else if( mess.stage == 3 )
	{
		DrawExtendGraph( stagebackx, 0, stagebackx + WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.stage3_back, FALSE );
		DrawExtendGraph( stagebackx + WINDOW_SIZE_X, 0, stagebackx + WINDOW_SIZE_X * 2, WINDOW_SIZE_Y, grp.stage3_back, FALSE );
		DrawExtendGraph( stagebackx - WINDOW_SIZE_X, 0, stagebackx, WINDOW_SIZE_Y, grp.stage3_back, FALSE );
	}
	else if( mess.stage == 4 )
	{
		DrawExtendGraph( stagebackx, 0, stagebackx + WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.stage4_back, FALSE );
		DrawExtendGraph( stagebackx + WINDOW_SIZE_X, 0, stagebackx + WINDOW_SIZE_X * 2, WINDOW_SIZE_Y, grp.stage4_back, FALSE );
		DrawExtendGraph( stagebackx - WINDOW_SIZE_X, 0, stagebackx, WINDOW_SIZE_Y, grp.stage4_back, FALSE );
	}
	else if( mess.stage == 5 )
	{
		DrawExtendGraph( stagebackx, 0, stagebackx + WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.stage5_back, FALSE );
		DrawExtendGraph( stagebackx + WINDOW_SIZE_X, 0, stagebackx + WINDOW_SIZE_X * 2, WINDOW_SIZE_Y, grp.stage5_back, FALSE );
		DrawExtendGraph( stagebackx - WINDOW_SIZE_X, 0, stagebackx, WINDOW_SIZE_Y, grp.stage5_back, FALSE );
	}
	else if( mess.stage == 6 )
	{
		DrawExtendGraph( stagebackx, 0, stagebackx + WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.stage6_back, FALSE );
		DrawExtendGraph( stagebackx + WINDOW_SIZE_X, 0, stagebackx + WINDOW_SIZE_X * 2, WINDOW_SIZE_Y, grp.stage6_back, FALSE );
		DrawExtendGraph( stagebackx - WINDOW_SIZE_X, 0, stagebackx, WINDOW_SIZE_Y, grp.stage6_back, FALSE );
	}
	else if( mess.stage == 7 )
	{
		DrawExtendGraph( stagebackx, 0, stagebackx + WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.stage7_back, FALSE );
		DrawExtendGraph( stagebackx + WINDOW_SIZE_X, 0, stagebackx + WINDOW_SIZE_X * 2, WINDOW_SIZE_Y, grp.stage7_back, FALSE );
		DrawExtendGraph( stagebackx - WINDOW_SIZE_X, 0, stagebackx, WINDOW_SIZE_Y, grp.stage7_back, FALSE );
	}
	else if( mess.stage == 8 )
	{
		DrawExtendGraph( stagebackx, 0, stagebackx + WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.stage8_back, FALSE );
		DrawExtendGraph( stagebackx + WINDOW_SIZE_X, 0, stagebackx + WINDOW_SIZE_X * 2, WINDOW_SIZE_Y, grp.stage8_back, FALSE );
		DrawExtendGraph( stagebackx - WINDOW_SIZE_X, 0, stagebackx, WINDOW_SIZE_Y, grp.stage8_back, FALSE );
	}
	else if( mess.stage == 9 )
	{
		DrawExtendGraph( stagebackx, 0, stagebackx + WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.stage9_back, FALSE );
		DrawExtendGraph( stagebackx + WINDOW_SIZE_X, 0, stagebackx + WINDOW_SIZE_X * 2, WINDOW_SIZE_Y, grp.stage9_back, FALSE );
		DrawExtendGraph( stagebackx - WINDOW_SIZE_X, 0, stagebackx, WINDOW_SIZE_Y, grp.stage9_back, FALSE );
	}
	else if( mess.stage == 10 )
	{
		DrawExtendGraph( stagebackx, 0, stagebackx + WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.stage10_back, FALSE );
		DrawExtendGraph( stagebackx + WINDOW_SIZE_X, 0, stagebackx + WINDOW_SIZE_X * 2, WINDOW_SIZE_Y, grp.stage10_back, FALSE );
		DrawExtendGraph( stagebackx - WINDOW_SIZE_X, 0, stagebackx, WINDOW_SIZE_Y, grp.stage10_back, FALSE );
	}

	for( i = 0; i < STAGE_HEIGHT; i++ )
	{
		for( j = 0; j < STAGE_WIDTH; j++ )
		{
			if( stage[ i ][ j ] == EMPTY || stage[ i ][ j ] == NO_ENTRY )
				continue;
			else if( stage[ i ][ j ] < STAR )
				DrawGraph( mess.mapchipx + j * MAP_SIZE, i * MAP_SIZE, grp.mapchip[ stage[i][j] - 1 ], TRUE );
			else if( stage[ i ][ j ] <= J_LIMIT )
				DrawGraph( mess.mapchipx + j * MAP_SIZE, i* MAP_SIZE, grp.item[ stage[i][j] - 17 ], TRUE );
		}
	}

	if( mess.stage != 0 )
	{
		DrawFormatString( 5, 5, GetColor(0,255,0), "%s　LIFE:%d  POINT:%d", mess.name, life, point );
		if( InvTime )
		{
			DrawExtendGraph( 5, 30, 45, 70, grp.state[ 0 ], TRUE );
			DrawBox( 5, 70, 5 + InvTime / 10, 80, GetColor(0,0,200), TRUE );
		}
		if( isProtecting )
			DrawExtendGraph( 5, 80, 45, 120, grp.state[ 1 ], TRUE );
		if( isReversing )
			DrawExtendGraph( 5, 120, 45, 160, grp.state[ 2 ], TRUE );
		if( isJ_Restricting )
			DrawExtendGraph( 5, 160, 45, 200, grp.state[ 3 ], TRUE );
		DrawBox( 500, 5, 500 + energy, 25, GetColor(0,200,0), TRUE );
		DrawLine( 500, 26, 500, 31, GetColor(255,0,0) );
		DrawLine( 500 + 20, 26, 500 + 20, 31, GetColor(200,50,0) );
		DrawLine( 500 + 40, 26, 500 + 40, 31, GetColor(100,50,0) );
		DrawLine( 500 + 60, 26, 500 + 60, 31, GetColor(50,100,0) );
		DrawLine( 500 + 80, 26, 500 + 80, 31, GetColor(0,200,0) );
		DrawLine( 600, 26, 600, 31, GetColor(0,255,0) );
	}


	movecount++;

	if( isMovingRight )
	{
		if( movecount >= 4 )
			movecount = 0;
	}
	else if( isMovingLeft )
	{
		if( movecount < 4 || movecount >= 8 )
			movecount = 4;
	}
	else if( isJumping )
	{
		if( movecount < 8 || movecount >= 12 )
			movecount = 8;
	}
	else if( !( isDamaging ) )
		if( movecount < 12 || movecount >= 14 )
			movecount = 12;

	if( isDamaging && movecount >= 4 )
		movecount = 0;

	if( isDamaging )
		DrawExtendGraph( mess.x, mess.y, mess.x + CHARA_SIZE, mess.y + CHARA_SIZE, grp.damage[ movecount ], TRUE );
	else
		DrawExtendGraph( mess.x, mess.y, mess.x + CHARA_SIZE, mess.y + CHARA_SIZE, grp.mess[ movecount ], TRUE );

	if( isAttack )
	{
		attackrightx = mess.x + CHARA_SIZE + 30;
		attackleftx = mess.x - 30;

		DrawGraph( attackrightx - 30, mess.y + CHARA_SIZE / 2, grp.attack[ attackmoverightcount ], TRUE );
		DrawGraph( attackleftx, mess.y + CHARA_SIZE / 2, grp.attack[ attackmoveleftcount ], TRUE );

		attackmoverightcount++;
		if( attackmoverightcount == 4 )
		{
			attackmoverightcount = 0;
			isAttack = 0;
		}

		attackmoveleftcount--;
		if( attackmoveleftcount == 3 )
		{
			attackmoveleftcount = 7;
			isAttack = 0;
		}
	}

	/* ♂ */
	if( mess.stage == 0 || mess.stage == 5 )
	{
		ossmovecount++;

		if( ossmove1 == 0 && ossmove2 == 0 )
		{
			ossmovecount = ossmovecount % 4;

			DrawExtendGraph( mess.mapchipx + ossx, ossy, mess.mapchipx + ossx + CHARA_SIZE, ossy + CHARA_SIZE, grp.oss[ ossmovecount ], TRUE );
		}
		else if( ossmove1 == 1 || ossmove2 == 1 )
		{
			if( ossmovecount < 4 || ossmovecount >= 8 )
				ossmovecount = 4;

			if( ossmove1 == 1 && ossmove2 == 0 )
				DrawExtendGraph( mess.mapchipx + ossx, ossy, mess.mapchipx + ossx + CHARA_SIZE, ossy + CHARA_SIZE, grp.oss[ ossmovecount ], TRUE );
			else if( ossmove1 == 0 && ossmove2 == 1 )
				DrawRotaGraph( mess.mapchipx + ossx + CHARA_SIZE / 2, ossy + CHARA_SIZE / 2, 0.64, PI / 2, grp.oss[ ossmovecount ], TRUE, FALSE );
		}

		if( osssay )
			DrawGraph( mess.mapchipx + ossx - 60, ossy - 60, grp.fukidashi, TRUE );

		if( mess.stage == 0 )
		{
			if( osssay == 1 )
				DrawString( mess.mapchipx + ossx - 50, ossy - 50, "やめろ！\"G\"を取るな！",GetColor(0,0,0) );
			else if( osssay == 2 )
				DrawString( mess.mapchipx + ossx - 50, ossy - 50, "ぐわあぁああぁっ・・・",GetColor(0,0,0) );
			else if( osssay == 3 )
				DrawString( mess.mapchipx + ossx - 50, ossy - 50, "まだ終わらんぞ・・・！",GetColor(0,0,100) );
		}
		else if( mess.stage == 5 )
		{
			if( osssay == 1 )
				DrawString( mess.mapchipx + ossx - 50, ossy - 50, "よくぞここまできたな。",GetColor(0,0,0) );
			else if( osssay == 2 )
				DrawString( mess.mapchipx + ossx - 50, ossy - 50, "次に会う時が最期だ・・",GetColor(0,0,0) );
		}
	}
	else if( mess.stage == 10 )
	{
		ossmovecount++;
		if( ossmovecount < 4 || ossmovecount >= 8 )
			ossmovecount = 4;

		if( ossdirection == 0 )
			DrawRotaGraph( mess.mapchipx + ossx + CHARA_SIZE / 2, ossy + CHARA_SIZE / 2, 0.64, (3 * PI) / 2, grp.oss[ ossmovecount ], TRUE, FALSE );
		else if( ossdirection == 1 )
			DrawRotaGraph( mess.mapchipx + ossx + CHARA_SIZE / 2, ossy + CHARA_SIZE / 2, 0.64, PI / 2, grp.oss[ ossmovecount ], TRUE, FALSE );

		if( osslife >= 0 )
			DrawBox( 500, 35, 500 + osslife, 55, GetColor(0,0,200), TRUE );
	}

	/* 敵1,2 */
	if( enemy1exist )
	{
		enemy1movecount++;

		if( enemy1direction == 0 )
			enemy1movecount = enemy1movecount % 4;
		else if( enemy1direction == 1 )
		{
			if( enemy1movecount < 4 || enemy1movecount >= 8 )
				enemy1movecount = 4;
		}

		DrawGraph( enemy1x + mess.mapchipx, enemy1y, grp.enemy[ enemy1movecount ], TRUE );
		DrawBox( enemy1x + mess.mapchipx, enemy1y - 10, enemy1x + enemy1life * 10 + mess.mapchipx, enemy1y - 5, GetColor(0,0,200), TRUE );
	}
	if( enemy2exist )
	{
		enemy2movecount++;

		if( enemy2direction == 0 )
		{
			if( enemy2movecount < 8 || enemy2movecount >= 12 )
				enemy2movecount = 8;
		}
		else if( enemy2direction == 1 )
		{
			if( enemy2movecount < 12 || enemy2movecount >= 16 )
				enemy2movecount = 12;
		}

		DrawGraph( enemy2x + mess.mapchipx, enemy2y, grp.enemy[ enemy2movecount ], TRUE );
		DrawBox( enemy2x + mess.mapchipx, enemy2y - 10, enemy2x + enemy2life * 10 + mess.mapchipx, enemy2y - 5, GetColor(0,0,200), TRUE );
	}

	ScreenFlip();
	WaitTimer( 80 );
	ClearDrawScreen();
}

/* 【関数】ストーリー：点滅 */
void Story_Flash( int ChangeBlock )
{
	int flash;
	int i, j;

	for( flash = 10; flash >= 1; flash-- )
	{
		Story_Draw();
		WaitTimer( flash * 100 );
		PlaySoundMem( se.flash, DX_PLAYTYPE_BACK );
		DrawBox( 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, GetColor(255,255,255), TRUE );
		ScreenFlip();
		WaitTimer( flash * 100 );
		ClearDrawScreen();
	}

	if( ChangeBlock )
	{
		for( i = 0; i < STAGE_HEIGHT; i++ )
		{
			for( j = 0; j < STAGE_WIDTH; j++ )
			{
				if( stage[ i ][ j ] == BLOCK )
					stage[ i ][ j ] = BRICK;
				else if( stage[ i ][ j ] == C_NEEDLE )
					stage[ i ][ j ] = NEEDLE;
			}
		}
	}
}

/* 【関数】ストーリー：♂逃げ移動 */
void Story_EscapeOss()
{
	ossmove1 = 1;
	ossmove2 = 0;

	while( ossy > 1 * MAP_SIZE )
	{
		ossy -= 10;
		Story_Draw();
	}

	ossmove1 = 0;
	ossmove2 = 1;
	osssay = 0;

	if( mess.stage == 0 )
	{
		while( ossx < WINDOW_SIZE_X )
		{
			ossx += 20;

			if( changesizemap[ ossy + CHARA_SIZE / 2 ][ ossx + CHARA_SIZE - mess.mapchipx ] == BLOCK )
			{
				stage[ 1 ][ 19 ] = EMPTY;
				stage[ 2 ][ 19 ] = EMPTY;
			}

			Story_Draw();
		}
	}
	else if( mess.stage == 5 )
	{
		stage[ 1 ][ 59 ] = EMPTY;
		stage[ 2 ][ 59 ] = EMPTY;

		while( ossx < STAGE_WIDTH * MAP_SIZE )
		{
			ossx += 10;

			Story_Draw();
		}
	}
}

/* 【関数】ストーリー：ダメージ移動 */
void Story_DamageMove()
{
	int damagey0 = -30;
	int damageg = 5;
	int damagey;

	isDamaging = 1;

	damagey = damagey0;

	PlaySoundMem( se.damage, DX_PLAYTYPE_BACK );

	while( mess.y <= WINDOW_SIZE_Y )
	{
		if( ProcessMessage() == -1 )
			DxLib_End();
		mess.y += damagey;
		damagey += damageg;

		Story_Draw();
	}

	isDamaging = 0;
}

/* 【関数】ストーリー：当たり判定・処理 */
void Story_Hit()
{
	int r;

	switch( changesizemap[ mess.y + CHARA_SIZE / 2 ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] )
	{
	case NEEDLE:
	case C_NEEDLE:
		isMovingRight = 0;
		isMovingLeft = 0;
		isJumping = 0;
		Story_DamageMove();
		life--;
		Story_InitAfterDamage();
		break;
	case SAVE:
		Story_ChangeEmpty( SAVE );
		Save = 1;
		break;
	case GOAL:
		Story_ChangeEmpty( GOAL );
		StageClear = 1;
		break;
	case G:
		Story_ChangeEmpty( G );
		StageClear = 1;
		break;
	case STAR:
		PlaySoundMem( se.star, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( STAR );
		point += 10;
		break;
	case BSTAR:
		PlaySoundMem( se.bigstar, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( BSTAR );
		point += 100;
		break;
	case LIFE:
		PlaySoundMem( se.medicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( LIFE );
		life++;
		break;
	case ATTACK:
		PlaySoundMem( se.medicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( ATTACK );
		power++;
		break;
	case INV:
		PlaySoundMem( se.medicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( INV );
		InvTime = 1000;
		break;
	case E:
		PlaySoundMem( se.medicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( E );
		energy = 100;
		break;
	case B:
		PlaySoundMem( se.medicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( B );
		isProtecting = 1;
		break;
	case J:
		PlaySoundMem( se.medicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( J );
		g--;
		break;
	case D:
		PlaySoundMem( se.medicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( D );
		movex += 2;
		break;
	case TIMER:
		PlaySoundMem( se.medicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( TIMER );
		break;
	case BROKEN_LIFE:
		PlaySoundMem( se.badmedicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( BROKEN_LIFE );
		if( life > 1 )
			life--;
		break;
	case BROKEN_ATTACK:
		PlaySoundMem( se.badmedicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( BROKEN_ATTACK );
		if( power > 1 )
			power--;
		break;
	case L_R:
		PlaySoundMem( se.badmedicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( L_R );
		if( isReversing )
			isReversing = 0;
		else if( !(isReversing) )
			isReversing = 1;
		break;
	case S_DOWN:
		PlaySoundMem( se.badmedicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( S_DOWN );
		movex -= 2;
		break;
	case J_LIMIT:
		PlaySoundMem( se.badmedicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( J_LIMIT );
		if( isJ_Restricting )
			isJ_Restricting = 0;
		else if( !(isJ_Restricting) )
			isJ_Restricting = 1;
		break;
	}

	Story_MapRenew();

	switch( changesizemap[ mess.y + CHARA_SIZE / 2 - 1 ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] )
	{
	case NEEDLE:
	case C_NEEDLE:
		isMovingRight = 0;
		isMovingLeft = 0;
		isJumping = 0;
		Story_DamageMove();
		life--;
		Story_InitAfterDamage();
		break;
	case SAVE:
		Story_ChangeEmpty( SAVE );
		Save = 1;
		break;
	case GOAL:
		Story_ChangeEmpty( GOAL );
		StageClear = 1;
		break;
	case G:
		Story_ChangeEmpty( G );
		StageClear = 1;
		break;
	case STAR:
		PlaySoundMem( se.star, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( STAR );
		point += 10;
		break;
	case BSTAR:
		PlaySoundMem( se.bigstar, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( BSTAR );
		point += 100;
		break;
	case LIFE:
		PlaySoundMem( se.medicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( LIFE );
		life++;
		break;
	case ATTACK:
		PlaySoundMem( se.medicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( ATTACK );
		power++;
		break;
	case INV:
		PlaySoundMem( se.medicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( INV );
		InvTime = 1000;
		break;
	case E:
		PlaySoundMem( se.medicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( E );
		energy = 100;
		break;
	case B:
		PlaySoundMem( se.medicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( B );
		isProtecting = 1;
		break;
	case J:
		PlaySoundMem( se.medicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( J );
		g--;
		break;
	case D:
		PlaySoundMem( se.medicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( D );
		movex += 2;
		break;
	case TIMER:
		PlaySoundMem( se.medicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( TIMER );
		break;
	case BROKEN_LIFE:
		PlaySoundMem( se.badmedicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( BROKEN_LIFE );
		if( life > 1 )
			life--;
		break;
	case BROKEN_ATTACK:
		PlaySoundMem( se.badmedicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( BROKEN_ATTACK );
		if( power > 1 )
			power--;
		break;
	case L_R:
		PlaySoundMem( se.badmedicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( L_R );
		if( isReversing )
			isReversing = 0;
		else if( !(isReversing) )
			isReversing = 1;
		break;
	case S_DOWN:
		PlaySoundMem( se.badmedicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( S_DOWN );
		movex -= 2;
		break;
	case J_LIMIT:
		PlaySoundMem( se.badmedicine, DX_PLAYTYPE_BACK );
		Story_ChangeEmpty( J_LIMIT );
		if( isJ_Restricting )
			isJ_Restricting = 0;
		else if( !(isJ_Restricting) )
			isJ_Restricting = 1;
		break;
	}

	if( changesizemap[ mess.y + CHARA_SIZE ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] == FREEZE )
	{
		r = GetRand( 1 );
		if( r == 0 )
			mess.x += 5;
		else if( r == 1 )
			mess.x -= 5;
	}
	else if( changesizemap[ mess.y + CHARA_SIZE ][ mess.x + CHARA_SIZE / 2 - mess.mapchipx ] == POISON )
	{
		if( energy > 4 )
			energy -= 5;
	}
}

/* 【関数】ストーリー：敵当たり判定・処理 */
void Story_EnemyHit()
{
	/* 敵ダメージ */
	if( enemy1exist && isAttack &&
		mess.x + CHARA_SIZE - mess.mapchipx < enemy1x && attackrightx - mess.mapchipx > enemy1x &&
		mess.y + CHARA_SIZE > enemy1y && mess.y < enemy1y + MAP_SIZE ||
		enemy1exist && isAttack &&
		mess.x - mess.mapchipx > enemy1x + MAP_SIZE && attackleftx - mess.mapchipx < enemy1x + MAP_SIZE &&
		mess.y + CHARA_SIZE > enemy1y && mess.y < enemy1y + MAP_SIZE )
		enemy1life -= power;
	if( enemy2exist && isAttack &&
		mess.x + CHARA_SIZE - mess.mapchipx < enemy2x && attackrightx - mess.mapchipx > enemy2x &&
		mess.y + CHARA_SIZE > enemy2y && mess.y < enemy2y + MAP_SIZE ||
		enemy2exist && isAttack &&
		mess.x - mess.mapchipx > enemy2x + MAP_SIZE && attackleftx - mess.mapchipx < enemy2x + MAP_SIZE &&
		mess.y + CHARA_SIZE > enemy2y && mess.y < enemy2y + MAP_SIZE )
		enemy2life -= power;

	if( enemy1exist && InvTime &&
		mess.x + CHARA_SIZE - mess.mapchipx > enemy1x && mess.x - mess.mapchipx < enemy1x + MAP_SIZE &&
		mess.y + CHARA_SIZE > enemy1y && mess.y < enemy1y + MAP_SIZE )
	{
		PlaySoundMem( se.effect, DX_PLAYTYPE_BACK );
		enemy1life = 0;
	}
	if( enemy2exist && InvTime &&
		mess.x + CHARA_SIZE - mess.mapchipx >= enemy2x && mess.x - mess.mapchipx <= enemy2x + MAP_SIZE &&
		mess.y + CHARA_SIZE >= enemy2y && mess.y <= enemy2y + MAP_SIZE )
	{
		PlaySoundMem( se.effect, DX_PLAYTYPE_BACK );
		enemy2life = 0;
	}

	if( enemy1life <= 0 )
		enemy1exist = 0;
	if( enemy2life <= 0 )
		enemy2exist = 0;

	/* ♂ダメージ */
	if( mess.stage == 10 )
	{
		if( isAttack &&
			mess.x + CHARA_SIZE - mess.mapchipx < ossx && attackrightx - mess.mapchipx > ossx &&
			mess.y + CHARA_SIZE > ossy && mess.y < ossy + CHARA_SIZE ||
			isAttack &&
			mess.x - mess.mapchipx > ossx + CHARA_SIZE && attackleftx - mess.mapchipx < ossx + CHARA_SIZE &&
			mess.y + CHARA_SIZE > ossy && mess.y < ossy + CHARA_SIZE )
			osslife -= power;
	}

	if( osslife <= 0 )
		StageClear = 1;

	/* ♀ダメージ */
	if( enemy1exist &&
		mess.x + CHARA_SIZE - mess.mapchipx > enemy1x && mess.x - mess.mapchipx < enemy1x + MAP_SIZE &&
		mess.y + CHARA_SIZE > enemy1y && mess.y < enemy1y + MAP_SIZE )
	{
		if( isProtecting )
		{
			isMovingRight = 0;
			isMovingLeft = 0;
			isJumping = 0;
			isProtecting = 0;
			PlaySoundMem( se.effect, DX_PLAYTYPE_BACK );
			Story_InitAfterDamage();
		}
		else
		{
			isMovingRight = 0;
			isMovingLeft = 0;
			isJumping = 0;
			Story_DamageMove();
			life--;
			Story_InitAfterDamage();
		}
	}
	if( enemy2exist &&
		mess.x + CHARA_SIZE - mess.mapchipx >= enemy2x && mess.x - mess.mapchipx <= enemy2x + MAP_SIZE &&
		mess.y + CHARA_SIZE >= enemy2y && mess.y <= enemy2y + MAP_SIZE )
	{
		if( isProtecting )
		{
			isMovingRight = 0;
			isMovingLeft = 0;
			isJumping = 0;
			isProtecting = 0;
			PlaySoundMem( se.effect, DX_PLAYTYPE_BACK );
			Story_InitAfterDamage();
		}
		else
		{
			isMovingRight = 0;
			isMovingLeft = 0;
			isJumping = 0;
			Story_DamageMove();
			life--;
			Story_InitAfterDamage();
		}
	}
}

/* 【関数】ストーリー：♀移動 */
void Story_Move()
{
	if( key[ KEY_INPUT_RIGHT ] )
	{
		if( !( isReversing ) )
		{
			isMovingRight = 1;

			if( mess.x + CHARA_SIZE < WINDOW_SIZE_X * 2 / 3 )
				mess.x += movex;
			else
			{
				stagebackx -= movex;
				mess.mapchipx -= movex;
			}

			while( Story_CheckMapHitRight() )
				mess.x--;
		}
		else if( isReversing )
		{
			isMovingLeft = 1;

			if( mess.x > WINDOW_SIZE_X / 3 )
				mess.x -= movex;
			else
			{
				stagebackx += movex;
				mess.mapchipx += movex;
			}

			while( Story_CheckMapHitLeft() )
				mess.x++;
		}
	}
	else if( key[ KEY_INPUT_LEFT ] )
	{
		if( !( isReversing ) )
		{
			isMovingLeft = 1;

			if( mess.x > WINDOW_SIZE_X / 3 )
				mess.x -= movex;
			else
			{
				stagebackx += movex;
				mess.mapchipx += movex;
			}

			while( Story_CheckMapHitLeft() )
				mess.x++;
		}
		else if( isReversing )
		{
			isMovingRight = 1;

			if( mess.x + CHARA_SIZE < WINDOW_SIZE_X * 2 / 3 )
				mess.x += movex;
			else
			{
				stagebackx -= movex;
				mess.mapchipx -= movex;
			}

			while( Story_CheckMapHitRight() )
				mess.x--;
		}
	}
	else if( !( key[ KEY_INPUT_RIGHT ] ) && !( key[ KEY_INPUT_LEFT ] ) )
	{
		isMovingRight = 0;
		isMovingLeft = 0;
	}

	if( key[ KEY_INPUT_Z ] && energy >= 20 && !( isAttack ) )
	{
		PlaySoundMem( se.attack, DX_PLAYTYPE_BACK );
		isAttack = 1;
		energy -= 20;
	}

	if( key[ KEY_INPUT_LSHIFT ] && key[ KEY_INPUT_I ] && !(InvTime) )
	{
		PlaySoundMem( se.medicine, DX_PLAYTYPE_BACK );
		InvTime = 1000;
	}

	/* ※SAVE, GOAL, G, BROKEN_FLOOR, STAR, BSTAR, LIFE, ATTACK, INV, E, B, J, D については、空気と同扱い */
	if( isJumping )
	{
		mess.y += movey;

		if( Story_CheckMapHitOver( -1 ) )
		{
			while( Story_CheckMapHitOver( 0 ) )
				mess.y++;

			movey = 0;
		}

		if( Story_CheckMapHitUnder( 0 ) )
		{
			while( Story_CheckMapHitUnder( -1 ) )
				mess.y--;

			isJumping = 0;
		}

		movey += g;
		if( movey > MOVEY_MAX )
			movey = MOVEY_MAX;
	}
	else
	{
		if( key[ KEY_INPUT_UP ] && !( Story_CheckMapHitOver( -1 ) ) && !( isJ_Restricting ) )
		{
			isJumping = 1;
			movey = movey0;
		}

		if( !( Story_CheckMapHitUnder( 0 ) ) )
		{
			isJumping = 1;
			movey = 0;
		}
	}
}

/* 【関数】ストーリー：敵移動 */
void Story_EnemyMove()
{
	if( enemy1exist )
	{
		if( enemy1direction == 0 )
		{
			enemy1x -= 10;

			if( changesizemap[ enemy1y ][ enemy1x + 1 ] != EMPTY )
				enemy1direction = 1;
		}
		else if( enemy1direction == 1 )
		{
			enemy1x += 10;

			if( changesizemap[ enemy1y ][ enemy1x + MAP_SIZE - 1 ] != EMPTY )
				enemy1direction = 0;
		}
	}
	if( enemy2exist )
	{
		if( enemy2direction == 0 )
		{
			enemy2x -= 10;

			if( changesizemap[ enemy2y ][ enemy2x + 1 ] != EMPTY )
				enemy2direction = 1;
		}
		else if( enemy2direction == 1 )
		{
			enemy2x += 10;

			if( changesizemap[ enemy2y ][ enemy2x + MAP_SIZE - 1 ] != EMPTY )
				enemy2direction = 0;
		}
	}
}

/* 【関数】ストーリー：♂移動 */
void Story_OssMove()
{
	if( ossdirection == 0 )
	{
		ossx -= 15;

		if( changesizemap[ ossy ][ ossx + 1 ] != EMPTY )
			ossdirection = 1;
	}
	else if( ossdirection == 1 )
	{
		ossx += 15;

		if( changesizemap[ ossy ][ ossx + MAP_SIZE - 1 ] != EMPTY )
			ossdirection = 0;
	}
}

/* 【関数】ストーリー：セーブ */
void Story_Save()
{
	FILE *fp;

	if( mess.datanum == 1 )
		fp = fopen( "./Mess-tory_data/savedata1.dat", "wb" );
	else if( mess.datanum == 2 )
		fp = fopen( "./Mess-tory_data/savedata2.dat", "wb" );
	else if( mess.datanum == 3 )
		fp = fopen( "./Mess-tory_data/savedata3.dat", "wb" );

	if( fp == NULL )
		return;

	fwrite( &mess, sizeof(mess), 1, fp );

	fclose( fp );
}

/* 【関数】ストーリー：ゲームオーバー */
void Story_GameOver()
{
	int gameovery = 50;
	int up = 0;
	int down = 0;

	while( 1 )
	{
		if( ProcessMessage() == -1 )
			DxLib_End();

		if( CheckHitKeyAll() )
			break;

		DrawGraph( WINDOW_SIZE_X / 2 - 200, gameovery, grp.gameover, TRUE );
		DrawExtendGraph( WINDOW_SIZE_X / 2 - CHARA_SIZE / 2, WINDOW_SIZE_Y / 2 - CHARA_SIZE / 2,
			WINDOW_SIZE_X / 2 + CHARA_SIZE / 2, WINDOW_SIZE_Y / 2 + CHARA_SIZE / 2, grp.mess[15], TRUE );
		DrawString( 400, WINDOW_SIZE_Y - 25, "PRESS ANY KEY...",GetColor(0,0,0) );

		switch( gameovery )
		{
		case 50:
			up = 0;
			down = 1;
			break;
		case 70:
			up = 1;
			down = 0;
			break;
		}

		if( up )
			gameovery--;
		else if( down )
			gameovery++;

		ScreenFlip();
		ClearDrawScreen();
	}
}

/* 【関数】ストーリー */
void Story( int Continue )
{
	SetFontSize( 20 );

	if( !( Continue ) )
	{
		Story_Opening();
		ossmove1 = 0;
		ossmove2 = 0;
		osssay = 1;
	}

	Story_StageInput();

	Story_Init( Continue );

	life = 3;
	point = 0;
	movex = 10;
	g = 10;
	osslife = 100;

	while( mess.stage <= 10 )
	{
		if( ProcessMessage() == -1 )
			DxLib_End();

		Input();

		if( key[ KEY_INPUT_ESCAPE ] )
			return;

		if( energy < 100 )
			energy++;

		if( InvTime > 0 )
			InvTime--;

		Story_MapRenew();

		Story_Move();

		Story_EnemyMove();

		if( mess.stage == 10 )
			Story_OssMove();

		Story_Hit();

		Story_EnemyHit();

		Story_Draw();

		if( Save )
		{
			PlaySoundMem( se.save, DX_PLAYTYPE_BACK );

			Story_Save();

			Save = 0;
		}

		if( StageClear )
		{
			StopMusic();
			PlaySoundMem( se.clear, DX_PLAYTYPE_NORMAL );

			isMovingRight = 0;
			isMovingLeft = 0;

			if( mess.stage == 0 )
			{
				osssay = 2;
				Story_Flash( 1 );
				WaitTimer( 2000 );

				Story_Draw();
				WaitTimer( 2000 );

				osssay = 3;
				Story_Draw();
				WaitTimer( 2000 );

				Story_EscapeOss();

				Story_AfterOpening();
			}
			else if( mess.stage == 5 )
			{
				osssay = 2;
				Story_Draw();
				WaitTimer( 2000 );

				Story_EscapeOss();
			}
			else if( mess.stage == 10 )
			{
				Story_Flash( 0 );
				WaitTimer( 1000 );
				mess.stage = 0;
				Story_Save();
				StageClear = 0;
				break;
			}

			mess.stage++;

			Story_StageInput();

			Story_Init( 0 );

			Story_Save();

			StageClear = 0;
		}

		if( life == 0 )
		{
			StopMusic();
			PlaySoundMem( se.gameover, DX_PLAYTYPE_BACK );
			mess.stage = 0;

			Story_Save();

			Story_GameOver();

			WaitTimer( 500 );
			return;
		}
	}

	Story_Ending();
}

/* 【関数】対戦：ステージ選択 */
int Battle_StageSelect()
{
	int stagenum = 1;

	PlayMusic( "./Mess-tory_data/Sound/battleselectbgm.mp3", DX_PLAYTYPE_LOOP );

	while( 1 )
	{
		if( ProcessMessage() == -1 )
			DxLib_End();

		Input();

		if( key[ KEY_INPUT_SPACE ] || key[ KEY_INPUT_RETURN ] )
		{
			PlaySoundMem( se.enter, DX_PLAYTYPE_NORMAL );
			StopMusic();
			return stagenum;
		}
		else if( key[ KEY_INPUT_RIGHT ] )
		{
			PlaySoundMem( se.select, DX_PLAYTYPE_BACK );
			stagenum++;
			if( stagenum > 3 )
				stagenum = 1;
		}
		else if( key[ KEY_INPUT_LEFT ] )
		{
			PlaySoundMem( se.select, DX_PLAYTYPE_BACK );
			stagenum--;
			if( stagenum < 1 )
				stagenum = 3;
		}

		DrawGraph( 0, 0, grp.main_back, FALSE );

		DrawString( 150, 10, "ステージ選択", GetColor(0,0,0) );

		switch( stagenum )
		{
		case 1:
			DrawString( 50, 100, "ステージ１「GUIsland」", GetColor(0,0,0) );
			DrawExtendGraph( 50, 160, 590, 470, grp.battleview1, FALSE );
			break;
		case 2:
			DrawString( 50, 100, "ステージ２「文字世界」", GetColor(0,0,0) );
			DrawExtendGraph( 50, 160, 590, 470, grp.battleview2, FALSE );
			break;
		case 3:
			DrawString( 50, 100, "ステージ３「一本道」", GetColor(0,0,0) );
			DrawExtendGraph( 50, 160, 590, 470, grp.battleview3, FALSE );
			break;
		}

		ScreenFlip();
		WaitKey();
		ClearDrawScreen();
	}

}

/* 【関数】対戦：初期化 */
void Battle_Init()
{
	movex_b1 = 10;
	movex_b2 = 10;

	g_b1 = 9;
	g_b2 = 9;

	life_b1 = 100;
	life_b2 = 100;

	energy_b1 = 100;
	energy_b2 = 100;

	if( battlestage == 1 )
	{
		PlayMusic( "./Mess-tory_data/Sound/b_stage1bgm.mp3", DX_PLAYTYPE_LOOP );

		b_mess1.x = 3 * MAP_SIZE;
		b_mess1.y = 10 * MAP_SIZE;
		b_mess2.x = 17 * MAP_SIZE;
		b_mess2.y = 10 * MAP_SIZE;
	}
	else if( battlestage == 2 )
	{
		PlayMusic( "./Mess-tory_data/Sound/b_stage2bgm.mp3", DX_PLAYTYPE_LOOP );

		b_mess1.x = 1 * MAP_SIZE;
		b_mess1.y = 12 * MAP_SIZE;
		b_mess2.x = 17 * MAP_SIZE;
		b_mess2.y = 12 * MAP_SIZE;
	}
	else if( battlestage == 3 )
	{
		PlayMusic( "./Mess-tory_data/Sound/b_stage3bgm.mp3", DX_PLAYTYPE_LOOP );

		b_mess1.x = 2 * MAP_SIZE;
		b_mess1.y = 12 * MAP_SIZE;
		b_mess2.x = 17 * MAP_SIZE;
		b_mess2.y = 12 * MAP_SIZE;
	}
}

/* 【関数】対戦：拡大マップ更新 */
void Battle_MapRenew()
{
	int i, j;

	for( i = 0; i < STAGE_HEIGHT * MAP_SIZE; i++ )
	{
		for( j = 0; j < STAGE_BATTLEWIDTH * MAP_SIZE; j++ )
			changesizemap_b[ i ][ j ] = battlemap[ battlestage - 1 ][ i / MAP_SIZE ][ j / MAP_SIZE ];
	}
}

/* 【関数】対戦：マップ当たり判定・右 */
int Battle_CheckMapHitRight( int whosey, int whosex )
{
	int i;
	int isMapHit = 0;

	for( i = whosey; i < whosey + CHARA_SIZE; i++ )
	{
		if( changesizemap_b[ i ][ whosex + CHARA_SIZE - 1 ] != EMPTY &&
			changesizemap_b[ i ][ whosex + CHARA_SIZE - 1 ] != BROKEN_FLOOR &&
			changesizemap_b[ i ][ whosex + CHARA_SIZE - 1 ] != LIFE &&
			changesizemap_b[ i ][ whosex + CHARA_SIZE - 1 ] != ATTACK &&
			changesizemap_b[ i ][ whosex + CHARA_SIZE - 1 ] != E &&
			changesizemap_b[ i ][ whosex + CHARA_SIZE - 1 ] != B &&
			changesizemap_b[ i ][ whosex + CHARA_SIZE - 1 ] != J &&
			changesizemap_b[ i ][ whosex + CHARA_SIZE - 1 ] != D )
			isMapHit = 1;
	}

	return isMapHit;
}

/* 【関数】対戦：マップ当たり判定・左 */
int Battle_CheckMapHitLeft( int whosey, int whosex )
{
	int i;
	int isMapHit = 0;

	for( i = whosey; i < whosey + CHARA_SIZE; i++ )
	{
		if( changesizemap_b[ i ][ whosex + 1 ] != EMPTY &&
			changesizemap_b[ i ][ whosex + 1 ] != BROKEN_FLOOR &&
			changesizemap_b[ i ][ whosex + 1 ] != LIFE &&
			changesizemap_b[ i ][ whosex + 1 ] != ATTACK &&
			changesizemap_b[ i ][ whosex + 1 ] != E &&
			changesizemap_b[ i ][ whosex + 1 ] != B &&
			changesizemap_b[ i ][ whosex + 1 ] != J &&
			changesizemap_b[ i ][ whosex + 1 ] != D )
			isMapHit = 1;
	}

	return isMapHit;
}

/* 【関数】対戦：マップ当たり判定・上 */
int Battle_CheckMapHitOver( int whosey, int whosex, int look )
{
	int isMapHit = 0;


	if( changesizemap_b[ whosey + look ][ whosex + CHARA_SIZE / 2 ] != EMPTY &&
		changesizemap_b[ whosey + look ][ whosex + CHARA_SIZE / 2 ] != BROKEN_FLOOR &&
		changesizemap_b[ whosey + look ][ whosex + CHARA_SIZE / 2 ] != LIFE &&
		changesizemap_b[ whosey + look ][ whosex + CHARA_SIZE / 2 ] != ATTACK &&
		changesizemap_b[ whosey + look ][ whosex + CHARA_SIZE / 2 ] != E &&
		changesizemap_b[ whosey + look ][ whosex + CHARA_SIZE / 2 ] != B &&
		changesizemap_b[ whosey + look ][ whosex + CHARA_SIZE / 2 ] != J &&
		changesizemap_b[ whosey + look ][ whosex + CHARA_SIZE / 2 ] != D )
		isMapHit = 1;

	return isMapHit;
}

/* 【関数】対戦：マップ当たり判定・下 */
int Battle_CheckMapHitUnder( int whosey, int whosex, int look )
{
	int isMapHit = 0;

	if( changesizemap_b[ whosey + CHARA_SIZE + look ][ whosex + CHARA_SIZE / 2 ] != EMPTY &&
		changesizemap_b[ whosey + CHARA_SIZE + look ][ whosex + CHARA_SIZE / 2 ] != BROKEN_FLOOR &&
		changesizemap_b[ whosey + CHARA_SIZE + look ][ whosex + CHARA_SIZE / 2 ] != LIFE &&
		changesizemap_b[ whosey + CHARA_SIZE + look ][ whosex + CHARA_SIZE / 2 ] != ATTACK &&
		changesizemap_b[ whosey + CHARA_SIZE + look ][ whosex + CHARA_SIZE / 2 ] != E &&
		changesizemap_b[ whosey + CHARA_SIZE + look ][ whosex + CHARA_SIZE / 2 ] != B &&
		changesizemap_b[ whosey + CHARA_SIZE + look ][ whosex + CHARA_SIZE / 2 ] != J &&
		changesizemap_b[ whosey + CHARA_SIZE + look ][ whosex + CHARA_SIZE / 2 ] != D )
		isMapHit = 1;

	return isMapHit;
}

/* 【関数】対戦：移動 */
void Battle_Move()
{
	if( key[ KEY_INPUT_ESCAPE ] )
		BattleEnd = 1;
	//////////////b_mess1//////////////

	if( key[ KEY_INPUT_RIGHT ] )
	{
		isMovingRight_b1 = 1;
		b_mess1.x += movex_b1;

		while( Battle_CheckMapHitRight( b_mess1.y, b_mess1.x ) )
			b_mess1.x--;
	}
	else if( key[ KEY_INPUT_LEFT ] )
	{
		isMovingLeft_b1 = 1;
		b_mess1.x -= movex_b1;

		while( Battle_CheckMapHitLeft( b_mess1.y, b_mess1.x ) )
			b_mess1.x++;
	}
	else
	{
		isMovingRight_b1 = 0;
		isMovingLeft_b1 = 0;
	}

	if( key[ KEY_INPUT_Z ] && energy_b1 >= 20 && !( isAttack_b1 ) )
	{
		PlaySoundMem( se.attack, DX_PLAYTYPE_BACK );
		isAttack_b1 = 1;
		energy_b1 -= 20;
	}

	if( isJumping_b1 )
	{
		b_mess1.y += movey_b1;

		if( Battle_CheckMapHitOver( b_mess1.y, b_mess1.x, -1 ) )
		{
			while( Battle_CheckMapHitOver( b_mess1.y, b_mess1.x, 0 ) )
				b_mess1.y++;

			movey_b1 = 0;
		}

		if( Battle_CheckMapHitUnder( b_mess1.y, b_mess1.x, 0 ) )
		{
			while( Battle_CheckMapHitUnder( b_mess1.y, b_mess1.x, -1 ) )
				b_mess1.y--;

			isJumping_b1 = 0;
		}

		movey_b1 += g_b1;
		if( movey_b1 > MOVEY_MAX )
			movey_b1 = MOVEY_MAX;
	}
	else
	{
		if( key[ KEY_INPUT_UP ] && !( Battle_CheckMapHitOver( b_mess1.y, b_mess1.x, -1 ) ) )
		{
			isJumping_b1 = 1;
			movey_b1 = movey0;
		}

		if( !( Battle_CheckMapHitUnder( b_mess1.y, b_mess1.x, 0 ) ) )
		{
			isJumping_b1 = 1;
			movey_b1 = 0;
		}
	}

	///////////////b_mess2///////////////

	if( key[ KEY_INPUT_NUMPAD6 ] )
	{
		isMovingRight_b2 = 1;
		b_mess2.x += movex_b2;

		while( Battle_CheckMapHitRight( b_mess2.y, b_mess2.x ) )
			b_mess2.x--;
	}
	else if( key[ KEY_INPUT_NUMPAD4 ] )
	{
		isMovingLeft_b2 = 1;
		b_mess2.x -= movex_b2;

		while( Battle_CheckMapHitLeft( b_mess2.y, b_mess2.x ) )
			b_mess2.x++;
	}
	else
	{
		isMovingRight_b2 = 0;
		isMovingLeft_b2 = 0;
	}

	if( key[ KEY_INPUT_NUMPAD5 ] && energy_b2 >= 20 && !( isAttack_b2 ) )
	{
		PlaySoundMem( se.attack, DX_PLAYTYPE_BACK );
		isAttack_b2 = 1;
		energy_b2 -= 20;
	}

	if( isJumping_b2 )
	{
		b_mess2.y += movey_b2;

		if( Battle_CheckMapHitOver( b_mess2.y, b_mess2.x, -1 ) )
		{
			while( Battle_CheckMapHitOver( b_mess2.y, b_mess2.x, 0 ) )
				b_mess2.y++;

			movey_b2 = 0;
		}

		if( Battle_CheckMapHitUnder( b_mess2.y, b_mess2.x, 0 ) )
		{
			while( Battle_CheckMapHitUnder( b_mess2.y, b_mess2.x, -1 ) )
				b_mess2.y--;

			isJumping_b2 = 0;
		}

		movey_b2 += g_b2;
		if( movey_b2 > MOVEY_MAX )
			movey_b2 = MOVEY_MAX;
	}
	else
	{
		if( key[ KEY_INPUT_NUMPAD8 ] && !( Battle_CheckMapHitOver( b_mess2.y, b_mess2.x, -1 ) ) )
		{
			isJumping_b2 = 1;
			movey_b2 = movey0;
		}

		if( !( Battle_CheckMapHitUnder( b_mess2.y, b_mess2.x, 0 ) ) )
		{
			isJumping_b2 = 1;
			movey_b2 = 0;
		}
	}
}

/* 【関数】対戦：表示 */
void Battle_Draw()
{
	int i, j;
	static int movecount_b1 = 0;
	static int movecount_b2 = 0;
	static int attackmoverightcount_b1 = 0;
	static int attackmoverightcount_b2 = 0;
	static int attackmoveleftcount_b1 = 7;
	static int attackmoveleftcount_b2 = 7;

	switch( battlestage )
	{
	case 1:
		DrawExtendGraph( 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.battlestage1, FALSE );
		break;
	case 2:
		DrawExtendGraph( 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.battlestage2, FALSE );
		break;
	case 3:
		DrawExtendGraph( 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.battlestage3, FALSE );
		break;
	}

	for( i = 0; i < STAGE_HEIGHT; i++ )
	{
		for( j = 0; j < STAGE_BATTLEWIDTH; j++ )
		{
			if( battlemap[ battlestage - 1 ][ i ][ j ] == EMPTY || battlemap[ battlestage - 1 ][ i ][ j ] == NO_ENTRY )
				continue;
			else if( battlemap[ battlestage - 1 ][ i ][ j ] < STAR )
				DrawGraph( j * MAP_SIZE, i * MAP_SIZE, grp.mapchip[ battlemap[ battlestage - 1 ][i][j] - 1 ], TRUE );
			else
				DrawGraph( j * MAP_SIZE, i* MAP_SIZE, grp.item[ battlemap[ battlestage - 1 ][i][j] - 17 ], TRUE );
		}
	}

	DrawBox( 10, 5, 10 + life_b1, 25, GetColor(100,0,100), TRUE );
	DrawBox( 170, 5, 170 + energy_b1, 25, GetColor(0,200,0), TRUE );
	DrawLine( 170, 26, 170, 31, GetColor(255,0,0) );
	DrawLine( 170 + 20, 26, 170 + 20, 31, GetColor(200,50,0) );
	DrawLine( 170 + 40, 26, 170 + 40, 31, GetColor(100,50,0) );
	DrawLine( 170 + 60, 26, 170 + 60, 31, GetColor(50,100,0) );
	DrawLine( 170 + 80, 26, 170 + 80, 31, GetColor(0,200,0) );
	DrawLine( 270, 26, 270, 31, GetColor(0,255,0) );

	DrawBox( 340, 5, 340 + life_b2, 25, GetColor(255,150,150), TRUE );
	DrawBox( 500, 5, 500 + energy_b2, 25, GetColor(0,200,0), TRUE );
	DrawLine( 500, 26, 500, 31, GetColor(255,0,0) );
	DrawLine( 500 + 20, 26, 500 + 20, 31, GetColor(200,50,0) );
	DrawLine( 500 + 40, 26, 500 + 40, 31, GetColor(100,50,0) );
	DrawLine( 500 + 60, 26, 500 + 60, 31, GetColor(50,100,0) );
	DrawLine( 500 + 80, 26, 500 + 80, 31, GetColor(0,200,0) );
	DrawLine( 600, 26, 600, 31, GetColor(0,255,0) );

	movecount_b1++;
	movecount_b2++;

	if( isMovingRight_b1 )
	{
		if( movecount_b1 >= 4 )
			movecount_b1 = 0;
	}
	else if( isMovingLeft_b1 )
	{
		if( movecount_b1 < 4 || movecount_b1 >= 8 )
			movecount_b1 = 4;
	}
	else if( isJumping_b1 )
	{
		if( movecount_b1 < 8 || movecount_b1 >= 12 )
			movecount_b1 = 8;
	}
	else if( !( isDamaging_b1 ) )
		if( movecount_b1 < 12 || movecount_b1 >= 14 )
			movecount_b1 = 12;

	if( isDamaging_b1 && movecount_b1 >= 4 )
		movecount_b1 = 0;

	if( isDamaging_b1 )
		DrawExtendGraph( b_mess1.x, b_mess1.y, b_mess1.x + CHARA_SIZE, b_mess1.y + CHARA_SIZE, grp.battledamage1[ movecount_b1 ], TRUE );
	else
		DrawExtendGraph( b_mess1.x, b_mess1.y, b_mess1.x + CHARA_SIZE, b_mess1.y + CHARA_SIZE, grp.battlemess1[ movecount_b1 ], TRUE );

	if( isMovingRight_b2 )
	{
		if( movecount_b2 >= 4 )
			movecount_b2 = 0;
	}
	else if( isMovingLeft_b2 )
	{
		if( movecount_b2 < 4 || movecount_b2 >= 8 )
			movecount_b2 = 4;
	}
	else if( isJumping_b2 )
	{
		if( movecount_b2 < 8 || movecount_b2 >= 12 )
			movecount_b2 = 8;
	}
	else if( !( isDamaging_b2 ) )
		if( movecount_b2 < 12 || movecount_b2 >= 14 )
			movecount_b2 = 12;

	if( isDamaging_b2 && movecount_b2 >= 4 )
		movecount_b2 = 0;

	if( isDamaging_b2 )
		DrawExtendGraph( b_mess2.x, b_mess2.y, b_mess2.x + CHARA_SIZE, b_mess2.y + CHARA_SIZE, grp.battledamage2[ movecount_b2 ], TRUE );
	else
		DrawExtendGraph( b_mess2.x, b_mess2.y, b_mess2.x + CHARA_SIZE, b_mess2.y + CHARA_SIZE, grp.battlemess2[ movecount_b2 ], TRUE );


	if( isAttack_b1 )
	{
		attackrightx_b1 = b_mess1.x + CHARA_SIZE + 30;
		attackleftx_b1 = b_mess1.x - 30;

		DrawGraph( attackrightx_b1 - 30, b_mess1.y + CHARA_SIZE / 2, grp.attack[ attackmoverightcount_b1 ], TRUE );
		DrawGraph( attackleftx_b1, b_mess1.y + CHARA_SIZE / 2, grp.attack[ attackmoveleftcount_b1 ], TRUE );

		attackmoverightcount_b1++;
		if( attackmoverightcount_b1 == 4 )
		{
			attackmoverightcount_b1 = 0;
			isAttack_b1 = 0;
		}

		attackmoveleftcount_b1--;
		if( attackmoveleftcount_b1 == 3 )
		{
			attackmoveleftcount_b1 = 7;
			isAttack_b1 = 0;
		}
	}
	if( isAttack_b2 )
	{
		attackrightx_b2 = b_mess2.x + CHARA_SIZE + 30;
		attackleftx_b2 = b_mess2.x - 30;

		DrawGraph( attackrightx_b2 - 30, b_mess2.y + CHARA_SIZE / 2, grp.attack[ attackmoverightcount_b2 ], TRUE );
		DrawGraph( attackleftx_b2, b_mess2.y + CHARA_SIZE / 2, grp.attack[ attackmoveleftcount_b2 ], TRUE );

		attackmoverightcount_b2++;
		if( attackmoverightcount_b2 == 4 )
		{
			attackmoverightcount_b2 = 0;
			isAttack_b2 = 0;
		}

		attackmoveleftcount_b2--;
		if( attackmoveleftcount_b2 == 3 )
		{
			attackmoveleftcount_b2 = 7;
			isAttack_b2 = 0;
		}
	}

	ScreenFlip();
	WaitTimer( 80 );
	ClearDrawScreen();
}

/* 【関数】対戦：ダメージ移動 */
void Battle_DamageMove( int Who )
{
	int damagey0 = -30;
	int damageg = 5;
	int damagey;

	damagey = damagey0;

	if( Who == 1 )
	{
		isDamaging_b1 = 1;

		while( b_mess1.y <= WINDOW_SIZE_Y )
		{
			if( ProcessMessage() == -1 )
				DxLib_End();
			b_mess1.y += damagey;
			damagey += damageg;

			Battle_Draw();
		}

		isDamaging_b1 = 0;
	}
	else if( Who == 2 )
	{
		isDamaging_b2 = 1;

		while( b_mess2.y <= WINDOW_SIZE_Y )
		{
			if( ProcessMessage() == -1 )
				DxLib_End();
			b_mess2.y += damagey;
			damagey += damageg;

			Battle_Draw();
		}

		isDamaging_b2 = 0;
	}
}

/* 【関数】対戦：当たり判定 */
void Battle_Hit()
{
	if( isAttack_b1 )
	{
		if( b_mess1.x + CHARA_SIZE < b_mess2.x && attackrightx_b1 > b_mess2.x &&
			b_mess1.y + CHARA_SIZE > b_mess2.y && b_mess1.y < b_mess2.y + CHARA_SIZE ||
			b_mess1.x > b_mess2.x + CHARA_SIZE && attackleftx_b1 < b_mess2.x + CHARA_SIZE &&
			b_mess1.y + CHARA_SIZE > b_mess2.y && b_mess1.y < b_mess2.y + CHARA_SIZE )
			life_b2 -= 5;
	}
	if( isAttack_b2 )
	{

		if( b_mess2.x + CHARA_SIZE < b_mess1.x && attackrightx_b2 > b_mess1.x &&
			b_mess2.y + CHARA_SIZE > b_mess1.y && b_mess2.y < b_mess1.y + CHARA_SIZE ||
			b_mess2.x > b_mess1.x + CHARA_SIZE && attackleftx_b2 < b_mess1.x + CHARA_SIZE &&
			b_mess2.y + CHARA_SIZE > b_mess1.y && b_mess2.y < b_mess1.y + CHARA_SIZE )
			life_b1 -= 5;
	}

	if( life_b1 <= 0 )
	{
		PlaySoundMem( se.damage, DX_PLAYTYPE_BACK );
		Battle_DamageMove( 1 );
		BattleEnd = 1;
	}
	if( life_b2 <= 0 )
	{
		PlaySoundMem( se.damage, DX_PLAYTYPE_BACK );
		Battle_DamageMove( 2 );
		BattleEnd = 1;
	}
}

/* 【関数】対戦 */
void Battle()
{
	battlestage = Battle_StageSelect();

	SetFontSize( 20 );

	Battle_Init();

	while( 1 )
	{
		if( ProcessMessage() == -1 )
			DxLib_End();

		Input();

		if( energy_b1 < 100 )
			energy_b1++;
		if( energy_b2 < 100 )
			energy_b2++;

		Battle_MapRenew();

		Battle_Move();

		Battle_Hit();

		Battle_Draw();

		if( BattleEnd )
		{
			StopMusic();
			PlaySoundMem( se.battleend, DX_PLAYTYPE_NORMAL );
			BattleEnd = 0;
			break;
		}
	}
}

/* 【関数】説明 */
void Explanation()
{
	int page = 1;
	double angle;

	PlayMusic( "./Mess-tory_data/Sound/expbgm.mp3", DX_PLAYTYPE_LOOP );

	for( angle = 0.0; angle <= (PI*2); angle += (PI*2/30.0) )
	{
		DrawRotaGraph( WINDOW_SIZE_X/2, WINDOW_SIZE_Y/2, 1.3, angle, grp.exp1, FALSE, FALSE );
		ScreenFlip();
		WaitTimer( 50 );
		ClearDrawScreen();
	}

	while(1)
	{
		if( ProcessMessage() == -1 )	//メッセージ処理
		{
			DxLib_End();				//DXライブラリ使用の終了処理
		}

		Input();

		if( page != 1 && key[ KEY_INPUT_LEFT ] )
		{
			PlaySoundMem( se.expnext, DX_PLAYTYPE_BACK );
			page--;
		}
		if( page != 5 && key[ KEY_INPUT_RIGHT ] )
		{
			PlaySoundMem( se.expnext, DX_PLAYTYPE_BACK );
			page++;
		}
		if( page == 5 && key[ KEY_INPUT_SPACE ] )
		{
			PlaySoundMem( se.exit, DX_PLAYTYPE_NORMAL );
			StopMusic();
			break;
		}

		if( page == 1 )
			DrawExtendGraph( 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.exp1, FALSE );
		else if( page == 2 )
			DrawExtendGraph( 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.exp2, FALSE );
		else if( page == 3 )
			DrawExtendGraph( 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.exp3, FALSE );
		else if( page == 4 )
			DrawExtendGraph( 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.exp4, FALSE );
		else if( page == 5 )
			DrawExtendGraph( 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, grp.exp5, FALSE );

		ScreenFlip();
		WaitKey();
		ClearDrawScreen();
	}
}