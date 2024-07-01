#pragma once
#ifndef PROFILE_FUNCTION_H_SENTRY
#define PROFILE_FUNCTION_H_SENTRY
#include "settings.h"
char* get_steam_profile();
int check_profile(std::string& url, const char* fileName,
	void (*get_browser_code_in_file)(const std::string&, std::ofstream*));
char* set_url(settings& usr_stng);
#endif