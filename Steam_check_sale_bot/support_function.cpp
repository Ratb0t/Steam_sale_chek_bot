#include <Windows.h>
#include <string>
#include <fstream>
#include "support_function.h"
#include "functions.h"
#include "macrosses.h"
#include "settings.h"
#include "dir_creator.h"
#include "saver_8bytes_var.h"

#pragma warning(disable : 4996)
static const std::string standart_path = std::string(getenv("USERPROFILE")) + R"(\AppData\Local)";
#pragma warning(default : 4996)
void init_settings(settings& usr_settings)
{
	if (!is_path_exist((standart_path + R"(\Steam check sale bot)").c_str()))
		create_dir(standart_path, "Steam check sale bot");
	usr_settings.init_default = true;
	std::string config_path = standart_path + R"(\Steam check sale bot\config.cfg)";
	
	saver::filehandler file;
	if (is_path_exist(config_path.c_str()))
		file.open_on_read(config_path);

	
	
	if (file.is_open_on_read())
	{
		file.read_param(0,&usr_settings, sizeof(settings));
		usr_settings.init_default = false;
		file.close();
	}
	
	if (usr_settings.init_default)
	{
		usr_settings.url_is_save = false;
	}
}

void save_settings(settings& usr_settings)
{
	std::string config_path = standart_path;
	config_path = create_dir(config_path, "Steam check sale bot");
	if (!is_path_exist(config_path.c_str()))
		return;
	
	config_path += "\\config.cfg";
	
	saver::filehandler file;
	file.open_on_write(config_path);
	if (file.is_open_on_write() == false)
		return;

	file.write_param(0,&usr_settings, sizeof(settings));
	file.close();

}

bool save_url(std::string url)
{
	if(!is_path_exist((standart_path + R"(\Steam check sale bot)").c_str()))
		return false;
	std::ofstream outfile;
	outfile.open((standart_path + R"(\Steam check sale bot\steam_profile.sp)"), std::ios::out);
	outfile << url;
	return true;
}

char* read_url(settings const &stng)
{
	std::string url;
	if (!is_path_exist(
		(standart_path + R"(\Steam check sale bot\steam_profile.sp)").c_str()) &&
		!stng.url_is_save)
		return nullptr;


	std::ifstream infile;
	infile.open((standart_path + R"(\Steam check sale bot\steam_profile.sp)"), std::ios::in);
	infile >> url;
	infile.close();
	int url_size = url.size();
	char* url_ret = new char[url_size];
	std::getline(infile, url);
	int i = 0;
	for ( ; i < url_size; ++i)
	{
		url_ret[i] = url[i];
	}
	url_ret[i-1] = 0;
	return url_ret;
}

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

