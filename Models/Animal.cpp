#include "Animal.h"
#include <ostream>
#include <stdexcept>

using namespace std;

Animal::Animal(int id, string name, string species, double weight){
    if (weight <=0.0){
        throw invalid_argument("Waga musi być większa od zera.");
    }
    this->id = id;
    this->name = name;
    this->species = species;
    this->weight = weight;
}

void Animal::addVisit(Visit&& visit) {
    history.push_back(move(visit));
}

const vector<Visit>& Animal::getHistory() const { 
    return history;
}
void Animal::serialize(ostream& os, const string& ownerPhone) const {
    os << "ANIMAL|" << id << "|" << name << "|" << species << "|" << weight <<"|"<<ownerPhone<< "\n";
}