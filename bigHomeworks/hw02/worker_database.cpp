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
#endif /* __PROGTEST__ */


struct Frequency {
    unsigned int salary;
    int num;
};


class Worker {
public:
    Worker(const string &email, const string &name, const string &surname,
           unsigned int salary) : email(email), name(name), surname(surname), salary(salary) {}

    static int emailCmp(const Worker &a, const string &b) {
        return a.email < b;
    }

    static int nameCmp(const Worker &a, const string name[2]) {
        if (a.surname < name[1]) {
            return true;
        } else if (a.surname == name[1]) {
            return a.name < name[0];
        }
        return false;
    }

    static int salaryCmp(const Frequency &a, unsigned int b) {
        return a.salary < b;
    }

    bool isEmail(const string &searchedEmail) const {
        return searchedEmail == email;
    }

    bool isName(const string &searchedName, const string &searchedSurname) const {
        return name == searchedName && surname == searchedSurname;
    }

    string getName() const {
        return name;
    }

    string getSurname() const {
        return surname;
    }

    string getEmail() const {
        return email;
    }

    unsigned int getSalary() const {
        return salary;
    }

    void setName(const string &newName) {
        name = newName;
    }

    void setSurname(const string &newSurname) {
        surname = newSurname;
    }

    void setEmail(const string &newEmail) {
        email = newEmail;
    }

    void setSalary(unsigned int newSalary) {
        salary = newSalary;
    }

    void print() {
        cout << email << " | " << name << " " << surname << " | " << salary << endl;
    }

private:
    string email;
    string name;
    string surname;
    unsigned int salary;
};


class CPersonalAgenda {
private:
    // todo
    vector<Worker> workerByEmail;
    vector<Worker> workerByName;
    vector<Frequency> salaryFrequency;

    auto findEmailConst(const string &email) const {
        return lower_bound(workerByEmail.begin(), workerByEmail.end(), email, Worker::emailCmp);
    }

    auto findEmail(const string &email) {
        return lower_bound(workerByEmail.begin(), workerByEmail.end(), email, Worker::emailCmp);
    }

    auto findNameConst(const string &name, const string &surname) const {
        const string names[2] = {name, surname};
        return lower_bound(workerByName.begin(), workerByName.end(), names, Worker::nameCmp);
    }

    auto findName(const string &name, const string &surname) {
        const string names[2] = {name, surname};
        return lower_bound(workerByName.begin(), workerByName.end(), names, Worker::nameCmp);
    }

    auto findSalaryConst(unsigned int salary) const {
        return lower_bound(salaryFrequency.begin(), salaryFrequency.end(), salary, Worker::salaryCmp);
    }

    auto findSalary(unsigned int salary) {
        return lower_bound(salaryFrequency.begin(), salaryFrequency.end(), salary, Worker::salaryCmp);
    }

    template<typename input_iterator>
    bool emailExists(input_iterator foundEmail, const string &email) const {
        // email already exists
        if (foundEmail != workerByEmail.end() && foundEmail->isEmail(email)) {
            return true;
        }

        return false;
    }

    template<typename input_iterator>
    bool nameExists(input_iterator foundName, const string &name, const string &surname) const {
        // email already exists
        if (foundName != workerByName.end() && foundName->isName(name, surname)) {
            return true;
        }

        return false;
    }

    template<typename input_iterator>
    bool salaryExists(input_iterator foundSalary, unsigned int salary) const {
        // email already exists
        if (foundSalary != salaryFrequency.end() && foundSalary->salary == salary) {
            return true;
        }

        return false;
    }

    void addSalary(unsigned int salary) {
        auto salaryIter = findSalary(salary);
        if (salaryExists(salaryIter, salary)) {
            salaryIter->num++;
        } else {
            struct Frequency temp = {salary, 1};
            salaryFrequency.insert(salaryIter, temp);
        }
    }

    void removeSalary(unsigned int salary) {
        auto salaryIter = findSalary(salary);
        if (salaryExists(salaryIter, salary)) {
            salaryIter->num--;
            if (salaryIter->num <= 0) {
                salaryFrequency.erase(salaryIter);
            }
        }
    }

    bool addEmail(const string &name, const string &surname, const string &email,
                  unsigned int salary) {
        Worker newWorker(email, name, surname, salary);

        auto emailIter = findEmailConst(email);
        if (emailExists(emailIter, email)) {
            return false;
        }
        workerByEmail.insert(emailIter, newWorker);

        return true;
    }

