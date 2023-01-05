//
// Created by illus on 28/12/2021.
//

#ifndef CITIZEN_H
#define CITIZEN_H
#include <string>
#include <iostream>
#include <vector>
#include "Skill.h"
using std::string;
using namespace mtm;
using std::vector;

class Citizen
{
    int id;
    string firstName;
    string lastName;
    int birthYear;

public:
    Citizen(int id, string firstName, string lastName, int birthYear);
    Citizen(const Citizen&)=default;
    virtual ~Citizen()=default;
    int getId() const;
    string getFirstName() const;
    string getLastName() const;
    int getBirthYear() const;
    virtual std::ostream& printShort(std::ostream& stream) const = 0;
    virtual std::ostream& printLong(std::ostream& stream) const = 0;
    virtual Citizen* clone() const=0;
    friend bool operator==(const Citizen& citizen1, const Citizen& citizen2);
    friend bool operator>(const Citizen& citizen1, const Citizen& citizen2);
    void citizenPrintShortAndLong (std::ostream& stream, bool isPrintLong) const;
    virtual void setSalary(const int& salaryToSet) = 0;
    virtual int getSalary() const = 0;
    virtual void setEmployeesSalaries(int SalaryToSet) const;
    virtual void learnSkill(const Skill& skill);
    virtual void forgetSkill(const int& id);
    virtual bool hasSkill(const int& id) const;
    virtual void setScore(const int& scoreToSet);
    virtual void addEmployee(Citizen* employee);
    virtual vector<Citizen*> getEmployees() const;
    virtual void removeEmployee(int id);
    virtual void removeAllEmployees(int groupSalaryToReduce);
    virtual int getScore() const;
};

bool operator!=(const Citizen& citizen1, const Citizen& citizen2);
bool operator<=(const Citizen& citizen1, const Citizen& citizen2);
bool operator<(const Citizen& citizen1, const Citizen& citizen2);
bool operator>=(const Citizen& citizen1, const Citizen& citizen2);
bool compareCitizensByPointer(Citizen* citizen1, Citizen* citizen2);

#endif //CITIZEN_H
