#define CURL_STATICLIB
#include <Windows.h>
//#include <nlohmann/json.hpp>
#include <iostream>
#include "functions.h"
#include <curl\curl.h>
#include <fstream>
#include <regex>
#include <nlohmann/json.hpp>
#include "profile_function.h"
#include "support_function.h"
#include "macrosses.h"
int MY_ERROR = -1;



void get_BrowserCode_in_file(const std::string& url, std::ofstream* outfile)
{
	CURL* curl;
	CURLcode response;
	curl = curl_easy_init();
	if (!curl)
	{
		PRINT_ERROR("�� ������� ���������������� curl\n");
		return;
	}
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // ������������� URL
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); // ������������� ������� ��������� ������ ��� ������ ������
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, outfile); // ������������� ����, � ������� ����� ���������� ������
	response = curl_easy_perform(curl); // ��������� �������� 
	curl_easy_cleanup(curl); // ��������� ����� libcurl

}

int get_path()
{
	OPENFILENAME ofn; // ��������� ��������� OPENFILENAME ��� �������� ���������� � ���������� ����
	wchar_t fileName[MAX_PATH]; // ��������� ������ wchar_t ��� �������� ���� � ���������� �����
	ZeroMemory(&fileName, sizeof(fileName)); // �������������� ������ fileName ������
	ZeroMemory(&ofn, sizeof(ofn)); // �������������� ��������� ofn ������
	ofn.lStructSize = sizeof(OPENFILENAME); // ������������� ������ ��������� ofn
	ofn.hwndOwner = NULL; // ������������� ��������� ����������� ���� (� ������ ������ ��� ���������)
	ofn.lpstrFilter = L"All Files\0*.*\0"; // ������������� ������ ��� ����������� ������ � ���������� ����
	ofn.lpstrFile = fileName; // ���������, ���� ��������� ���� � ���������� �����
	ofn.nMaxFile = MAX_PATH; // ������������� ������������ ������ ���� � �����
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY; // ������������� ����� ��� ����������� ����
	ofn.lpstrDefExt = L""; // ������������� ���������� �� ��������� ��� ���������� �����

	if (GetOpenFileName(&ofn)) {
		// ���� ��� ������ �������
		return 0; // ofn.lpstrFile;
	}
	else {
		// ��������� ������
		DWORD error = CommDlgExtendedError();
		switch (error) {
		case FNERR_BUFFERTOOSMALL:
			// ����� ���� ��������� MAX_PATH
			std::cout << MY_COLOR_ERR_RED
				"������: ����� ���� ��������� �����������" 
				"���������� (" << MAX_PATH << " ��������).\n";
			std::cout << MY_COLOR_RESET
				"����������, ������� ���� � �����, "
				"������� ������ ��� ����� " << MAX_PATH << " ��������.\n";
			break;
			// ������ ���� ������...
		case FNERR_FILENOTCHOSED:
			MY_ERROR = FNERR_FILENOTCHOSED;
			return 0;
			break;
		default:
			system("cls");
			std::cerr << MY_COLOR_ERR_RED << error;
			PRINT_ERROR("- ����������� ������");
			return -1;
			break;
		}
	}
	return 0;
}


void get_wishlist(std::string &url, const char* fileName,
	void (*get_browser_code_in_file)(const std::string&, std::ofstream*))
{
	if (FILENAME_MAX < strlen(fileName))
	{
		PRINT_ERROR("������� ������� ��� �����\n");
		MY_ERROR = TOO_LONG_FILENAME;
		return;
	}
	std::ofstream outfile;
	outfile.open(fileName, std::ios::out | std::ios::binary); // ��������� ���� ��� ������

	if (!outfile.is_open())
	{
		MY_ERROR = FILE_NOT_OPENED;
		return;
	}

	get_browser_code_in_file(url, &outfile);
	outfile.close();

}

