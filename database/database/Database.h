#pragma once
#include <fstream>
#include <vector>
#include <map>
#include "../src/Compressor.h"
#include "../src/BPlusIndex.h"
#include "../src/FileHandler.h"

using namespace std;

class Database
{
public:
    void put(const string &key, const string &value);
    string get(const string &key) const;
    void remove(const string &key);
    bool contains(const string &key) const;

private:
    void message(const string &text);
};