#include <string>
#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);
	//ChangeWindowMode(TRUE);
	SetGraphMode(800, 450, 32);
	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;        // エラーが起きたら直ちに終了
	}
	// 描画先画面を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

#define ORANGE	GetColor(255,127,0)
#define WHITE	GetColor(255,255,255)
#define RED		GetColor(255,0,0)
#define BLUE	GetColor(0,0,255)

#define FONT_SS 16
#define FONT_S 24
#define FONT_M 36
#define FONT_L 48

	char op[16][8] = {"and not","nor","nor not","not","and not","not","xor","nand","and","iff","","→","","←","or","or not"};

	int const titlebg = LoadGraph("title.png");
	int const enjoubg = LoadGraph("fire.png");
	int const trickgr = LoadGraph("trick.png");
	int const treatgr = LoadGraph("treat.png");
	int const truckgr = LoadGraph("truck.png");

	int const centerx = 400;
	int const titley = 120;
	int const titley2 = 300;
	int const tr_y = 80;
	int const tfy = 220;
	int const grx[2] = { 130,570 }; int const gry = 300;
	int const cly = 160; int const cry = 230;
	int const goy = 65; int const resy = 165;
	int const trucky = 325; int truckf = 0;

	int gamemode = 0;
	int counter;
	int reason; int const sr = 500;
	int logoper;
	bool trick; bool treat; bool tril;
	int ans; bool cor;
	std::string trleft = ""; std::string trright = "";
	int candy;

	SetBackgroundColor(63, 31, 31);
	ChangeFontType(DX_FONTTYPE_ANTIALIASING);
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();
		switch (gamemode)
		{
		case 0:
			reason = sr;
			candy = 0;
			gamemode = 1;
			DrawGraph(0, 0, titlebg, TRUE);
			SetFontSize(FONT_L);
			DrawString(centerx - GetDrawStringWidth("- Trick or Treat -",strlen("- Trick or Treat -"))/2, titley, "- Trick or Treat -", ORANGE);
			SetFontSize(FONT_S);
			DrawString(centerx - GetDrawStringWidth("Press ∀ Key", strlen("Press ∀ Key"))/2, titley2, "Press ∀ Key", WHITE);
			ScreenFlip();
			WaitKey();
			if (CheckHitKey(KEY_INPUT_ESCAPE))break;
			ClearDrawScreen();
			SetFontSize(FONT_SS);
			DrawFormatString(80, 60, WHITE, 
				"ルール説明\n自然減少する理性が無くなる前に、できるだけキャンディを集めるゲームです。\n画面上部にTrick or Treatなどの論理式が表示されるので、\n画面下部のイラストがその論理式を満たすとき→キー、\n満たさない時は←キーを押してください。\n正解ならばキャンディが1つもらえ、理性が少し回復します。"
				, reason, candy);
			SetFontSize(FONT_S);
			DrawString(grx[0]+15, gry-30, "Trick", ORANGE);
			DrawString(grx[1]+25, gry-30, "Treat", ORANGE);
			DrawGraph(grx[0], gry, trickgr, TRUE);
			DrawGraph(grx[1], gry, treatgr, TRUE);
			DrawString(centerx - GetDrawStringWidth("Press ∀ Key to Start", strlen("Press ∀ Key to Start")) / 2, titley2, "Press ∀ Key to Start", WHITE);
			ScreenFlip();
			WaitKey();
		case 1:
			trick = GetRand(1); treat = GetRand(1); tril = GetRand(1);
			logoper = GetRand(15);
			switch (logoper)
			{
			case 0:
			case 15:
				if (GetRand(2)) {
					trleft = "Trick"; trright = "Trick";
				}
				else {
					trleft = "Treat"; trright = "Treat";
				}
				break;
			case 3:
			case 12:
				trleft = ""; trright = "Trick";
				break;
			case 5:
			case 10:
				trleft = ""; trright = "Treat";
				break;
			default:
				trleft = "Trick"; trright = "Treat";
				break;
			}
			ans = 1 << trick * 2 << treat & logoper;
			gamemode = 2;
		case 2:
			SetFontSize(FONT_S);
			DrawFormatString(0, 0, WHITE, "あなたの理性:%d\n集めたキャンディ:%d個", reason, candy);
			SetFontSize(FONT_L);
			DrawFormatString(centerx - GetDrawFormatStringWidth("%s %s %s", trleft.c_str(), op[logoper], trright.c_str()) / 2, tr_y,
				ORANGE, "%s %s %s",trleft.c_str(), op[logoper], trright.c_str());
			SetFontSize(FONT_M);
			DrawString(centerx - GetDrawStringWidth("← False　　　True →", strlen("← False　　　True →")) / 2, tfy, "← False　　　True →", WHITE);
			if (trick) DrawGraph(grx[tril], gry, trickgr, TRUE);
			if (treat) DrawGraph(grx[1 - tril], gry, treatgr, TRUE);
			if (CheckHitKey(KEY_INPUT_LEFT) && ans == 0 || CheckHitKey(KEY_INPUT_RIGHT) && ans > 0) {
				cor = true;
				gamemode = 3;
				counter = 60;
				candy++;
			}
			if (CheckHitKey(KEY_INPUT_LEFT) && ans > 0 || CheckHitKey(KEY_INPUT_RIGHT) && ans == 0) {
				cor = false;
				gamemode = 3;
				counter = 60;
			}
			if (reason <= 0)gamemode = 4;
			reason--;
			break;
		case 3:
			SetFontSize(FONT_S);
			DrawFormatString(0, 0, WHITE, "あなたの理性:%d\n集めたキャンディ:%d個", reason, candy);
			SetFontSize(FONT_L);
			DrawFormatString(centerx - GetDrawFormatStringWidth("%s %s %s", trleft.c_str(), op[logoper], trright.c_str()) / 2, tr_y,
				ORANGE, "%s %s %s", trleft.c_str(), op[logoper], trright.c_str());
			SetFontSize(FONT_M);
			DrawString(centerx - GetDrawStringWidth("← False　　　True →", strlen("← False　　　True →")) / 2, tfy, "← False　　　True →", WHITE);
			if (trick) DrawGraph(grx[tril], gry, trickgr, TRUE);
			if (treat) DrawGraph(grx[1 - tril], gry, treatgr, TRUE);
			SetFontSize(FONT_L);
			if (cor) {
				DrawString(centerx - GetDrawStringWidth("正解！", strlen("正解！")) / 2, cly, "正解！", RED);
				reason += 1 + GetRand(reason / 250);
			}
			else {
				DrawString(centerx - GetDrawStringWidth("不正解…", strlen("不正解…")) / 2, cly, "不正解…", BLUE);
				reason -= 1 + GetRand(reason / 100);
			}
			SetFontSize(FONT_M);
			if (ans)DrawString(centerx - GetDrawStringWidth("TRUE", strlen("TRUE")) / 2, cry, "TRUE", ORANGE);
			else DrawString(centerx - GetDrawStringWidth("FALSE", strlen("FALSE")) / 2, cry, "FALSE", ORANGE);
			counter--;
			if (counter == 0)gamemode = 1;
			break;
		case 4:
			DrawGraph(0, 0, enjoubg, TRUE);
			SetFontSize(FONT_L);
			DrawFormatString(centerx - GetDrawFormatStringWidth("%s %s %s", trleft.c_str(), op[logoper], trright.c_str()) / 2, tr_y,
				ORANGE, "%s %s %s", trleft.c_str(), op[logoper], trright.c_str());
			SetFontSize(FONT_M);
			DrawString(centerx - GetDrawStringWidth("← False　　　True →", strlen("← False　　　True →")) / 2, tfy, "← False　　　True →", WHITE);
			if (trick) DrawGraph(grx[tril], gry, trickgr, TRUE);
			if (treat) DrawGraph(grx[1 - tril], gry, treatgr, TRUE);
			DrawRotaGraph(centerx, trucky, 1, 0.01*truckf, truckgr, TRUE);
			SetFontSize(FONT_L);
			DrawString(centerx - GetDrawStringWidth("GAME OVER…", strlen("GAME OVER…")) / 2, goy, "GAME OVER…", WHITE);
			SetFontSize(FONT_M);
			DrawFormatString(centerx - GetDrawFormatStringWidth("集めたキャンディ:%d個", candy) / 2, resy, WHITE, "集めたキャンディ:%d個", candy);
			SetFontSize(FONT_S);
			DrawString(centerx - GetDrawStringWidth("PRESS ENTER KEY", strlen("PRESS ENTER KEY")) / 2, 380, "PRESS ENTER KEY", BLUE);
			truckf++;
			if (CheckHitKey(KEY_INPUT_RETURN))gamemode = 0;
		default:
			break;
		}
		ScreenFlip();
	}

	// ＤＸライブラリ使用の終了処理
	DxLib_End();

	return 0;            // ソフトの終了
}