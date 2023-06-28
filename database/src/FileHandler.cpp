#include "FileHandler.h"
#include <sys/stat.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

bool file_exists(const std::string &name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

ull file_size(std::string filename)
{
	struct stat stat_buf;
	int rc = stat(filename.c_str(), &stat_buf);
	return stat_buf.st_size;
}

FileHandler::~FileHandler()
{
	file.close();
}

bool FileHandler::open(const string &_filename)
{
	filename = _filename;
	write_pos = file_size(filename);
	file.open(filename, ios::in | ios::out | ios::app | ios::binary);
	return file.is_open();
}

bool FileHandler::create(const string &_filename)
{
	file.open(_filename, ios::out | ios::binary);
	if (!file.is_open())
		return false;
	file.close();
	return open(_filename);
}

void FileHandler::close()
{
	file.close();
}

Fragment FileHandler::write_vector(const vector<size_t> &vec)
{
	Fragment frag(write_pos, write_pos);
	frag.end += write_uint(vec.size());
	for (auto k : vec)
		frag.end += write_uint(k);
	write_pos = frag.end;
	return frag;
}

Fragment FileHandler::write_string(const string &str)
{
	Fragment frag(write_pos, write_pos);
	frag.end += write_uint(str.size());
	for (auto c : str)
		frag.end += write_char(c);
	write_pos = frag.end;
	return frag;
}

vector<size_t> FileHandler::read_vector()
{
	vector<size_t> res;
	size_t length = read_uint();
	for (size_t i = 0; !eof() && i < length; i++)
		res.push_back(read_uint());
	return res;
}

vector<size_t> FileHandler::read_vector(ull pos)
{
	read_pos(pos);
	return read_vector();
}

string FileHandler::read_string()
{
	string res;
	size_t length = read_uint();
	for (size_t i = 0; !eof() && i < length; i++)
		res.push_back(read_char());
	return res;
}

string FileHandler::read_string(ull pos)
{
	read_pos(pos);
	return read_string();
}

void FileHandler::read_pos(ull pos)
{
	file.seekp(pos * sizeof(unsigned char), ios::beg);
}

void FileHandler::clear()
{
	file.close();
	file.open(filename, ios::in | ios::out | ios::binary);
	write_pos = 0;
}

bool FileHandler::eof()
{
	char c;
	file.read(&c, sizeof(char));
	if (!file.eof())
	{
		file.putback(c);
		return 0;
	}
	return 1;
}

ull FileHandler::get_file_size()
{
	return write_pos;
}

size_t FileHandler::write_uint(size_t val)
{
	size_t counter = 0;
	if (val == 0)
	{
		unsigned char c = 128;
		file.write((const char *)&c, sizeof(char));
		return sizeof(unsigned char);
	}
	while (val)
	{
		unsigned char c = val % 128;
		if (val / 128 == 0)
			c += 128;
		file.write((const char *)&c, sizeof(unsigned char));
		val /= 128;
		counter++;
	}
	return counter * sizeof(unsigned char);
}

size_t FileHandler::write_char(char c)
{
	file.write((const char *)&c, sizeof(unsigned char));
	return sizeof(unsigned char);
}

size_t FileHandler::read_uint()
{
	unsigned char c;
	size_t acc = 0;
	size_t power = 1;

	do
	{
		file.read((char *)(&c), sizeof(unsigned char));
		acc += (c & ~128) * power;
		power *= 128;
	} while (!(c & 128));

	// cout << acc << "\n";
	return acc;
}

char FileHandler::read_char()
{
	char c;
	file.read((char *)(&c), sizeof(char));
	return c;
}

ostream &operator<<(ostream &stream, const Fragment &frag)
{
	stream << "Fragment(" << frag.begin << "," << frag.end << ")";
	return stream;
}
