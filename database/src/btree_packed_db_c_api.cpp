#include "BTreePackedDb.h"

extern "C" struct btree_packed_db *btree_packed_db_open(const char *basename)
{
    auto db = new BTreePackedDb(basename);
    if (!db->open())
    {
        delete db;
        return nullptr;
    }
    return reinterpret_cast<struct btree_packed_db *>(db);
}

extern "C" struct btree_packed_db *btree_packed_db_create(const char *basename)
{
    auto db = new BTreePackedDb(basename);
    if (!db->create())
    {
        delete db;
        return nullptr;
    }
    return reinterpret_cast<struct btree_packed_db *>(db);
}

extern "C" void btree_packed_db_close(struct btree_packed_db *db)
{
    delete reinterpret_cast<BTreePackedDb *>(db);
}

extern "C" void btree_packed_db_put(struct btree_packed_db *db, const char *key, const char *value)
{
    reinterpret_cast<BTreePackedDb *>(db)->put(key, value);
}

extern "C" const char *btree_packed_db_get(struct btree_packed_db *db, const char *key)
{
    return reinterpret_cast<BTreePackedDb *>(db)->get(key);
}
