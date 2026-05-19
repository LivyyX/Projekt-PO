#include "Visit.h"
#include <iostream>

using namespace std;

Visit::Visit(string date, string description) {
    this->date = date;
    this->description = description;
}

void Visit::addProcedure(unique_ptr<MedicalProcedure> procedure) {
    if (procedure != nullptr) {
        procedures.push_back(move(procedure));
        cout << "Dodano procedure do wizyty." << endl;
    }
}

void Visit::finalize() {
    cout << "Finalizowanie wizyty z dnia: " << date << endl;
    cout << "Opis wizyty: " << description << endl;
    
    cout << "Wykonane procedury: " << endl;
    for (const auto& proc : procedures) {
        proc->perform();
        cout << " - " << proc->getDetails() << endl;
    }
    
    cout << "Laczny koszt wizyty: " << calculateTotalCost() << " zl" << endl;
    cout << "Wizyta zostala zamknieta." << endl;
}

double Visit::calculateTotalCost() const {
    double total = 0.0;
    
    for (const auto& proc : procedures) {
        total += proc->getPrice();
    }
    return total;
    }
}