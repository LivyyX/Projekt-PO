#include <iostream>
#include <memory>
#include <string>
#include <limits>

#include "Core/ClinicManager.h"
#include "Models/Animal.h"
#include "Models/Owner.h"
#include "Models/MedicalProcedure.h"
#include "Procedures/Vaccination.h"
#include "Procedures/Surgery.h"
#include "Procedures/DiagnosticTest.h"

using namespace std;

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void displayMenu() {
    cout << "\n========================================\n";
    cout << "   SYSTEM ZARZADZANIA KLINIKA WET.      \n";
    cout << "========================================\n";
    cout << "1. Zarejestruj nowego pacjenta i właściciela\n";
    cout << "2. Wyszukaj pacjenta i wyswietl historie leczenia\n";
    cout << "3. Dodaj nowa wizyte (i wykonaj procedury medyczne)\n";
    cout << "4. Zapisz dane i zamknij system\n";
    cout << "========================================\n";
    cout << "Wybor: ";
}

int main() {
    ClinicManager clinic;
    const string dataFileid = "baza_kliniki.txt";

    cout << "Inicjalizacja systemu...\n";
    clinic.loadFromFile(dataFileid);

    int choice = 0;
    while (true) {
        displayMenu();
        if (!(cin >> choice)) {
            cout << "Blad! Wprowadz liczbe.\n";
            clearInput();
            continue;
        }
        clearInput();

        switch (choice) {
            case 1: {
                cout << "\n--- REJESTRACJA NOWEGO PACJENTA ---\n";
                string fName, lName, phoneNb, name, species;
                int id;
                double weight;

                cout << "Podaj numer telefonu wlasciciela: "; getline(cin, phoneNb);
                cout << "Podaj ID zwierzaka: "; cin >> id; clearInput();
                cout << "Imię zwierzaka: "; getline(cin, name);
                cout << "Gatunek: "; getline(cin, species);
                cout << "Waga (kg): "; cin >> weight; clearInput();

                if(clinic.findOwnerByPhoneNb(phoneNb) == nullptr){
                    cout << "\n--- REJESTRACJA NOWEGO WLASCICIELA ---\n";
                    cout << "Imię : "; getline(cin, fName);
                    cout << "Nazwisko: "; getline(cin, lName);
                    clinic.addOwner(Owner(fName, lName, phoneNb)); 
                }

                Animal newAnimal(id, name, species, weight);
                clinic.registerAnimal(move(newAnimal), phoneNb); 
                break; 
            }

            case 2: {
                cout << "\n--- WYSZUKIWANIE PACJENTA ---\n";
                int id;
                cout << "Podaj id zwierzaka do wyszukania: ";
                cin >> id;

                Animal* patient = clinic.findAnimal(id);
                if (patient != nullptr) {
                    cout << "\nZnaleziono pacjenta!\n";
                    cout << "ID: " << patient->getId() << " | Imie: " << patient->getName() << "\n";
                    cout << "--- HISTORIA LECZENIA ---\n";
                    
                    const auto& history = patient->getHistory();
                    if (history.empty()) {
                        cout << "[Brak zarejestrowanych wizyt dla tego pacjenta]\n";
                    } else {
                        for (auto& visit : history) {
                            visit.finalize();
                        }
                    }
                } else {
                    cout << "Nie znaleziono pacjenta o podanym ID.\n";
                }
            break;
            }
            case 3: {
                cout << "\n--- KREATOR WIZYTY MEDYCZNEJ ---\n";
                int animalid;
                cout << "Dla jakiego zwierzaka chcesz dodac wizyte? (Podaj id): ";
                cin>>animalid;
                clearInput();

                Animal* patient = clinic.findAnimal(animalid);
                if (patient == nullptr) {
                    cout << "Nie mozna dodac wizyty. Brak pacjenta w bazie.\n";
                    break;
                }

                string date, description;
                cout << "Data wizyty (DD-MM-YYYY): "; getline(cin, date);
                cout << "Ogólny opis/powód wizyty: "; getline(cin, description);

                Visit newVisit(date, description);

                bool addingProcedures = true;
                while (addingProcedures) {
                    cout << "\nDodaj procedure do tej wizyty:\n";
                    cout << "1. Szczepienie\n";
                    cout << "2. Operacja (Chirurgia)\n";
                    cout << "3. Badanie diagnostyczne\n";
                    cout << "4. Zakoncz dodawanie procedur i sfinalizuj wizyte\n";
                    cout << "Wybor: ";
                    
                    int procChoice;
                    cin >> procChoice; clearInput();

                    if (procChoice == 1) {
                        double price;
                        string vType, nextDate;
                        cout << "Cena szczepienia (zl): "; cin >> price; clearInput();
                        cout << "Nazwa szczepionki: "; getline(cin, vType);
                        cout << "Data kolejnej dawki: "; getline(cin, nextDate);
                        
                        newVisit.addProcedure(make_unique<Vaccination>(price, vType, nextDate));
                    } 
                    else if (procChoice == 2) {
                        double price, anesthesia;
                        int complexity;
                        cout << "Cena operacji (zl): "; cin >> price; clearInput();
                        cout << "Dawka narkozy (ml): "; cin >> anesthesia; clearInput();
                        cout << "Poziom skomplikowania (1-5): "; cin >> complexity; clearInput();

                        newVisit.addProcedure(make_unique<Surgery>(price, anesthesia, complexity));
                    } 
                    else if (procChoice == 3) {
                        double price;
                        string tType, val;
                        cout << "Cena badania (zl): "; cin >> price; clearInput();
                        cout << "Typ badania (np. Krew, Usg): "; getline(cin, tType);
                        cout << "Wynik badania: "; getline(cin, val);

                        newVisit.addProcedure(make_unique<DiagnosticTest>(price, tType, val));
                    } 
                    else if (procChoice == 4) {
                        addingProcedures = false;
                    }
                }
                newVisit.finalize(); 
                cout << "Koszt calkowity procedur: " << newVisit.calculateTotalCost() << " zl\n";

                patient->addVisit(move(newVisit));
                cout << "Wizyta zostala pomyslnie dopisana do karty pacjenta.\n";
                break;
            }

            case 4: {
                cout << "Zapisywanie danych do pliku...\n";
                clinic.saveToFile(dataFileid);
                cout << "Dane zapisane pomyślnie. Zamykanie aplikacji.\n";
                return 0;
            }

            default:
                cout << "Niepoprawny wybor! Sprobuj ponownie.\n";
                break;
        }
    }
    return 0;
}