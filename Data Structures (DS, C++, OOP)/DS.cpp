#include "DS.h"
#include <cassert>
#include <cmath>
#include <memory>

namespace DS {

    CompaniesInfo::CompaniesInfo() : NumEmployees(0){
        companies = new AvlTree<Key<int,int>,std::shared_ptr<Company>>;
        companies_with_employees = new AvlTree<Key<int,int>,std::shared_ptr<Company>>;
        employees_by_id = new AvlTree<Key<int,int>,std::shared_ptr<Employee<Company>>>;
        employees_by_salary = new AvlTree<Key<int,int>,std::shared_ptr<Employee<Company>>>;
    }

    CompaniesInfo::~CompaniesInfo() {
        delete companies;
        delete companies_with_employees;
        delete employees_by_id;
        delete employees_by_salary;
    }

    class toRemove1 {
    public:
        bool operator() (AvlNode<Key<int,int>, std::shared_ptr<Employee<Company>>>* node, bool isUsedForExitRule) {
            if (isUsedForExitRule) {
                return true;
            }
            std::shared_ptr<Employee<Company>> employee = node->getData();
            std::cout << "Employee " << employee->getId() << " salary: " << employee->getSalary();
            std::cout << " left son: ";
            if (node->left)
                std::cout << node->left->getData()->getId();
            else
                std::cout << "NULL";
            std::cout << " right son: ";
            if (node->right)
                std::cout << node->right->getData()->getId();
            else
                std::cout << "NULL";
            std::cout << " parent: ";
            if (node->parent)
                std::cout << node->parent->getData()->getId();
            else
                std::cout << "NULL";
            std::cout << std::endl;
            return true;
        }
    };

    DSStatus CompaniesInfo::AddCompanyToDS(int CompanyID, int Value) {
        Key<int,int> id(CompanyID,CompanyID);
        if (companies->findNode(id))
        {
            return DS_FAILURE;
        }
        std::shared_ptr<Company> company;
        try {
            std::shared_ptr<Company> companyTry(new Company(CompanyID, Value));
            company=companyTry;
            companies->insertNode(id,company);
        }
        catch (std::bad_alloc &)
        {
            return DS_ALLOCATION_ERROR;
        }
        return DS_SUCCESS;
    }

    DSStatus CompaniesInfo::AddEmployeeToDS(int EmployeeID, int CompanyID, int Salary, int Grade) {
        Key<int,int> employeeIdKey(EmployeeID,EmployeeID), employeeSalaryKey(Salary,EmployeeID), companyId(CompanyID,CompanyID);
        AvlNode<Key<int,int>,std::shared_ptr<Company>>* companyNode=companies->findNode(companyId);
        if (!companyNode || employees_by_id->findNode(employeeIdKey) || employees_by_salary->findNode(employeeSalaryKey))
        {
            return DS_FAILURE;
        }
        std::shared_ptr<Company> company = companyNode->getData();
        assert(company);
        std::shared_ptr<Employee<Company>> employee=nullptr;
        try {
            std::shared_ptr<Employee<Company>> newEmployee(new Employee<Company>(EmployeeID,Salary,Grade, company.get()));
            employee=newEmployee;
        if (companyNode->getData()->getNumOfEmployees() == 0)
        {
            companies_with_employees->insertNode(Key<int,int>(CompanyID,CompanyID),company);
        }
        companyNode->getData()->addEmployee(EmployeeID,Salary,Grade, employee);
        employees_by_salary->insertNode(employeeSalaryKey,employee);
        employees_by_id->insertNode(employeeIdKey,employee);
        if (!highestEarner)
        {
            highestEarner=employee;
        }
        else
        {
            int currHighestEarnerSalary = highestEarner->getSalary();
            if (currHighestEarnerSalary < Salary || (currHighestEarnerSalary == Salary && highestEarner->getId() > EmployeeID))
            {
                highestEarner=employee;
            }
        }
        }
        catch (std::bad_alloc &)
        {
            return DS_ALLOCATION_ERROR;
        }
        NumEmployees++;
        return DS_SUCCESS;
    }

