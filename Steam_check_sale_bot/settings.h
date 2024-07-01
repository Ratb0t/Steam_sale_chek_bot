#pragma once
#ifndef SETTINGS_H_SENTRY
#define SETTINGS_H_SENTRY
#include <string>
struct settings
{
	bool init_default;
	bool url_is_save;
	int color_old_price;
	int color_new_price;
};

void init_settings(settings& usr_settings);
void save_settings(settings& usr_settings);
bool save_url(std::string url);
char* read_url(settings const& stng);

#endif // !SETTINGS_H_SENTRY