void get_games(const std::string& url, const char* fileName, 
	void (*get_browser_code_in_file)(const std::string&, std::ofstream*))
{
	if (FILENAME_MAX < strlen(fileName))
	{
		PRINT_ERROR("������� ������� ��� �����\n");
		MY_ERROR = TOO_LONG_FILENAME;
	}

	std::ofstream outfile; //games.db
	outfile.open(fileName, std::ios::out | std::ios::binary); // ��������� ���� ��� ������
	if (!outfile.is_open())
	{
		MY_ERROR = FILE_NOT_OPENED;
		return;
	}
	
	std::ifstream infile;
	infile.open("wishlist.json");
	if (!infile.is_open())
	{
		MY_ERROR = FILE_NOT_OPENED;
		return;
	}

	std::string line, json_data;
	while (getline(infile, line))
	{
		if (line.find("g_rgWishlistData") != std::string::npos)
		{
			size_t start_line = line.find('[');
			size_t end_line = line.find(']');
			json_data = line.substr(start_line, end_line-2);
			json_data[end_line - start_line + 1] = 0;
			break;
		}
	}
	infile.close();
	nlohmann::json json;
	try
	{
		json = nlohmann::json::parse(json_data);
	}
	catch (nlohmann::json::parse_error& e)
	{
		std::cout << "������ ������� JSON: " << e.what() << '\n';
		system("pause");
	}

	CURL* curl;
	CURLcode response;
	curl = curl_easy_init();
	if (!curl)
	{
		PRINT_ERROR("�� ������� ���������������� curl\n");
		return;
	}
	
	Game game;
	
	std::string site_code;
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_response_to_string); // ������������� ������� ��������� ������ ��� ������ ������
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &site_code); // ������������� ����, � ������� ����� ���������� ������
	 // ��������� �������� 

	std::regex rgxDiscount(R"(<p class="game_purchase_discount_countdown">(.*?)</p>)");
	std::regex rgxPrice(R"(role="link" aria-label="(.*?)\"><div class=)");
	std::regex rgxName(R"(class="apphub_AppName">(.*?)</div>)");
	std::smatch matches;
	std::string tmp;

	size_t size;
	int i = 0;
	const std::string base_game_url = "https://store.steampowered.com/app/";
	for (const nlohmann::json_abi_v3_11_3::basic_json<>::value_type& item : json)
	{

		int appid = item["appid"];
		tmp = base_game_url + std::to_string(appid);
		curl_easy_setopt(curl, CURLOPT_URL, tmp.c_str());
		response = curl_easy_perform(curl); // ��������� �������� 
		
		if (site_code.find("game_purchase_action_bg") == std::string::npos)
		{
			continue;
		}
		if (std::regex_search(site_code, matches, rgxDiscount)) {
			game.countdown = matches[1].str();
		}
		if (std::regex_search(site_code, matches, rgxPrice)) {
			game.discount_info = matches[1].str();
		}
		if (std::regex_search(site_code, matches, rgxName)) {
			game.name = matches[1].str();
		}
		
		site_code.clear();
		
		change_rubls(game.discount_info);

		size = game.name.size();
		outfile.write(reinterpret_cast<const char*>(&size), sizeof(size));
		outfile.write(game.name.c_str(), size);
		
		size = game.discount_info.size();
		outfile.write(reinterpret_cast<const char*>(&size), sizeof(size));
		outfile.write(game.discount_info.c_str(), size);
		
		size = game.countdown.size();
		outfile.write(reinterpret_cast<const char*>(&size), sizeof(size));
		outfile.write(game.countdown.c_str(), size);
		if (i == 10)
		{
			system("cls");
			i = 0;
		}
		else
		{
			std::cout << '-';
			i++;
		}
#ifdef DEBUG
		std::cout << game.name << game.discount_info << '\n';
#endif
	}
	curl_easy_cleanup(curl);// ��������� ����� libcurl

	outfile.close();
#ifdef DEBUG
	system("pause");
#endif
}

