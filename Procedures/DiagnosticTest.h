#ifndef DIAGNOSTICTEST_H
#define DIAGNOSTICTEST_H

#include "../Models/MedicalProcedure.h"

class DiagnosticTest : public MedicalProcedure {
private:
    string testType;
    string resultValue;

public:
    DiagnosticTest(double price, string type, string result="Prawidlowy");

    void perform() override;
    string getDetails() const override;
    void serialize(ostream& os) const override;

};

#endif