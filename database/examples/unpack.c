#include <stdio.h>
#include <string.h>
#include "btree_packed_db.h"

// Program accepts three arguments:
// input database name
// input text file with keys
// output file with values
// and unpackes vaues from database into text file
//
//  example:
//
//  unpack base keys.txt values.txt
//
// Files base.index, base.data and base.dict must exist
//
int main(int argc, char *argv[])
{
    // Show help if not enough arguments
    if (argc < 4)
    {
        // show ppurpose of program
        printf("This program unpacks values from database into text file\n");
        printf("Key size: %d\n", BTREE_KEY_SIZE);
        printf("Node size: %d\n", BTREE_NODE_SIZE);
        printf("Usage: unpack base keys.txt values.txt\n");
        return 1;
    }

    // Create named arguments
    const char *basename = argv[1];
    const char *keysname = argv[2];
    const char *valuesname = argv[3];

    // Open database
    struct btree_packed_db *db = btree_packed_db_open(basename);
    if (!db)
    {
        printf("Error opening database\n");
        return 1;
    }

    // Open files with keys and values
    FILE *keys = fopen(keysname, "r");
    FILE *values = fopen(valuesname, "w");

    // Read keys and values from files
    char key[BTREE_KEY_SIZE];
    const char *value;
    while (fgets(key, sizeof(key), keys))
    {
        // remove trailing newline
        key[strlen(key) - 1] = 0;

        // get value from database
        value = btree_packed_db_get(db, key);

        // write value to file
        fprintf(values, "%s\n", value);
    }

    btree_packed_db_close(db);

    return 0;
}