    DSStatus CompaniesInfo::DSRemoveCompany(int CompanyID) {
        Key<int,int> companyIdKey(CompanyID,CompanyID);
        AvlNode<Key<int,int>,std::shared_ptr<Company>>* companyNode = companies->findNode(companyIdKey);
        if (!companyNode)
        {
            return DS_FAILURE;
        }
        std::shared_ptr<Company> company=companyNode->getData();
        assert(company);
        if (company->getNumOfEmployees() > 0)
        {
            return DS_FAILURE;
        }
        companies->removeNode(companyIdKey);
        companies_with_employees->removeNode(companyIdKey);
        return DS_SUCCESS;
    }

    DSStatus CompaniesInfo::DSRemoveEmployee(int EmployeeID) {
        Key<int,int> idKey(EmployeeID,EmployeeID);
        AvlNode<Key<int,int>,std::shared_ptr<Employee<Company>>>* employeeNode = employees_by_id->findNode(idKey);
        if (!employeeNode)
        {
            return DS_FAILURE;
        }
        std::shared_ptr<Employee<Company>> employee = employeeNode->getData();
        assert(employee);
        bool setNewHighestEarner=false;
        if (employee->getId() == highestEarner->getId())
        {
            setNewHighestEarner=true;
        }
        Key<int,int> salaryKey(employee->getSalary(), EmployeeID);
        assert(employees_by_salary->findNode(salaryKey) && employee->getEmployer());
        if (employee->getEmployer()->getNumOfEmployees() == 1)
        {
            int companyId=employee->getEmployer()->getId();
            companies_with_employees->removeNode(Key<int,int>(companyId,companyId));
        }
        employee->getEmployer()->removeEmployee(EmployeeID,employee->getSalary());
        employees_by_id->removeNode(idKey);
        employees_by_salary->removeNode(salaryKey);
        if (setNewHighestEarner)
        {
            auto max_node = employees_by_salary->getMaxNode();
            highestEarner=max_node ? employees_by_salary->getMaxNode()->getData() : nullptr;
        }
        NumEmployees--;
        return DS_SUCCESS;
    }

    std::shared_ptr<Company> CompaniesInfo::findCompany(int CompanyID) {
        AvlNode<Key<int,int>,std::shared_ptr<Company>>* companyNode = companies->findNode(Key<int,int>(CompanyID,CompanyID));
        return companyNode ? companyNode->getData() : nullptr;
    }

    std::shared_ptr<Employee<Company>> CompaniesInfo::findEmployee(int EmployeeID) {
        AvlNode<Key<int,int>,std::shared_ptr<Employee<Company>>>* employeeNode = employees_by_id->findNode(Key<int,int>(EmployeeID,EmployeeID));
        return employeeNode ? employeeNode->getData() : nullptr;
    }

    DSStatus CompaniesInfo::DSIncreaseCompanyValue(int CompanyID, int ValueIncrease) {
        AvlNode<Key<int,int>,std::shared_ptr<Company>>* companyNode = companies->findNode(Key<int,int>(CompanyID,CompanyID));
        if (!companyNode)
        {
            return DS_FAILURE;
        }
        std::shared_ptr<Company> company = companyNode->getData();
        assert(company);
        company->SetCompanyValue(company->getValue() + ValueIncrease);
        return DS_SUCCESS;
    }

    DSStatus CompaniesInfo::MoveEmployeeToCompany(int EmployeeID, int CompanyID) {
        AvlNode<Key<int,int>,std::shared_ptr<Employee<Company>>>* employeeNode = employees_by_id->findNode(Key<int,int>(EmployeeID,EmployeeID));
        if (!employeeNode)
        {
            return DS_FAILURE;
        }
        std::shared_ptr<Employee<Company>> employee = employeeNode->getData();
        assert(employee);
        if (employee->getEmployer()->getId() == CompanyID)
        {
            return DS_FAILURE;
        }
        AvlNode<Key<int,int>,std::shared_ptr<Company>>* newCompanyNode = companies->findNode(Key<int,int>(CompanyID,CompanyID));
        if (!newCompanyNode)
        {
            return DS_FAILURE;
        }
        std::shared_ptr<Company> newCompany = newCompanyNode->getData();
        Company* existingCompany = employee->getEmployer();
        assert(newCompany && existingCompany);
        int employee_salary = employee->getSalary(), employee_grade=employee->getGrade();
        if (DSRemoveEmployee(EmployeeID) == DS_FAILURE)
        {
            return DS_FAILURE;
        }
        DSStatus add_result = AddEmployeeToDS(EmployeeID,newCompany->getId(),employee_salary,employee_grade);
        if (add_result !=DS_SUCCESS)
        {
            AddEmployeeToDS(EmployeeID,existingCompany->getId(), employee_salary, employee_grade);
            return add_result;
        }
        return DS_SUCCESS;
    }

