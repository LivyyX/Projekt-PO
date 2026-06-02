#include "Visit.h"
#include <iostream>

using namespace std;

bool isValidDate(const string& date) {
    if (date.length() != 10) return false;
    if (date[2] != '-' || date[5] != '-') return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(date[i])) return false;
    }
    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    if (year < 2000 || year > 2100) return false; // Zabezpieczenie przed dziwnymi latami
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (isLeap && day > 29) return false;
        if (!isLeap && day > 28) return false;
    }
    return true;
}

Visit::Visit(string date, string description) {
    if (!isValidDate(date)) {
        throw invalid_argument("Nieprawidlowy format lub wartosc daty. Oczekiwano DD-MM-YYYY.");
    }
    this->date = date;
    this->description = description;
}

void Visit::addProcedure(unique_ptr<MedicalProcedure> procedure) {
    if (procedure != nullptr) {
        procedures.push_back(move(procedure));
        cout << "Dodano procedure do wizyty." << endl;
    }
}

void Visit::finalize() const {
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

void Visit::serialize(ostream& os,int animalid) const {
    os << "VISIT|" << date << "|" << description <<"|"<< animalid <<"\n";
}