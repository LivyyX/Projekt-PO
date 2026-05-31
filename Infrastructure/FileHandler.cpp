#include "FileHandler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

using namespace std;

void FileHandler::save(const string& filename, const string& data) {
    ofstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Nie mozna otworzyc pliku do zapisu: " + filename);
    }

    file << data;

    if (file.bad()){
        file.close();
        throw runtime_error("Krytyczny blad podczas zapisu danych do pliku: " + filename);
    }
    file.close();
}


string FileHandler::read(string filename) {
    ifstream file(filename);
    stringstream buffer;

    if (!file.is_open()) {
        throw runtime_error("Nie mozna otworzyc pliku do odczytu: " + filename);
    }

    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}