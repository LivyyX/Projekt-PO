#ifndef VISIT_H
#define VISIT_H

#include <string>
#include <vector>
#include <memory> // Potrzebne do unique_ptr
#include "../Models/MedicalProcedure.h"

using namespace std;

class Visit {
private:
    string date;
    string description;
    // Lista procedur medycznych jako inteligentne wskaźniki
    vector<unique_ptr<MedicalProcedure>> procedures;

public:
    Visit(string date, string description);

    void addProcedure(unique_ptr<MedicalProcedure> procedure);

    void finalize();

    double calculateTotalCost() const;

    void serialize(ostream& os,int animalid) const;

    string getDate() const { return date; }
    string getDescription() const { return description; }
};

#endif