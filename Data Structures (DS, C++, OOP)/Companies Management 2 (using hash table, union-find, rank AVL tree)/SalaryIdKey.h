//
// Created by illus on 30/04/2022.
//

#ifndef WET_1_SALARYIDKEY_H
#define WET_1_SALARYIDKEY_H

class salaryIdKey {
    long long salary;
    int id;
public:
    salaryIdKey(long long salary, int id) : salary(salary), id(id) {}
    salaryIdKey()=default;
    friend bool operator<(const salaryIdKey& key1, const salaryIdKey& key2) {
        if (key2.salary > key1.salary || (key2.salary==key1.salary && key2.id > key1.id))
        {
            return true;
        }
        return false;
    }
    friend bool operator==(const salaryIdKey& key1, const salaryIdKey& key2) {
        return key1.salary == key2.salary && key1.id == key2.id;
    }

    friend bool operator>(const salaryIdKey& key1, const salaryIdKey& key2) {
        return !(key1<key2) && !(key1==key2);
    }

    friend bool operator>=(const salaryIdKey& key1, const salaryIdKey& key2) {
        return !(key1<key2);
    }

    friend bool operator<=(const salaryIdKey& key1, const salaryIdKey& key2) {
        return key1==key2 || key1 < key2;
    }

    long long getSalary() const {
        return salary;
    }

    int getId() const {
        return id;
    }
};


#endif //WET_1_SALARYIDKEY_H
