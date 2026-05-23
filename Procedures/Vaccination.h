#ifndef VACCINATION_H
#define VACCINATION_H

#include "../Models/MedicalProcedure.h"

class Vaccination : public MedicalProcedure {
private:
    string vaccineType;
    string nextDueDate;

public:
    Vaccination(double price, string type, string date);

    void perform() override;
    string getDetails() const override;
};

#endif