#include "ClinicManager.h"
#include "../Procedures/Vaccination.h"
#include "../Procedures/Surgery.h"
#include "../Procedures/DiagnosticTest.h"

#include <algorithm> 
#include <iostream>
#include <sstream>

using namespace std;

/**
 * @file main.cpp
 * @brief Główny plik startowy systemu bazy kliniki.
 */

/**
 * @brief Rejestruje nowego użytkownika (np. pacjenta lub lekarza) w systemie.
 * * Funkcja sprawdza unikalność loginu w bazie danych kliniki, a następnie
 * bezpiecznie zapisuje profil użytkownika.
 * * @param ownerPhone Unikalny identyfikator (login) użytkownika.
 */
void ClinicManager::registerAnimal(Animal animal, string ownerPhone) {
    /**
 * @brief Autoryzuje użytkownika w systemie kliniki.
 * @param animal zwierzę.
 * @param ownerPhone id właściciela.
 */
    Owner* owner = findOwnerByPhoneNb(ownerPhone);

    if (owner != nullptr) {
        patients.push_back(move(animal));

        Animal* animalPtr = &patients.back();

        owner->addAnimal(animalPtr);
        cout << "Zarejestrowano " << animalPtr->getName() << " dla wlasciciela: " << owner->getFullName() << endl;
    } else {
        cout << "Blad: Nie znaleziono wlasciciela o numerze telefonu: " << ownerPhone << endl;
    }
}

Owner* ClinicManager::findOwnerByAnimalId(int animalId) {
    for (auto& owner : owners) {        
        const vector<Animal*>& animals = owner.getAnimals();  //&--alias dla istniejącej listy| const oznacza że lista jest tylko do odczytu
        for (Animal* animalPtr : animals) {
            if (animalPtr != nullptr && animalPtr->getId() == animalId) {
                return &owner;
            }
        }
    }  
    return nullptr;
}

Owner* ClinicManager::findOwnerByPhoneNb(string PhoneNumber) {
    for (auto& owner : owners) {
        if (owner.getPhoneNumber() == PhoneNumber) {
            return &owner;         }
    }
    return nullptr; 
}

Animal* ClinicManager::findAnimal(int id) {
    for (auto& animal : patients) {
        if (animal.getId() == id) {
            return &animal;
        }
    }
    return nullptr;
}



void ClinicManager::addOwner(const Owner& owner) {
    owners.push_back(owner);
}

void ClinicManager::saveToFile(string filename) {
    stringstream ss;
//zapis wlascicieli
    for (const auto& owner : owners) {
        owner.serialize(ss);
    }

    //zapis zwierzat
    for (const auto& animal : patients) {
        string ownerPhone = "";
        //wyciagnienie numeru tel
        Owner* ownerPtr = findOwnerByAnimalId(animal.getId());
        if (ownerPtr != nullptr) {
            ownerPhone = ownerPtr->getPhoneNumber();
        }
        
        animal.serialize(ss, ownerPhone);
    }

    fileHandler.save(filename, ss.str());
    cout << "Pomyslnie wygenerowano dane i przekazano do zapisu." << endl;
    cout << ss.str();
}

