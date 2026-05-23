#include "Animal.h"
using namespace std;

Animal::Animal(int id, string name, string species, double weight)
    : id(id), name(name), species(species), weight(weight) {}

void Animal::addVisit(Visit&& visit) {
    history.push_back(move(visit));
}

const vector<Visit>& Animal::getHistory() const { 
    return history;
}