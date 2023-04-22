#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include <algorithm>
#include <functional>

#include <stdexcept>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdint>
#include <cctype>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
template <typename T>
class CSet {
private:
    class Node {
    public:
        Node(const T newData): nextNode(nullptr), val(newData) {};
        Node * nextNode;
        T val;

        const T &getVal() const {
            return val;
        }
    };

    Node * newNode(const T new_data)
    {
        Node * newN = new Node(new_data);
        newN->val = new_data;
        newN->nextNode = nullptr;

        return newN;
    }

    Node * headNode = nullptr;
    int size;

public:
    CSet(): headNode(nullptr), size(0) {};

    void copyy(CSet const &to_copy) {
        Node * ptr = to_copy.getPtr();
        headNode = nullptr;
        size = 0;

        while(ptr != nullptr) {
            Insert(ptr->getVal());
            ptr = ptr->nextNode;
        }
    };

    CSet(CSet const &to_copy): headNode(nullptr), size(0) {
        copyy(to_copy);
    };

    Node * getPtr() const {
        return headNode;
    }

    ~CSet() {
        while(headNode != nullptr) {
            Node * tmp = headNode->nextNode;
            delete headNode;
            headNode = tmp;
        }
    }

    CSet &operator= (CSet const &to_copy) {
        CSet x;
        Node * ptr = to_copy.getPtr();
        while(ptr != nullptr) {
            x.Insert(ptr->getVal());
            ptr = ptr->nextNode;
        }
        this->~CSet();
        this->copyy(x);
        x.~CSet();
        return *this;
    }

    void printList() {
        Node *temp = headNode;
        while (temp != nullptr) {
            cout << "'" << temp->getVal() << "'" << "->";
            temp = temp->nextNode;
        }
        cout << "nullptr" << endl;
    }

    bool Insert(const T value) {
        Node * curNode;

        if (headNode == nullptr || !(headNode->getVal() < value)) {
            if (headNode != nullptr && !(value < headNode->getVal()) && !(headNode->getVal() < value)) {
                return false;
            }
            Node * nNode = newNode(value);
            nNode->nextNode = headNode;
            headNode = nNode;
        }
        else {
            curNode = headNode;
            while (curNode->nextNode != nullptr) {
                if (!(value < curNode->nextNode->getVal()) && !(curNode->nextNode->getVal() < value)) {
                    return false;
                }
                else if (curNode->nextNode->getVal() < value) {
                    curNode = curNode->nextNode;
                }
                else {
                    break;
                }
            }
            Node * nNode = newNode(value);
            nNode->nextNode = curNode->nextNode;
            curNode->nextNode = nNode;
        }
        size++;
        return true;
    };

    bool Remove(const T value) {

        Node * curNode;
        Node * prevNode = nullptr;

        curNode = headNode;
        if (curNode != nullptr && !(value < curNode->getVal()) && !(curNode->getVal() < value)) {

            headNode = curNode->nextNode;
            size--;

            delete curNode;

            return true;
        }

        while (curNode != nullptr) {
            if (!(value < curNode->getVal()) && !(curNode->getVal() < value)) {
                prevNode->nextNode = curNode->nextNode;
                delete curNode;

                size--;
                return true;
            }
            else if (curNode->getVal() < value) {
                prevNode = curNode;
                curNode = curNode->nextNode;
            }
            else {
                break;
            }
        }

        return false;
    };

    bool Empty() const {
        return headNode == nullptr;
    };

    size_t Size() const {
        return size;
    };

    bool Contains(const T value) const {
        Node *curNode = headNode;
        if (headNode != nullptr && !(value < headNode->val) && !(headNode->val < value)) {
            return true;
        }
        while(curNode->nextNode != nullptr) {
            if (!(value < curNode->nextNode->getVal()) && !(curNode->nextNode->val < value)) {
                return true;
            }
            if (value < curNode->nextNode->val) {
                return false;
            }
            curNode = curNode->nextNode;
        }

        return false;
    };

};

#ifndef __PROGTEST__
#include <cassert>

class Time {
public:

    Time(int year, int month): year(year), month(month) {};

    Time(const Time & time) {
        year = time.year;
        month = time.month;
    }

    Time & operator=(const Time & time) {
        year = time.year;
        month = time.month;

        return *this;
    }

    bool operator<(const Time & time) const {
        if (year > time.year) {
            return false;
        }
        else if (year < time.year) {
            return true;
        }
        return month < time.month;
    }

private:
    Time() = default;

    int year;
    int month;
};

struct CSetTester
{
    static void test0()
    {
        CSet<string> x0;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
    }

    static void test1()
    {
        CSet<string> x0;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        x0.printList();
        assert( !x0 . Remove( "Pavlik Jan" ) );
        CSet<string> x1 ( x0 );
        assert( x0 . Insert( "Vodickova Saskie" ) );
        assert( x1 . Insert( "Kadlecova Kvetslava" ) );
        assert( x0 . Size() == 3 );
        assert( x1 . Size() == 3 );
        assert( x0 . Contains( "Vodickova Saskie" ) );
        assert( !x1 . Contains( "Vodickova Saskie" ) );
        assert( !x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Contains( "Kadlecova Kvetslava" ) );
    }

    static void test2()
    {
        CSet<string> x0;
        CSet<string> x1;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        x1 = x0;
        assert( x0 . Insert( "Vodickova Saskie" ) );
        assert( x1 . Insert( "Kadlecova Kvetslava" ) );
        assert( x0 . Size() == 3 );
        assert( x1 . Size() == 3 );
        assert( x0 . Contains( "Vodickova Saskie" ) );
        assert( !x1 . Contains( "Vodickova Saskie" ) );
        assert( !x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Contains( "Kadlecova Kvetslava" ) );
    }

    static void test4()
    {
        CSet<int> x0;
        assert( x0 . Insert( 4 ) );
        assert( x0 . Insert( 8 ) );
        assert( x0 . Insert( 1 ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( 4 ) );
        assert( !x0 . Contains( 5 ) );
        assert( !x0 . Remove( 5 ) );
        assert( x0 . Remove( 4 ) );
    }

    static void test5()
    {
        CSet<Time> x0;
        assert( x0 . Insert( Time(2024, 5) ) );
        assert( x0 . Insert( Time(2028, 5) ) );
        assert( x0 . Insert( Time(2021, 5) ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( Time(2024, 5) ) );
        assert( !x0 . Contains( Time(2025, 5) ) );
        assert( !x0 . Remove( Time(2025, 5) ) );
        assert( x0 . Remove( Time(2024, 5) ) );
    }

};

int main ()
{
    CSetTester::test0();
    CSetTester::test1();
    CSetTester::test2();
    CSetTester::test4();
    return 0;
}
#endif /* __PROGTEST__ */
