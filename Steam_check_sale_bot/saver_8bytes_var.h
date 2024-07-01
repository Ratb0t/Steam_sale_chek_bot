#pragma once
#ifndef SAVER_8BYTES_VAR_H_SENTRY
#define SAVER_8BYTES_VAR_H_SENTRY
#include <fstream>

namespace saver {
	class filehandler
	{
		bool on_write_opend;
		bool on_read_opend;
		std::ifstream infile;
		std::ofstream outfile;
		void write(const char* str, std::streamsize _Count);
		void read(char* str, std::streamsize _Count);
	public:
		filehandler();
		void open_on_write(std::string absolute_path_file);
		void open_on_read(std::string absolute_path_file);
		void close();
		void seekp(int pos);
		void seekg(int pos);
		void write_param(int pos, void* param, int max_type_size_in_file);
		void read_param(int pos, void* ret_param, int max_type_size_in_file);
		bool is_open_on_read();
		bool is_open_on_write();
		~filehandler();
	};
}

#endif // !SAVER_8BYTES_VAR_H_SENTRY

