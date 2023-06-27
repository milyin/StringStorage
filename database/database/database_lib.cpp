// database.cpp : Defines the entry point for the application.
//

#include "database_lib.h"
#include "Database.h"
#include "../src/FileHandler.h"
#include <iostream>

using namespace std;

int main()
{
	Dictionary dict;
	dict.create("dict.data");
	auto code = compress(dict, "biba");
	cout << decompress(dict, code);

	FileHandler fh; // ("test.data");
	fh.create("test.data");

	auto v1 = fh.write_vector({0});
	auto v2 = fh.write_vector({256});
	auto v3 = fh.write_vector({1024, 124243, 12312, 1337});

	cout << v1 << "\n"; // 0-1
	cout << v2 << "\n"; // 1-3
	cout << v3 << "\n"; // 1-3

	auto r1 = fh.read_vector(v1.begin);
	auto r2 = fh.read_vector(v2.begin);
	auto r3 = fh.read_vector(v3.begin);

	cout << "a";

	FileHandler fhs;
	if (!fhs.create("strs.data"))
	{
		cout << "Error creating file\n";
		return 1;
	}
	fhs.write_string("biba");
	fhs.write_string("boba");
	fhs.write_string("\n:-)\n");

	fhs.read_pos(0);
	while (!fhs.eof())
	{
		cout << "STR: " << fhs.read_string() << "\n";
	}

	return 0;
}
