#pragma once
#ifndef DIR_CREATOR_H_SENTRY
#define DIR_CREATOR_H_SENTRY
#include <string>
std::string create_dir(std::string path, std::string dir_name);
bool is_path_exist(const char* path);
#endif // !DIR_CREATOR_H_SENTRY
