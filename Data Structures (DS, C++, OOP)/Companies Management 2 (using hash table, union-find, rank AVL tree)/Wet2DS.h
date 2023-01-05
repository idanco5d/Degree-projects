#ifndef WET2_WET2DS_H
#define WET2_WET2DS_H

#include "CompanyUnionFind.h"
#include "hash.h"
#include "library2.h"
#include "UDCompanyTree.h"

class wet2DS {
    companyUnionFind* companies;
    RankTree<UDCompanyTree>* employeesWithSalary;
    employeeHash<UDCompanyTree>* employees;

public:
    long long numOfCompanies;
    wet2DS(long long numOfCompanies);
    ~wet2DS();
    void addEmployee (int employeeId, int companyId, int Grade, int salary=0);
    void removeEmployee (int employeeId);
    bool doesEmployeeExist (int employeeId) const;
    void PromoteEmployee(int employeeId, int gradeBump);
    companyUnionFind* getCompanies() const;
    UDCompanyTree **getCompaniesArray() const;
    employeeHash<UDCompanyTree>* getEmployees() const;
    RankTree<UDCompanyTree>* getEmployeesWithSalary() const;
};

#endif //WET2_WET2DS_H
