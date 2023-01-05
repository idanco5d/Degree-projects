#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <memory>

namespace DS
{

    template<class C>
    class Employee
    {
        typedef std::shared_ptr<Employee> Employee_ptr;
    private:
        int id;
        long long salary;
        long long grade;
        C* company;


        Employee *clone() const {
            return new Employee(*this);
        }
    public:
        Employee(const int id, const long long salary, const long long grade, C* company) : id(id),
                    salary(salary), grade(grade), company(company) {};
        Employee()=default;
        long long getSalary() const{
            return salary;
        }
        int getId() const{
            return id;
        }
        long long getGrade() const
        {
            return grade;
        }

        void setGrade(int gradeBump){
            this->grade += gradeBump;
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
        C* getEmployer()
        {
            if (company->getParent() != -1 && company->getParent() != company->getId()) {
                setCompany(company->getRoot());
            }
            return company;
        }

        ~Employee() = default;

        Employee(std::shared_ptr<Employee> sharedPtr) : id(sharedPtr->id), salary(sharedPtr->salary),
                    grade(sharedPtr->grade), company(sharedPtr->company) {};

        friend std::ostream& operator<<(std::ostream& os, Employee& employee) {
            os << "employees salary: " << employee.getSalary() << ", employees grade: " << employee.getGrade()
               << ", employees employer: " << employee.getEmployer()->getId();


            return os;
        }

    };


}

#endif
