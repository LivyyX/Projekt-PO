#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
using namespace std;

class FileHandler {
public:
    void save(const string& filename, const string& data);
    string read(string filename);
};

#endif