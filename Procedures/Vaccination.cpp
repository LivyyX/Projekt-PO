#include "Vaccination.h"
#include <iostream>
#include <string>

using namespace std;

Vaccination::Vaccination(double price, string type, string date) 
    : MedicalProcedure(price) {
    this->vaccineType = type;
    this->nextDueDate = date;
}

void Vaccination::perform() {
    cout << "Wykonano szczepienie. Podano preparat: " << vaccineType << "." << endl;
}

string Vaccination::getDetails() const {
    return "Szczepienie [" + vaccineType + "] - Termin kolejnej dawki: " + nextDueDate;
}
void Vaccination::serialize(ostream& os) const {
    os << "PROCEDURE|VACCINATION|" << basePrice << "|" << vaccineType << "|" << nextDueDate << "\n";
}