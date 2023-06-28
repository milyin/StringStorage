#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "btree_packed_db.h"

// Program accepts three arguments:
// database name
// key
// file name
// and extracts value by key from database into file
//
//  example:
//
//  unpack_file base key file.txt
//

int main(int argc, char *argv[])
{
    // Show help if not enough arguments
    if (argc < 4)
    {
        // show ppurpose of program
        printf("This program extracts contents of file to packed database\n");
        printf("Key size: %d\n", BTREE_KEY_SIZE);
        printf("Node size: %d\n", BTREE_NODE_SIZE);
        printf("Usage: unpack_file base key file.txt\n");
        return 1;
    }

    // Create named arguments
    const char *basename = argv[1];
    const char *key = argv[2];
    const char *filename = argv[3];

    // Open database
    struct btree_packed_db *db = btree_packed_db_open(basename);
    if (!db)
    {
        printf("Error opening database\n");
        return 1;
    }

    // Create file and open for writing
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        printf("Error opening file %s\n", filename);
        return 1;
    }

    // Get value from database
    const char *value = btree_packed_db_get(db, key);
    if (!value)
    {
        printf("Error getting value\n");
        return 1;
    }

    // Write value to file
    fprintf(file, "%s", value);

    // Close file
    fclose(file);

    btree_packed_db_close(db);

    return 0;
}