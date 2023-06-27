#include "BTreePackedDb.h"
#include <iostream>

BTreePackedDb::BTreePackedDb(const char *basename)
    : index_path(basename), data_path(basename), dictionary_path(basename)
{
    index_path += ".index";
    data_path += ".data";
    dictionary_path += ".dict";
}

bool BTreePackedDb::open()
{
    if (!index.open(index_path.c_str()))
        return false;
    if (!data.open(data_path.c_str()))
        return false;
    if (!dictionary.open(dictionary_path.c_str()))
        return false;
    return true;
}

bool BTreePackedDb::create()
{
    if (!index.create(index_path.c_str()))
        return false;
    if (!data.create(data_path.c_str()))
        return false;
    if (!dictionary.create(dictionary_path.c_str()))
        return false;
    return true;
}

void BTreePackedDb::put(const std::string &key, const std::string &value)
{
    // Compress value
    auto compressed = compress(dictionary, value);
    // Add to data file
    auto fragment = data.write_vector(compressed);
    // Add position in data file to index
    // auto fragment = data.write_string(value);
    index.put(key.c_str(), (size_t)fragment.begin); // TODO: allow long long as index value
}

const char *BTreePackedDb::get(const std::string &key)
{
    // Get position in data file from index
    auto pos = index.get(key.c_str());
    if (!pos.has_value())
        return nullptr;

    // // Get compressed value from data file
    auto compressed = data.read_vector(pos.value());

    // // Decompress value
    buf = decompress(dictionary, compressed);

    // buf = data.read_string(pos.value());
    return buf.c_str();
}