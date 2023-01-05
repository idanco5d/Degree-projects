#ifndef HASH_H
#define HASH_H

#include<iostream>
#include<math.h>
#include "Employee.h"

using namespace DS;

template <class Company>
struct Node {
    int key;         // holds the key
    std::shared_ptr<Employee<Company>> data;       // holds the data
    Node *next;
    Node(const int &key, std::shared_ptr<Employee<Company>> data) : key(key), data(data), next(nullptr) {};
    // copy constructor - default
    //Node(const Node &node) : key(nullptr), data(nullptr), next(nullptr) {};
    Node() = default;
    // destructor
//    ~Node() {
//        delete next;
//        //delete data;
//    }
//    ~Node() {
//        Node* nextPtr = next;
//        while (nextPtr) {
//            Node* tmp = nextPtr;
//            nextPtr = nextPtr->next;
//            delete tmp;
//        }
//    }

//    void deleteAllChildren (Node* node) {
//        if (node->next) {
//            deleteAllChildren(node->next);
//        }
//        delete node;
//    }

    std::shared_ptr<Employee<Company>> getData()
    {
        return this->data;
    }
    Node* getNext()
    {
        return this->next;
    }

    int getKey()
    {
        return this->key;
    }

    void setValue(std::shared_ptr<Employee<Company>> data)
    {
        this->data = data;
    }

    void setNext(Node *next) {

        this->next = next;
    }
};

template <class Company>
class employeeHash{
    long long elements_in_array;
    long long table_size;
    Node<Company> **table;
    long long numEmployeesNoSalary;
    long long gradesSumOfNoSalary;

    void resize_table() {
        long long oldTableSize = table_size;
        table_size *= 2;

        Node<Company> **old_table = table;
        table = new Node<Company>*[table_size];
        for (long long i = 0; i < table_size; i++)
            table[i] = nullptr;
        elements_in_array = 0;
        numEmployeesNoSalary = 0;
        gradesSumOfNoSalary = 0;
        for (long long hash = 0; hash < oldTableSize; hash++){
            if (old_table[hash] != nullptr) {
                Node<Company> *entry = old_table[hash];
                while (entry != nullptr) {
                    Node<Company>* entryCopy = new Node<Company> (entry->key, entry->data);
                    Node<Company>* temp = entry;
                    Insert(entryCopy);
                    entry = entry->getNext();
                    delete temp;
                }

            }
        }
//        for (int hash = 0; hash < oldTableSize; hash++) {
//            if (old_table[hash] != nullptr) {
//
//            }
//        }
        delete[] old_table;
    }

public:
    employeeHash() {
        table_size=11;
        gradesSumOfNoSalary=0;
        numEmployeesNoSalary=0;
        table = new Node<Company>*[table_size];
        for (int i = 0; i < table_size; i++)
            table[i] = nullptr;
        elements_in_array = 0;
    }
    long long HashFunc(long long key) {
        return key % table_size;
    }

    void updateGradesSum (long long updateBy) {
        gradesSumOfNoSalary+=updateBy;
    }

    void updateNumNoSalary (long long updateBy) {
        numEmployeesNoSalary+=updateBy;
    }

    long long getGradesSum () const {
        return gradesSumOfNoSalary;
    }

    long long getNumNoSalary() const {
        return numEmployeesNoSalary;
    }

    //to delete!! just for testing
    bool isMatchingSumAndRank () const {
        int sum = 0, rank = 0;
        for (int hash = 0; hash < table_size; hash++) {
            if (table[hash]) {
                Node<Company>* current = table[hash];
                while (current) {
                    if (current->data->getSalary() < 1) {
                        sum+=current->data->getGrade();
                        rank++;
                    }
                    current=current->getNext();
                }
            }
        }
        return sum == gradesSumOfNoSalary && rank == numEmployeesNoSalary;
    }

    bool isThereEmployerWithSuchId(int companyID) const {
        for (int hash = 0; hash < table_size; hash++) {
            if (table[hash]) {
                Node<Company>* current = table[hash];
                while (current) {
                    if (current->data->getEmployer()->getId() == companyID) {
                        return true;
                    }
                    current=current->getNext();
                }
            }
        }
        return false;
    }
    //not to delete

    void Insert(Node<Company> *node) {
        long long h_key = HashFunc(node->getKey());
        if (node->data->getSalary() < 1) {
            updateGradesSum(node->data->getGrade());
            numEmployeesNoSalary++;
        }
        Node<Company> *new_node = table[h_key];
        node->next = new_node;
        table[h_key] = node;

        elements_in_array++;
        if(elements_in_array > 0.8*table_size) {
            resize_table();
        }
    }

    long long SearchKey(long long key) {
        long long hashed = HashFunc(key);
        Node<Company> *tmp = table[hashed];
        while (tmp != nullptr) {
            if(tmp->getKey() == key){
                return tmp->getKey();
            }
            tmp = tmp->getNext();
        }

        return -1;
    }

    Node<Company>* getNode(long long key) {
        long long hashed = HashFunc(key);
        Node<Company> *tmp = table[hashed];
        while (tmp != nullptr) {
            if(tmp->getKey() == key){
                return tmp;
            }
            tmp = tmp->getNext();
        }
        return tmp;
    }


    void Remove(long long key) {
        long long hashed = HashFunc(key);
        if(SearchKey(key) == -1){
            return;
        }

        Node<Company> *tmp = table[hashed];
        Node<Company> *last = nullptr;
        while (tmp != nullptr) {
            if(tmp->getKey() == key){
                if (last) {
                    last->next = tmp->next;
                }
                else {
                    table[hashed] = tmp->next;
                }
                if (tmp->data->getSalary() < 1) {
                    updateGradesSum(-tmp->data->getGrade());
                    numEmployeesNoSalary--;
                }
                delete tmp;
                elements_in_array--;
                return;
            }
            last = tmp;
            tmp = tmp->getNext();
        }
        elements_in_array--;
    }

    void printTable() {
        std::cout << "Sum grades with no salary: " << gradesSumOfNoSalary << " amount no salary: " << numEmployeesNoSalary << std::endl;
        for (int i = 0; i < table_size; i++) {
            if(table[i] == nullptr) {
                std::cout << i << "-------" << std::endl;
            } else{
                std::cout << i;
                Node<Company> *tmp = table[i];
                while(tmp != nullptr) {
                    std::cout << "[Employees Id- " << tmp->getKey() << ", employees data- " << *tmp->getData() << "], " << std::endl;
                    tmp = tmp->getNext();
                }

            }
        }
    }

    void mergeTwoHashs(employeeHash* other, Company* newCompany){
        for (long long hash = 0; hash < table_size; hash++) {
            if (table[hash] != nullptr) {
                Node<Company> *entry = table[hash];
                while (entry != nullptr) {
                    Node<Company>* entryCopy = new Node<Company> (entry->key,entry->data);
                    entryCopy->data->setCompany(newCompany);
                    other->Insert(entryCopy);
                    entry = entry->getNext();
                }
            }
        }
        delete this;
        //delete[] table;
    }

    ~employeeHash() {
        for (long long hash = 0; hash < table_size; hash++) {
            if (table[hash] != nullptr) {
                Node<Company> *prev_entry = nullptr;
                Node<Company> *entry = table[hash];
                //entry = entry->getNext();
                while (entry != nullptr) {
                    prev_entry = entry;
                    entry = entry->getNext();
                    delete prev_entry;
                }
            }
        }
        delete[] table;
    }
};

#endif