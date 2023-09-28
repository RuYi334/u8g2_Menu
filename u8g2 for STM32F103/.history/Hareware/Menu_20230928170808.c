#include "Menu.h"
#include "Key.h"
#include "u8g2.h"
#include "picture.h"
#include <stdio.h>
#include "Delay.h"
#include <string.h>

Speed_ENUM Speed_choose;

extern u8g2_t u8g2;
int16_t display = 48;
int16_t diaplay_trg = 1;
uint8_t circle_num;
uint8_t KeyNum;
static uint8_t Picture_Flag = 0; // ����ѡ���־λ
const char words[][10] = {{"GAME\0"}, {"MESSAGE\0"}, {"SETTING\0"}};
uint8_t Game_Menu_Flag = 0;

uint8_t MenuStatus;

uint8_t MainMenuQuantity = 0;

/*-----------------------------*/
mytime_t myTime = {13, 14, 0, 2023, 9, 26};
mytime_t myTimeTemp = {13, 14, 0, 2023, 9, 26};

void MyTimeSecondRun()
{
	static int timeSecondCount = 0;
	timeSecondCount++;
	if (timeSecondCount > 33)
	{
		timeSecondCount = 0;
		myTime.Second++;
	}
}

void MyTimeFun()
{
	if (myTime.Second > 59)
	{
		myTime.Second = 0;
		myTime.Minute++;
	}
	if (myTime.Minute > 59)
	{
		myTime.Minute = 0;
		myTime.Hour++;
	}
	if (myTime.Hour > 23)
	{
		myTime.Hour = 0;
	}
}

/*-----------------------------*/

void ui_run(char *a, char *a_trg, int b)
{
	if (*a < *a_trg)
	{
		*a += b;
		if (*a > *a_trg) // ��ֹ�ӹ�ͷ
			*a = *a_trg;
	}
	if (*a > *a_trg)
	{
		*a -= b;
		if (*a < *a_trg) // ��ֹ����ͷ
			*a = *a_trg;
	}
}

void ui_right_one_Picture(int16_t *a, int b)
{
	uint8_t i = 0;
	if ((i <= 48))
	{
		*a += b;
		i += b;
	}
}

void ui_left_one_Picture(int16_t *a, int b)
{
	uint8_t i = 0;
	if ((i <= 48))
	{
		*a -= b;
		i += b;
	}
}

/*�������˵�*/
MainMenu MainMenu_Config[3] = {
	{game, "GAME", NULL},
	{wechat, "MESSAGE", NULL},
	{setting, "SETTING", NULL}};

/*��������Ӳ˵��ṹ��*/
ChildMenu GameMune[4] = {
	{"game1:FlyBird", NULL, NULL, MAIN_TO_CHILD},
	{"game2:Dinosaur Rex", NULL, NULL, MAIN_TO_CHILD},
	{"game3:Stick Fight", NULL, NULL, MAIN_TO_CHILD},
	{"game4:Tetris", NULL, NULL, MAIN_TO_CHILD}};

ChildMenu SettingMune[4] = {
	{"setting1:FlyBird", NULL, NULL, MAIN_TO_CHILD},
	{"setting2:Dinosaur Rex", NULL, NULL, MAIN_TO_CHILD},
	{"setting3:Stick Fight", NULL, NULL, MAIN_TO_CHILD},
	{"setting4:Tetris", NULL, NULL, MAIN_TO_CHILD}};

ChildMenu GameChild4[4] = {
	{"this is set1", NULL, NULL, CHILD_TOCHILD},
	{"this is set2", NULL, NULL, CHILD_TOCHILD},
	{"this is set3", NULL, NULL, CHILD_TOCHILD},
	{"this is set4", NULL, NULL, CHILD_TOCHILD}};

/*����MainMenu��ChildMenu*/
void UIConnect(void)
{
	MainMenu_Config[0].ChildMenu = GameMune;
	MainMenu_Config[2].ChildMenu = SettingMune;
	GameMune[3].child = GameChild4;
}

void Show_Menu_Config(void)
{
	UIConnect();
	MainMenuQuantity = sizeof(MainMenu_Config) / sizeof(MainMenu);
	u8g2_SetFontMode(&u8g2, 1);					  // ��������ģʽ��mode����ģʽ�����ã�1������ã�0��͸��ģʽ
	u8g2_SetFontDirection(&u8g2, 0);			  // �������巽��
	u8g2_SetFont(&u8g2, u8g2_font_spleen6x12_mf); // ���������ʽ

	u8g2_DrawXBM(&u8g2, 44, 36, 40, 40, arrowhead); // �����ͷ��־ͼƬ
	for (int i = 0; i < MainMenuQuantity; i++)
	{
		u8g2_DrawXBM(&u8g2, display + 48 * i, 16, 32, 32, MainMenu_Config[i].ImageData);
	}
	u8g2_SendBuffer(&u8g2); // ��Ҫ��ʾ����Ϣд�뻺��
}

