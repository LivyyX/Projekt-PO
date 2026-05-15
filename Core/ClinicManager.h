#ifndef CLINICMANAGER_H
#define CLINICMANAGER_H

#include <vector>
#include <string>
#include "../Models/Animal.h"
#include "../Models/Owner.h"
#include "../Infrastructure/FileHandler.h"

using namespace std;

class ClinicManager {
private:
    vector<Animal> patients; 
    vector<Owner> owners;     
    FileHandler fileHandler;      

public:
    ClinicManager(){patients.reserve(1000);};
    void registerAnimal(Animal animal,string ownerPhone);
    Animal findAnimal(int id);     
    void saveToFile(string filename);
    void loadFromFile(string filename);
    Owner* findOwnerByAnimalId(int animalId);
    Owner* findOwnerByPhoneNb(string PhoneNumber);
};

#endif