    bool addName(const string &name, const string &surname, const string &email,
                 unsigned int salary) {
        Worker newWorker(email, name, surname, salary);

        auto nameIter = findNameConst(name, surname);
        if (nameExists(nameIter, name, surname)) {
            return false;
        }
//        cout << "Name iter: " << nameIter - workerByName.begin() << endl;
        workerByName.insert(nameIter, newWorker);

        return true;
    }

    bool delEmail(const string &email) {
        auto emailIter = findEmailConst(email);
        if (!emailExists(emailIter, email)) {
            return false;
        }

        workerByEmail.erase(emailIter);

        return true;
    }

    bool delName(const string &name, const string &surname) {
        auto nameIter = findNameConst(name, surname);
        if (!nameExists(nameIter, name, surname)) {
            return false;
        }
        workerByName.erase(nameIter);

        return true;
    }

public:
    CPersonalAgenda(void) {};

    ~CPersonalAgenda(void) {};

    bool add(const string &name, const string &surname, const string &email,
             unsigned int salary) {

        Worker newWorker(email, name, surname, salary);

        // email already exists
        auto emailIter = findEmail(email);
        if (emailExists(emailIter, email)) {
            return false;
        }

        // name already exists
        auto nameIter = findName(name, surname);
        if (nameExists(nameIter, name, surname)) {
            return false;
        }

        workerByEmail.insert(emailIter, newWorker);
        workerByName.insert(nameIter, newWorker);

        addSalary(salary);

        return true;
    };

    bool del(const string &name, const string &surname) {

        auto nameIter = findName(name, surname);
        if (!nameExists(nameIter, name, surname)) {
            return false;
        }
        auto emailIter = findEmail(nameIter->getEmail());

        removeSalary(nameIter->getSalary());
        workerByName.erase(nameIter);
        workerByEmail.erase(emailIter);

        return true;
    };

    bool del(const string &email) {
        auto emailIter = findEmailConst(email);
        if (!emailExists(emailIter, email)) {
            return false;
        }
        auto nameIter = findNameConst(emailIter->getName(), emailIter->getSurname());

        removeSalary(emailIter->getSalary());
        workerByName.erase(nameIter);
        workerByEmail.erase(emailIter);

        return true;
    };

    bool changeName(const string &email, const string &newName, const string &newSurname) {
        auto emailIter = findEmail(email);
        auto nameIter = findNameConst(newName, newSurname);

        if (!emailExists(emailIter, email) || nameExists(nameIter, newName, newSurname)) {
            return false;
        }

        delName(emailIter->getName(), emailIter->getSurname());
        addName(newName, newSurname, email, emailIter->getSalary());

        emailIter->setName(newName);
        emailIter->setSurname(newSurname);

        return true;
    };

    bool changeEmail(const string &name, const string &surname, const string &newEmail) {
        auto nameIter = findName(name, surname);
        auto emailIter = findEmail(newEmail);

        if (!nameExists(nameIter, name, surname) || emailExists(emailIter, newEmail)) {
            return false;
        }

        delEmail(nameIter->getEmail());
        addEmail(name, surname, newEmail, nameIter->getSalary());

        nameIter->setEmail(newEmail);

        return true;
    };

    bool setSalary(const string &name,
                   const string &surname,
                   unsigned int salary) {

        auto nameIter = findName(name, surname);
        if (!nameExists(nameIter, name, surname)) {
            return false;
        }
        auto emailIter = findEmail(nameIter->getEmail());

        // binary search old salary
        // if num below 1, erase salary else decrease
        // lower bound search new salary
        // if found, increase num, if not, insert
        if (salary != nameIter->getSalary()) {
            removeSalary(nameIter->getSalary());
            addSalary(salary);
        }

        nameIter->setSalary(salary);
        emailIter->setSalary(salary);

        return true;
    };

    bool setSalary(const string &email,
                   unsigned int salary) {

        auto emailIter = findEmail(email);
        if (!emailExists(emailIter, email)) {
            return false;
        }
        auto nameIter = findName(emailIter->getName(), emailIter->getSurname());

        if (salary != emailIter->getSalary()) {
            removeSalary(emailIter->getSalary());
            addSalary(salary);
        }

        nameIter->setSalary(salary);
        emailIter->setSalary(salary);

        return true;
    };

    unsigned int getSalary(const string &name,
                           const string &surname) const {

        auto nameIter = findNameConst(name, surname);

        if (!nameExists(nameIter, name, surname)) {
            return 0;
        }
        return nameIter->getSalary();

    };

    unsigned int getSalary(const string &email) const {

        auto emailIter = findEmailConst(email);

        if (!emailExists(emailIter, email)) {
            return 0;
        }
        return emailIter->getSalary();

    };