void Show_MenuRE(Speed_ENUM Speed_choose)
{
	if (MenuStatus == MENU_SHOW)
	{
		while (1)
		{
			// ͬ��ʱ��
			MyTimeSecondRun();
			Show_Menu(fast_speed);
			if (Game_Menu_Flag == 4)
			{
				MenuStatus = MENU_MAIN;
				// Show_MainPage();
				// printf("key4\r\n");
				break;
			}
		}
	}
}

void Show_Menu(Speed_ENUM Speed_choose) // ��ʾ�˵�
{
	KeyNum = Key_GetNum();
	Game_Menu_Flag = KeyNum;

	if ((KeyNum == 1) && (display > -48))
	{
		Picture_Flag++;					// ͼƬ��־λ����ʾ"GAME","MESSAGE","SETTING"
		circle_num = 48 / Speed_choose; // ѭ�����ٴΣ��붯̬�ٶ��й�ϵ���ٶ�Խ��Լ˿��
		while (circle_num)
		{
			u8g2_ClearBuffer(&u8g2); // �����������ڴ�λ�ã��Ų��ᵼ�»��汻��գ�
			ui_left_one_Picture(&display, Speed_choose);
			u8g2_DrawXBM(&u8g2, 44, 36, 40, 40, arrowhead);
			/*ѭ����ʾ�˵�ͼ��*/
			for (int i = 0; i < MainMenuQuantity; i++)
			{
				u8g2_DrawXBM(&u8g2, display + 48 * i, 16, 32, 32, MainMenu_Config[i].ImageData);
			}
			u8g2_DrawStr(&u8g2, 52, 10, "MENU:");
			u8g2_SendBuffer(&u8g2);

			circle_num--;
		}

		KeyNum = 0;
	}

	else if ((KeyNum == 2) && (display < 48))
	{
		Picture_Flag--;

		circle_num = 48 / Speed_choose;
		while (circle_num)
		{
			u8g2_ClearBuffer(&u8g2);
			ui_right_one_Picture(&display, Speed_choose);
			u8g2_DrawXBM(&u8g2, 44, 36, 40, 40, arrowhead);
			u8g2_DrawXBM(&u8g2, display, 16, 32, 32, game);
			u8g2_DrawXBM(&u8g2, display + 48, 16, 32, 32, wechat);
			u8g2_DrawXBM(&u8g2, display + 96, 16, 32, 32, setting);
			u8g2_DrawStr(&u8g2, 52, 10, "MENU:");
			u8g2_SendBuffer(&u8g2);
			circle_num--;
		}
		KeyNum = 0;
	}
	else
		;

	switch (Picture_Flag)
	{
	case 0:
		u8g2_DrawStr(&u8g2, 52, 10, "MENU:");
		u8g2_DrawStr(&u8g2, 82, 10, &words[0][0]);
		u8g2_SendBuffer(&u8g2);
		Creat_Childv2_Menu(MainMenu_Config->ChildMenu, Picture_Flag, MainMenu_Config[Picture_Flag].MenuName);
		break;

	case 1:
		u8g2_DrawStr(&u8g2, 52, 10, "MENU:");
		u8g2_DrawStr(&u8g2, 82, 10, &words[1][0]);
		u8g2_SendBuffer(&u8g2);
		break;

	case 2:
		u8g2_DrawStr(&u8g2, 52, 10, "MENU:");
		u8g2_DrawStr(&u8g2, 82, 10, &words[2][0]);
		u8g2_SendBuffer(&u8g2);
		Creat_Childv2_Menu(MainMenu_Config->ChildMenu, Picture_Flag, MainMenu_Config[Picture_Flag].MenuName);
		break;
	}
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void Creat_Childv2_Menu(ChildMenu *menuitem, uint8_t Picture_Flag, char *Title)
{
	static char Box_x = 1; // ����С������ֲ�����ʼֵ
	static char Box_y = 14;
	static char Box_w = 84;
	static char Box_h = 13;

	static char Box_x_trg; // ����С����Ŀ��ֵ
	static char Box_y_trg;
	static char Box_w_trg;
	static char Box_h_trg;

	static int8_t Box_Flag = 0;
	static int8_t Box_Flag_tmp = 0;

	if (Game_Menu_Flag == 3) // ȷ�ϼ�
	{
		// To_Game_Menu_Display();
		Box_Flag = 0;
		Menu_To_X_Display(menuitem);
		while (1)
		{

			Box_Flag_tmp = Box_Flag;
			char *child_father_name = menuitem[Box_Flag].name;
			Game_Menu_Flag = Key_GetNum();
			u8g2_ClearBuffer(&u8g2);
			// TODO:���ݱ��ⳤ�Ƚ��о��д���
			u8g2_DrawStr(&u8g2, 52, 10, Title);

			u8g2_DrawLine(&u8g2, 1, 13, 128, 13);
			for (int i = 0; i < 4; i++)
			{
				u8g2_DrawStr(&u8g2, 3, 24 + i * 12, menuitem[i].name);
			}
			ui_run(&Box_x, &Box_x_trg, 1);
			ui_run(&Box_y, &Box_y_trg, 1);
			ui_run(&Box_w, &Box_w_trg, 2);
			ui_run(&Box_h, &Box_h_trg, 1);

			u8g2_DrawFrame(&u8g2, Box_x, Box_y, Box_w, Box_h);
			u8g2_SendBuffer(&u8g2);

			if (Game_Menu_Flag == 1)
			{
				Box_Flag++;
				if (Box_Flag >= 3)
					Box_Flag = 3;
			}

			if (Game_Menu_Flag == 2)
			{
				Box_Flag--;
				if (Box_Flag <= 0)
					Box_Flag = 0;
			}

			if (Game_Menu_Flag == 4)
			{
				u8g2_ClearBuffer(&u8g2);
				X_To_Menu_Display(menuitem);
				Game_Menu_Flag = 0;
				break;
			}

			switch (Box_Flag) // ��ȡ�������Ŀ��ֵ
			{
			case 0:
				Box_x_trg = 1;
				Box_y_trg = 14;
				Box_w_trg = strlen(menuitem[Box_Flag].name) * 6 + 6;
				Box_h_trg = 13;
				if (menuitem[Box_Flag].child != NULL)
					Creat_Childv2_Menu(menuitem[Box_Flag].child, Picture_Flag, child_father_name);
				break;
			case 1:
				Box_x_trg = 1;
				Box_y_trg = 27;
				Box_w_trg = strlen(menuitem[Box_Flag].name) * 6 + 6;
				Box_h_trg = 13;
				if (menuitem[Box_Flag].child != NULL)
					Creat_Childv2_Menu(menuitem[Box_Flag].child, Picture_Flag, child_father_name);
				break;
			case 2:
				Box_x_trg = 1;
				Box_y_trg = 38;
				Box_w_trg = strlen(menuitem[Box_Flag].name) * 6 + 6;
				Box_h_trg = 13;
				if (menuitem[Box_Flag].child != NULL)
					Creat_Childv2_Menu(menuitem[Box_Flag].child, Picture_Flag, child_father_name);
				break;
			case 3:
				Box_x_trg = 1;
				Box_y_trg = 51;
				Box_w_trg = strlen(menuitem[Box_Flag].name) * 6 + 6;
				Box_h_trg = 12;
				if (menuitem[Box_Flag].child != NULL)
					Creat_Childv2_Menu(menuitem[Box_Flag].child, Picture_Flag, child_father_name);
				break;
			}
		}
	}
	display = 48 - Picture_Flag * 48;
	Picture_Flag = Picture_Flag;
}

/*
�˵����Ӳ˵�����
@menuitem��ָ��MenuItem�ṹ�������ָ��
*/
void Menu_To_X_Display(ChildMenu *menuitem) // �˵�����Ϸ�˵����ɶ���
{

	char Game_Menu_Display = 10;
	char Game_Menu_Display_trg = 74;

	while (Game_Menu_Display != Game_Menu_Display_trg)
	{

		u8g2_ClearBuffer(&u8g2);
		u8g2_DrawStr(&u8g2, 52, Game_Menu_Display, "MENU:");					   // YҪ��Ϊ0��72��MENU���Ż���ȫ��ʧ
		u8g2_DrawXBM(&u8g2, 44, Game_Menu_Display + 26, 40, 40, arrowhead);		   // �����ͷ��־ͼƬ
		u8g2_DrawXBM(&u8g2, display, Game_Menu_Display + 6, 32, 32, game);		   // ������Ϸͼ��ͼƬ
		u8g2_DrawXBM(&u8g2, display + 48, Game_Menu_Display + 6, 32, 32, wechat);  // ������Ϣͼ��ͼƬ
		u8g2_DrawXBM(&u8g2, display + 96, Game_Menu_Display + 6, 32, 32, setting); // ��������ͼ��ͼƬ
		ui_run(&Game_Menu_Display, &Game_Menu_Display_trg, 8);
		u8g2_SendBuffer(&u8g2); // ��Ҫ��ʾ����Ϣд�뻺��
	}

	Game_Menu_Display = 74;
	Game_Menu_Display_trg = 10;

	while (Game_Menu_Display != Game_Menu_Display_trg)
	{
		u8g2_ClearBuffer(&u8g2);
		u8g2_DrawStr(&u8g2, 52, Game_Menu_Display, "GAME"); // Game_Menu_Display+16+62Ϊ72����ʱ��Game����ʧ
		u8g2_DrawLine(&u8g2, 1, Game_Menu_Display + 3, 128, Game_Menu_Display + 3);
		u8g2_DrawStr(&u8g2, 3, Game_Menu_Display + 14, menuitem[0].name);
		u8g2_DrawStr(&u8g2, 3, Game_Menu_Display + 26, menuitem[1].name);
		u8g2_DrawStr(&u8g2, 3, Game_Menu_Display + 38, menuitem[2].name);
		u8g2_DrawStr(&u8g2, 3, Game_Menu_Display + 50, menuitem[3].name);
		ui_run(&Game_Menu_Display, &Game_Menu_Display_trg, 8);
		u8g2_SendBuffer(&u8g2); // ��Ҫ��ʾ����Ϣд�뻺��
	}
}
/*
�Ӳ˵��������˵�����
@menuitem��ָ��MenuItem�ṹ�������ָ��
*/
void X_To_Menu_Display(ChildMenu *menuitem)
{
	char Game_Menu_Display = 10;
	char Game_Menu_Display_trg = 74;

	while (Game_Menu_Display != Game_Menu_Display_trg)
	{
		u8g2_ClearBuffer(&u8g2);
		u8g2_DrawStr(&u8g2, 52, Game_Menu_Display, "GAME"); // Game_Menu_Display+16+62Ϊ72����ʱ��Game����ʧ
		u8g2_DrawLine(&u8g2, 1, Game_Menu_Display + 3, 128, Game_Menu_Display + 3);
		u8g2_DrawStr(&u8g2, 3, Game_Menu_Display + 14, menuitem[0].name);
		u8g2_DrawStr(&u8g2, 3, Game_Menu_Display + 26, menuitem[1].name);
		u8g2_DrawStr(&u8g2, 3, Game_Menu_Display + 38, menuitem[2].name);
		u8g2_DrawStr(&u8g2, 3, Game_Menu_Display + 50, menuitem[3].name);
		ui_run(&Game_Menu_Display, &Game_Menu_Display_trg, 8);
		u8g2_SendBuffer(&u8g2); // ��Ҫ��ʾ����Ϣд�뻺��
	}
	Game_Menu_Display = 74;
	Game_Menu_Display_trg = 10;

	while (Game_Menu_Display != Game_Menu_Display_trg)
	{
		u8g2_ClearBuffer(&u8g2);
		ui_run(&Game_Menu_Display, &Game_Menu_Display_trg, 8);
		u8g2_DrawStr(&u8g2, 52, Game_Menu_Display, "MENU:");					   // YҪ��Ϊ0��>=72��MENU���Ż���ȫ��ʧ
		u8g2_DrawXBM(&u8g2, 44, Game_Menu_Display + 26, 40, 40, arrowhead);		   // �����ͷ��־ͼƬ
		u8g2_DrawXBM(&u8g2, display, Game_Menu_Display + 6, 32, 32, game);		   // ������Ϸͼ��ͼƬ
		u8g2_DrawXBM(&u8g2, display + 48, Game_Menu_Display + 6, 32, 32, wechat);  // ������Ϣͼ��ͼƬ
		u8g2_DrawXBM(&u8g2, display + 96, Game_Menu_Display + 6, 32, 32, setting); // ��������ͼ��ͼƬ
		u8g2_SendBuffer(&u8g2);													   // ��Ҫ��ʾ����Ϣд�뻺��
	}
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void Show_MainPage(void)
{
	if (MenuStatus == MENU_MAIN)
	{

		MyTimeFun();
		KeyNum = Key_GetNum();
		u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr);
		u8g2_ClearBuffer(&u8g2);
		// ������
		for (int i = 1; i < 15; i++)
		{
			u8g2_DrawHLine(&u8g2, 0, 8, 128);
			u8g2_DrawHLine(&u8g2, 0, 56, 128);
		}

		// ��ʾʱ��
		char timeStr[9];
		sprintf(timeStr, "%02d:%02d:%02d", myTime.Hour, myTime.Minute, myTime.Second);
		u8g2_DrawStr(&u8g2, 25, 28, timeStr);

		// ��ʾ����
		u8g2_SetFont(&u8g2, u8g2_font_helvR12_tr);
		char dateStr[11];
		sprintf(dateStr, "%04d/%02d/%02d", myTime.Year, myTime.Month, myTime.Day);
		u8g2_DrawStr(&u8g2, 22, 48, dateStr);
		u8g2_SendBuffer(&u8g2);
	}
}
