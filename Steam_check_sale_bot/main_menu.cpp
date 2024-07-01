#include <Windows.h>
#include <conio.h>
#include <iostream>
#include "main_menu.h"
#include "functions.h"
#include "macrosses.h"
#include "dir_creator.h"
#include "support_function.h"
#include "saver_8bytes_var.h"
#include "settings.h"
#include "profile_function.h"

void main_menu()
{
	settings user_settings;
	init_settings(user_settings);
	char* url = nullptr;
	int x = 0;
	bool clean_console = false;
	const char escape = 27;
	fflush(stdout);
	PRINT_MENU;
	while ((x = _getch()) != escape)
	{
		switch (x)
		{
		case '1':
			url = set_url(user_settings);
			save_url(url);
			clean_console = true;
			break;
		case '2':
			get_path();
			MY_ERROR = 0;
			break;
		case '3':
			show_discont_data();
			Sleep(1500);
			//clean_console = true;
			break;
		case '4':
			if (user_settings.url_is_save)
			{
				system("cls");
				std::cout << "Начался поиск. Может занять какое-то время\n";
				if (url == nullptr)
					url = read_url(user_settings);
				find_games(url);
			}
			else
			{
				PRINT_ERROR("Ссылка на профиль steam не установлена\n");
			}
			clean_console = true;
			break;
		default:
			continue;
			break;
		}
		if (clean_console)
		{
			system("cls");
			clean_console = false;
		}
		PRINT_MENU;
	}
	if (url)
		delete[] url;
	save_settings(user_settings);
}