#include "Owner.h"
#include <ostream>
using namespace std;

bool isValidPhone(const string& phone) {
    if (phone.length() != 9) return false;
    for (int i = 0; i < 9; i++) {
        if (!isdigit(phone[i])) {
            return false; 
        }
    }
    return true;
}

Owner::Owner(string fName, string lName, string phone){
    if (!isValidPhone(phone)) {
        throw invalid_argument("Niepoprawny numer telefonu. Oczekiwano 9 cyfr bez spacji.");
    }
    this->firstName = fName;
    this->lastName = lName;
    this->phoneNumber = phone;
}
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