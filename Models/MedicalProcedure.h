#ifndef MEDICALPROCEDURE_H
#define MEDICALPROCEDURE_H

#include <string>

using namespace std;

class MedicalProcedure {
//protected zamiast private, aby klasy dziedziczące mogły korzystać z basePrice
protected:
    double basePrice;

public:
    MedicalProcedure(double basePrice) : basePrice(basePrice){}

    virtual ~MedicalProcedure() = default;

    virtual void perform() = 0;

    virtual string getDetails() const = 0;

    virtual double getPrice() const {
        return basePrice;
    }
    virtual void serialize(ostream& os) const = 0;

};

#endif