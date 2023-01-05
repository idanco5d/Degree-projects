#include "hash.h"
#include "Employee.h"
#include "library2.h"
#include "CompanyUnionFind.h"
#include "rankTreeActual.h"
#include "UDCompanyTree.h"
#include "Wet2DS.h"
#include <cassert>
#include <cmath>

void *Init(int k) {
    wet2DS* ds = new wet2DS(k);
    return (void*)ds;
}

StatusType AddEmployee(void *DS, int employeeID, int companyID, int grade) {
    if(DS == nullptr || employeeID <= 0 || companyID <= 0 || grade < 0) {
        return INVALID_INPUT;
    }
    wet2DS* data = (wet2DS*)DS;
    if(data->numOfCompanies < companyID) {
        return INVALID_INPUT;
    }
    if(data->getEmployees()->SearchKey(employeeID) != -1) {
        return FAILURE;
    }
    try {
        data->addEmployee(employeeID, companyID, grade);
    } catch(std::bad_alloc& ba) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType RemoveEmployee(void *DS, int employeeID) {
    if (!DS || employeeID<=0) {
        return INVALID_INPUT;
    }
    wet2DS* data = (wet2DS*)DS;
    if (!data->doesEmployeeExist(employeeID)) {
        return FAILURE;
    }
    data->removeEmployee(employeeID);
    return SUCCESS;
}

StatusType AcquireCompany(void *DS, int companyID1, int companyID2, double factor){
    if (!DS || companyID1<=0 || companyID2<=0 || factor<=0 || companyID2==companyID1) {
        return INVALID_INPUT;
    }
    wet2DS* data = (wet2DS*)DS;
    if (data->numOfCompanies < companyID1 || data->numOfCompanies < companyID2 || data->getCompanies()->getCompanyRoot(companyID2) == data->getCompanies()->getCompanyRoot(companyID1)) {
        return INVALID_INPUT;
    }
    companyUnionFind* companies = data->getCompanies();
    try {
        companies->acquire(companyID1,companyID2,factor);
    }
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

#define DO_NOT_UPDATE_GRADE (-19745)

StatusType EmployeeSalaryIncrease(void *DS, int employeeID, int salaryIncrease) {
    if (!DS || employeeID<=0 || salaryIncrease <=0) {
        return INVALID_INPUT;
    }
    wet2DS* data = (wet2DS*)DS;
    employeeHash<UDCompanyTree>* employees = data->getEmployees();
    Node<UDCompanyTree>* employeeNode = employees->getNode(employeeID);
    if (!employeeNode) {
        return FAILURE;
    }
    std::shared_ptr<Employee<UDCompanyTree>> employee=employeeNode->data;
    //if the employee has salary 0 before change, insert it to the tree of employees with salary in the company
    UDCompanyTree* company = employee->getEmployer();
    RankTree<UDCompanyTree>* employeesWithSalaryTree = company->getEmployeesWithSalary();
    RankTree<UDCompanyTree>* generalTree = data->getEmployeesWithSalary();
    if (employee->getSalary() < 1) {
        employee->promoteEmployee(salaryIncrease,DO_NOT_UPDATE_GRADE);
        try {
            employeesWithSalaryTree->insertNode(salaryIdKey(salaryIncrease,employeeID),employee);
            generalTree->insertNode(salaryIdKey(salaryIncrease,employeeID), employee);
            employees->updateGradesSum(-employee->getGrade());
            employees->updateNumNoSalary(-1);
            company->getEmployees()->updateGradesSum(-employee->getGrade());
            company->getEmployees()->updateNumNoSalary(-1);
        }
        catch (std::bad_alloc&) {
            employee->promoteEmployee(-salaryIncrease,DO_NOT_UPDATE_GRADE);
            return ALLOCATION_ERROR;
        }
    }
    else {
        employeesWithSalaryTree->removeNode(salaryIdKey(employee->getSalary(),employeeID));
        generalTree->removeNode(salaryIdKey(employee->getSalary(),employeeID));
        employee->promoteEmployee(salaryIncrease,DO_NOT_UPDATE_GRADE);
        try {
            employeesWithSalaryTree->insertNode(salaryIdKey(employee->getSalary(),employeeID),employee);
            generalTree->insertNode(salaryIdKey(employee->getSalary(),employeeID), employee);
        }
        catch (std::bad_alloc&) {
            employee->promoteEmployee(-salaryIncrease,DO_NOT_UPDATE_GRADE);
            return ALLOCATION_ERROR;
        }
    }
    return SUCCESS;
}

StatusType PromoteEmployee(void *DS, int employeeID, int bumpGrade) {
    if(!DS || employeeID <= 0) {
        return INVALID_INPUT;
    }
    wet2DS* data = (wet2DS*)DS;
    Node<UDCompanyTree>* employeeNode = data->getEmployees()->getNode(employeeID);
    if(!employeeNode) {
        return FAILURE;
    }
    std::shared_ptr<Employee<UDCompanyTree>> employee = employeeNode->data;
    if(bumpGrade <= 0) {
        return SUCCESS;
    } else {
        data->PromoteEmployee(employeeID, bumpGrade);
        if (employee->getSalary() < 1) {
            employee->getEmployer()->getEmployees()->updateGradesSum(bumpGrade);
            data->getEmployees()->updateGradesSum(bumpGrade);
        }
    }
    return SUCCESS;
}

StatusType CompanyValue(void *DS, int companyID) {
    if(DS == nullptr || companyID <= 0 || static_cast<wet2DS*>(DS)->numOfCompanies < companyID) {
        return INVALID_INPUT;
    }
    printf("CompanyValue: %.1f\n", static_cast<wet2DS*>(DS)->getCompaniesArray()[companyID-1]->getValue());
    return SUCCESS;
}

StatusType SumOfBumpGradeBetweenTopWorkersByGroup(void *DS, int companyID, int m) {
    if(DS == nullptr  || m <= 0 || companyID < 0 ||
            companyID > static_cast<wet2DS*>(DS)->numOfCompanies) {
        return INVALID_INPUT;
    }
    if(companyID == 0) {
        if((long long)static_cast<wet2DS*>(DS)->getEmployeesWithSalary()->num_of_nodes < m ) {
            return FAILURE;
        }
        printf("SumOfBumpGradeBetweenTopWorkersByGroup: %lld\n", static_cast<wet2DS*>(DS)->getEmployeesWithSalary()->getSumGrades(m));
        return SUCCESS;
    }
    if((long long)static_cast<wet2DS*>(DS)->getCompanies()->getCompanyRoot(companyID)->getEmployeesWithSalary()->num_of_nodes < m) {
        return FAILURE;
    }
    printf("SumOfBumpGradeBetweenTopWorkersByGroup: %lld\n", static_cast<wet2DS*>(DS)->getCompanies()->getCompanyRoot(companyID)
            ->getEmployeesWithSalary()->getSumGrades(m));
    return SUCCESS;
}

void removeNotInRange (long long* sum, long long* rank, RankTree<UDCompanyTree>::Node* node, int lowerSalary, int higherSalary) {
    if (!node) {
        return;
    }
    RankTree<UDCompanyTree>::Node* findFirst = node;
    while (findFirst && (findFirst->data->getSalary() < lowerSalary || findFirst->data->getSalary() > higherSalary)) {
        if (findFirst->data->getSalary() < lowerSalary) {
            if (findFirst->left) {
                (*sum)-=findFirst->left->sum;
                (*rank)-=findFirst->left->rank;
            }
            (*sum)-=findFirst->data->getGrade();
            (*rank)-=1;
            findFirst = findFirst->right;
        }
        if (findFirst && findFirst->data->getSalary() > higherSalary) {
            if (findFirst->right) {
                (*sum)-=findFirst->right->sum;
                (*rank)-=findFirst->right->rank;
            }
            (*sum)-=findFirst->data->getGrade();
            (*rank)-=1;
            findFirst = findFirst->left;
        }
    }
    if (findFirst) {
        RankTree<UDCompanyTree>::Node* smaller = findFirst;
        while (smaller) {
            if (smaller->data->getSalary() < lowerSalary) {
                if (smaller->left) {
                    (*sum)-=(smaller->left->sum);
                    (*rank)-=(smaller->left->rank);
                }
                (*sum)-=smaller->data->getGrade();
                (*rank)-=1;
                smaller = smaller->right;
            }
            else {
                smaller = smaller->left;
            }
        }
        RankTree<UDCompanyTree>::Node* greater = findFirst;
        while (greater) {
            if (greater->data->getSalary() > higherSalary) {
                    if (greater->right) {
                        (*sum)-=(greater->right->sum);
                        (*rank)-=(greater->right->rank);
                    }
                (*sum)-=greater->data->getGrade();
                (*rank)-=1;
                greater = greater->left;
            }
            else {
                greater = greater->right;
            }
        }
    }
}

StatusType AverageBumpGradeBetweenSalaryByGroup(void *DS, int companyID, int lowerSalary, int higherSalary) {
    if (!DS || lowerSalary <0 || higherSalary < 0 || companyID < 0 || lowerSalary > higherSalary) {
        return INVALID_INPUT;
    }
    wet2DS* data = (wet2DS*)DS;
    if (data->numOfCompanies < companyID) {
        return INVALID_INPUT;
    }
    RankTree<UDCompanyTree>* employees;
    //if company id is 0 take all employees with salary, otherwise the company's employees
    long long sum = 0, rank = 0;
    if (companyID == 0) {
        employees = data->getEmployeesWithSalary();
        if (lowerSalary == 0) {
            sum+=data->getEmployees()->getGradesSum();
            rank+=data->getEmployees()->getNumNoSalary();
        }
    }
    else {
        companyUnionFind* companies = data->getCompanies();
        UDCompanyTree* company = companies->getCompanyRoot(companyID);
        employees = company->getEmployeesWithSalary();
        if (lowerSalary==0) {
            sum+=company->getEmployees()->getGradesSum();
            rank+=company->getEmployees()->getNumNoSalary();
        }
    }
    if (higherSalary == 0) {
        double averageGrade = (double)sum/rank;
        if (rank > 0) {
            printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", averageGrade);
        }
        else {
            return FAILURE;
        }
        return SUCCESS;
    }
    //look for the first node that satisfies the conditions. If there is none, we don't have such employees.
    //RankTree<UDCompanyTree>::Node* firstInRange = employees->findFirstNodeInRange(lowerSalary,higherSalary);
    if (!employees->getRoot()) {
        if (rank > 0) {
            double averageGrade = (double)sum/rank;
            printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", averageGrade);
            return SUCCESS;
        }
        return FAILURE;
    }
    //initialize the sum and rank with the node's sum and rank
    sum += employees->getRoot()->sum;
    rank += employees->getRoot()->rank;
    removeNotInRange(&sum,&rank,employees->getRoot(),lowerSalary,higherSalary);
    if (rank < 1 ) {
        return FAILURE;
    }
    double averageGrade = (double)sum/rank;
    printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", averageGrade);
    return SUCCESS;
}

void Quit(void** DS) {
    wet2DS** data_structs = (wet2DS**)DS;
    delete *data_structs;
    *DS = nullptr;
}
