#ifndef ANIMAL_H
#define ANIMAL_H

#include <string>
#include <vector>
#include "../Core/Visit.h" 

using namespace std;

class Animal {
private:
    int id;
    string name;
    string species;
    double weight;
    vector<Visit> history; 

public:
    Animal(int id, string name, string species, double weight);

    void addVisit(Visit visit);
    vector<Visit> getHistory() const;

    int getId() const { return id; }
    string getName() const { return name; }
};

#endif