#ifndef COMPANY_UNION_FIND_COMPANYUNIONFIND_H
#define COMPANY_UNION_FIND_COMPANYUNIONFIND_H

#include "UDCompanyTree.h"
#include "Employee.h"

class companyUnionFind {
    UDCompanyTree** companiesArray;
    int numOfCompanies;
public:
    explicit companyUnionFind(int numOfCompanies);
    ~companyUnionFind();
    void acquire(int acquirerId, int targetId, double factor);
    UDCompanyTree* getCompanyRoot (int companyId);
    void addEmployee (int companyId, std::shared_ptr<Employee<UDCompanyTree>> employee);
    void removeEmployee (int companyId, int employeeSalary, int employeeId);
    void print() const;
    UDCompanyTree** getCompaniesArray() const;
};

#endif //COMPANY_UNION_FIND_COMPANYUNIONFIND_H
