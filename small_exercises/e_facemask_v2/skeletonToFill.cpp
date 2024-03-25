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
    // constructor ( y, m, d, h, min, s )
    // TODO
};

class CContact {
public:
    // constructor ( timeStamp, phone1, phone2 )
    // TODO
};

class CEFaceMask {
public:
    // default constructor
    // addContact ( contact )
    // getSuperSpreaders ( from, to )
    // TODO
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