    void CompaniesInfo::mergeArrays(std::shared_ptr<Employee<Company>> *firstArray,
                                        std::shared_ptr<Employee<Company>> *secondArray,
                                        std::shared_ptr<Employee<Company>> *targetArray,
                                        bool isMergeById,
                                        int firstArraySize, int secondArraySize) const {
        int firstArrayBottom=0, secondArrayBottom=0, i=0;
        while (firstArrayBottom < firstArraySize && secondArrayBottom < secondArraySize)
        {
            assert(firstArray[firstArrayBottom] && secondArray[secondArrayBottom]);
            if (isMergeById)
            {
                if (firstArray[firstArrayBottom]->getId() < secondArray[secondArrayBottom] -> getId())
                {
                    targetArray[i++] = firstArray[firstArrayBottom++];
                }
                else {
                    targetArray[i++] = secondArray[secondArrayBottom++];
                }
            }
            if (!isMergeById)
            {
                if (firstArray[firstArrayBottom] -> getSalary() < secondArray[secondArrayBottom] -> getSalary()
                || (firstArray[firstArrayBottom]->getSalary() == secondArray[secondArrayBottom] -> getSalary() &&
                        firstArray[firstArrayBottom]->getId() > secondArray[secondArrayBottom] -> getId()))
                {
                    targetArray[i++] = firstArray[firstArrayBottom++];
                }
                else {
                    targetArray[i++] = secondArray[secondArrayBottom++];
                }
            }
        }
        while (firstArrayBottom < firstArraySize)
        {
            targetArray[i++] = firstArray[firstArrayBottom++];
        }
        while (secondArrayBottom < secondArraySize)
        {
            targetArray[i++] = secondArray[secondArrayBottom++];
        }
    }

    AvlNode<Key<int,int>,std::shared_ptr<Employee<Company>>>* CompaniesInfo::createAllEmployeesTreeNodes (int* keys_array, AvlNode<Key<int,int>, std::shared_ptr<Employee<Company>>>* root,
                                                     int counter, int treeSize, AvlNode<Key<int,int>, std::shared_ptr<Employee<Company>>>* parent) const {
        if (counter < treeSize)
        {
            std::shared_ptr<Employee<Company>> currEmployee(new Employee<Company>(keys_array[counter],0,0,nullptr));
            AvlNode<Key<int,int>,std::shared_ptr<Employee<Company>>>* node (new AvlNode<Key<int,int>,std::shared_ptr<Employee<Company>>>(Key<int,int>(keys_array[counter],keys_array[counter]),currEmployee));
            root = node;
            if (parent) {
                root->parent=parent;
            }
            root->left = createAllEmployeesTreeNodes(keys_array,root->left,2*counter + 1, treeSize, root);
            root->right = createAllEmployeesTreeNodes(keys_array,root->right,2*counter + 2,treeSize, root);
            return root;
        }
        return nullptr;
    }

    class functorTrimLeaves {
        int requiredTreeSize;
        int currentTreeSize;
    public:
        functorTrimLeaves(int requiredTreeSize, int currentTreeSize) : requiredTreeSize(requiredTreeSize), currentTreeSize(currentTreeSize) {}
        bool operator() (AvlNode<Key<int,int>,std::shared_ptr<Employee<Company>>>* node, bool isUsedForExitRule) {
            if (isUsedForExitRule)
            {
                return currentTreeSize > requiredTreeSize;
            }
            if (currentTreeSize > requiredTreeSize)
            {
                if (node->parent)
                {
                    if (node->parent->left == node)
                    {
                        node->parent->left = nullptr;
                        currentTreeSize--;
                    }
                    if (node->parent->right == node)
                    {
                        node->parent->right = nullptr;
                        currentTreeSize--;
                    }
                    delete node;
                }
            }
            return true;
        }
    };

