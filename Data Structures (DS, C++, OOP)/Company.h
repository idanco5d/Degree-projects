#ifndef COMPANY_H
#define COMPANY_H

#include <iostream>
#include <memory>
#include <string>

#include "Node.h"
#include "AVLTree.h"
#include "Employee.h"
#include "library1.h"

namespace DS
{

    template <class Key1, class Key2>
    struct Key {
        Key1 primary_key;
        Key2 secondary_key;

        Key(const Key1 &key1, const Key2 &key2) : primary_key(key1), secondary_key(key2) {}
        Key(const Key &other) = default;
        ~Key() = default;

        Key &operator=(const Key &key) = default;

        friend bool operator<(const Key &curr, const Key &other)
        {
            if( curr.primary_key < other.primary_key) {
                return true;
            } else if( curr.primary_key == other.primary_key && curr.secondary_key > other.secondary_key) {
                return true;
            }
            return false;

        }
        friend bool operator>(const Key &curr, const Key &other)
        {
            if( curr->primary_key > other.secondary_key) {
                return true;
            } else if( curr.primary_key == other.primary_key && curr.secondary_key < other.secondary_key) {
                return true;
            }
            return false;
        }
        friend bool operator==(const Key &curr, const Key &other)
        {
            return curr.primary_key == other.primary_key && curr.secondary_key == other.secondary_key;
        }
        friend bool operator<=(const Key &curr, const Key &other)
        {
            return !(curr > other);
        }
        friend bool operator>=(const Key &curr, const Key &other)
        {
            return !(curr < other);
        }

        friend std::ostream &operator<<(std::ostream &os, const Key<Key1, Key2> &key)
        {

            os << "(" << key.primary_key << ", " << key.secondary_key << ")";

            return os;
        }
    };

    class Company //: public DS::AbstractCompany<Employee<Company>>
    {
        typedef std::shared_ptr<Employee<Company>> Employee_ptr;

    private:
        int id;
        int value;
        int num_of_employees;
        Employee_ptr bestEarner;

        AvlTree<Key<int, int>, Employee_ptr>* employees_by_id;
        AvlTree<Key<int, int>, Employee_ptr>* employees_by_salary;

    public:
        Company(int id, int value);
        ~Company();
        int getId() const;
        int getNumOfEmployees() const;
        int getValue() const;
        Employee_ptr getBestEarner();
        AvlTree<Key<int, int>, Employee_ptr>* getEmployeesById () const;
        AvlTree<Key<int, int>, Employee_ptr>* getEmployeesBySalary () const;
        StatusType addEmployee(const int &employees_id, const int &salary, const int &grade, const Employee_ptr& existing_employee);
        void setBestEarner(Employee_ptr employee);
        StatusType removeEmployee(const int employees_id, const int salary);
        void updateBestEarner();
        void SetCompanyValue(int valueToSet);
        void removeAllEmployees();
        void setNumEmployees(int newNumEmployees);
        void setEmployeesById (AvlTree<Key<int, int>, Employee_ptr>* newTree);
        void setEmployeesBySalary (AvlTree<Key<int, int>, Employee_ptr>* newTree);

        friend bool operator<(const Company &w1, const Company &w2)
        {
            return w1.getId() < w2.getId();
        }

    };
}

#endif // HWWET1_COMPANY_H
