#include "Company.h"
#include <memory>

using DS::AvlTree;
using DS::Company;
using DS::Employee;
//
typedef DS::AvlNode<DS::Key<int, int>, std::shared_ptr<Employee<Company>>> EmployeeNode;
typedef std::shared_ptr<Employee<Company>> Employee_ptr;

Company::Company(int id, int value) : id(id),value(value), num_of_employees(0), bestEarner(nullptr)
{
    employees_by_id = new AvlTree<Key<int,int>, DS::Company::Employee_ptr>();
    employees_by_salary = new AvlTree<Key<int,int>, DS::Company::Employee_ptr>();
}
Company::~Company() {
    delete employees_by_id;
    delete employees_by_salary;
}

int Company::getId() const {
    return this -> id;
}
int Company::getNumOfEmployees() const {
    return this -> num_of_employees;
}
int Company::getValue() const {
    return this -> value;
}
Employee_ptr Company::getBestEarner(){
    return (this -> bestEarner);
}


StatusType Company::addEmployee(const int &employees_id, const int &salary, const int &grade, const Employee_ptr& existing_employee) {
    Employee_ptr new_employee;
    if (existing_employee)
    {
        new_employee = existing_employee;
    }
    else {
        try
        {
            new_employee=std::make_shared<Employee<Company>>(Employee<Company>(employees_id, salary, grade, this));
        }
        catch (const std::bad_alloc &)
        {

            return StatusType::ALLOCATION_ERROR;
        }
    }
    bool inserted_to_employees = false;
    try
    {
        inserted_to_employees = true;
        employees_by_id->insertNode(Key<int, int>(employees_id, employees_id), new_employee);
        employees_by_salary->insertNode(Key<int, int>(salary, employees_id), new_employee);
    }
    catch (const std::bad_alloc &) //if there was a problem with the insertion
    {
        if(inserted_to_employees)
        {
            employees_by_id->removeNode(Key<int, int>(employees_id, employees_id));
            employees_by_salary->removeNode(Key<int, int>(salary, employees_id));
        }
        new_employee = nullptr;
        return StatusType::ALLOCATION_ERROR;
    }
    num_of_employees += 1;

    if( !bestEarner) {
        bestEarner = new_employee;
    } else if(new_employee->getSalary() > bestEarner->getSalary()) {
        bestEarner = new_employee;
    }

    return StatusType::SUCCESS;
}

void Company::setBestEarner(Employee_ptr employee) {
    this->bestEarner = employee;
}
void Company::updateBestEarner(){
    if (!employees_by_salary->getRoot())
    {
        setBestEarner(nullptr);
        return;
    }
    this->setBestEarner(((employees_by_salary->getMaxNode())->data));
}

StatusType Company::removeEmployee(const int employees_id, const int salary) {
    employees_by_id->removeNode(Key<int, int>(employees_id, employees_id));
    employees_by_salary->removeNode(Key<int, int>(salary, employees_id));
    if (employees_id == bestEarner->getId())
    {
        updateBestEarner();
    }
    num_of_employees--;

    return SUCCESS;
}

void Company::SetCompanyValue(int valueToSet) {
    value = valueToSet;
}

AvlTree<DS::Key<int, int>, Employee_ptr>* Company::getEmployeesById() const {
    return employees_by_id;
}

AvlTree<DS::Key<int, int>, Employee_ptr>* Company::getEmployeesBySalary() const {
    return employees_by_salary;
}

void Company::removeAllEmployees() {
    employees_by_salary->getRoot()->deleteAllChildren();
    employees_by_id->getRoot()->deleteAllChildren();
    this->bestEarner=nullptr;
    num_of_employees=0;
}
void Company::setNumEmployees(int newNumEmployees) {
    num_of_employees=newNumEmployees;
}
void Company::setEmployeesById(AvlTree<Key <int, int>, Employee_ptr>* newTree) {
    AvlTree<Key<int,int>,DS::Company::Employee_ptr>* oldTree = employees_by_id;
    employees_by_id = newTree;
    delete oldTree;
}

void Company::setEmployeesBySalary(AvlTree<Key < int, int>, Employee_ptr>* newTree) {
    AvlTree<Key<int,int>,DS::Company::Employee_ptr>* oldTree = employees_by_salary;
    employees_by_salary = newTree;
    delete oldTree;
}