#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "btree_packed_db.h"

// Program accepts two arguments:
// database name
// file name
// and adds file content to database uisng file name as key
//
//  example:
//
//  pack base file.txt
//
// Files base.index, base.data and base.dict will be created or updated
//

int main(int argc, char *argv[])
{
    // Show help if not enough arguments
    if (argc < 3)
    {
        // show ppurpose of program
        printf("This program adds contents of file to packed database\n");
        printf("Key size: %d\n", BTREE_KEY_SIZE);
        printf("Node size: %d\n", BTREE_NODE_SIZE);
        printf("Usage: pack base keys.txt values.txt\n");
        return 1;
    }

    // Create named arguments
    const char *basename = argv[1];
    const char *filename = argv[2];

    // Open or create database
    struct btree_packed_db *db = btree_packed_db_open(basename);
    if (!db)
    {
        db = btree_packed_db_create(basename);
        if (!db)
        {
            printf("Error creating database\n");
            return 1;
        }
    }

    // read whole file content
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error opening file\n");
        return 1;
    }

    // allocate buffer for file content
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = malloc(size + 1);
    if (!buffer)
    {
        printf("Error allocating memory\n");
        return 1;
    }

    // read file content
    size_t read = fread(buffer, 1, size, file);
    if (read != size)
    {
        printf("Error reading file\n");
        return 1;
    }

    // add trailing zero
    buffer[size] = 0;

    // close file
    fclose(file);

    // add file content to database
    btree_packed_db_put(db, filename, buffer);

    btree_packed_db_close(db);

    return 0;
}