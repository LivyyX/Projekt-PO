#include "FileHandler.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

void FileHandler::save(const string& filename, const string& data) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Blad: Nie mozna otworzyc pliku do zapisu: " << filename << endl;
        return;
    }

    file << data;
    file.close();
}


string FileHandler::read(string filename) {
    ifstream file(filename);
    stringstream buffer;

    if (file.is_open()) {
        buffer << file.rdbuf(); 
        file.close();
        return buffer.str();
    } else {
        cerr << "Blad: Nie mozna otworzyc pliku do odczytu: " << filename << endl;
        return "";
    }
}