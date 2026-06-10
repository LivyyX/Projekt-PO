#include <iostream>
#include <memory>
#include <string>
#include <limits>
#include <stdexcept>

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
    cout << "4. Zmiana numeru telefonu wlasciciela\n"; 
    cout << "5. Zapisz dane i zamknij system\n";
    cout << "========================================\n";
    cout << "Wybor: ";
}

int main() {
    ClinicManager clinic;
    const string dataFileid = "baza_kliniki.txt";

    cout << "Inicjalizacja systemu...\n";
    try {
        clinic.loadFromFile(dataFileid);
    } catch (const exception& e) {
        cout << "[BLAD KRYTYCZNY] Nie udalo sie poprawnie wczytac bazy: " << e.what() << "\n";
        cout << "System uruchomia sie z pusta baza danych.\n";
    }

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
                cout << "Podaj ID zwierzaka: "; 
                if (!(cin >> id)){cout << "Blad. ID musi byc liczba.\n"; clearInput(); break;}
                clearInput();
                cout << "Imię zwierzaka: "; getline(cin, name);
                cout << "Gatunek: "; getline(cin, species);
                cout << "Waga (kg): ";
                if (!(cin >> weight)){cout << "Blad. Waga musi byc liczba.\n"; clearInput(); break;}
                clearInput();

                try{
                    if (phoneNb.empty() || name.empty() || species.empty()) {
                        throw invalid_argument("Pola danych (telefon, imie, gatunek) nie moga byc puste.");
                    }  
                    if (clinic.findAnimal(id) != nullptr) {
                        throw invalid_argument("Zwierzak o podanym ID " + to_string(id) + " juz istnieje w bazie danych!");
                    }
                    if(clinic.findOwnerByPhoneNb(phoneNb) == nullptr){
                        cout << "\n--- REJESTRACJA NOWEGO WLASCICIELA ---\n";
                        cout << "Imię : "; getline(cin, fName);
                        cout << "Nazwisko: "; getline(cin, lName);
                        if (fName.empty() || lName.empty()) {
                            throw invalid_argument("Imie i nazwisko wlasciciela nie moga byc puste!");
                        }
                        clinic.addOwner(Owner(fName, lName, phoneNb)); 
                    }

                Animal newAnimal(id, name, species, weight);
                clinic.registerAnimal(move(newAnimal), phoneNb); 
                cout << "Rejestracja zakonczona sukcesem.\n";
            }
                catch(const invalid_argument& e){
                    cout << "\n[BLAD WALIDACJI] Dane pacjenta sa niepoprawne.\n";
                    cout << "Szczegoly: " << e.what() << "\n";
                    cout << "Przerwano rejestracje. Sprobuj ponownie.\n";
                }
                break;
            }

            case 2: {
                cout << "\n--- WYSZUKIWANIE PACJENTA ---\n";
                int id;
                cout << "Podaj id zwierzaka do wyszukania: ";
                if (!(cin >> id)) { cout << "Blad. ID musi byc liczba.\n"; clearInput(); break; }
                clearInput();

                Animal* patient = clinic.findAnimal(id);
                if (patient != nullptr) {
                    cout << "\nZnaleziono pacjenta!\n";
                    cout << "ID: " << patient->getId() << " | Imie: " << patient->getName() << "\n";
                    cout << "--- HISTORIA LECZENIA ---\n";
                    
                    const auto& history = patient->getHistory();
                    if (history.empty()) {
                        cout << "[Brak zarejestrowanych wizyt dla tego pacjenta]\n";
                    } else {
                        for (const auto& visit : history) {
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
                if (!(cin >> animalid)) { cout << "Blad. ID musi byc liczba.\n"; clearInput(); break; }
                clearInput();

                Animal* patient = clinic.findAnimal(animalid);
                if (patient == nullptr) {
                    cout << "Nie mozna dodac wizyty. Brak pacjenta w bazie.\n";
                    break;
                }

                string date, description;
                cout << "Data wizyty (DD-MM-YYYY): "; getline(cin, date);
                cout << "Ogólny opis/powód wizyty: "; getline(cin, description);
                
                try{
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
                    if(!( cin >> procChoice)){
                        cout << "wybor musi byc liczba.\n";
                        clearInput();
                        continue;
                    }
                    clearInput();

                    try{    
                        if (procChoice == 1) {
                            double price;
                            string vType, nextDate;
                            cout << "Cena szczepienia (zl): ";
                            if (!(cin >> price)) throw invalid_argument("Wprowadzona cena musi byc liczba!");
                            clearInput();
                            cout << "Nazwa szczepionki: "; getline(cin, vType);
                            cout << "Data kolejnej dawki: "; getline(cin, nextDate);
                        
                            newVisit.addProcedure(make_unique<Vaccination>(price, vType, nextDate));
                            cout << "[Dodano szczepienie]\n";
                        } 
                        else if (procChoice == 2) {
                            double price, anesthesia;
                            int complexity;
                            cout << "Cena operacji (zl): ";
                            if (!(cin >> price)) throw invalid_argument("Wprowadzona cena musi byc liczba.");
                            clearInput();
                            cout << "Dawka narkozy (ml): ";
                            if (!(cin >> anesthesia)) throw invalid_argument("Wprowadzona dawka musi byc liczba.");
                            clearInput();
                            cout << "Poziom skomplikowania (1-5): ";
                            if (!(cin >> complexity)) throw invalid_argument("Wprowadzony poziom musi byc liczba.");
                            clearInput();

                        newVisit.addProcedure(make_unique<Surgery>(price, anesthesia, complexity));
                        cout << "[Dodano operacje]\n";
                        } 
                        else if (procChoice == 3) {
                            double price;
                            string tType, val;
                            cout << "Cena badania (zl): ";
                            if (!(cin >> price)) throw invalid_argument("Wprowadzona cena musi byc liczba.");
                            clearInput();
                            cout << "Typ badania (np. Krew, Usg): "; getline(cin, tType);
                            cout << "Wynik badania: "; getline(cin, val);

                            newVisit.addProcedure(make_unique<DiagnosticTest>(price, tType, val));
                            cout << "[Dodano badanie diagnostyczne]\n";
                        } 
                        else if (procChoice == 4) {
                            addingProcedures = false;
                        }else{
                            cout <<"Niepoprawny wybor procedury.\n";
                        }
                    }
                    catch(const invalid_argument& e){
                        cout << "\n[BLAD PROCEDURY] Podano nieprawidlowe dane szczegolowe.\n";
                        cout << "Przyczyna: " << e.what() << "\n";
                        cout << "Ta konkretna procedura nie zostala dodana. Sprobuj jeszcze raz.\n";
                    }
                }
                newVisit.finalize(); 
                cout << "Koszt calkowity procedur: " << newVisit.calculateTotalCost() << " zl\n";

                patient->addVisit(move(newVisit));
                cout << "Wizyta zostala pomyslnie dopisana do karty pacjenta.\n";
                }catch(const invalid_argument& e) {
                    cout << "\n[BLAD REJESTRACJI WIZYTY]: " << e.what() << "\n";
                    cout << "Powrot do głównego menu. Sprobuj dodac wizyte ponownie z poprawnymi danymi.\n";
                }
                break;
            }
            case 4: {
                cout << "\n--- ZMIANA NUMERU TELEFONU WLASCICIELA ---\n";
                string oldPhone, newPhone;

                cout << "Podaj AKTUALNY numer telefonu właściciela: ";
                getline(cin, oldPhone);

                cout << "Podaj NOWY numer telefonu: ";
                getline(cin, newPhone);

                if (clinic.updateOwnerPhone(oldPhone, newPhone)) {
                    cout << "[SUKCES] Numer telefonu został pomyślnie zaktualizowany.\n";
                    cout << "[INFO] Pamiętaj, aby zapisać dane przed wyjściem z programu (opcja 5)!\n";
                } else {
                    cout << "[BŁĄD] Nie znaleziono właściciela o numerze: " << oldPhone << "\n";
                }
                break;
            }
            case 5: {
                cout << "Zapisywanie danych do pliku...\n";
                try{
                    clinic.saveToFile(dataFileid);
                    cout << "Dane zapisane pomyślnie. Zamykanie aplikacji.\n";
                }catch (const exception& e){
                    cout << "[BLAD KRYTYCZNYZAPISU] Pliknie zostal zapisany: " << e.what() << "\n";
                    cout << "Upewnij sie, ze masz uprawnienia do zapisu i wolne miejsce na dysku.\n";
                }
                return 0;
            }

            default:
                cout << "Niepoprawny wybor! Sprobuj ponownie.\n";
                break;
        }
    }
    return 0;
}