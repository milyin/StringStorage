#include <stdio.h>
#include <string.h>
#include "btree_packed_db.h"

// Program accepts three arguments:
// database name
// text file with keys
// text file with values
// and packs them into database
//
//  example:
//
//  pack base keys.txt values.txt
//
// Files base.index, base.data and base.dict will be created
//
int main(int argc, char *argv[])
{
    // Show help if not enough arguments
    if (argc < 4)
    {
        // show ppurpose of program
        printf("This program creates packed database from two text files: with keys and with values\n");
        printf("Key size: %d\n", BTREE_KEY_SIZE);
        printf("Node size: %d\n", BTREE_NODE_SIZE);
        printf("Usage: pack base keys.txt values.txt\n");
        return 1;
    }

    // Create named arguments
    const char *basename = argv[1];
    const char *keysname = argv[2];
    const char *valuesname = argv[3];

    // Open database
    struct btree_packed_db *db = btree_packed_db_create(basename);
    if (!db)
    {
        printf("Error creating database\n");
        return 1;
    }

    // Open files with keys and values
    FILE *keys;
    fopen_s(&keys, keysname, "r");
    if (!keys)
    {
        printf("Error opening keys file\n");
        return 1;
    }

    FILE *values;
    fopen_s(&values, valuesname, "r");
    if (!values)
    {
        printf("Error opening values file\n");
        return 1;
    }

    // Read keys and values from files
    char key[BTREE_KEY_SIZE];
    char value[64 * 1024]; // limit value size to 64K
    while (fgets(key, sizeof(key), keys) && fgets(value, sizeof(value), values))
    {
        // remove trailing newline
        key[strlen(key) - 1] = 0;
        value[strlen(value) - 1] = 0;

        // put key and value into database
        btree_packed_db_put(db, key, value);
    }

    btree_packed_db_close(db);

    return 0;
}