    bool            getRank(const string &name,
                 const string &surname,
                 int &rankMin,
                 int &rankMax) const {

        auto nameIter = findNameConst(name, surname);
        if (!nameExists(nameIter, name, surname)) {
            return false;
        }

//        auto salaryIter = findSalaryConst(nameIter->getSalary());
        long salariesSoFar = 0;
        size_t correctIndex = 0;
        unsigned int salary = nameIter->getSalary();
        for (size_t i = 0; i < salaryFrequency.size(); i++) {
            if (salaryFrequency[i].salary < salary) {
                salariesSoFar += salaryFrequency[i].num;
            } else {
                correctIndex = i;
                break;
            }
        }

        rankMin = salariesSoFar;
        rankMax = rankMin + salaryFrequency[correctIndex].num - 1;

        return true;
    };

    bool getRank(const string &email,
                 int &rankMin,
                 int &rankMax) const {

        auto emailIter = lower_bound(workerByEmail.begin(), workerByEmail.end(), email, Worker::emailCmp);
        if (!emailExists(emailIter, email)) {
            return false;
        }

        long salariesSoFar = 0;
        size_t correctIndex = 0;
        unsigned int salary = emailIter->getSalary();
        for (size_t i = 0; i < salaryFrequency.size(); i++) {
            if (salaryFrequency[i].salary < salary) {
                salariesSoFar += salaryFrequency[i].num;
            } else {
                correctIndex = i;
                break;
            }
        }

        rankMin = salariesSoFar;
        rankMax = rankMin + salaryFrequency[correctIndex].num - 1;
//
//        auto salaryIter = findSalaryConst(emailIter->getSalary());
//
//        rankMin = salaryIter - salaryFrequency.begin();
//        rankMax = rankMin + salaryIter->num - 1;

        return true;
    };

    bool getFirst(string &outName,
                  string &outSurname) const {
        if (workerByName.empty()) {
            return false;
        }

        outName = workerByName[0].getName();
        outSurname = workerByName[0].getSurname();

        return true;
    };

    bool getNext(const string &name,
                 const string &surname,
                 string &outName,
                 string &outSurname) const {

        auto nameIter = findNameConst(name, surname);

        if (!nameExists(nameIter, name, surname)) {
            return false;
        }
        size_t nameIndex = nameIter - workerByName.begin();
        if (nameIndex + 1 >= workerByName.size()) {
            return false;
        }
        outName = workerByName[nameIndex + 1].getName();
        outSurname = workerByName[nameIndex + 1].getSurname();

        return true;
    };

    void printAllWorkers() {
        cout << "-------------------" << endl;
        cout << "Workers by email:" << endl;
        for (size_t i = 0; i < workerByEmail.size(); i++) {
            workerByEmail[i].print();
        }

        cout << "-------------------" << endl;
        cout << "Workers by name:" << endl;
        for (size_t i = 0; i < workerByName.size(); i++) {
            workerByName[i].print();
        }

        cout << "-------------------" << endl;
        cout << "Salary Frequency:" << endl;
        for (size_t i = 0; i < salaryFrequency.size(); i++) {
            cout << salaryFrequency[i].salary << " : " << salaryFrequency[i].num << endl;
        }
        cout << "-------------------\n" << endl;
    }

};


#ifndef __PROGTEST__

