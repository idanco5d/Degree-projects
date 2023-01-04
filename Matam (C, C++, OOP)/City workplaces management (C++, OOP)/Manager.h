//
// Created by illus on 28/12/2021.
//

#ifndef MANAGER_H
#define MANAGER_H
#include "Citizen.h"
#include <iostream>
#include "Employee.h"
#include <memory>
#include <vector>

using std::vector;
using namespace std;

namespace mtm{

class Manager : public Citizen
{
    int salary;
    vector<Citizen*> employees;

public:
    Manager(int id, string firstName, string lastName, int birthYear);
    Manager(const Manager& manager);
    Manager& operator=(const Manager&)=default;
    ~Manager() override=default;
    int getSalary() const override;
    void addEmployee(Citizen* employee) override;
    void removeEmployee(int id) override;
    void setSalary(const int& salaryToSet) override;
    std::ostream& printShort(std::ostream& stream) const override;
    std::ostream& printLong(std::ostream& stream) const override;
    Manager* clone() const;
    vector<Citizen*> getEmployees() const override;
    void setEmployeesSalaries(int SalaryToSet) const;
    void removeAllEmployees(int groupSalaryToReduce) override;

};

}
#endif //MANAGER_H
