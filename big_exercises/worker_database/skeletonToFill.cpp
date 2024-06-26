#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>

using namespace std;

class CPersonalAgenda {
public:
    CPersonalAgenda(void);

    bool add(const string &name,
             const string &surname,
             const string &email,
             unsigned int salary);

    bool del(const string &name,
             const string &surname);

    bool del(const string &email);

    bool changeName(const string &email,
                    const string &newName,
                    const string &newSurname);

    bool changeEmail(const string &name,
                     const string &surname,
                     const string &newEmail);

    bool setSalary(const string &name,
                   const string &surname,
                   unsigned int salary);

    bool setSalary(const string &email,
                   unsigned int salary);

    unsigned int getSalary(const string &name,
                           const string &surname) const;

    unsigned int getSalary(const string &email) const;

    bool getRank(const string &name,
                 const string &surname,
                 int &rankMin,
                 int &rankMax) const;

    bool getRank(const string &email,
                 int &rankMin,
                 int &rankMax) const;

    bool getFirst(string &outName,
                  string &outSurname) const;

    bool getNext(const string &name,
                 const string &surname,
                 string &outName,
                 string &outSurname) const;

private:
    // todo
};

#ifndef __PROGTEST__

int main(void) {
    string outName, outSurname;
    int lo, hi;

    CPersonalAgenda b1;
    assert(b1.add("John", "Smith", "john", 30000));
    assert(b1.add("John", "Miller", "johnm", 35000));
    assert(b1.add("Peter", "Smith", "peter", 23000));
    assert(b1.getFirst(outName, outSurname)
           && outName == "John"
           && outSurname == "Miller");
    assert(b1.getNext("John", "Miller", outName, outSurname)
           && outName == "John"
           && outSurname == "Smith");
    assert(b1.getNext("John", "Smith", outName, outSurname)
           && outName == "Peter"
           && outSurname == "Smith");
    assert(!b1.getNext("Peter", "Smith", outName, outSurname));
    assert(b1.setSalary("john", 32000));
    assert(b1.getSalary("john") == 32000);
    assert(b1.getSalary("John", "Smith") == 32000);
    assert(b1.getRank("John", "Smith", lo, hi)
           && lo == 1
           && hi == 1);
    assert(b1.getRank("john", lo, hi)
           && lo == 1
           && hi == 1);
    assert(b1.getRank("peter", lo, hi)
           && lo == 0
           && hi == 0);
    assert(b1.getRank("johnm", lo, hi)
           && lo == 2
           && hi == 2);
    assert(b1.setSalary("John", "Smith", 35000));
    assert(b1.getSalary("John", "Smith") == 35000);
    assert(b1.getSalary("john") == 35000);
    assert(b1.getRank("John", "Smith", lo, hi)
           && lo == 1
           && hi == 2);
    assert(b1.getRank("john", lo, hi)
           && lo == 1
           && hi == 2);
    assert(b1.getRank("peter", lo, hi)
           && lo == 0
           && hi == 0);
    assert(b1.getRank("johnm", lo, hi)
           && lo == 1
           && hi == 2);
    assert(b1.changeName("peter", "James", "Bond"));
    assert(b1.getSalary("peter") == 23000);
    assert(b1.getSalary("James", "Bond") == 23000);
    assert(b1.getSalary("Peter", "Smith") == 0);
    assert(b1.getFirst(outName, outSurname)
           && outName == "James"
           && outSurname == "Bond");
    assert(b1.getNext("James", "Bond", outName, outSurname)
           && outName == "John"
           && outSurname == "Miller");
    assert(b1.getNext("John", "Miller", outName, outSurname)
           && outName == "John"
           && outSurname == "Smith");
    assert(!b1.getNext("John", "Smith", outName, outSurname));
    assert(b1.changeEmail("James", "Bond", "james"));
    assert(b1.getSalary("James", "Bond") == 23000);
    assert(b1.getSalary("james") == 23000);
    assert(b1.getSalary("peter") == 0);
    assert(b1.del("james"));
    assert(b1.getRank("john", lo, hi)
           && lo == 0
           && hi == 1);
    assert(b1.del("John", "Miller"));
    assert(b1.getRank("john", lo, hi)
           && lo == 0
           && hi == 0);
    assert(b1.getFirst(outName, outSurname)
           && outName == "John"
           && outSurname == "Smith");
    assert(!b1.getNext("John", "Smith", outName, outSurname));
    assert(b1.del("john"));
    assert(!b1.getFirst(outName, outSurname));
    assert(b1.add("John", "Smith", "john", 31000));
    assert(b1.add("john", "Smith", "joHn", 31000));
    assert(b1.add("John", "smith", "jOhn", 31000));

    CPersonalAgenda b2;
    assert(!b2.getFirst(outName, outSurname));
    assert(b2.add("James", "Bond", "james", 70000));
    assert(b2.add("James", "Smith", "james2", 30000));
    assert(b2.add("Peter", "Smith", "peter", 40000));
    assert(!b2.add("James", "Bond", "james3", 60000));
    assert(!b2.add("Peter", "Bond", "peter", 50000));
    assert(!b2.changeName("joe", "Joe", "Black"));
    assert(!b2.changeEmail("Joe", "Black", "joe"));
    assert(!b2.setSalary("Joe", "Black", 90000));
    assert(!b2.setSalary("joe", 90000));
    assert(b2.getSalary("Joe", "Black") == 0);
    assert(b2.getSalary("joe") == 0);
    assert(!b2.getRank("Joe", "Black", lo, hi));
    assert(!b2.getRank("joe", lo, hi));
    assert(!b2.changeName("joe", "Joe", "Black"));
    assert(!b2.changeEmail("Joe", "Black", "joe"));
    assert(!b2.del("Joe", "Black"));
    assert(!b2.del("joe"));
    assert(!b2.changeName("james2", "James", "Bond"));
    assert(!b2.changeEmail("Peter", "Smith", "james"));
    assert(!b2.changeName("peter", "Peter", "Smith"));
    assert(!b2.changeEmail("Peter", "Smith", "peter"));
    assert(b2.del("Peter", "Smith"));
    assert(!b2.changeEmail("Peter", "Smith", "peter2"));
    assert(!b2.setSalary("Peter", "Smith", 35000));
    assert(b2.getSalary("Peter", "Smith") == 0);
    assert(!b2.getRank("Peter", "Smith", lo, hi));
    assert(!b2.changeName("peter", "Peter", "Falcon"));
    assert(!b2.setSalary("peter", 37000));
    assert(b2.getSalary("peter") == 0);
    assert(!b2.getRank("peter", lo, hi));
    assert(!b2.del("Peter", "Smith"));
    assert(!b2.del("peter"));
    assert(b2.add("Peter", "Smith", "peter", 40000));
    assert(b2.getSalary("peter") == 40000);

    return EXIT_SUCCESS;
}
