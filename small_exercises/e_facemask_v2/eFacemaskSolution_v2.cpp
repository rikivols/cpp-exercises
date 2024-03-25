#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <numeric>
#include <vector>
#include <array>
#include <set>
#include <map>
#include <deque>
#include <queue>
#include <stack>
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

    }

    void print() {
        for (size_t i = 0; i < contacts.size(); i++) {
            cout << "(" << contacts[i].getContactNum1() << ", " << contacts[i].getContactNum2() << "), ";
        }
        cout << endl;
    }

    vector<int> getSuperSpreaders(CTimeStamp from, CTimeStamp to) {
        map<int, int> spreaderNum;
        map<int, set<int>> spreaderMeetings;
        int biggestNum = INT32_MIN;

        for (size_t i = 0; i < contacts.size(); i++) {
            // we havent found the value
            if (contacts[i].getTimestamp().isBetween(from, to)) {
                int contactNum1 = contacts[i].getContactNum1();
                int contactNum2 = contacts[i].getContactNum2();

                if (contactNum1 == contactNum2) continue;

                if (spreaderNum.find(contactNum1) == spreaderNum.end()) {
                    spreaderNum[contactNum1] = 0;
                }

                if (spreaderNum.find(contactNum2) == spreaderNum.end()) {
                    spreaderNum[contactNum2] = 0;
                }

                if (spreaderMeetings.find(contactNum1) == spreaderMeetings.end()) {
                    spreaderMeetings[contactNum1] = {};
                }

                if (spreaderMeetings.find(contactNum2) == spreaderMeetings.end()) {
                    spreaderMeetings[contactNum2] = {};
                }

                if (spreaderMeetings[contactNum1].find(contactNum2) == spreaderMeetings[contactNum1].end()) {
                    spreaderNum[contactNum1]++;
                    spreaderMeetings[contactNum1].insert(contactNum2);
                }

                if (spreaderMeetings[contactNum2].find(contactNum1) == spreaderMeetings[contactNum2].end()) {
                    spreaderNum[contactNum2]++;
                    spreaderMeetings[contactNum2].insert(contactNum1);
                }

                if (spreaderNum[contactNum1] > biggestNum) {
                    biggestNum = spreaderNum[contactNum1];
                }

                if (spreaderNum[contactNum2] > biggestNum) {
                    biggestNum = spreaderNum[contactNum2];
                }
            }
        }

        vector<int> finalRes;

        for (auto const &[key, val]: spreaderNum) {
            if (val == biggestNum) {
                finalRes.push_back(key);
            }
        }

        sort(finalRes.begin(), finalRes.end());

        return finalRes;

    }

private:
    vector <CContact> contacts;
};


int main() {
    CEFaceMask test;

    test.addContact(CContact(CTimeStamp(2021, 1, 10, 12, 40, 10), 111111111, 222222222));
    test.addContact(CContact(CTimeStamp(2021, 1, 12, 12, 40, 10), 333333333, 222222222))
            .addContact(CContact(CTimeStamp(2021, 2, 14, 15, 30, 28), 222222222, 444444444));
    test.addContact(CContact(CTimeStamp(2021, 2, 15, 18, 0, 0), 555555555, 444444444));
    assert(test.getSuperSpreaders(CTimeStamp(2021, 1, 1, 0, 0, 0), CTimeStamp(2022, 1, 1, 0, 0, 0)) ==
           (vector < int > {222222222}));
    test.addContact(CContact(CTimeStamp(2021, 3, 20, 18, 0, 0), 444444444, 666666666));
    test.addContact(CContact(CTimeStamp(2021, 3, 25, 0, 0, 0), 111111111, 666666666));
    assert(test.getSuperSpreaders(CTimeStamp(2021, 1, 1, 0, 0, 0), CTimeStamp(2022, 1, 1, 0, 0, 0)) ==
           (vector < int > {222222222, 444444444}));
    return 0;
}
