#include "Surgery.h"
#include <iostream>
#include <string>

using namespace std;

Surgery::Surgery(double price, double dose, int level) 
    : MedicalProcedure(price) {
    this->anesthesiaDose = dose;
    this->complexityLevel = level;
}

void Surgery::perform() {
    cout << "Przygotowanie sali operacyjnej..." << endl;
    cout << "Podano narkoze. Dawka: " << anesthesiaDose << " mg." << endl;
    cout << "Przeprowadzanie operacji o poziomie trudnosci: " << complexityLevel << "/5..." << endl;
    cout << "Operacja zakonczona sukcesem. Pacjent przeniesiony na sale wybudzen." << endl;
}

string Surgery::getDetails() const {
    return "Operacja - Poziom trudnosci: " + to_string(complexityLevel) 
           + "/5, Dawka narkozy: " + to_string(anesthesiaDose) + " mg";
}

double Surgery::getPrice() const {
    double finalPrice = basePrice + (complexityLevel * 150.0);
    return finalPrice;
}

void Surgery::serialize(ostream& os) const {
    os << "PROCEDURE|SURGERY|" << basePrice << "|" << anesthesiaDose << "|" << complexityLevel << "\n";
}