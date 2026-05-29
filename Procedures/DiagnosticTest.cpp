#include "DiagnosticTest.h"
#include <iostream>
#include <string>

using namespace std;

DiagnosticTest::DiagnosticTest(double price, string type, string result) 
    : MedicalProcedure(price) {
    this->testType = type;
    this->resultValue = result;
}

void DiagnosticTest::perform() {
    cout << "Wykonano badanie: " << testType << "." << endl;
    cout << "Odczytany wynik: " << resultValue << endl;
}

string DiagnosticTest::getDetails() const {
    return "Badanie diagnostyczne [" + testType + "] - Wynik: " + resultValue;
}

void DiagnosticTest::serialize(ostream& os) const {
    os << "PROCEDURE|DIAGNOSTIC TEST|" << basePrice << "|" << testType << "|" << resultValue << "\n";
}