set(BTREE_KEY_SIZE 128 CACHE STRING "B+ tree key size (in characters)")
set(BTREE_NODE_SIZE 512 CACHE STRING "B+ tree node size (number of keys)")

add_library(btree_packed_db)
target_sources(btree_packed_db PRIVATE
    BTreePackedDb.cpp
    btree_packed_db_c_api.cpp
    Compressor.cpp
    FileHandler.cpp
    )
target_include_directories(btree_packed_db PUBLIC
    ../include)

# Commented out - they should be passed as compile definitions
# target_compile_definitions(btree_packed_db PUBLIC
#     BTREE_KEY_SIZE=${BTREE_KEY_SIZE}
#     BTREE_NODE_SIZE=${BTREE_NODE_SIZE})

# Store compile definitions into a header file
file(WRITE "../include/btree_packed_db_config.h"
    "#define BTREE_KEY_SIZE ${BTREE_KEY_SIZE}\n"
    "#define BTREE_NODE_SIZE ${BTREE_NODE_SIZE}\n")