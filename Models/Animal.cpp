#include "Animal.h"
#include <ostream>
using namespace std;

Animal::Animal(int id, string name, string species, double weight)
    : id(id), name(name), species(species), weight(weight) {}

void Animal::addVisit(Visit&& visit) {
    history.push_back(move(visit));
}

const vector<Visit>& Animal::getHistory() const { 
    return history;
}
void Animal::serialize(ostream& os, const string& ownerPhone) const {
    os << "ANIMAL|" << id << "|" << name << "|" << species << "|" << weight <<"|"<<ownerPhone<< "\n";

    for (const auto& visit:history){
        visit.serialize(os,id);
    }
}