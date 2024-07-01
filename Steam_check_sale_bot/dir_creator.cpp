#include <Windows.h>
#include <direct.h>
#include <iostream>
#include <sys/stat.h>
#include "dir_creator.h"
std::string create_dir(std::string path, std::string dir_name)
{
	int path_size = path.size();
	int dir_name_size = dir_name.size();
	std::string full_path = path + '\\' + dir_name;

	int result = _mkdir(full_path.c_str());
	
	return full_path;
}

bool is_path_exist(const char* path)
{
	struct stat buf;
	if (stat(path, &buf) == 0)
		return 1;
	else
		return 0;
}
