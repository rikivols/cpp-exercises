#include <cstring>
#include <cstdlib>
#include <iostream>
using namespace std;

class CLinkedSetTester;


class CLinkedSet {
private:
    struct CNode {
        CNode *m_Next;
        char * c;

        const char *Value() const {
            return c;
        };
    };

    CNode* newNode(const char * new_data)
    {
        CNode* newNode = (CNode *)malloc(sizeof(*newNode));
        newNode->c = (char *)malloc(sizeof(new_data)*(strlen(new_data)+1));
        newNode->c = strcpy(newNode->c, new_data);
        newNode->m_Next = nullptr;

        return newNode;
    }

    CNode *m_Begin;
    int size;

public:
    // default constructor
    CLinkedSet(): m_Begin(nullptr), size(0) {};

    CNode * getPtr() const {
        return m_Begin;
    }

    // copy constructor
    void copyy(CLinkedSet const &to_copy) {
        CNode * ptr = to_copy.getPtr();
        m_Begin = nullptr;
        size = 0;

        while(ptr != nullptr) {
            Insert(ptr->Value());
            ptr = ptr->m_Next;
        }
    };

    CLinkedSet(CLinkedSet const &to_copy): m_Begin(nullptr), size(0) {
        copyy(to_copy);
    };

    // operator=
    CLinkedSet &operator= (CLinkedSet const &to_copy) {
        CLinkedSet x;
        CNode * ptr = to_copy.getPtr();

        while(ptr != nullptr) {
            x.Insert(ptr->Value());
            ptr = ptr->m_Next;
        }

        this->~CLinkedSet();

        this->copyy(x);

        x.~CLinkedSet();

        return *this;
    }

    // destructor
    ~CLinkedSet() {
        while(m_Begin != nullptr) {
            CNode * tmp = m_Begin->m_Next;
            free(m_Begin->c);
            free(m_Begin);
            m_Begin = tmp;
        }
    }

    void print() {
        CNode *temp = m_Begin;
        while (temp != nullptr) {
            cout << "'" << temp->c << "'" << "->";
            temp = temp->m_Next;
        }
        cout << "nullptr" << endl;
    }

    bool Insert(const char *value) {
        CNode* current_node;

        if (m_Begin == nullptr || strcmp(m_Begin->Value(), value) >= 0) {
            if (m_Begin != nullptr && strcmp(m_Begin->Value(), value) == 0) {
                return false;
            }
            CNode* new_node = newNode(value);
            new_node->m_Next = m_Begin;
            m_Begin = new_node;
        }
        else {
            current_node = m_Begin;
            while (current_node->m_Next != nullptr) {
                if (strcmp(current_node->m_Next->Value(), value) == 0) {
                    return false;
                }
                else if (strcmp(current_node->m_Next->Value(), value) < 0) {
                    current_node = current_node->m_Next;
                }
                else {
                    break;
                }
            }
            CNode* new_node = newNode(value);
            new_node->m_Next = current_node->m_Next;
            current_node->m_Next = new_node;
        }
        size++;
        return true;
    };

    bool Remove(const char *value) {

        CNode* currentNode;
        CNode* prevNode = nullptr;

        currentNode = m_Begin;
        if (currentNode != nullptr && strcmp(currentNode->Value(), value) == 0) {

            m_Begin = currentNode->m_Next;

            free(currentNode->c);
            free(currentNode);
            return true;
        }

        while (currentNode != nullptr) {
            if (strcmp(currentNode->Value(), value) == 0) {
                prevNode->m_Next = currentNode->m_Next;
                free(currentNode->c);
                free(currentNode);
                size--;
                return true;
            }
            else if (strcmp(currentNode->Value(), value) < 0) {
                prevNode = currentNode;
                currentNode = currentNode->m_Next;
            }
            else {
                break;
            }
        }

        return false;
    };

    bool Empty() const {
        return m_Begin == nullptr;
    };

    size_t Size() const {
        return size;
    };

    bool Contains(const char *value) const {
        CNode *currentNode = m_Begin;
        if (m_Begin != nullptr && strcmp(m_Begin->Value(), value) == 0) {
            return true;
        }
        while(currentNode->m_Next != nullptr) {
            if (strcmp(currentNode->m_Next->Value(), value) == 0) {
                return true;
            }
            if (strcmp(currentNode->m_Next->Value(), value) > 0) {
                return false;
            }
            currentNode = currentNode->m_Next;
        }

        return false;
    };

    friend class ::CLinkedSetTester;
};


#include <cassert>

struct CLinkedSetTester {

    static void test0() {
        CLinkedSet x0;
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Size() == 3);
        assert(x0.Contains("Dusek Zikmund"));
        assert(!x0.Contains("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
        assert(!x0.Empty());
    }

    static void test1() {
        CLinkedSet x0;
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Size() == 3);
        assert(x0.Contains("Dusek Zikmund"));
        assert(!x0.Contains("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
        CLinkedSet x1(x0);
        assert(x0.Insert("Vodickova Saskie"));
        assert(x1.Insert("Kadlecova Kvetslava"));
        assert(x0.Size() == 3);
        assert(x1.Size() == 3);
        assert(x0.Contains("Vodickova Saskie"));
        assert(!x1.Contains("Vodickova Saskie"));
        assert(!x0.Contains("Kadlecova Kvetslava"));
        assert(x1.Contains("Kadlecova Kvetslava"));
    }

    static void test2() {
        CLinkedSet x0;
        CLinkedSet x1;
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Size() == 3);
        assert(x0.Contains("Dusek Zikmund"));
        assert(!x0.Contains("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
        x1 = x0;
        assert(x0.Insert("Vodickova Saskie"));
        assert(x1.Insert("Kadlecova Kvetslava"));
        assert(x0.Size() == 3);
        assert(x1.Size() == 3);
        assert(x0.Contains("Vodickova Saskie"));
        assert(!x1.Contains("Vodickova Saskie"));
        assert(!x0.Contains("Kadlecova Kvetslava"));
        assert(x1.Contains("Kadlecova Kvetslava"));
    }

};


int main() {
//    CLinkedSet x0, x1, x2;
//    assert(x0.Insert("1"));
//    assert(x0.Insert("2"));
//    assert(x0.Insert("3"));
//    x0.Insert("3");
//    x0.Insert("3");
//    x0.Insert("1");
//    x0.Insert("1");
//
//    x0 = x0;
////    x2 = x0;
//
//    x0.print();
//    x2.print();
//
//    x1 = x2;
//    x2.print();
//    x1.print();

//    x0.Remove("");
//    x0.Remove("kutbhdhilsrnqdjoyrr");
//
//    x0.Insert("kutbhdhilsrnqdjoyrr");
//    x0.Insert("kutbhdhilsrnqdjoyrr");
//    x0.Insert("");
//    x0.Insert("kutbhdhilsrnqdjoyrr");
//
//    assert(x1.Insert("sgsg"));
//    assert(x1.Insert("Dusek1 Zikmund"));
//    assert(x1.Insert("Dusek2 Zikmund"));
//    assert(x1.Insert("Dusek3 Zikmund"));
//    assert(x1.Insert("Dusek4 Zikmund"));
//    assert(x1.Insert(""));
//    assert(x1.Insert("Dusek6 Zikmund"));
//    x1.Remove("Dusek3 Zikmund");
//    x1.Remove("Dusek3 Zikmund");
//
//    x2.Insert("");

//    x0.print();

    CLinkedSetTester::test0();
    CLinkedSetTester::test1();
    CLinkedSetTester::test2();
    return 0;
}
