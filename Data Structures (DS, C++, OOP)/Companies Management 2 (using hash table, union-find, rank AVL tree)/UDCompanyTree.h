//
// Created by illus on 01/06/2022.
//

#ifndef COMPANY_UNION_FIND_UDCOMPANYTREE_H
#define COMPANY_UNION_FIND_UDCOMPANYTREE_H

#include "rankTreeActual.h"
#include "SalaryIdKey.h"
#include "Employee.h"
#include "hash.h"

using namespace DS;

class UDCompanyTree {
    int companyId;
    double value;
    RankTree<UDCompanyTree>* employeesWithSalary;
    UDCompanyTree* parent;
    int size;
    double purchaseFactor;
    employeeHash<UDCompanyTree>* employees;
    void mergeArrays(std::shared_ptr<Employee<UDCompanyTree>>* src1, std::shared_ptr<Employee<UDCompanyTree>>* src2, std::shared_ptr<Employee<UDCompanyTree>>* target,
                     int src1Size, int src2Size);
    void mergeWithEmployeesOf (UDCompanyTree* companyTree);
    void leavesTrimmer(RankTree<UDCompanyTree>* tree, int requiredSize, RankTree<UDCompanyTree>::Node* root);
    RankTree<UDCompanyTree>* createEmptyTree(int treeSize);
    RankTree<UDCompanyTree>::Node* createEmptyTreeNodes(int* keysArray,RankTree<UDCompanyTree>::Node* root, int counter, int treeSize, RankTree<UDCompanyTree>::Node* parent=nullptr);
public:
    explicit UDCompanyTree(const int& companyId);
    ~UDCompanyTree();
    void makeSon (UDCompanyTree* tree);
    const int& getSize() const;
    double getValue();
    void setValue(double newValue);
    void makeThisRoot(UDCompanyTree* currentRoot);
    UDCompanyTree* getRoot();
    void setParent(UDCompanyTree* newParent);
    void addEmployee(std::shared_ptr<Employee<UDCompanyTree>> employee);
    void removeEmployee(int employeeSalary, int employeeId);
    RankTree<UDCompanyTree>* getEmployeesWithSalary();
    employeeHash<UDCompanyTree>* getEmployees();
    int getParent();
    int getId() const;
    void increasePurchaseFactor(double increaseBy);
    double getPurchaseFactor() const;
};

#endif //COMPANY_UNION_FIND_UDCOMPANYTREE_H
