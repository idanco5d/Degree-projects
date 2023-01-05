#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "Node.h"
#include "AVLTree.h"
#include "library1.h"
#include <memory>

namespace DS
{

//    template <class T>
//    class AbstractCompany
//    {
//
//    public:
//        AbstractCompany() { } ;
//        virtual ~AbstractCompany() {};
//
//        virtual int getId() const = 0;
//        virtual int getNumOfEmployees() const = 0;
//        virtual int getValue() const = 0;
//        virtual T* getBestEarner() = 0;
//
//        virtual StatusType addEmployee(const int &employees_id, const int &salary, const int &grade) = 0;
//        virtual StatusType removeEmployee(const int employees_id, const int salary) = 0;
//        virtual AbstractCompany *clone() const = 0;
//        virtual void updateBestEarner() = 0;
//    };

    template<class C>
    class Employee
    {
        typedef std::shared_ptr<Employee> Employee_ptr;
    private:
        int id;
        int salary;
        int grade;
        C* company;


        Employee *clone() const {
            return new Employee(*this);
        }
    public:
        Employee(const int id, const int salary, const int grade, C* company) : id(id),
                    salary(salary), grade(grade), company(company) {};
        StatusType removeEmployee(){
            this->company->removeEmployee(this -> id, this -> salary);
            delete this->company;
            delete this;

            return SUCCESS;
        }

        int getSalary() const{
            return salary;
        }
        int getId() const{
            return id;
        }
        C* getEmployer() const
        {
            return company;
        }
        int getGrade() const
        {
            return grade;
        }

        void promoteEmployee(int SalaryIncrease, int BumpGrade) {
            this->salary += SalaryIncrease;
            if(BumpGrade > 0){
                this->grade ++;
            }
        }

        void setCompany(C* company) {
            this->company = company;
        }

        ~Employee() = default;

        Employee(std::shared_ptr<Employee> sharedPtr) : id(sharedPtr->id), salary(sharedPtr->salary),
                    grade(sharedPtr->grade), company(sharedPtr->company) {};
    };


}

#endif
