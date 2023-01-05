//
// Created by illus on 28/12/2021.
//

#include "Citizen.h"
#include "Skill.h"
#include "exceptions.h"
#include <vector>
using std::endl;
using namespace mtm;
using std::vector;

Citizen::Citizen(int id, string firstName, string lastName, int birthYear): id(id), firstName(firstName), lastName(lastName), birthYear(birthYear)
{}

int Citizen::getId() const
{
    return id;
}

string Citizen::getFirstName() const
{
    return firstName;
}

string Citizen::getLastName() const
{
    return lastName;
}

int Citizen::getBirthYear() const
{
    return birthYear;
}

bool operator==(const Citizen& citizen1, const Citizen& citizen2)
{
    return citizen1.id == citizen2.id;
}

bool operator>(const Citizen& citizen1, const Citizen& citizen2)
{
    return citizen1.id > citizen2.id;
}

bool operator!=(const Citizen& citizen1, const Citizen& citizen2)
{
    return !(citizen1==citizen2);
}

bool operator<=(const Citizen& citizen1, const Citizen& citizen2)
{
    return !(citizen1>citizen2);
}

bool operator<(const Citizen& citizen1, const Citizen& citizen2)
{
    return (citizen1<=citizen2) && (citizen1!=citizen2);
}

bool operator>=(const Citizen& citizen1, const Citizen& citizen2)
{
    return !(citizen1<citizen2);
}

void Citizen::citizenPrintShortAndLong (std::ostream& stream, bool isPrintLong) const
{
    stream << getFirstName() << " " << getLastName() << endl;
    if (isPrintLong)
    {
        stream << "id - " << getId() << " birth_year - " << getBirthYear() << endl;
    }
    stream << "Salary: ";
}

bool compareCitizensByPointer(Citizen* citizen1, Citizen* citizen2)
{
    return (*citizen1)<(*citizen2);
}

void Citizen::setEmployeesSalaries(int SalaryToSet) const
{
    throw ManagerDoesNotExist();
}

void Citizen::learnSkill(const Skill& skill)
{
    throw EmployeeDoesNotExist();
}

void Citizen::forgetSkill(const int& id)
{
    throw EmployeeDoesNotExist();
}

bool Citizen::hasSkill(const int& id) const
{
    return false;
}
void Citizen::setScore(const int& scoreToSet)
{
    throw EmployeeDoesNotExist();
}

void Citizen::addEmployee(Citizen* employee)
{
    throw EmployeeNotSelected();
}

vector<Citizen*> Citizen::getEmployees() const
{
    vector<Citizen*> dummy_vec;
    return dummy_vec;
}

void Citizen::removeEmployee(int id)
{

}
void Citizen::removeAllEmployees(int groupSalaryToReduce)
{
    throw ManagerIsNotHired();
}

int Citizen::getScore() const
{
    return 0;
}