#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>

using namespace std;


class CTimeStamp {
public:
    CTimeStamp(long year, long month, long day, long hour, long minute, long second) {
        datetime[0] = year;
        datetime[1] = month;
        datetime[2] = day;
        datetime[3] = hour;
        datetime[4] = minute;
        datetime[5] = second;
    }

    bool isBetween(const CTimeStamp &start, const CTimeStamp &end) {
        // find if its before
        for (int i = 0; i < 6; i++) {
//                if (start.datetime[i] > end.datetime[i]) {
//                    return false;
//                }
            if (start.datetime[i] > datetime[i]) {
                return false;
            }
            if (start.datetime[i] < datetime[i]) {
                break;
            }
        }

        for (int i = 0; i < 6; i++) {
//                if (start.datetime[i] > end.datetime[i]) {
//                    return false;
//                }
            if (end.datetime[i] < datetime[i]) {
                return false;
            }
            if (end.datetime[i] > datetime[i]) {
                break;
            }
        }

        return true;
    };

private:

    long datetime[6] = {0};
};

class CContact {
public:
    CContact(const CTimeStamp &cTimeS, int contact1, int contact2) :
            cTimeStamp(cTimeS), contactNum1(contact1), contactNum2(contact2) {}

    int getContactNum1() const {
        return contactNum1;
    }

    int getContactNum2() const {
        return contactNum2;
    }

    CTimeStamp getTimestamp() const {
        return cTimeStamp;
    }

private:
    CTimeStamp cTimeStamp;
    int contactNum1;
    int contactNum2;
};

class CEFaceMask {
public:

    CEFaceMask &addContact(const CContact &contact) {
//            CEFaceMask c;
        if (contact.getContactNum1() != contact.getContactNum2()) {
            contacts.push_back(contact);
        }
        return *this;
    }

    void addToRes(int contactNum, vector<int> &res, const CContact &contact) const {
        int contactNum2;
        if (contactNum == contact.getContactNum1()) {
            contactNum2 = contact.getContactNum2();
        } else if (contactNum == contact.getContactNum2()) {
            contactNum2 = contact.getContactNum1();
        } else {
            return;
        }
        // we havent found the value
        for (size_t i = 0; i < res.size(); i++) {
            if (res[i] == contactNum2)
                return;
        }
        res.push_back(contactNum2);

//            if (find(res.begin(), res.end(), contactNum2) == res.end() && *res.end() != contactNum2 && contactNum != contactNum2) {
//                res.push_back(contactNum2);
//            }
    }

    vector<int> listContacts(int contactNum) const {
        vector<int> res = {};
        for (size_t i = 0; i < contacts.size(); i++) {
            // we havent found the value
            CEFaceMask::addToRes(contactNum, res, contacts[i]);
        }

        return res;
    }

    vector<int> listContacts(int contactNum, CTimeStamp start, CTimeStamp end) const {
        vector<int> res;
        for (size_t i = 0; i < contacts.size(); i++) {
            // we havent found the value
            if (contacts[i].getTimestamp().isBetween(start, end)) {
                CEFaceMask::addToRes(contactNum, res, contacts[i]);
            }
        }

        return res;
    }

    void print() {
        for (size_t i = 0; i < contacts.size(); i++) {
            cout << "(" << contacts[i].getContactNum1() << ", " << contacts[i].getContactNum2() << "), ";
        }
        cout << endl;
    }

private:
    vector <CContact> contacts;
};


void printVector(const vector<int> &contacts) {
    for (size_t i = 0; i < contacts.size(); i++) {
        cout << contacts[i] << " ";
    }
    cout << endl;
}


int main() {
    CEFaceMask test;

    test.addContact(CContact(CTimeStamp(2021, 1, 10, 12, 40, 10), 123456789, 999888777));
    test.addContact(CContact(CTimeStamp(2021, 1, 12, 12, 40, 10), 123456789, 111222333))
            .addContact(CContact(CTimeStamp(2021, 2, 5, 15, 30, 28), 999888777, 555000222));
    test.addContact(CContact(CTimeStamp(2021, 2, 21, 18, 0, 0), 123456789, 999888777));
    test.addContact(CContact(CTimeStamp(2021, 1, 5, 18, 0, 0), 123456789, 456456456));
    test.addContact(CContact(CTimeStamp(2021, 2, 1, 0, 0, 0), 123456789, 123456789));
    assert(test.listContacts(123456789) == (vector < int > {999888777, 111222333, 456456456}));
    assert(test.listContacts(999888777) == (vector < int > {123456789, 555000222}));
    assert(test.listContacts(191919191) == (vector < int > {}));
    assert(test.listContacts(123456789, CTimeStamp(2021, 1, 5, 18, 0, 0), CTimeStamp(2021, 2, 21, 18, 0, 0)) ==
           (vector < int > {999888777, 111222333, 456456456}));
    assert(test.listContacts(123456789, CTimeStamp(2021, 1, 5, 18, 0, 1), CTimeStamp(2021, 2, 21, 17, 59, 59)) ==
           (vector < int > {999888777, 111222333}));
    assert(test.listContacts(123456789, CTimeStamp(2021, 1, 10, 12, 41, 9), CTimeStamp(2021, 2, 21, 17, 59, 59)) ==
           (vector < int > {111222333}));

    return 0;
}