int find_user(const char* url)
{//.com/
	const int offsets_size = 256, img_size = 5;
	char *offsets = new char [offsets_size];
	ZeroMemory(offsets, offsets_size);
	char img[img_size] = { '.', 'c', 'o', 'm', '/', };
	int url_len = strlen(url);
	if (url_len < img_size)
	{
		system("cls");
		std::cerr << MY_COLOR_ERR_RED"������ ���� ������� ������� ��������" MY_COLOR_RESET;
		return -1;
	}

	for (int i = 1, j = img_size-2; i < img_size; ++i, --j)
	{
		offsets[img[j]] = i;
	}
	if (offsets[img[img_size - 1]] == 0)
		offsets[img[img_size - 1]] = img_size;


 	int i = img_size - 1, j = img_size - 1;
	for (; j && i < url_len;)
	{
		if (url[i] == img[j])
		{
			--i;
			--j;
		}
		else
		{
			if (offsets[url[i]] == 0)
				i += img_size;
			else
				i += offsets[url[i]];
			j = img_size-1;
		}
	}
	delete[] offsets;
	if (j != 0)
		return -1;
	return i + img_size-1;

	
}

std::string create_url_on_wishlist(const std::string &url)
{//.com/
	if (!is_url(url))
	{
		return "";
	}
	std::string const base_wishlist_url = "https://store.steampowered.com/wishlist";
	int index_usr_start = find_user(url.c_str());
	if (index_usr_start == -1)
	{
		PRINT_ERROR("ID ������������ �� ������\n");
		MY_ERROR = USER_ID_NOT_FOUND;
		return "";
	}
	std::string tmp = (url.c_str() + index_usr_start);
	tmp = base_wishlist_url + tmp;
	return tmp;
}

void find_games(const char* url)
{
	std::string url_steam_prifile = url;
	if (check_profile(url_steam_prifile, "wishlist.json", get_BrowserCode_in_file) == -1)
	{
		MY_ERROR = -1;
		return;
	}


	std::string url_wish = create_url_on_wishlist(url);
	if (MY_ERROR == USER_ID_NOT_FOUND)
	{
		MY_ERROR = -1;
		return;
	}
	
	
	get_wishlist(url_wish, "wishlist.json", get_BrowserCode_in_file);
	if (MY_ERROR == FILE_NOT_OPENED)
	{
		MY_ERROR = -1;
		return;
	}
		
	get_games(url_wish, "games.db", get_BrowserCode_in_file);
	if (MY_ERROR == FILE_NOT_OPENED)
	{
		MY_ERROR = -1;
		return;
	}
}


void show_discont_data()
{
	Game game;
	std::fstream infile;
	infile.open("games.db");
	if (!infile.is_open())
	{
		PRINT_ERROR("���� games.db �� ��������");
		return;
	}
	size_t size;
	while (infile.read(reinterpret_cast<char*>(&size), sizeof(size)))
	{
		try
		{			
			game.name.resize(size);
			infile.read(&game.name[0], size);
		}
		catch (const std::exception& e)
		{
			std::cerr << "������ ��� ������ ����� ����: "; PRINT_ERROR(e.what());
			goto quit;
		}


		try
		{
			infile.read(reinterpret_cast<char*>(&size), sizeof(size));
			game.discount_info.resize(size);
			infile.read(&game.discount_info[0], size);
		}
		catch (const std::exception& e)
		{
			std::cerr << "������ ��� ������ ��������: "; PRINT_ERROR(e.what());
			goto quit;
		}

		try
		{
			infile.read(reinterpret_cast<char*>(&size), sizeof(size));
			game.countdown.resize(size);
			infile.read(&game.countdown[0], size);
		}
		catch (const std::exception& e)
		{
			std::cerr << "������ ��� ������ ����� ������: "; PRINT_ERROR(e.what());
			goto quit;
		}


		std::cout << game.name << ' ' << game.discount_info << '\n' << game.countdown << '\n';
		std::cout << "-----------------------------------------------------------------------------------\n";
		game.name = "", game.discount_info = "", game.countdown = "";
	}
quit:
	infile.close();
}