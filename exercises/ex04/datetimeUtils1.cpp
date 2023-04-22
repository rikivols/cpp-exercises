#ifndef __PROGTEST__
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
#endif /* __PROGTEST__ */

class CTimeStamp
{
    public:
        CTimeStamp(long year, long month, long day, long hour, long minute, long second){
            datetime[0] = year;
            datetime[1] = month;
            datetime[2] = day;
            datetime[3] = hour;
            datetime[4] = minute;
            datetime[5] = second;
        }

        bool isBetween(const CTimeStamp &start, const CTimeStamp &end) {
            // find if its before
            for (int i=0; i<6; i++) {
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

            for (int i=0; i<6; i++) {
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

class CContact
{
    public:
        CContact(const CTimeStamp & cTimeS, int contact1, int contact2):
                cTimeStamp(cTimeS), contactNum1(contact1), contactNum2(contact2)
        {}

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

class CEFaceMask
{
    public:

        CEFaceMask &addContact(const CContact & contact) {
//            CEFaceMask c;
            if (contact.getContactNum1() != contact.getContactNum2()) {
                contacts.push_back(contact);
            }
            return *this;
        }
        void addToRes(int contactNum, vector<int> & res, const CContact &contact) const {
            int contactNum2;
            if (contactNum == contact.getContactNum1()) {
                contactNum2 = contact.getContactNum2();
            }
            else if (contactNum == contact.getContactNum2()) {
                contactNum2 = contact.getContactNum1();
            }
            else {
                return;
            }
            // we havent found the value
            for (size_t i=0; i<res.size(); i++) {
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
            for (size_t i=0; i<contacts.size(); i++) {
                // we havent found the value
                CEFaceMask::addToRes(contactNum, res, contacts[i]);
            }

            return res;
        }

        vector<int> listContacts(int contactNum, CTimeStamp start, CTimeStamp end) const {
            vector<int> res;
            for (size_t i=0; i<contacts.size(); i++) {
                // we havent found the value
                if (contacts[i].getTimestamp().isBetween(start, end)) {
                    CEFaceMask::addToRes(contactNum, res, contacts[i]);
                }
            }

            return res;
        }

        void print() {
            for (size_t i=0; i<contacts.size(); i++) {
                cout << "(" << contacts[i].getContactNum1() << ", " << contacts[i].getContactNum2() <<"), ";
            }
            cout << endl;
        }

    private:
        vector <CContact> contacts;
};


void printVector(const vector<int> &contacts) {
    for (size_t i=0; i<contacts.size(); i++) {
        cout << contacts[i] << " ";
    }
    cout << endl;
}


#ifndef __PROGTEST__
int main ()
{
    return 0;
}
#endif /* __PROGTEST__ */
