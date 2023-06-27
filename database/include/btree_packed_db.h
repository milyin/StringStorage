#pragma once

#include "btree_packed_db_config.h"

#ifdef __cplusplus
extern "C"
{
#endif

    struct btree_packed_db
    {
        void *data;
    };

    struct btree_packed_db *btree_packed_db_open(const char *basename);

    struct btree_packed_db *btree_packed_db_create(const char *basename);

    void btree_packed_db_close(struct btree_packed_db *db);

    void btree_packed_db_put(struct btree_packed_db *db, const char *key, const char *value);

    // returns pointer to internal buffer, do not free
    // buffer is valid until next call to btree_packed_db get or close
    const char *btree_packed_db_get(struct btree_packed_db *db, const char *key);

#ifdef __cplusplus
} // extern "C"
#endif
