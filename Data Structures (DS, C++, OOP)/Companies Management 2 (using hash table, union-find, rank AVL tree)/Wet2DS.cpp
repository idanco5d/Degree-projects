#include "Wet2DS.h"
#include "SalaryIdKey.h"

wet2DS::wet2DS(long long numOfCompanies) {
    this->numOfCompanies = numOfCompanies;
    companies = new companyUnionFind(numOfCompanies);
    employeesWithSalary = new RankTree<UDCompanyTree>;
    employees = new employeeHash<UDCompanyTree>;
}

wet2DS::~wet2DS() {
    delete employeesWithSalary;
    delete employees;
    delete companies;
}

void wet2DS::addEmployee(int employeeId, int companyId, int Grade, int salary) {
    UDCompanyTree* company = companies->getCompanyRoot(companyId);
    auto employee = std::make_shared<Employee<UDCompanyTree>>(employeeId,salary,Grade,company);
    companies->addEmployee(companyId,employee);
    Node<UDCompanyTree>* employeeNode = new Node<UDCompanyTree>(employeeId,employee);
    employees->Insert(employeeNode);
    if (salary>0) {
        employeesWithSalary->insertNode(salaryIdKey(salary,employeeId),employee);
    }
}

void wet2DS::removeEmployee(int employeeId) {
    Node<UDCompanyTree>* employeeNode = employees->getNode(employeeId);
    if (!employeeNode) {
        return;
    }
    UDCompanyTree* company = employeeNode->data->getEmployer();
    companies->removeEmployee(company->getId(),employeeNode->data->getSalary(),employeeNode->data->getId());
    if (employeeNode->data->getSalary() > 0) {
        employeesWithSalary->removeNode(salaryIdKey(employeeNode->data->getSalary(),employeeId));
    }
    employees->Remove(employeeId);
}

bool wet2DS::doesEmployeeExist(int employeeId) const {
    return employees->SearchKey(employeeId) != -1;
}

//this was a problem! we have a pointer so updating the data in the hash would update all relevant nodes everywhere. plus, we need to re-calculate the grade sum.
void wet2DS::PromoteEmployee(int employeeId, int gradeBump) {
    Node<UDCompanyTree>* employeeNode = employees->getNode(employeeId);
    employeeNode->getData()->setGrade(gradeBump);
    //only if the employee has a salary we need to update the sums in the tree
    if (employeeNode -> getData() ->getSalary() > 0) {
        UDCompanyTree* company = employeeNode->getData()->getEmployer();
        RankTree<UDCompanyTree>* employeesTree = company->getEmployeesWithSalary();
        RankTree<UDCompanyTree>::Node* employeeInCompany = employeesTree->findNode(salaryIdKey(employeeNode->data->getSalary(),employeeId));
        employeesTree->updateSumOfNodeAndAllParents(employeeInCompany);
        employeesWithSalary->updateSumOfNodeAndAllParents(employeesWithSalary->findNode(salaryIdKey(employeeNode->getData()->getSalary(),employeeId)));
    }
}

companyUnionFind* wet2DS::getCompanies() const {
    return companies;
}

UDCompanyTree** wet2DS::getCompaniesArray() const{
    return companies->getCompaniesArray();
}

RankTree<UDCompanyTree>* wet2DS::getEmployeesWithSalary() const{
    return employeesWithSalary;
}

employeeHash<UDCompanyTree>* wet2DS::getEmployees() const {
    return employees;
}