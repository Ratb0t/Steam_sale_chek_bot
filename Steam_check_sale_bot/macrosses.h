#pragma once
#ifndef MACROSESS_H_SENTRY
#define MACROSESS_H_SENTRY
#include <iostream>

#define PRINT_ERROR(str) system("cls");std::cerr << MY_COLOR_ERR_RED <<(str)<< MY_COLOR_RESET; system("pause")
#define MY_COLOR_ERR_RED "\x1b[1;38;5;196m"
#define MY_COLOR_RESET "\x1b[0m"
#define XDEBUG
enum { FNERR_FILENOTCHOSED = 0, USER_ID_NOT_FOUND, FILE_NOT_OPENED, TOO_LONG_FILENAME, BAD_INIT_CURL, };
enum settings_pos{ init_default, url_is_save, };
//#define MAX_TYPE_SYZE_SETTING 8





#endif // !


