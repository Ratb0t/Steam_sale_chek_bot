#include <Windows.h>
#include <fstream>
#include <io.h>
#include "saver_8bytes_var.h"
#include "dir_creator.h"

namespace saver {
	filehandler::filehandler() : on_write_opend(false), on_read_opend(false) {}
	void filehandler::open_on_write(std::string absolute_path_file)
	{
		if (on_read_opend)
			throw std::exception("File opend on read\n");
		if (_access(absolute_path_file.c_str(), 0) != -1)
			outfile.open(absolute_path_file, std::ios::binary | std::ios::out | std::ios::in);
		else
			outfile.open(absolute_path_file, std::ios::binary | std::ios::out);
		on_write_opend = outfile.is_open();
	}
	void filehandler::open_on_read(std::string absolute_path_file)
	{
		if (on_write_opend)
			throw std::exception("File opend on read\n");
		infile.open(absolute_path_file);
		on_read_opend = true;
	}
	void filehandler::close()
	{
		if (on_read_opend)
		{
			infile.close();
			on_read_opend = false;
		}
		if (on_write_opend)
		{
			outfile.close();
			on_write_opend = false;
		}
	}
	void filehandler::seekp(int pos)
	{
		if(outfile.is_open() && on_write_opend)
			outfile.seekp(pos);
		else
			throw std::exception("File opend on read\n");
	}
	void filehandler::seekg(int pos)
	{
		if (infile.is_open() && on_read_opend)
			infile.seekg(pos);
		else
			throw std::exception("File opend on write\n");
	}
	void filehandler::write(const char* str, std::streamsize _Count)
	{
		if (outfile.is_open() && on_write_opend)
			outfile.write(str, _Count);
		else
			throw std::exception("File opend on read\n");
	}
	void filehandler::read(char* str, std::streamsize _Count)
	{
		if (infile.is_open() && on_read_opend)
			infile.read(str, _Count);
		else
			throw std::exception("File opend on read\n");
	}
	void filehandler::write_param(int pos, void* param, int max_type_size_in_file)
	{
		if (!this->is_open_on_write())
			throw std::exception("File is not opend on write\n");

		this->seekp(pos * max_type_size_in_file);
		this->write(reinterpret_cast<const char*>(param), max_type_size_in_file);
	}
	void filehandler::read_param(int pos, void* ret_param, int max_type_size_in_file)
	{

		if (!this->is_open_on_read())
			throw std::exception("File is not opend on read\n");

		this->seekg(pos * max_type_size_in_file);
		this->read(reinterpret_cast<char*>(ret_param), max_type_size_in_file);
	}
	bool filehandler::is_open_on_read()
	{
		return on_read_opend;
	}
	bool filehandler::is_open_on_write()
	{
		return on_write_opend;
	}
	filehandler::~filehandler()  
	{
		close();
	}
}
