#include <iostream>
#include <fstream>
#include "profile_function.h"
#include "functions.h"
#include "support_function.h"
#include "macrosses.h"
char* get_steam_profile()
{
	int size = 256;
	char* str = new char[size];
	int i = 0;
	int char_code = getc(stdin);
	while (char_code != EOF && char_code != '\n')
	{
		str[i] = char_code;
		++i;
		if (i == size - 1)
			size = str_transfer(&str, i, 1);
		else
			char_code = getc(stdin);
	}
	str[i] = 0;
	return str;
}

int check_profile(std::string& url, const char* fileName,
	void (*get_browser_code_in_file)(const std::string&, std::ofstream*))
{
	if (FILENAME_MAX < strlen(fileName))
	{
		PRINT_ERROR("Слишком длинное имя файла\n");
		MY_ERROR = TOO_LONG_FILENAME;
		return -1;
	}
	if (!is_url(url))
	{
		MY_ERROR = -1;
		return -1;
	}

	std::ofstream outfile;
	outfile.open(fileName, std::ios::out); // Открываем файл для записи

	if (!outfile.is_open())
	{
		MY_ERROR = FILE_NOT_OPENED;
		return -1;
	}

	get_browser_code_in_file(url, &outfile);
	outfile.close();

	std::string line;
	std::string private_profile = "This profile is private.";
	std::ifstream infile;

	infile.open(fileName);
	if (!infile.is_open())
	{
		infile.close();
		return -1;
	}
	while (!infile.eof())
	{
		infile >> line;
		if (line.find("private_profile") != std::string::npos)
		{
			PRINT_ERROR("Профиль скрыт\n");
			infile.close();
			return -1;
		}
	}

	return 0;
}

char* set_url(settings & usr_stng)
{
	system("cls");
	std::cout << "Enter: ";
	fflush(stdout);
	char* url = get_steam_profile();
	usr_stng.url_is_save = save_url(url);
	
	return url;
}
