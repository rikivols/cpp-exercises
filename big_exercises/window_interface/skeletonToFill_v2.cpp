#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>

using namespace std;

class CRect {
public:
    CRect(double x,
          double y,
          double w,
          double h)
            : m_X(x),
              m_Y(y),
              m_W(w),
              m_H(h) {
    }

    friend ostream &operator<<(ostream &os,
                               const CRect &x) {
        return os << '(' << x.m_X << ',' << x.m_Y << ',' << x.m_W << ',' << x.m_H << ')';
    }

    double m_X;
    double m_Y;
    double m_W;
    double m_H;
};


class CWindow {
public:
    CWindow(int id,
            const string &title,
            const CRect &absPos);
    // add
    // search
    // setPosition
};

class CPanel {
public:
    CPanel(int id,
           const CRect &relPos);
    // add
};


class CButton {
public:
    CButton(int id,
            const CRect &relPos,
            const string &name);
};

class CInput {
public:
    CInput(int id,
           const CRect &relPos,
           const string &value);
    // setValue 
    // getValue 
};

class CLabel {
public:
    CLabel(int id,
           const CRect &relPos,
           const string &label);
};

class CComboBox {
public:
    CComboBox(int id,
              const CRect &relPos);
    // add                                                                            
    // setSelected
    // getSelected
};

// output operators


template<typename T_>
string toString(const T_ &x) {
    ostringstream oss;
    oss << x;
    return oss.str();
}

