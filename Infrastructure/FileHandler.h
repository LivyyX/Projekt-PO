#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
using namespace std;

class FileHandler {
public:
    void save(string data, string filename);
    string read(string filename);
};

#endif