    void CompaniesInfo::leavesTrimmer(int requiredSize,
                                      AvlNode<Key<int, int>, std::shared_ptr<Employee<Company>>> *root,
                                      AvlTree<Key<int,int>, std::shared_ptr<Employee<Company>>>* tree) const {
        if ((int)tree->num_of_nodes > requiredSize && root) {
            if (!root->left && !root->right) {
                assert(root->parent);
                if (root->parent->right == root) {
                    root->parent->right=nullptr;
                }
                else {
                    root->parent->left=nullptr;
                }
                root->parent->updateHeight();
                root->parent->calculateBf();
                tree->num_of_nodes--;
                delete root;
            }
            leavesTrimmer(requiredSize,root->right, tree);
            leavesTrimmer(requiredSize,root->left,tree);
        }
    }

    AvlTree<Key<int,int>, std::shared_ptr<Employee<Company>>>* CompaniesInfo::buildNewEmployeesTree (int treeSize) const {
        int tree_height = ceil(log2(treeSize));
        int* keys_array;
        try {
            keys_array = new int[(int)pow(2,tree_height)];
        }
        catch (const std::bad_alloc &)
        {
            return nullptr;
        }
        int fullTreeSize = (int)pow(2,tree_height)-1;
        for (int i=0; i < fullTreeSize; i++)
        {
            keys_array[i]=i+1;
        }
        AvlNode<Key<int,int>, std::shared_ptr<Employee<Company>>>* root_node= nullptr;
        root_node = createAllEmployeesTreeNodes(keys_array,root_node,0,fullTreeSize);
        AvlTree<Key<int,int>, std::shared_ptr<Employee<Company>>>* tree;
        try {
            tree=new AvlTree<Key<int,int>,std::shared_ptr<Employee<Company>>>;
        }
        catch (const std::bad_alloc &) {
            root_node->deleteAllChildren();
            return nullptr;
        }
        tree->setRoot(root_node);
        tree->num_of_nodes=fullTreeSize;
        if (treeSize < fullTreeSize)
        {
            leavesTrimmer(treeSize,tree->getRoot(),tree);
        }
        delete[] keys_array;
        return tree;
    }

