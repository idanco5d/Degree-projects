#include "UDCompanyTree.h"
#include <cassert>
#include <cmath>

UDCompanyTree::UDCompanyTree(const int& companyId) : companyId(companyId), value(companyId), employeesWithSalary(), parent(nullptr), size(1), purchaseFactor(0.0){
    employeesWithSalary=new RankTree<UDCompanyTree>();
    employees = new employeeHash<UDCompanyTree>();
}

UDCompanyTree::~UDCompanyTree() {
    if (!parent || parent->companyId == companyId) {
        delete employeesWithSalary;
        delete employees;
    }
}

const int& UDCompanyTree::getSize() const {
    return size;
}

//go over all parents, and add their purchase factor to this company's value. Once done, set the root as this company's parent.
double UDCompanyTree::getValue() {
    UDCompanyTree* acquirer = this->parent;
    while (acquirer && acquirer->parent && acquirer->parent->companyId != acquirer->companyId) {
        purchaseFactor += acquirer->purchaseFactor;
        acquirer = acquirer->parent;
    }
    double realValue = value + purchaseFactor;
    if (acquirer && companyId != acquirer->companyId) {
        setParent(acquirer);
        realValue+=acquirer->purchaseFactor;
    }
    return realValue;
}

void UDCompanyTree::setValue(double newValue) {
    value=newValue;
}

void UDCompanyTree::makeThisRoot(UDCompanyTree *currentRoot) {
    assert(parent == currentRoot && currentRoot);
    employeesWithSalary = currentRoot->employeesWithSalary;
    currentRoot->parent = this;
    parent = nullptr;
}

UDCompanyTree* UDCompanyTree::getRoot()  {
    UDCompanyTree* root = this;
    while (root->parent) {
        if (root->companyId == root->parent->companyId) {
            break;
        }
        root=root->parent;
        if (root->parent) {
            value+=root->purchaseFactor;
        }
    }
    return root;
}

void UDCompanyTree::setParent(UDCompanyTree *newParent) {
    parent=newParent;
}

void UDCompanyTree::addEmployee(std::shared_ptr<Employee<UDCompanyTree>> employee) {
    if (employee->getSalary() > 0) {
        salaryIdKey key(employee->getSalary(),employee->getId());
        employeesWithSalary->insertNode(key,employee);
    }
    Node<UDCompanyTree>* employeeNode = new Node<UDCompanyTree>(employee->getId(),employee);
    employees->Insert(employeeNode);
}

void UDCompanyTree::removeEmployee(int employeeSalary, int employeeId) {
    employeesWithSalary->removeNode(salaryIdKey(employeeSalary,employeeId));
    employees->Remove(employeeId);
}

RankTree<UDCompanyTree>* UDCompanyTree::getEmployeesWithSalary() {
    return employeesWithSalary;
}
employeeHash<UDCompanyTree>* UDCompanyTree::getEmployees() {
    return employees;
}

int UDCompanyTree::getParent() {
    return parent ? parent->companyId : -1;
}

class putEmployeesWithSalaryInArrayFunc {
    std::shared_ptr<Employee<UDCompanyTree>>* arr;
    int numEmployeesWithSalary;
public:
    explicit putEmployeesWithSalaryInArrayFunc(std::shared_ptr<Employee<UDCompanyTree>>* arr) : arr(arr), numEmployeesWithSalary(0){}
    bool operator() (RankTree<UDCompanyTree>::Node* node, bool isUsedForExitRule) {
        if (isUsedForExitRule) {
            return true;
        }
        arr[numEmployeesWithSalary++]=node->data;
        return true;
    }
};

void UDCompanyTree::mergeArrays(std::shared_ptr<Employee<UDCompanyTree>>* src1, std::shared_ptr<Employee<UDCompanyTree>>* src2, std::shared_ptr<Employee<UDCompanyTree>>* target,
                 int src1Size, int src2Size) {
    int src1Bottom=0, src2Bottom=0, i=0;
    while (src1Bottom < src1Size && src2Bottom < src2Size) {
        if (!src1[src1Bottom] || !src2[src2Bottom]) {
            if (!src1[src1Bottom]) {
                src1Bottom++;
            }
            if (!src2[src2Bottom]) {
                src2Bottom++;
            }
            continue;
        }
        if (src1[src1Bottom]->getSalary() < src2[src2Bottom]->getSalary()
        || ((src1[src1Bottom]->getSalary() == src2[src2Bottom]->getSalary()) && (src1[src1Bottom]->getId() < src2[src2Bottom]->getId()))) {
            target[i++] = src1[src1Bottom++];
        }
        else {
            target[i++] = src2[src2Bottom++];
        }
    }
    while (src1Bottom < src1Size) {
        if (!src1[src1Bottom]) {
            src1Bottom++;
            continue;
        }
        target[i++] = src1[src1Bottom++];
    }
    while (src2Bottom < src2Size) {
        if (!src2[src2Bottom]) {
            src2Bottom++;
            continue;
        }
        target[i++] = src2[src2Bottom++];
    }
}

