#ifndef OWNER_H
#define OWNER_H

#include <string>
#include <vector>
#include "Animal.h"

using namespace std;

class Owner {
private:
    string firstName;
    string lastName;
    string phoneNumber;
    vector<Animal*> animals; 

public:
    
    Owner(string fName, string lName, string phone);

    void addAnimal(Animal* animal);
    const vector<Animal*>& getAnimals() const;


    string getFullName() const;
    string getPhoneNumber() const { return phoneNumber; }
    void serialize(ostream& os) const;
    void setPhoneNumber(string newPhone) { phoneNumber = newPhone; }
};

#endif