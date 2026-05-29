#include "Owner.h"
#include <ostream>
using namespace std;

Owner::Owner(string fName, string lName, string phone)
    : firstName(fName), lastName(lName), phoneNumber(phone) {}

void Owner::addAnimal(Animal* animal) {
    if (animal != nullptr) {
        animals.push_back(animal);
    }
}

const vector<Animal*>& Owner::getAnimals() const {
    return animals;
}

string Owner::getFullName() const {
    return firstName + " " + lastName;
}
void Owner::serialize(ostream& os) const {
    os << "OWNER|" << firstName << "|" << lastName << "|" << phoneNumber << "\n";
}