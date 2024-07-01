#include "settings.h"
#include "dir_creator.h"
#include "saver_8bytes_var.h"
#include "macrosses.h"
#include "support_function.h"
#include <string>
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
		file.read_param(settings_struct_pos, &usr_settings, sizeof(settings));
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

	file.write_param(settings_struct_pos, &usr_settings, sizeof(settings));
	file.close();

}

bool save_url(std::string url)
{
	if (!is_path_exist((standart_path + R"(\Steam check sale bot)").c_str()))
		return false;
	std::ofstream outfile;
	outfile.open((standart_path + R"(\Steam check sale bot\strings.sp)"), std::ios::out);
	outfile << url;
	return true;
}

char* read_url(settings const& stng)
{
	std::string url;
	if (!is_path_exist(
		(standart_path + R"(\Steam check sale bot\strings.sp)").c_str()) &&
		!stng.url_is_save)
		return nullptr;


	std::ifstream infile;
	infile.open((standart_path + R"(\Steam check sale bot\strings.sp)"), std::ios::in);
	infile >> url;
	infile.close();
	int url_size = url.size();
	char* url_ret = new char[url_size + 1];
	std::getline(infile, url);
	int i = 0;
	for (; i < url_size; ++i)
	{
		url_ret[i] = url[i];
	}
	url_ret[url_size] = 0;
	return url_ret;
}