RankTree<UDCompanyTree>::Node* UDCompanyTree::createEmptyTreeNodes(int *keysArray, RankTree<UDCompanyTree>::Node *root,
                                                                   int counter, int treeSize,
                                                                   RankTree<UDCompanyTree>::Node *parent) {
    if (counter < treeSize)
    {
        std::shared_ptr<Employee<UDCompanyTree>> currEmployee(new Employee<UDCompanyTree>(keysArray[counter],0,0,nullptr));
        RankTree<UDCompanyTree>::Node* node =new RankTree<UDCompanyTree>::Node();
        node->data=currEmployee;
        node->key=salaryIdKey(keysArray[counter],keysArray[counter]);
        root = node;
        if (parent) {
            root->parent=parent;
        }
        root->left = createEmptyTreeNodes(keysArray,root->left,2*counter + 1, treeSize, root);
        root->right = createEmptyTreeNodes(keysArray,root->right,2*counter + 2,treeSize, root);
        return root;
    }
    return nullptr;
}

void UDCompanyTree::leavesTrimmer(RankTree<UDCompanyTree> *tree, int requiredSize, RankTree<UDCompanyTree>::Node* root) {
    if ((int)tree->num_of_nodes > requiredSize && root) {
        if (!root->left && !root->right) {
            assert(root->parent);
            if (root->parent->right == root) {
                root->parent->right=nullptr;
            }
            else {
                root->parent->left=nullptr;
            }
            tree->num_of_nodes--;
            delete root;
        }
        else {
            leavesTrimmer(tree,requiredSize,root->right);
            leavesTrimmer(tree,requiredSize,root->left);
        }
    }
}

RankTree<UDCompanyTree>* UDCompanyTree::createEmptyTree(int treeSize) {
    RankTree<UDCompanyTree>* tree = new RankTree<UDCompanyTree>();
    int wholeTreeSize = pow(2,ceil(log2(treeSize)));
    int* keysArray = new int[wholeTreeSize];
    for (int i=0; i<wholeTreeSize; i++) {
        keysArray[i]=i;
    }
    RankTree<UDCompanyTree>::Node* treeRoot = nullptr;
    treeRoot = createEmptyTreeNodes(keysArray,treeRoot,0,wholeTreeSize);
    tree->setRoot(treeRoot);
    tree->num_of_nodes=wholeTreeSize;
    leavesTrimmer(tree,treeSize,tree->getRoot());
    return tree;
}

class putEmployeesWithSalaryInTreeFunc {
    std::shared_ptr<Employee<UDCompanyTree>>* allEmployeesWithSalaryArr;
    int addedSoFar;
public:
    explicit putEmployeesWithSalaryInTreeFunc(std::shared_ptr<Employee<UDCompanyTree>>* allEmployeesWithSalaryArr) : allEmployeesWithSalaryArr(allEmployeesWithSalaryArr), addedSoFar(0) {};
    bool operator() (RankTree<UDCompanyTree>::Node* node, bool isUsedForExitRule) {
        if (isUsedForExitRule) {
            return true;
        }
        if (!allEmployeesWithSalaryArr[addedSoFar]) {
            return true;
        }
        node->key=salaryIdKey(allEmployeesWithSalaryArr[addedSoFar]->getSalary(),allEmployeesWithSalaryArr[addedSoFar]->getId());
        node->data=allEmployeesWithSalaryArr[addedSoFar++];
        return true;
    }
};

