#pragma once

#include <string>
#include "BPlusIndex.h"
#include "Compressor.h"
#include "FileHandler.h"
#include "btree_packed_db_config.h"

class BTreePackedDb
{
public:
    BTreePackedDb(const char *basename);
    bool create();
    bool open();
    void put(const std::string &key, const std::string &value);
    const char *get(const std::string &key);

private:
    std::string index_path;
    std::string data_path;
    std::string dictionary_path;

    BPlusIndexCharBuf<BTREE_KEY_SIZE, BTREE_NODE_SIZE> index;
    FileHandler data;
    Dictionary dictionary;

    std::string buf;
};