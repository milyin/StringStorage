#include "Compressor.h"
#include <iostream>
#include <limits>

bool Dictionary::create(const string &filename)
{
	if (!fh.create(filename))
		return false;
	fh.close();
	return open(filename);
}

bool Dictionary::open(const string &filename)
{
	if (!fh.open(filename))
		return false;
	fh.read_pos(0);
	while (!fh.eof())
		add(fh.read_string(), 0);

	// TODO: if we don't reopen file, we cant paaend to the and, idk why
	fh.close();
	if (!fh.open(filename))
		return false;

	if (key_to_str.size() == 0)
	{
		for (unsigned k = 0; k < 256; k++)
		{
			string str = "";
			str += k;
			// cout << "!!! " <<  (int)k << " " << k << "\n";
			// cout << str << "\n";
			add(str);
		}
	}

	return true;
}

void Dictionary::add(const string &value, bool add_to_file)
{
	if (key_to_str.size() >= key_limit) {
		return;
	}
	// cout << "added \"" << value << "\"\n";
	size_t key = key_to_str.size();
	key_to_str.push_back(value);
	str_to_key[value] = key;
	if (add_to_file)
	{
		fh.write_string(value);
	}
}

string Dictionary::get_str(size_t key) const
{
	/*if (key < 256) {
		return "" + (char)key;
	}*/
	return key_to_str[key];
}

std::optional<size_t> Dictionary::get_key(const string &str) const
{
	/*if (str.size() == 1 && str[0] < 256) {
		return (size_t)str[0];
	}*/
	auto it = str_to_key.find(str);
	if (it == str_to_key.end())
		return {};
	return it->second;
}

bool Dictionary::contains(const string &str) const
{
	return str_to_key.count(str);
}

string Dictionary::operator[](size_t key) const
{
	return get_str(key);
}

size_t Dictionary::operator[](const string &str) const
{
	return get_key(str).value();
}

vector<size_t> compress(Dictionary &dict, const string &str)
{
	vector<size_t> res;
	if (str.length() == 0)
		return res;
	string acc = "";
	// std::cout << "str: '" << str << "' length = " << str.length() << "\n";
	for (char c : str)
	{
		// cout << "reading " << c << " : " << acc << "\n";
		if (dict.contains(acc + c))
		{
			// cout << "dict contains \"" + acc + c << "\"\n";
			acc += c;
		}
		else
		{
			// cout << "dict does not contain \"" + acc + c << "\"\n";
			res.push_back(dict[acc]);
			dict.add(acc + c);
			// cout << "added to dict \"" + acc + c << "\"\n";
			acc = c;
		}
	}
	// std::cout << "acc: " << acc << "\n";
	res.push_back(dict[acc]);
	return res;
}

string decompress(const Dictionary &dict, vector<size_t> arr)
{
	string res = "";
	if (arr.size() == 0)
		return res;
	for (auto key : arr)
	{
		res += dict[key];
	}
	return res;
}
