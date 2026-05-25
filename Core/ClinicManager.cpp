#include "ClinicManager.h"
#include <algorithm> 
#include <iostream>

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


void ClinicManager::saveToFile(string filename) {
    string data = "Tu coś jeszcze bedzie."; 
    fileHandler.save(data, filename);
}


void ClinicManager::loadFromFile(string filename) {
    string data = fileHandler.read(filename);
    cout << "Wczytano dane z pliku: " << filename << endl;
}

void ClinicManager::addOwner(const Owner& owner) {
    owners.push_back(owner);
}