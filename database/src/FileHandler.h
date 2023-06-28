#pragma once
#include <fstream>
#include <vector>

using namespace std;
using ull = unsigned long long;

bool file_exists(const std::string &name);
ull file_size(std::string filename);

struct Fragment
{
	ull begin = 0;
	ull end = 0;
	Fragment() {}
	Fragment(ull begin, ull end) : begin(begin), end(end) {}
	friend ostream &operator<<(ostream &stream, const Fragment &frag);
};

/// <summary>
/// The class is responsible for reading and writing arrays of integers to a file.
/// </summary>
class FileHandler
{
public:
	~FileHandler();

	bool open(const string &filename);
	bool create(const string &filename);
	void close();
	Fragment write_vector(const vector<size_t> &vec);
	Fragment write_string(const string &str);

	vector<size_t> read_vector();
	vector<size_t> read_vector(ull pos);
	string read_string();
	string read_string(ull pos);

	void read_pos(ull pos);
	void clear();

	bool eof();

	ull get_file_size();

private:
	string filename;
	ull write_pos = 0;
	fstream file;
	size_t write_uint(size_t val);
	size_t write_char(char c);
	size_t read_uint();
	char read_char();
};