    class functorAddAllEmployees {
    public:
        std::shared_ptr<Employee<Company>>* allEmployees;
        bool isAddingToArrayAndNotToTree;
        std::shared_ptr<Company> company;
        bool isIdKey;
        int employeesAdded;
        functorAddAllEmployees(std::shared_ptr<Employee<Company>>* allEmployees, bool isAddingToArrayAndNotToTree, bool isIdKey, std::shared_ptr<Company> company) : allEmployees(allEmployees),
        isAddingToArrayAndNotToTree(isAddingToArrayAndNotToTree), company(company), isIdKey(isIdKey), employeesAdded(0) {}
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
            if (isAddingToArrayAndNotToTree)
            {
                assert(node->getData());
                allEmployees[employeesAdded++] = node->getData();
                return true;
            }
            assert(allEmployees[employeesAdded] && allEmployees[employeesAdded]->getEmployer());
            if (allEmployees[employeesAdded]->getEmployer()->getId() != company->getId())
            {
                allEmployees[employeesAdded]->setCompany(company.get());
            }
            node->setData(allEmployees[employeesAdded]);
            int employeeId = allEmployees[employeesAdded]->getId();
            if (isIdKey) {
                node->key = Key<int,int>(employeeId, employeeId);
                employeesAdded++;
                return true;
            }
            else {
                int employeeSalary = allEmployees[employeesAdded++] ->getSalary();
                node->key = Key<int,int>(employeeSalary,employeeId);
                return true;
            }
        }
    };

    class functorArrangeTree {
    public:
        bool operator() (AvlNode<Key<int,int>,std::shared_ptr<Employee<Company>>>* node, bool isUsedForExitRule) {
            if (isUsedForExitRule) {
                return true;
            }
            node->updateHeight();
            node->calculateBf();
            return true;
        }
    };

    DSStatus CompaniesInfo::MoveAllEmployeesFromTargetToAcquirer(std::shared_ptr<Company> Acquirer, std::shared_ptr<Company> Target) {
        //If target has no employees do nothing,
        //If acquirer has no employees make the target's the acquirer's employees
        int targetNumOfEmployees = Target->getNumOfEmployees(), AcquirerNumOfEmployees = Acquirer->getNumOfEmployees();
        if (targetNumOfEmployees == 0)
        {
            return DS_SUCCESS;
        }
        if (AcquirerNumOfEmployees == 0)
        {
            Acquirer->setEmployeesById(Target->getEmployeesById()->clone());
            Acquirer->setEmployeesBySalary(Target->getEmployeesBySalary()->clone());
            Acquirer->setNumEmployees(Target->getNumOfEmployees());
            Acquirer->updateBestEarner();
            Target->setNumEmployees(0);
            return DS_SUCCESS;
        }
        //Algorithm: 1. create two sorted arrays with target's and acquirer's employees (O(n)) and merge into one sorted (O(n))
        // do 1 for both IDs and salaries
        //2. Create two empty trees (each O(n) complexity) for IDs tree and salaries tree
        //3. Perform inOrder in each to insert the matching array's employees
        try {
            std::shared_ptr<Employee<Company>>* acquirerEmployeesById = new std::shared_ptr<Employee<Company>>[AcquirerNumOfEmployees];
            std::shared_ptr<Employee<Company>>* targetEmployeesById = new std::shared_ptr<Employee<Company>>[targetNumOfEmployees];
            std::shared_ptr<Employee<Company>>* allEmployeesById = new std::shared_ptr<Employee<Company>>[targetNumOfEmployees + AcquirerNumOfEmployees];
            functorAddAllEmployees addAcquirerEmployeesById(acquirerEmployeesById, true, true,Acquirer), addTargetEmployeesById(targetEmployeesById,true, true,Acquirer);
            AvlTree<Key<int, int>, std::shared_ptr<Employee<Company>>>* targetEmployeesTreeById = Target->getEmployeesById();
            AvlTree<Key<int, int>, std::shared_ptr<Employee<Company>>>* acquirerEmployeesTreeById = Acquirer->getEmployeesById();
            AvlTree<Key<int, int>, std::shared_ptr<Employee<Company>>>* targetEmployeesTreeBySalary = Target->getEmployeesBySalary();
            AvlTree<Key<int, int>, std::shared_ptr<Employee<Company>>>* acquirerEmployeesTreeBySalary = Acquirer->getEmployeesBySalary();
            targetEmployeesTreeById->doInOrder<functorAddAllEmployees>(targetEmployeesTreeById->getRoot(), addTargetEmployeesById);
            acquirerEmployeesTreeById->doInOrder<functorAddAllEmployees>(acquirerEmployeesTreeById->getRoot(), addAcquirerEmployeesById);
            mergeArrays(acquirerEmployeesById, targetEmployeesById, allEmployeesById,true,
                        AcquirerNumOfEmployees, targetNumOfEmployees);
            std::shared_ptr<Employee<Company>>* acquirerEmployeesBySalary = new std::shared_ptr<Employee<Company>>[AcquirerNumOfEmployees];
            std::shared_ptr<Employee<Company>>* targetEmployeesBySalary = new std::shared_ptr<Employee<Company>>[targetNumOfEmployees];
            std::shared_ptr<Employee<Company>>* allEmployeesBySalary = new std::shared_ptr<Employee<Company>>[targetNumOfEmployees + AcquirerNumOfEmployees];
            functorAddAllEmployees addAcquirerEmployeesBySalary(acquirerEmployeesBySalary, true, false,Acquirer), addTargetEmployeesBySalary(targetEmployeesBySalary,true, false,Target);
            targetEmployeesTreeBySalary->doInOrder<functorAddAllEmployees>(targetEmployeesTreeBySalary->getRoot(), addTargetEmployeesBySalary);
            acquirerEmployeesTreeBySalary->doInOrder<functorAddAllEmployees>(acquirerEmployeesTreeBySalary->getRoot(), addAcquirerEmployeesBySalary);
            mergeArrays(acquirerEmployeesBySalary, targetEmployeesBySalary, allEmployeesBySalary,false,
                        AcquirerNumOfEmployees, targetNumOfEmployees);
            AvlTree<Key<int,int>, std::shared_ptr<Employee<Company>>>* newEmployeesByIdTree = buildNewEmployeesTree(targetNumOfEmployees + AcquirerNumOfEmployees);
            AvlTree<Key<int,int>, std::shared_ptr<Employee<Company>>>* newEmployeesBySalaryTree = buildNewEmployeesTree(targetNumOfEmployees + AcquirerNumOfEmployees);
            if (!newEmployeesByIdTree || !newEmployeesBySalaryTree)
            {
                delete[] acquirerEmployeesById;
                delete[] targetEmployeesById;
                delete[] allEmployeesById;
                delete[] acquirerEmployeesBySalary;
                delete[] targetEmployeesBySalary;
                delete[] allEmployeesBySalary;
                return DS_ALLOCATION_ERROR;
            }
            functorArrangeTree arranger;
            newEmployeesByIdTree->doPostOrder(newEmployeesByIdTree->getRoot(), arranger);
            newEmployeesBySalaryTree->doPostOrder(newEmployeesByIdTree->getRoot(), arranger);
            functorAddAllEmployees funcAddAllToTree(allEmployeesById, false, true,Acquirer);
            newEmployeesByIdTree->doInOrder<functorAddAllEmployees>(newEmployeesByIdTree->getRoot(), funcAddAllToTree);
            funcAddAllToTree.employeesAdded=0;
            funcAddAllToTree.isIdKey=false;
            funcAddAllToTree.allEmployees=allEmployeesBySalary;
            newEmployeesBySalaryTree->doInOrder<functorAddAllEmployees>(newEmployeesBySalaryTree->getRoot(),funcAddAllToTree);
            Acquirer->setEmployeesById(newEmployeesByIdTree);
            Acquirer->setEmployeesBySalary(newEmployeesBySalaryTree);
            Acquirer->setNumEmployees(targetNumOfEmployees + AcquirerNumOfEmployees);
            Acquirer->updateBestEarner();
            Target->setNumEmployees(0);
            delete[] acquirerEmployeesById;
            delete[] targetEmployeesById;
            delete[] allEmployeesById;
            delete[] acquirerEmployeesBySalary;
            delete[] targetEmployeesBySalary;
            delete[] allEmployeesBySalary;
        }
        catch (const std::bad_alloc &)
        {
            return DS_ALLOCATION_ERROR;
        }
        return DS_SUCCESS;
    }

    std::shared_ptr<Employee<Company>> CompaniesInfo::GetHighestEarner() {
        return highestEarner;
    }

    int CompaniesInfo::GetNumEmployees() const {
        return NumEmployees;
    }

    AvlTree<Key<int,int>,std::shared_ptr<Employee<Company>>>* CompaniesInfo::GetEmployeesById() {
        return employees_by_id;
    }

    AvlTree<Key<int,int>,std::shared_ptr<Company>>* CompaniesInfo::GetCompanies() {
        return companies;
    }

    AvlTree<Key<int,int>,std::shared_ptr<Employee<Company>>>* CompaniesInfo::GetEmployeesBySalary() {
        return employees_by_salary;
    }

    AvlTree<Key<int,int>,std::shared_ptr<Company>>* CompaniesInfo::GetCompaniesWithEmployees() {
        return companies_with_employees;
    }

    void CompaniesInfo::SetHighestEarner(std::shared_ptr<Employee<Company>> newHighestEarner) {
        highestEarner=newHighestEarner;
    }

    void CompaniesInfo::IncreaseNumOfEmployees() {
        NumEmployees++;
    }

}
