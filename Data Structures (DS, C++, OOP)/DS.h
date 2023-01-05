//
// Created by illus on 23/04/2022.
//

#ifndef WET_1_DS_H
#define WET_1_DS_H

#include "AVLTree.h"
#include "Company.h"
#include "Employee.h"

namespace DS {
    typedef enum {
        DS_SUCCESS, DS_FAILURE, DS_ALLOCATION_ERROR
    } DSStatus;
    class CompaniesInfo {
        AvlTree<Key<int,int>,std::shared_ptr<Company>>* companies;
        AvlTree<Key<int,int>,std::shared_ptr<Company>>* companies_with_employees;
        AvlTree<Key<int,int>,std::shared_ptr<Employee<Company>>>* employees_by_id;
        AvlTree<Key<int,int>,std::shared_ptr<Employee<Company>>>* employees_by_salary;
        int NumEmployees;
        std::shared_ptr<Employee<Company>> highestEarner;

        void mergeArrays(std::shared_ptr<Employee<Company>> firstArray[],
                             std::shared_ptr<Employee<Company>> secondArray[], std::shared_ptr<Employee<Company>> targetArray[],
                             bool isMergeById, int firstArraySize, int secondArraySize) const;
        AvlTree<Key<int,int>, std::shared_ptr<Employee<Company>>>* buildNewEmployeesTree (int treeSize) const;
        AvlNode<Key<int,int>,std::shared_ptr<Employee<Company>>>* createAllEmployeesTreeNodes (int* keys_array, AvlNode<Key<int,int>, std::shared_ptr<Employee<Company>>>* root,
                                          int counter, int treeSize, AvlNode<Key<int,int>, std::shared_ptr<Employee<Company>>>* parent=nullptr) const;
        void leavesTrimmer(int requiredSize, AvlNode<Key<int,int>, std::shared_ptr<Employee<Company>>>* root, AvlTree<Key<int,int>, std::shared_ptr<Employee<Company>>>* tree) const;


    public:
        CompaniesInfo();
        ~CompaniesInfo();
        DSStatus AddCompanyToDS(int CompanyID, int Value);
        DSStatus AddEmployeeToDS(int EmployeeID, int CompanyID, int Salary, int Grade);
        DSStatus DSRemoveCompany (int CompanyID);
        DSStatus DSRemoveEmployee (int EmployeeID);
        std::shared_ptr<Company> findCompany(int CompanyID);
        std::shared_ptr<Employee<Company>> findEmployee(int EmployeeID);
        DSStatus DSIncreaseCompanyValue (int CompanyID, int ValueIncrease);
        DSStatus MoveEmployeeToCompany (int EmployeeID, int CompanyID);
        DSStatus MoveAllEmployeesFromTargetToAcquirer(std::shared_ptr<Company> Acquirer, std::shared_ptr<Company> Target);
        std::shared_ptr<Employee<Company>> GetHighestEarner();
        int GetNumEmployees() const;
        AvlTree<Key<int,int>,std::shared_ptr<Employee<Company>>>* GetEmployeesById();
        AvlTree<Key<int,int>,std::shared_ptr<Company>>* GetCompanies();
        AvlTree<Key<int,int>,std::shared_ptr<Company>>* GetCompaniesWithEmployees();
        AvlTree<Key<int,int>,std::shared_ptr<Employee<Company>>>* GetEmployeesBySalary();
        void SetHighestEarner(std::shared_ptr<Employee<Company>> newHighestEarner);
        void IncreaseNumOfEmployees();
    };
}

#endif //WET_1_DS_H