void ClinicManager::loadFromFile(string filename) {
    string fileContent = fileHandler.read(filename);
    if (fileContent.empty()) {
        cout << "Plik bazy danych jest pusty lub nie istnieje." << endl;
        return;
    }
//usuwanie potencjalnych duplikatów
    owners.clear();
    patients.clear();

    stringstream ss(fileContent);
    string line;

    // Przetwarzamy plik linia po linii
    while (getline(ss, line)) {
        if (line.empty()) continue;

        stringstream lineStream(line);
        string type;
        getline(lineStream, type, '|');

        if (type == "OWNER") {
            string firstName, lastName, phoneNumber;
            
            getline(lineStream, firstName, '|');
            getline(lineStream, lastName, '|');
            getline(lineStream, phoneNumber, '\n');

            // Usuwa ewentualny znak nowej linii z końca (\r)
            if (!phoneNumber.empty() && phoneNumber.back() == '\r') {
                phoneNumber.pop_back();
            }

            Owner newOwner(firstName, lastName, phoneNumber);
            owners.push_back(move(newOwner));

        } else if (type == "ANIMAL") {
            string idStr, name, species, weightStr, ownerPhone;
            
            getline(lineStream, idStr, '|');
            getline(lineStream, name, '|');
            getline(lineStream, species, '|');
            getline(lineStream, weightStr, '|');
            getline(lineStream, ownerPhone, '\n');

            if (!ownerPhone.empty() && ownerPhone.back() == '\r') {
                ownerPhone.pop_back();
            }

            //zapis str na int/double
            int id = stoi(idStr);
            double weight = stod(weightStr);

            Animal newAnimal(id, name, species, weight);
            
            patients.push_back(move(newAnimal));
            
            Animal* animalPtr = &patients.back();

            Owner* owner = findOwnerByPhoneNb(ownerPhone);
            if (owner != nullptr) {
                owner->addAnimal(animalPtr);
            } else {
                cout << "[Ostrzezenie] Nie znaleziono wlasciciela o telefonie " << ownerPhone 
                     << " dla zwierzaka o ID " << id << endl;
            }
        }else if (type == "VISIT") {
            string date, description, strid;

            getline(lineStream, date, '|');
            getline(lineStream, description, '|');
            getline(lineStream, strid, '\n');

            if (!strid.empty() && strid.back() == '\r') {
                strid.pop_back();
            }

            Visit newVisit(date, description);
            int id = stoi(strid);

            string nextLine;
            streampos oldPos = ss.tellg(); 

            while (getline(ss, nextLine)) {
                if (nextLine.empty()) continue;

                stringstream procStream(nextLine);
                string subType;
                getline(procStream, subType, '|');

                if (subType == "PROCEDURE") {
                    string procType, priceStr;
                    getline(procStream, procType, '|');
                    getline(procStream, priceStr, '|');
                    double price = stod(priceStr);

                    if (procType == "Vaccination") {
                        string vacType, vacDate;
                        getline(procStream, vacType, '|');
                        getline(procStream, vacDate, '\n');
                        if (!vacDate.empty() && vacDate.back() == '\r') vacDate.pop_back();

                        newVisit.addProcedure(make_unique<Vaccination>(price, vacType, vacDate));
                    } 
                    else if (procType == "Surgery") {
                        string doseStr, levelStr;
                        getline(procStream, doseStr, '|');
                        getline(procStream, levelStr, '\n');
                        if (!levelStr.empty() && levelStr.back() == '\r') levelStr.pop_back();

                        double dose = stod(doseStr);
                        int level = stoi(levelStr);

                        newVisit.addProcedure(make_unique<Surgery>(price, dose, level));
                    } 
                    else if (procType == "DiagnosticTest") {
                        string testType, result;
                        getline(procStream, testType, '|');
                        getline(procStream, result, '\n');
                        if (!result.empty() && result.back() == '\r') result.pop_back();

                        newVisit.addProcedure(make_unique<DiagnosticTest>(price, testType, result));
                    }
                    
                    oldPos = ss.tellg();
                } else {
                    ss.seekg(oldPos);
                    break; 
                }
            }

            Animal* anim = findAnimal(id);
            if (anim != nullptr) {
                anim->addVisit(move(newVisit));
            } else {
                cout << "[Ostrzezenie] Nie znaleziono zwierzaka o ID " << id 
                    << " dla wczytywanej wizyty z dnia " << date << endl;
            }
        }
    }
    cout << "Pomyslnie wczytano baze danych z pliku: " << filename << endl;
    cout << "Wczytano wlascicieli: " << owners.size() << ", zwierzat: " << patients.size() << endl;
}