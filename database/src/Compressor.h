#pragma once
#include <string>
#include <memory>
#include <fstream>
#include <map>
#include <vector>
#include <optional>
#include "FileHandler.h"

using namespace std;

class Dictionary
{
public:
	bool open(const string &filename);
	bool create(const string &filename);

	void add(const string &value, bool add_to_file = 1);
	string get_str(size_t key) const;
	std::optional<size_t> get_key(const string &str) const;
	bool contains(const string &str) const;

	// Alloed to throw exception if key or string is not found
	string operator[](size_t key) const;
	size_t operator[](const string &str) const;

private:
	FileHandler fh;
	// Todo: change from string to descriptor
	vector<string> key_to_str;
	map<string, size_t> str_to_key;
	// map<string, bool> loaded;
};

vector<size_t> compress(Dictionary &dict, const string &str);
string decompress(const Dictionary &dict, vector<size_t>);