class updateTreeFields {
    RankTree<UDCompanyTree>* tree;
public:
    updateTreeFields(RankTree<UDCompanyTree>* tree) : tree(tree) {}
    bool operator() (RankTree<UDCompanyTree>::Node* node, bool isUsedForExitRule) {
        if (isUsedForExitRule) {
            return true;
        }
        tree->updateHeights(node);
        tree->updateRanks(node);
        tree->updateSums(node);
        return true;
    }
};

class toRemove {
public:
    bool operator() (RankTree<UDCompanyTree>::Node* node, bool isUsedForExit) {
        if (isUsedForExit) {
            return true;
        }
        std::cout << "Employee: " << node->data->getId() << " employer: " << node->data->getEmployer()->getId() << std::endl;
        return true;
    }
};

//this function only handles employeesWithSalary with salary. employeesWithSalary without salary should be handled in the hash66666666
void UDCompanyTree::mergeWithEmployeesOf(UDCompanyTree *companyTree) {
    int thisNumEmployees = (int)employeesWithSalary->num_of_nodes, companyNumEmployees = (int)companyTree->employeesWithSalary->num_of_nodes;
    //cases of no employeesWithSalary in one of the companies
    if (companyNumEmployees < 1 && !companyTree->employeesWithSalary->getRoot()) {
        return;
    }
    if (thisNumEmployees < 1 && !employeesWithSalary->getRoot()) {
        RankTree<UDCompanyTree>* temp = employeesWithSalary;
        employeesWithSalary=companyTree->employeesWithSalary;
        companyTree->employeesWithSalary=nullptr;
        updateTreeFields fieldUpdater(employeesWithSalary);
        employeesWithSalary->doPostOrder(employeesWithSalary->getRoot(),fieldUpdater);
        delete temp;
        return;
    }
    //put employeesWithSalary of two companies in an array
    std::shared_ptr<Employee<UDCompanyTree>>* thisEmployees = new std::shared_ptr<Employee<UDCompanyTree>>[thisNumEmployees];
    std::shared_ptr<Employee<UDCompanyTree>>* companyEmployees = new std::shared_ptr<Employee<UDCompanyTree>>[companyNumEmployees];
    putEmployeesWithSalaryInArrayFunc thisArrayFunc(thisEmployees), companyArrayFunc(companyEmployees);
    employeesWithSalary->doInOrder(employeesWithSalary->getRoot(),thisArrayFunc);
    companyTree->employeesWithSalary->doInOrder(companyTree->employeesWithSalary->getRoot(),companyArrayFunc);
    //merge two arrays into one
    std::shared_ptr<Employee<UDCompanyTree>>* allEmployeesArr = new std::shared_ptr<Employee<UDCompanyTree>>[thisNumEmployees + companyNumEmployees];
    mergeArrays(thisEmployees,companyEmployees,allEmployeesArr, thisNumEmployees, companyNumEmployees);
    //create new almost whole tree with enough size
    RankTree<UDCompanyTree>* allEmployeesTree = createEmptyTree(thisNumEmployees+companyNumEmployees);
    //put all employeesWithSalary from the array in the tree
    putEmployeesWithSalaryInTreeFunc funcPutInTree(allEmployeesArr);
    allEmployeesTree->doInOrder(allEmployeesTree->getRoot(),funcPutInTree);
    //update grade sums in the tree
    updateTreeFields funcUpdateSums(allEmployeesTree);
    allEmployeesTree->doPostOrder(allEmployeesTree->getRoot(),funcUpdateSums);
    //make the new tree the actual employeesWithSalary tree and remove the other two trees
    RankTree<UDCompanyTree>* temp = employeesWithSalary;
    employeesWithSalary=allEmployeesTree;
    //merge the two hashes of all employees, and put them in this company's employees. It also sets all the employers of the old company to be the new company.
    //companyTree->employees->mergeTwoHashs(employees,this);
    delete[] thisEmployees;
    delete[] companyEmployees;
    delete[] allEmployeesArr;
    delete temp;
    delete companyTree->employeesWithSalary;
}

void UDCompanyTree::makeSon(UDCompanyTree *tree) {
    tree->parent=this;
    size+=tree->size;
    mergeWithEmployeesOf(tree);
}

int UDCompanyTree::getId() const {
    return companyId;
}

void UDCompanyTree::increasePurchaseFactor(double increaseBy) {
    purchaseFactor+=increaseBy;
}

double UDCompanyTree::getPurchaseFactor() const {
    return purchaseFactor;
}