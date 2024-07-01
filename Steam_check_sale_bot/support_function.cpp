#include <Windows.h>
#include <string>
#include <fstream>
#include "support_function.h"
#include "functions.h"
#include "macrosses.h"
#include "settings.h"
#include "dir_creator.h"
#include "saver_8bytes_var.h"


bool is_url(const std::string& url)
{
	std::string steam_profile_base = "https://steamcommunity.com/";
	if (url.find(steam_profile_base) == std::string::npos)
	{
		PRINT_ERROR("Переданная строка: " + url + "\nне является ссылкой на профиль steam\n");
		MY_ERROR = USER_ID_NOT_FOUND;
		return 0;
	}
	return 1;
}

size_t get_response_to_string(void* contents, size_t size, size_t nmemb, void* str)
{
	((std::string*)str)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

size_t write_data(void* ptr, size_t size, size_t nmemb, std::ofstream* stream) {
	size_t written = size * nmemb; // Записываем данные в файл
	stream->write(static_cast<char*>(ptr), written);
	return written; // Возвращаем количество записанных элементов
}

int str_transfer(char** source, int size, bool mem_free = 0)
{
	int old_size = size;
	size += size / 2;
	char* tmp = new char[size];
	for (int i = 0; i < old_size; ++i)
	{
		tmp[i] = (*source)[i];
	}
	if (mem_free)
		delete[] * source;
	*source = tmp;
	return size;
}

void change_rubls(std::string& str)
{
	
	/*std::string rubs = "he33b";
	rubs[0] = 112;
	rubs[1] = -47;
	rubs[2] = -125;
	rubs[3] = -48;
	rubs[4] = -79;*/

	const int ofst_size = 5;
	int ofst[ofst_size] = {112,-47,-125,-48,-79};

	int i = 0, j = 0;
	int count_euqal = 0;
	for (; j < str.length(); j++)
	{
		if (str[j] == ofst[i])
		{
			count_euqal++;
			i++;
		}
		else
		{
			count_euqal = 0;
			i = 0;
		}

		if (count_euqal == ofst_size)
		{
			str[j - 4] = 'r';
			str[j - 3] = 'u';
			str[j - 2] = 'b';
			str[j - 1] = 'l';
			str[j] = 's';
			count_euqal = 0;
			j = 0;
		}
	}

}

const char* get_color(int color)
{
	switch (color)
	{
	case 0:
		return "\x1b[0m";
	case 1:
		return "\x1b[38;5;46m";
	case 2:
		return "\x1b[38;5;208m";
			
	}
}