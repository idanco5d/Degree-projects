//
// Created by illus on 23/04/2022.
//

#include "library1.h"
#include "DS.h"
#include <memory>
#include <cassert>
#include <cmath>

using namespace DS;
using std::endl;

void *Init()
{
    CompaniesInfo* ds = new CompaniesInfo();
    return (void*)ds;
}

StatusType returnValue(DSStatus returned)
{
    switch (returned)
    {
        case DS_SUCCESS:
            return SUCCESS;
        case DS_FAILURE:
            return FAILURE;
        case DS_ALLOCATION_ERROR:
            return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType AddCompany(void *DS, int CompanyID, int Value)
{
    if (!DS || CompanyID<=0 || Value <= 0)
    {
        return INVALID_INPUT;
    }
    CompaniesInfo* data = (CompaniesInfo*)(DS);
    DSStatus add_comp_result = data -> AddCompanyToDS(CompanyID,Value);
    return returnValue(add_comp_result);
}

StatusType AddEmployee(void *DS, int EmployeeID, int CompanyID, int Salary, int Grade)
{
    if (!DS || EmployeeID<=0 || CompanyID<=0 || Salary <=0 || Grade < 0)
    {
        return INVALID_INPUT;
    }
    CompaniesInfo* data = (CompaniesInfo*)(DS);
    DSStatus add_emp_result = data -> AddEmployeeToDS(EmployeeID, CompanyID, Salary, Grade);
    return returnValue(add_emp_result);
}

StatusType RemoveCompany(void *DS, int CompanyID)
{
    if (!DS || CompanyID<=0)
    {
        return INVALID_INPUT;
    }
    CompaniesInfo* data = (CompaniesInfo*)(DS);
    DSStatus remove_result = data -> DSRemoveCompany(CompanyID);
    return returnValue(remove_result);
}

StatusType RemoveEmployee(void *DS, int EmployeeID)
{
    if (!DS || EmployeeID<=0)
    {
        return INVALID_INPUT;
    }
    CompaniesInfo* data = (CompaniesInfo*)(DS);
    DSStatus remove_result = data -> DSRemoveEmployee(EmployeeID);
    return returnValue(remove_result);
}

StatusType GetCompanyInfo(void *DS, int CompanyID, int *Value, int *NumEmployees)
{
    if (!DS || CompanyID <=0 || !Value || !NumEmployees)
    {
        return INVALID_INPUT;
    }
    CompaniesInfo* data = (CompaniesInfo*)(DS);
    std::shared_ptr<Company> company = data->findCompany(CompanyID);
    if (!company)
    {
        return FAILURE;
    }
    *Value = company->getValue();
    *NumEmployees = company->getNumOfEmployees();
    return SUCCESS;
}

StatusType GetEmployeeInfo(void *DS, int EmployeeID, int *EmployerID, int *Salary, int *Grade)
{
    if (!DS || !EmployerID || !Salary || !Grade || EmployeeID<=0)
    {
        return INVALID_INPUT;
    }
    CompaniesInfo* data = (CompaniesInfo*)(DS);
    std::shared_ptr<Employee<Company>> employee = data ->findEmployee(EmployeeID);
    if (!employee)
    {
        return FAILURE;
    }
    assert(employee->getEmployer());
    *EmployerID=employee->getEmployer()->getId();
    *Salary=employee->getSalary();
    *Grade=employee->getGrade();
    return SUCCESS;
}

StatusType IncreaseCompanyValue(void *DS, int CompanyID, int ValueIncrease)
{
    if (!DS || CompanyID<=0 || ValueIncrease<=0)
    {
        return INVALID_INPUT;
    }
    CompaniesInfo* data = (CompaniesInfo*)(DS);
    DSStatus increase_result = data -> DSIncreaseCompanyValue(CompanyID, ValueIncrease);
    return returnValue(increase_result);
}

StatusType PromoteEmployee(void *DS, int EmployeeID, int SalaryIncrease, int BumpGrade)
{
    if(!DS || EmployeeID<=0 || SalaryIncrease <=0)
    {
        return INVALID_INPUT;
    }
    CompaniesInfo* data = (CompaniesInfo*)(DS);
    std::shared_ptr<Employee<Company>> employee = data->findEmployee(EmployeeID);
    if (!employee)
    {
        return FAILURE;
    }
    Company* company = employee->getEmployer();
    data->DSRemoveEmployee(EmployeeID);
    std::shared_ptr<Employee<Company>> newEmployeePtr(new Employee<Company>(EmployeeID,employee->getSalary(), employee->getGrade(), company));
    newEmployeePtr->promoteEmployee(SalaryIncrease, BumpGrade);
    company->addEmployee(EmployeeID,newEmployeePtr->getSalary(),newEmployeePtr->getGrade(),newEmployeePtr);
    AvlTree<Key<int,int>, std::shared_ptr<Employee<Company>>>* employeesById=data->GetEmployeesById();
    AvlTree<Key<int,int>, std::shared_ptr<Employee<Company>>>* employeesBySalary=data->GetEmployeesBySalary();
    try {
        employeesById->insertNode(Key<int,int>(EmployeeID,EmployeeID), newEmployeePtr);
        employeesBySalary->insertNode(Key<int,int>(newEmployeePtr->getSalary(),EmployeeID), newEmployeePtr);
    }
    catch (std::bad_alloc &)
    {
        return ALLOCATION_ERROR;
    }
    std::shared_ptr<Employee<Company>> currHighestEarner=data->GetHighestEarner();
    if (!currHighestEarner)
    {
        data->SetHighestEarner(newEmployeePtr);
    }
    else
    {
        if (currHighestEarner->getSalary() < newEmployeePtr->getSalary() ||
        (currHighestEarner->getSalary() == newEmployeePtr->getSalary() && currHighestEarner->getId() > newEmployeePtr->getId()))
        {
            data->SetHighestEarner(newEmployeePtr);
        }
    }
    data->IncreaseNumOfEmployees();
    return SUCCESS;
}

StatusType HireEmployee(void *DS, int EmployeeID, int NewCompanyID)
{
    if (!DS || EmployeeID<=0 || NewCompanyID<=0)
    {
        return INVALID_INPUT;
    }
    CompaniesInfo* data = (CompaniesInfo*)(DS);
    DSStatus HireResult = data -> MoveEmployeeToCompany(EmployeeID,NewCompanyID);
    return returnValue(HireResult);
}

StatusType AcquireCompany(void *DS, int AcquirerID, int TargetID, double Factor)
{
    if (!DS || AcquirerID<=0 || TargetID<=0 || Factor<1 || TargetID==AcquirerID)
    {
        return INVALID_INPUT;
    }
    CompaniesInfo* data = (CompaniesInfo*)(DS);
    std::shared_ptr<Company> Acquirer = data ->findCompany(AcquirerID);
    std::shared_ptr<Company> Target = data ->findCompany(TargetID);
    if (!Acquirer || !Target || Acquirer->getValue() < 10*Target->getValue())
    {
        return FAILURE;
    }
    int acquirer_orig_value = Acquirer->getValue();
    Acquirer->SetCompanyValue(floor((Acquirer->getValue() + Target->getValue())*Factor));
    DSStatus move_emp_result = data ->MoveAllEmployeesFromTargetToAcquirer(Acquirer, Target);
    if (move_emp_result != DS_SUCCESS)
    {
        Acquirer->SetCompanyValue(acquirer_orig_value);
        return returnValue(move_emp_result);
    }
    DSStatus delete_target_result = data ->DSRemoveCompany(TargetID);
    if (delete_target_result != DS_SUCCESS)
    {
        data ->MoveAllEmployeesFromTargetToAcquirer(Target, Acquirer);
        Acquirer->SetCompanyValue(acquirer_orig_value);
        return returnValue(delete_target_result);
    }
    return SUCCESS;
}

StatusType setIDToHighestEarner(int *EmployeeID, std::shared_ptr<Company> company, CompaniesInfo* data)
{
    std::shared_ptr<Employee<Company>> highest_earner;
    if (company)
    {
        highest_earner = company -> getBestEarner();
    }
    else
    {
        highest_earner = data -> GetHighestEarner();
    }
    if (!highest_earner)
    {
        return FAILURE;
    }
    *EmployeeID = highest_earner->getId();
    return SUCCESS;
}

StatusType GetHighestEarner(void *DS, int CompanyID, int *EmployeeID)
{
    if (!DS || CompanyID==0 || !EmployeeID)
    {
        return INVALID_INPUT;
    }
    CompaniesInfo* data = (CompaniesInfo*)(DS);
    if (CompanyID>0)
    {
        std::shared_ptr<Company> company = data ->findCompany(CompanyID);
        if (!company)
        {
            return FAILURE;
        }
        return setIDToHighestEarner(EmployeeID, company, data);
    }
    return setIDToHighestEarner(EmployeeID, nullptr, data);
}

class FunctorArrayBySalaryInserter{
    int *EmployeeIDsArray;
    int currentNum;
public:
    FunctorArrayBySalaryInserter(int* EmployeeIDsArray) : EmployeeIDsArray(EmployeeIDsArray), currentNum(0) {};
    bool operator() (AvlNode<Key<int,int>,std::shared_ptr<Employee<Company>>>* node, bool isUsedForExitRule)
    {
        if (!node)
        {
            return false;
        }
        if (isUsedForExitRule)
        {
            return true;
        }
        std::shared_ptr<Employee<Company>> employee_data=node->getData();
        if (employee_data)
        {
            EmployeeIDsArray[currentNum++]=employee_data->getId();
        }
        return true;
    }
};

StatusType GetAllEmployeesBySalary(void *DS, int CompanyID, int **Employees, int *NumOfEmployees)
{
    if (!DS || CompanyID==0 || !Employees || !NumOfEmployees)
    {
        return INVALID_INPUT;
    }
    CompaniesInfo* data = (CompaniesInfo*)(DS);
    AvlTree<Key<int,int>, std::shared_ptr<Employee<Company>>>* employees_in_ds;
    if (CompanyID>0) {
        std::shared_ptr<Company> company = data->findCompany(CompanyID);
        if (!company) {
            return FAILURE;
        }
        *NumOfEmployees = company->getNumOfEmployees();
        employees_in_ds = company->getEmployeesBySalary();
    }
    else
    {
        *NumOfEmployees = data -> GetNumEmployees();
        employees_in_ds = data -> GetEmployeesBySalary();
    }
    int *localEmployees = (int*)malloc(sizeof(int) * *NumOfEmployees);
    if (!localEmployees)
    {
        return ALLOCATION_ERROR;
    }
    FunctorArrayBySalaryInserter array_inserter(localEmployees);
    employees_in_ds->doOppositeInOrder<FunctorArrayBySalaryInserter>(employees_in_ds->getRoot(),array_inserter);
    *Employees=localEmployees;
    return SUCCESS;
}

class functorHighestEarner {
    int& NumOfCompaniesCopy;
    int* localEmployees;
public:
    int AmountEmployeesInserted;
    functorHighestEarner(int& NumOfCompaniesCopy, int* localEmployees) : NumOfCompaniesCopy(NumOfCompaniesCopy), localEmployees(localEmployees),
    AmountEmployeesInserted(0){};
    bool operator() (AvlNode<Key<int,int>,std::shared_ptr<Company>>* node, bool isUsedForExitRule) {
        if (isUsedForExitRule)
        {
            if (NumOfCompaniesCopy <=0)
            {
                return false;
            }
            return true;
        }
        if (!node)
        {
            return false;
        }
        if (node->getData()->getBestEarner() && NumOfCompaniesCopy > 0)
        {
            localEmployees[AmountEmployeesInserted++]=node->getData()->getBestEarner()->getId();
            NumOfCompaniesCopy--;
        }
        return true;
    }
};

StatusType GetHighestEarnerInEachCompany(void *DS, int NumOfCompanies, int **Employees)
{
    if (!DS || NumOfCompanies < 1 || !Employees)
    {
        return INVALID_INPUT;
    }
    CompaniesInfo* data = (CompaniesInfo*)(DS);
    int* localEmployees = (int*)malloc(sizeof(int)*NumOfCompanies);
    if (!localEmployees)
    {
        return ALLOCATION_ERROR;
    }
    AvlTree<Key<int,int>,std::shared_ptr<Company>>* companies=data->GetCompaniesWithEmployees();
    int numOfCompaniesCopy=NumOfCompanies;
    functorHighestEarner highestEarnerFunc(numOfCompaniesCopy,localEmployees);
    if (companies->getRoot())
    {
        companies->doInOrder<functorHighestEarner>(companies->getRoot(),highestEarnerFunc);
    }
    else
    {
        return FAILURE;
    }
    if (highestEarnerFunc.AmountEmployeesInserted < NumOfCompanies)
    {
        return FAILURE;
    }
    *Employees=localEmployees;
    return SUCCESS;
}

void GetNumEmployeesMatchingHelper(AvlNode<Key<int,int>,std::shared_ptr<Employee<Company>>>* root, int MinEmployeeID, int MaxEmployeeID,
                                   int *TotalNumOfEmployees, int* TotalEmployeesInserted=nullptr, std::shared_ptr<Employee<Company>>* TotalEmployeesArray=nullptr)
{
    if (!root)
    {
        return;
    }
    assert(root->getData());
    int currentEmployeeId = root->getData()->getId();
    if (currentEmployeeId > MinEmployeeID)
    {
        GetNumEmployeesMatchingHelper(root->left, MinEmployeeID,MaxEmployeeID,TotalNumOfEmployees,TotalEmployeesInserted,TotalEmployeesArray);
    }
    if (currentEmployeeId >= MinEmployeeID && currentEmployeeId <= MaxEmployeeID)
    {
        if (!TotalEmployeesArray && !TotalEmployeesInserted)
        {
            (*TotalNumOfEmployees)++;
        }
        else
        {
            TotalEmployeesArray[*TotalEmployeesInserted]=root->getData();
            (*TotalEmployeesInserted)++;
        }
    }
    if (currentEmployeeId < MaxEmployeeID)
    {
        GetNumEmployeesMatchingHelper(root->right,MinEmployeeID,MaxEmployeeID,TotalNumOfEmployees,TotalEmployeesInserted,TotalEmployeesArray);
    }
}

StatusType GetNumEmployeesMatching(void *DS, int CompanyID, int MinEmployeeID, int MaxEmployeeId,
                                   int MinSalary, int MinGrade, int *TotalNumOfEmployees, int *NumOfEmployees)
{
    if (!DS || CompanyID==0 || MinEmployeeID < 0 || MaxEmployeeId < 0 || MinSalary < 0 || MinGrade < 0 || MinEmployeeID > MaxEmployeeId
    || !TotalNumOfEmployees || !NumOfEmployees)
    {
        return INVALID_INPUT;
    }
    CompaniesInfo* data = (CompaniesInfo*)(DS);
    AvlTree<Key<int,int>,std::shared_ptr<Employee<Company>>>* employees;
    if (CompanyID>0)
    {
        std::shared_ptr<Company> company=data->findCompany(CompanyID);
        if (!company)
        {
            return FAILURE;
        }
        employees=company->getEmployeesById();
    }
    else
    {
        employees=data->GetEmployeesById();
    }
    if (!employees->getRoot())
    {
        return FAILURE;
    }
    (*TotalNumOfEmployees)=(*NumOfEmployees)=0;
    GetNumEmployeesMatchingHelper(employees->getRoot(),MinEmployeeID,MaxEmployeeId,TotalNumOfEmployees);
    if (*TotalNumOfEmployees < 1 && *TotalNumOfEmployees >=0)
    {
        (*NumOfEmployees)=0;
        return SUCCESS;
    }
    std::shared_ptr<Employee<Company>>* TotalEmployeesArray;
    try {
        TotalEmployeesArray = new std::shared_ptr<Employee<Company>>[*TotalNumOfEmployees];
    }
    catch (std::bad_alloc &)
    {
        return ALLOCATION_ERROR;
    }
    int counter=0;
    GetNumEmployeesMatchingHelper(employees->getRoot(),MinEmployeeID,MaxEmployeeId,TotalNumOfEmployees,&counter, TotalEmployeesArray);
    for (int i=0; i<*TotalNumOfEmployees; i++)
    {
        assert(TotalEmployeesArray[i]);
        if (TotalEmployeesArray[i]->getSalary() >= MinSalary && TotalEmployeesArray[i]->getGrade() >= MinGrade)
        {
            (*NumOfEmployees)++;
        }
    }
    delete[] TotalEmployeesArray;
    return SUCCESS;
}

void Quit(void** DS) {
    CompaniesInfo** data_structs = (CompaniesInfo**)DS;
    delete *data_structs;
    *DS=nullptr;
}