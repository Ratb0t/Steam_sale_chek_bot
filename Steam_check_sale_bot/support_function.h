#pragma once
#ifndef SUPPORT_FUNCTION_H_SENTRY
#define SUPPORT_FUNCTION_H_SENTRY
#include "settings.h"

struct Game {
	std::string name;
	std::string discount_info;
	std::string countdown;
};

bool is_url(const std::string& url);
size_t write_data(void* ptr, size_t size, size_t nmemb, std::ofstream* stream);
int str_transfer(char** source, int size, bool mem_free);
size_t get_response_to_string(void* contents, size_t size, size_t nmemb, void* str);

void change_rubls(std::string & str);

void init_settings(settings &usr_settings);
void save_settings(settings& usr_settings);
bool save_url(std::string url);
char* read_url(settings const& stng);
#endif