int main(void) {
    assert(sizeof(CButton) - sizeof(string) < sizeof(CComboBox) - sizeof(vector < string > ));
    assert(sizeof(CInput) - sizeof(string) < sizeof(CComboBox) - sizeof(vector < string > ));
    assert(sizeof(CLabel) - sizeof(string) < sizeof(CComboBox) - sizeof(vector < string > ));
    assert(sizeof(CButton) - sizeof(string) <= sizeof(CPanel) - sizeof(vector < void * > ));
    assert(sizeof(CInput) - sizeof(string) <= sizeof(CPanel) - sizeof(vector < void * > ));
    assert(sizeof(CLabel) - sizeof(string) <= sizeof(CPanel) - sizeof(vector < void * > ));
    CWindow a(0, "Sample window", CRect(10, 10, 600, 480));
    a.add(CButton(1, CRect(0.1, 0.8, 0.3, 0.1), "Ok")).add(CButton(2, CRect(0.6, 0.8, 0.3, 0.1), "Cancel"));
    a.add(CLabel(10, CRect(0.1, 0.1, 0.2, 0.1), "Username:"));
    a.add(CInput(11, CRect(0.4, 0.1, 0.5, 0.1), "chucknorris"));
    a.add(CPanel(12, CRect(0.1, 0.3, 0.8, 0.7)).add(
            CComboBox(20, CRect(0.1, 0.3, 0.8, 0.1)).add("Karate").add("Judo").add("Box").add("Progtest")));
    assert(toString(a) ==
           "[0] Window \"Sample window\" (10,10,600,480)\n"
           "+- [1] Button \"Ok\" (70,394,180,48)\n"
           "+- [2] Button \"Cancel\" (370,394,180,48)\n"
           "+- [10] Label \"Username:\" (70,58,120,48)\n"
           "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
           "+- [12] Panel (70,154,480,336)\n"
           "   +- [20] ComboBox (118,254.8,384,33.6)\n"
           "      +->Karate<\n"
           "      +- Judo\n"
           "      +- Box\n"
           "      +- Progtest\n");
    CWindow b = a;
    assert(toString(*b.search(20)) ==
           "[20] ComboBox (118,254.8,384,33.6)\n"
           "+->Karate<\n"
           "+- Judo\n"
           "+- Box\n"
           "+- Progtest\n");
    assert(dynamic_cast<CComboBox &> ( *b.search(20)).getSelected() == 0);
    dynamic_cast<CComboBox &> ( *b.search(20)).setSelected(3);
    assert(dynamic_cast<CInput &> ( *b.search(11)).getValue() == "chucknorris");
    dynamic_cast<CInput &> ( *b.search(11)).setValue("chucknorris@fit.cvut.cz");
    CPanel &p = dynamic_cast<CPanel &> ( *b.search(12));
    p.add(CComboBox(21, CRect(0.1, 0.5, 0.8, 0.1)).add("PA2").add("OSY").add("Both"));
    assert(toString(b) ==
           "[0] Window \"Sample window\" (10,10,600,480)\n"
           "+- [1] Button \"Ok\" (70,394,180,48)\n"
           "+- [2] Button \"Cancel\" (370,394,180,48)\n"
           "+- [10] Label \"Username:\" (70,58,120,48)\n"
           "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
           "+- [12] Panel (70,154,480,336)\n"
           "   +- [20] ComboBox (118,254.8,384,33.6)\n"
           "   |  +- Karate\n"
           "   |  +- Judo\n"
           "   |  +- Box\n"
           "   |  +->Progtest<\n"
           "   +- [21] ComboBox (118,322,384,33.6)\n"
           "      +->PA2<\n"
           "      +- OSY\n"
           "      +- Both\n");
    assert(toString(a) ==
           "[0] Window \"Sample window\" (10,10,600,480)\n"
           "+- [1] Button \"Ok\" (70,394,180,48)\n"
           "+- [2] Button \"Cancel\" (370,394,180,48)\n"
           "+- [10] Label \"Username:\" (70,58,120,48)\n"
           "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
           "+- [12] Panel (70,154,480,336)\n"
           "   +- [20] ComboBox (118,254.8,384,33.6)\n"
           "      +->Karate<\n"
           "      +- Judo\n"
           "      +- Box\n"
           "      +- Progtest\n");
    assert(toString(p) ==
           "[12] Panel (70,154,480,336)\n"
           "+- [20] ComboBox (118,254.8,384,33.6)\n"
           "|  +- Karate\n"
           "|  +- Judo\n"
           "|  +- Box\n"
           "|  +->Progtest<\n"
           "+- [21] ComboBox (118,322,384,33.6)\n"
           "   +->PA2<\n"
           "   +- OSY\n"
           "   +- Both\n");
    b.setPosition(CRect(20, 30, 640, 520));
    assert(toString(b) ==
           "[0] Window \"Sample window\" (20,30,640,520)\n"
           "+- [1] Button \"Ok\" (84,446,192,52)\n"
           "+- [2] Button \"Cancel\" (404,446,192,52)\n"
           "+- [10] Label \"Username:\" (84,82,128,52)\n"
           "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
           "+- [12] Panel (84,186,512,364)\n"
           "   +- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
           "   |  +- Karate\n"
           "   |  +- Judo\n"
           "   |  +- Box\n"
           "   |  +->Progtest<\n"
           "   +- [21] ComboBox (135.2,368,409.6,36.4)\n"
           "      +->PA2<\n"
           "      +- OSY\n"
           "      +- Both\n");
    p.add(p);
    assert(toString(p) ==
           "[12] Panel (84,186,512,364)\n"
           "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
           "|  +- Karate\n"
           "|  +- Judo\n"
           "|  +- Box\n"
           "|  +->Progtest<\n"
           "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
           "|  +->PA2<\n"
           "|  +- OSY\n"
           "|  +- Both\n"
           "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
           "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
           "   |  +- Karate\n"
           "   |  +- Judo\n"
           "   |  +- Box\n"
           "   |  +->Progtest<\n"
           "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
           "      +->PA2<\n"
           "      +- OSY\n"
           "      +- Both\n");
    p.add(p);
    assert(toString(p) ==
           "[12] Panel (84,186,512,364)\n"
           "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
           "|  +- Karate\n"
           "|  +- Judo\n"
           "|  +- Box\n"
           "|  +->Progtest<\n"
           "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
           "|  +->PA2<\n"
           "|  +- OSY\n"
           "|  +- Both\n"
           "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
           "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
           "|  |  +- Karate\n"
           "|  |  +- Judo\n"
           "|  |  +- Box\n"
           "|  |  +->Progtest<\n"
           "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
           "|     +->PA2<\n"
           "|     +- OSY\n"
           "|     +- Both\n"
           "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
           "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
           "   |  +- Karate\n"
           "   |  +- Judo\n"
           "   |  +- Box\n"
           "   |  +->Progtest<\n"
           "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
           "   |  +->PA2<\n"
           "   |  +- OSY\n"
           "   |  +- Both\n"
           "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
           "      +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
           "      |  +- Karate\n"
           "      |  +- Judo\n"
           "      |  +- Box\n"
           "      |  +->Progtest<\n"
           "      +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
           "         +->PA2<\n"
           "         +- OSY\n"
           "         +- Both\n");
    p.add(p);
    assert(toString(p) ==
           "[12] Panel (84,186,512,364)\n"
           "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
           "|  +- Karate\n"
           "|  +- Judo\n"
           "|  +- Box\n"
           "|  +->Progtest<\n"
           "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
           "|  +->PA2<\n"
           "|  +- OSY\n"
           "|  +- Both\n"
           "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
           "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
           "|  |  +- Karate\n"
           "|  |  +- Judo\n"
           "|  |  +- Box\n"
           "|  |  +->Progtest<\n"
           "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
           "|     +->PA2<\n"
           "|     +- OSY\n"
           "|     +- Both\n"
           "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
           "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
           "|  |  +- Karate\n"
           "|  |  +- Judo\n"
           "|  |  +- Box\n"
           "|  |  +->Progtest<\n"
           "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
           "|  |  +->PA2<\n"
           "|  |  +- OSY\n"
           "|  |  +- Both\n"
           "|  +- [12] Panel (176.16,371.64,327.68,178.36)\n"
           "|     +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
           "|     |  +- Karate\n"
           "|     |  +- Judo\n"
           "|     |  +- Box\n"
           "|     |  +->Progtest<\n"
           "|     +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
           "|        +->PA2<\n"
           "|        +- OSY\n"
           "|        +- Both\n"
           "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
           "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
           "   |  +- Karate\n"
           "   |  +- Judo\n"
           "   |  +- Box\n"
           "   |  +->Progtest<\n"
           "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
           "   |  +->PA2<\n"
           "   |  +- OSY\n"
           "   |  +- Both\n"
           "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
           "   |  +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
           "   |  |  +- Karate\n"
           "   |  |  +- Judo\n"
           "   |  |  +- Box\n"
           "   |  |  +->Progtest<\n"
           "   |  +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
           "   |     +->PA2<\n"
           "   |     +- OSY\n"
           "   |     +- Both\n"
           "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
           "      +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
           "      |  +- Karate\n"
           "      |  +- Judo\n"
           "      |  +- Box\n"
           "      |  +->Progtest<\n"
           "      +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
           "      |  +->PA2<\n"
           "      |  +- OSY\n"
           "      |  +- Both\n"
           "      +- [12] Panel (208.928,425.148,262.144,124.852)\n"
           "         +- [20] ComboBox (235.142,462.604,209.715,12.4852)\n"
           "         |  +- Karate\n"
           "         |  +- Judo\n"
           "         |  +- Box\n"
           "         |  +->Progtest<\n"
           "         +- [21] ComboBox (235.142,487.574,209.715,12.4852)\n"
           "            +->PA2<\n"
           "            +- OSY\n"
           "            +- Both\n");
    assert(toString(b) ==
           "[0] Window \"Sample window\" (20,30,640,520)\n"
           "+- [1] Button \"Ok\" (84,446,192,52)\n"
           "+- [2] Button \"Cancel\" (404,446,192,52)\n"
           "+- [10] Label \"Username:\" (84,82,128,52)\n"
           "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
           "+- [12] Panel (84,186,512,364)\n"
           "   +- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
           "   |  +- Karate\n"
           "   |  +- Judo\n"
           "   |  +- Box\n"
           "   |  +->Progtest<\n"
           "   +- [21] ComboBox (135.2,368,409.6,36.4)\n"
           "   |  +->PA2<\n"
           "   |  +- OSY\n"
           "   |  +- Both\n"
           "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
           "   |  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
           "   |  |  +- Karate\n"
           "   |  |  +- Judo\n"
           "   |  |  +- Box\n"
           "   |  |  +->Progtest<\n"
           "   |  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
           "   |     +->PA2<\n"
           "   |     +- OSY\n"
           "   |     +- Both\n"
           "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
           "   |  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
           "   |  |  +- Karate\n"
           "   |  |  +- Judo\n"
           "   |  |  +- Box\n"
           "   |  |  +->Progtest<\n"
           "   |  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
           "   |  |  +->PA2<\n"
           "   |  |  +- OSY\n"
           "   |  |  +- Both\n"
           "   |  +- [12] Panel (176.16,371.64,327.68,178.36)\n"
           "   |     +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
           "   |     |  +- Karate\n"
           "   |     |  +- Judo\n"
           "   |     |  +- Box\n"
           "   |     |  +->Progtest<\n"
           "   |     +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
           "   |        +->PA2<\n"
           "   |        +- OSY\n"
           "   |        +- Both\n"
           "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
           "      +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
           "      |  +- Karate\n"
           "      |  +- Judo\n"
           "      |  +- Box\n"
           "      |  +->Progtest<\n"
           "      +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
           "      |  +->PA2<\n"
           "      |  +- OSY\n"
           "      |  +- Both\n"
           "      +- [12] Panel (176.16,371.64,327.68,178.36)\n"
           "      |  +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
           "      |  |  +- Karate\n"
           "      |  |  +- Judo\n"
           "      |  |  +- Box\n"
           "      |  |  +->Progtest<\n"
           "      |  +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
           "      |     +->PA2<\n"
           "      |     +- OSY\n"
           "      |     +- Both\n"
           "      +- [12] Panel (176.16,371.64,327.68,178.36)\n"
           "         +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
           "         |  +- Karate\n"
           "         |  +- Judo\n"
           "         |  +- Box\n"
           "         |  +->Progtest<\n"
           "         +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
           "         |  +->PA2<\n"
           "         |  +- OSY\n"
           "         |  +- Both\n"
           "         +- [12] Panel (208.928,425.148,262.144,124.852)\n"
           "            +- [20] ComboBox (235.142,462.604,209.715,12.4852)\n"
           "            |  +- Karate\n"
           "            |  +- Judo\n"
           "            |  +- Box\n"
           "            |  +->Progtest<\n"
           "            +- [21] ComboBox (235.142,487.574,209.715,12.4852)\n"
           "               +->PA2<\n"
           "               +- OSY\n"
           "               +- Both\n");
    assert(toString(a) ==
           "[0] Window \"Sample window\" (10,10,600,480)\n"
           "+- [1] Button \"Ok\" (70,394,180,48)\n"
           "+- [2] Button \"Cancel\" (370,394,180,48)\n"
           "+- [10] Label \"Username:\" (70,58,120,48)\n"
           "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
           "+- [12] Panel (70,154,480,336)\n"
           "   +- [20] ComboBox (118,254.8,384,33.6)\n"
           "      +->Karate<\n"
           "      +- Judo\n"
           "      +- Box\n"
           "      +- Progtest\n");
    return EXIT_SUCCESS;
}
