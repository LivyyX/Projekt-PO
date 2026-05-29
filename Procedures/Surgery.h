#ifndef SURGERY_H
#define SURGERY_H

#include "../Models/MedicalProcedure.h"

class Surgery : public MedicalProcedure {
private:
    double anesthesiaDose;
    int complexityLevel;

public:
    Surgery(double price, double dose, int level);

    void perform() override;
    string getDetails() const override;
    double getPrice() const override; 
    void serialize(ostream& os) const override;
};

#endif