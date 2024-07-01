#include <Windows.h>
#include <conio.h>
#include <iostream>
#include "main_menu.h"
#include "functions.h"
#include "macrosses.h"
#define CONSOLE_1251 1251


int set_low_process_priority()
{
	if (!(SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS))) {
		std::cerr << "Ошибка при установке приоритета процесса: " << GetLastError() << std::endl;
		return -1;
	}
	return 0;
}

int main()
{	

	int errcode = 0;
	std::setlocale(LC_ALL, "Russian");
	const int old_console_cp = GetConsoleCP();
	SetConsoleCP(CONSOLE_1251);
	if (GetConsoleCP() != CONSOLE_1251)
		errcode = -1;
	

	if (set_low_process_priority())
	{
		errcode = -2;
		goto exit;
	}
	main_menu();
	
exit:
#ifdef DEBUG
	SetConsoleCP(866);
#endif // DEBUG
#ifndef DEBUG
	SetConsoleCP(old_console_cp);
#endif // !DEBUG
	return errcode;

}