int main(void) {
    string outName, outSurname;
    int lo, hi;

    CPersonalAgenda b1;
    assert (b1.add("John", "Smith", "john", 30000));
    assert (b1.add("John", "Miller", "johnm", 35000));
    assert (b1.add("Peter", "Smith", "peter", 23000));
    assert (b1.getFirst(outName, outSurname)
            && outName == "John"
            && outSurname == "Miller");
    assert (b1.getNext("John", "Miller", outName, outSurname)
            && outName == "John"
            && outSurname == "Smith");
    assert (b1.getNext("John", "Smith", outName, outSurname)
            && outName == "Peter"
            && outSurname == "Smith");
    assert (!b1.getNext("Peter", "Smith", outName, outSurname));
    assert (b1.setSalary("john", 32000));
    assert (b1.getSalary("john") == 32000);
    assert (b1.getSalary("John", "Smith") == 32000);
    assert (b1.getRank("John", "Smith", lo, hi)
            && lo == 1
            && hi == 1);
    assert (b1.getRank("john", lo, hi)
            && lo == 1
            && hi == 1);
    assert (b1.getRank("peter", lo, hi)
            && lo == 0
            && hi == 0);
    assert (b1.getRank("johnm", lo, hi)
            && lo == 2
            && hi == 2);
    assert (b1.setSalary("John", "Smith", 35000));
    assert (b1.getSalary("John", "Smith") == 35000);
    assert (b1.getSalary("john") == 35000);
    assert (b1.getRank("John", "Smith", lo, hi)
            && lo == 1
            && hi == 2);
    assert (b1.getRank("john", lo, hi)
            && lo == 1
            && hi == 2);
    assert (b1.getRank("peter", lo, hi)
            && lo == 0
            && hi == 0);
    assert (b1.getRank("johnm", lo, hi)
            && lo == 1
            && hi == 2);
    assert (b1.changeName("peter", "James", "Bond"));
    assert (b1.getSalary("peter") == 23000);
//    b1.printAllWorkers();
//    cout << "salary is: " << b1.getSalary("James", "Bond") << endl;
    assert (b1.getSalary("James", "Bond") == 23000);
    assert (b1.getSalary("Peter", "Smith") == 0);
    assert (b1.getFirst(outName, outSurname)
            && outName == "James"
            && outSurname == "Bond");
    assert (b1.getNext("James", "Bond", outName, outSurname)
            && outName == "John"
            && outSurname == "Miller");
    assert (b1.getNext("John", "Miller", outName, outSurname)
            && outName == "John"
            && outSurname == "Smith");
    assert (!b1.getNext("John", "Smith", outName, outSurname));
    assert (b1.changeEmail("James", "Bond", "james"));
    assert (b1.getSalary("James", "Bond") == 23000);
    assert (b1.getSalary("james") == 23000);
    assert (b1.getSalary("peter") == 0);
    assert (b1.del("james"));
    assert (b1.getRank("john", lo, hi)
            && lo == 0
            && hi == 1);
    assert (b1.del("John", "Miller"));
    assert (b1.getRank("john", lo, hi)
            && lo == 0
            && hi == 0);
    assert (b1.getFirst(outName, outSurname)
            && outName == "John"
            && outSurname == "Smith");
    assert (!b1.getNext("John", "Smith", outName, outSurname));
    assert (b1.del("john"));
    assert (!b1.getFirst(outName, outSurname));
    assert (b1.add("John", "Smith", "john", 31000));
    assert (b1.add("john", "Smith", "joHn", 31000));
    assert (b1.add("John", "smith", "jOhn", 31000));

    CPersonalAgenda b2;
    assert (!b2.getFirst(outName, outSurname));
    assert (b2.add("James", "Bond", "james", 70000));
    assert (b2.add("James", "Smith", "james2", 30000));
    assert (b2.add("Peter", "Smith", "peter", 40000));
    assert (!b2.add("James", "Bond", "james3", 60000));
    assert (!b2.add("Peter", "Bond", "peter", 50000));
    assert (!b2.changeName("joe", "Joe", "Black"));
    assert (!b2.changeEmail("Joe", "Black", "joe"));
    assert (!b2.setSalary("Joe", "Black", 90000));
    assert (!b2.setSalary("joe", 90000));
    assert (b2.getSalary("Joe", "Black") == 0);
    assert (b2.getSalary("joe") == 0);
    assert (!b2.getRank("Joe", "Black", lo, hi));
    assert (!b2.getRank("joe", lo, hi));
    assert (!b2.changeName("joe", "Joe", "Black"));
    assert (!b2.changeEmail("Joe", "Black", "joe"));
    assert (!b2.del("Joe", "Black"));
    assert (!b2.del("joe"));
    assert (!b2.changeName("james2", "James", "Bond"));
    assert (!b2.changeEmail("Peter", "Smith", "james"));
    assert (!b2.changeName("peter", "Peter", "Smith"));
    assert (!b2.changeEmail("Peter", "Smith", "peter"));
    assert (b2.del("Peter", "Smith"));
    assert (!b2.changeEmail("Peter", "Smith", "peter2"));
    assert (!b2.setSalary("Peter", "Smith", 35000));
    assert (b2.getSalary("Peter", "Smith") == 0);
    assert (!b2.getRank("Peter", "Smith", lo, hi));
    assert (!b2.changeName("peter", "Peter", "Falcon"));
    assert (!b2.setSalary("peter", 37000));
    assert (b2.getSalary("peter") == 0);
    assert (!b2.getRank("peter", lo, hi));
    assert (!b2.del("Peter", "Smith"));
    assert (!b2.del("peter"));
    assert (b2.add("Peter", "Smith", "peter", 40000));
    assert (b2.getSalary("peter") == 40000);

    return EXIT_SUCCESS;

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
