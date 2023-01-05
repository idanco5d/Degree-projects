//
// Created by illus on 06/01/2022.
//

#include "City.h"
#include "Faculty.h"
#include <string>
#include <set>
#include <memory>
#include <algorithm>
using std::set;
using std::string;
using namespace std;

namespace mtm {

using namespace mtm;

City::City(string cityName) : cityName(cityName), citizens{}, workplaces{}, faculties{}
{
}

City::~City()
{
    for (unsigned int i=0; i<citizens.size(); i++)
    {
        delete citizens[i];
    }
    set<Workplace*>::iterator it2= workplaces.begin();
    while(it2!=workplaces.end())
    {
        set<Workplace*>::iterator tmp_iterator2=it2;
        it2++;
        delete (*tmp_iterator2);
    }
    set<Faculty<Condition>*>::iterator it3= faculties.begin();
    while(it3!=faculties.end())
    {
        set<Faculty<Condition>*>::iterator tmp_iterator3=it3;
        it3++;
        delete (*tmp_iterator3);
    }
}

void City::addEmployee(int id, string firstName, string lastName, int birthYear) {
    if(isCitizenInCity(id))
    {
        throw CitizenAlreadyExists();
    }
    Employee* employee=new Employee(id,firstName,lastName,birthYear);
    citizens.push_back(employee);
    sort(citizens.begin(), citizens.end(), compareCitizensByPointer);
}


void City::addManager(int id, string firstName, string lastName, int birthYear)
{
    if(isCitizenInCity(id))
    {
        throw CitizenAlreadyExists();
    }
    Manager* manager=new Manager(id, firstName, lastName, birthYear);
    citizens.push_back(manager);
    sort(citizens.begin(), citizens.end(), compareCitizensByPointer);
}

void City::addFaculty(int id,const Skill& skill,int AddedPoints,Condition* condition)
{
    for(set<Faculty<Condition>*>::iterator it=faculties.begin();it!=faculties.end();it++)
    {
        if((*it)->getId()==id)
        {
            throw FacultyAlreadyExists();
        }
    }
    Faculty<Condition>* faculty=new Faculty<Condition>(id, skill, AddedPoints, condition);
    faculties.insert(faculty);
}

void City::createWorkplace(const int& id, const string& workplaceName, const int& workersSalary, const int& managersSalary)
{
    for(set<Workplace*>::iterator it=workplaces.begin();it!=workplaces.end();it++)
    {
        if((*it)->getId()==id)
        {
            throw WorkplaceAlreadyExists();
        }
    }
    Workplace* workplace=new Workplace(id,workplaceName,workersSalary,managersSalary);
    workplaces.insert(workplace);
}

void City::hireManagerAtWorkplace(int managerId, int workplaceId)
{
    for (unsigned int i=0; i<citizens.size(); i++)
    {
        if (citizens[i]->getId() == managerId)
        {
            for (set<Workplace*>::iterator workplace=workplaces.begin(); workplace!=workplaces.end(); workplace++)
            {
                if ((*workplace)->getId() == workplaceId)
                {
                    (*workplace)->hireManager(citizens[i]);
                    return;
                }
            }
            throw WorkplaceDoesNotExist();
        }
    }
    throw ManagerDoesNotExist();
}

void City::fireEmployeeAtWorkplace (int employeeId, int managerId, int workplaceId)
{
    for (set<Workplace*>::iterator workplace = workplaces.begin(); workplace != workplaces.end(); workplace++)
    {
        if ((*workplace)->getId() == workplaceId)
        {
            try {
                (*workplace)->fireEmployee(employeeId,managerId);
            }
            catch(ManagerIsNotHired&)
            {
                throw ManagerDoesNotExist();
            }
            catch (EmployeeIsNotHired&)
            {
                throw EmployeeDoesNotExist();
            }
            return;
        }
    }
    throw WorkplaceDoesNotExist();
}

void City::fireManagerAtWorkplace (int managerId, int workplaceId){
    for (set<Workplace*>::iterator workplace= workplaces.begin(); workplace != workplaces.end(); workplace++)
    {
        if ((*workplace)->getId() == workplaceId)
        {
            try {
                vector<Citizen*> managers=(*workplace)->getManagers();
                for(unsigned int i=0; i<managers.size(); i++)
                {
                    if(managers[i]->getId()==managerId)
                    {
                        managers[i]->removeAllEmployees((*workplace)->getWorkersSalary());
                    }
                }
                (*workplace)->fireManager(managerId);
            }
            catch (ManagerIsNotHired&)
            {
                throw ManagerDoesNotExist();
            }
            return;
        }
    }
    throw WorkplaceDoesNotExist();
}

void City::teachAtFaculty(int employeeId, int facultyId)
{
    for (unsigned int i=0; i<citizens.size(); i++)
    {
        if (citizens[i]->getId() == employeeId) {
            for (set<Faculty<Condition> *>::iterator faculty = faculties.begin();
                 faculty != faculties.end(); faculty++) {
                if ((*faculty)->getId() == facultyId) {
                    (*faculty)->teach(dynamic_cast<Employee*>(citizens[i]));
                    return;
                }
            }
            throw FacultyDoesNotExist();
        }
    }
    throw EmployeeDoesNotExist();
}


int City::getAllAboveSalary(std::ostream& ostream, int salary) const
{
    int counter=0;
    for (unsigned int i=0; i<citizens.size(); i++)
    {
        if (citizens[i]->getSalary() >= salary)
        {
            citizens[i]->printShort(ostream);
            counter++;
        }
    }
    return counter;
}

bool City::isWorkingInTheSameWorkplace(int employeeId1, int employeeId2) const {
    if (!isCitizenInCity(employeeId1)) {
        throw EmployeeDoesNotExist();
    }
    for (set<Workplace *>::iterator workplace = workplaces.begin(); workplace != workplaces.end(); workplace++) {
        if ((*workplace)->isEmployeeInWorkplace(employeeId1) && (*workplace)->isEmployeeInWorkplace(employeeId2))
        {
            return true;
        }
    }
    return false;
}

void City::printAllEmployeesWithSkill(std::ostream& stream, const int& skillId) const {
    for (unsigned int i=0; i<citizens.size(); i++)
    {
        if (citizens[i]->hasSkill(skillId))
        {
            citizens[i]->printShort(stream);
        }
    }
}

bool City::isCitizenInCity(int citizenId) const
{
    for (unsigned int i=0; i<citizens.size(); i++)
    {
        if (citizens[i]->getId() == citizenId)
        {
            return true;
        }
    }
    return false;
}

}