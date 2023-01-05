#include "CompanyUnionFind.h"
#include <iostream>
#include <cassert>

companyUnionFind::companyUnionFind(int numOfCompanies) : numOfCompanies(numOfCompanies) {
    try {
        companiesArray = new UDCompanyTree*[numOfCompanies];
    }
    catch (std::bad_alloc&) {
        companiesArray=nullptr;
        return;
    }
    try {
        for (int i=1; i<=numOfCompanies; i++) {
            UDCompanyTree* companyNode = new UDCompanyTree(i);
            companiesArray[i-1]=companyNode;
        }
    }
    catch (std::bad_alloc&) {
        delete[] companiesArray;
        companiesArray=nullptr;
    }
}

companyUnionFind::~companyUnionFind() {
    for (int i=0; i<numOfCompanies; i++) {
        delete companiesArray[i];
    }
    delete[] companiesArray;
}

class toRemoveTreePrint {
public:
    bool operator() (RankTree<UDCompanyTree>::Node* node, bool exitRule) {
        if (exitRule) {
            return true;
        }
        std::shared_ptr<Employee<UDCompanyTree>> employee = node->data;
        std::cout << *node->data << std::endl;
        return true;
    }
};

void companyUnionFind::acquire(int acquirerId, int targetId, double factor) {
    assert(acquirerId <= numOfCompanies && targetId <=numOfCompanies);
    UDCompanyTree* acquirer=getCompanyRoot(acquirerId);
    UDCompanyTree* target=getCompanyRoot(targetId);
    int acquirerSize = acquirer->getSize();
    int targetSize = target->getSize();
    acquirer->increasePurchaseFactor(factor*target->getValue());
    target->increasePurchaseFactor(-acquirer->getPurchaseFactor());
    if (acquirerSize >= targetSize) {
        acquirer->makeSon(target);
        target->getEmployees()->mergeTwoHashs(acquirer->getEmployees(),acquirer);
    }
    else {
        target->makeSon(acquirer);
        target->getEmployees()->mergeTwoHashs(target->getEmployees(),acquirer);
        acquirer->makeThisRoot(target);
    }
}

UDCompanyTree* companyUnionFind::getCompanyRoot(int companyId) {
    UDCompanyTree* requiredCompany = companiesArray[companyId-1];
    UDCompanyTree* root = requiredCompany->getRoot();
    //After finding the company's root, make the root the company's parent (shrinking)
    if(root->getId() != companyId) {
        requiredCompany->setParent(root);
    }
    return root;
}

void companyUnionFind::addEmployee(int companyId, std::shared_ptr<Employee<UDCompanyTree>> employee) {
    UDCompanyTree* company = getCompanyRoot(companyId);
    //Employee<UDCompanyTree> employee(employeeId,employeeSalary,employeeGrade,company);
    company->addEmployee(employee);
}

void companyUnionFind::removeEmployee(int companyId, int employeeSalary, int employeeId) {
    getCompanyRoot(companyId)->removeEmployee(employeeSalary,employeeId);
}

void companyUnionFind::print() const {
    std::cout<< "Companies: " <<std::endl;
    for (int i=0; i<numOfCompanies; i++) {
        std::cout << "Company: " << i+1 << " value: " << companiesArray[i]->getValue() << " Parent: " << companiesArray[i]->getParent() << " Employees:" <<std::endl;
        if (companiesArray[i]->getParent()!=-1) {
            continue;
        }
        RankTree<UDCompanyTree>* tree=companiesArray[i]->getEmployeesWithSalary();
        tree->displayAvl(tree->getRoot());
    }
}

UDCompanyTree** companyUnionFind::getCompaniesArray() const {
    return companiesArray;
}