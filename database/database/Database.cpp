#include "Database.h"
#include <sys/stat.h>
#include <string>
#include <fstream>
#include <iostream>

void Database::message(const string& text) {
    cout << text << "\n";
}