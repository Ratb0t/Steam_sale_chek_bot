#pragma once
#ifndef FUNCTIONS_H_SENTRY
#define FUNCTIONS_H_SENTRY
#include <string>
#include "settings.h"
char* get_steam_profile();
wchar_t* get_path();
void find_games(const char*);
void show_discont_data(const settings &usr_stng);
extern int MY_ERROR;
#endif