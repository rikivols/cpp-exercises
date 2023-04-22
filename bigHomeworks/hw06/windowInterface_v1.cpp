#ifndef __PROGTEST__

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
#include <utility>
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

#endif /* __PROGTEST__ */

CRect copyCRect(const CRect &cRect) {
    return {cRect.m_X, cRect.m_Y, cRect.m_W, cRect.m_H};
}

class Attr {
public:
    Attr(int id, const CRect &pos, const string & txt): id(id), pos(copyCRect(pos)), absPos(copyCRect(pos)), txt(txt), printConf(1), positionChanged(false) {}
    Attr(const Attr & attr): id(attr.id), pos(copyCRect(attr.pos)), absPos(copyCRect(pos)), txt(attr.txt), printConf(attr.printConf), positionChanged(attr.positionChanged) {
    };
    void swapAttr(Attr & attr) {
        swap(id, attr.id);
        swap(pos, attr.pos);
        swap(absPos, attr.absPos);
        swap(txt, attr.txt);
        swap(printConf, attr.printConf);
        swap(positionChanged, attr.positionChanged);
    }
    virtual ~Attr() noexcept = default;
    virtual Attr * clone() const = 0;
    virtual ostream &print(ostream &out) const = 0;
    void attrString(const string & attrName, ostream &out, bool printTxt=true, bool convertAbs=true) const {
        out << "[" << id << "] " << attrName << " ";
        if (printTxt) {
            out << "\"" << txt << "\" ";
        }
        out << (convertAbs ? calculateAbsolutePosition() : pos) << endl;
    }

    CRect calculateAbsolutePosition() const {
        return {absPos.m_X + pos.m_X * absPos.m_W, absPos.m_Y + pos.m_Y * absPos.m_H,
                    pos.m_W * absPos.m_W, pos.m_H * absPos.m_H};
    }

    int getId() const {
        return id;
    }

    void setPrintConf(int conf) {
        printConf = conf;
    }

    void setAbsPos(const CRect & pos) {
        absPos = copyCRect(pos);
    }

protected:
    int id;
    CRect pos;
    CRect absPos;
    string txt;
    int printConf;
    bool positionChanged;
};


class CWindow: public Attr {
public:
    CWindow(int id,
            const string &title,
            const CRect &absPos): Attr(id, absPos, title) {};

    CWindow(const CWindow & cw): Attr(cw) {
        copyMap(cw);
    }

    CWindow & operator=(CWindow cw) {
        swapAttr(cw);
        swap(windowAttributes, cw.windowAttributes);
        return *this;
    }

    void copyMap(const CWindow & cw) {
        map<int, Attr *> newWindowAttributes;

        for (const auto [id, attr]: cw.windowAttributes) {
            newWindowAttributes[id] = attr->clone();
        }
        windowAttributes = newWindowAttributes;
    }

    CWindow * clone() const override{
        return new CWindow(*this);
    }

    // add
    CWindow & add(const Attr & attr) {
        int attrId = attr.getId();

        if (windowAttributes.find(attrId) == windowAttributes.end()) {
            windowAttributes[attrId] = attr.clone();
        }

        return *this;
    }

    // search
    Attr * search(int id) {
        if (windowAttributes.find(id) == windowAttributes.end()) {
            return nullptr;
        }
        windowAttributes[id]->setAbsPos(pos);
        windowAttributes[id]->setPrintConf(1);

        return windowAttributes[id];
    }

    // setPosition
    Attr & setPosition(const CRect & newPos) {
        pos = copyCRect(newPos);
        return *this;
    }

    [[nodiscard]] ostream &print(ostream &out) const override
    {
        attrString("Window", out, true, false);
        int counter = 0;

        for (const auto [id, attr]: windowAttributes) {
            counter++;
            (counter == windowAttributes.size()) ? attr->setPrintConf(3) : attr->setPrintConf(2);
            attr->setAbsPos(pos);
            out << "+- ";
            attr->print(out);
        }

        return out;
    }
private:
    map<int, Attr *> windowAttributes;
};

class CButton: public Attr {
public:
    CButton(int id,
            const CRect &relPos,
            const string &name): Attr(id, relPos, name) {};

    CButton * clone() const override{
        return new CButton(*this);
    }

    [[nodiscard]] ostream &print(ostream &out) const override
    {
        attrString("Button", out);
        return out;
    }
};

class CInput: public Attr {
public:
    CInput(int id,
           const CRect &relPos,
           const string &value): Attr(id, relPos, value) {};

    CInput * clone() const override{
        return new CInput(*this);
    }

    // setValue
    CInput & setValue(const string &value) {
        txt = value;
        return *this;
    }

    // getValue
    [[nodiscard]] string getValue() const {
        return txt;
    }

    [[nodiscard]] ostream &print(ostream &out) const override
    {
        attrString("Input", out);
        return out;
    }
};

class CLabel: public Attr {
public:
    CLabel(int id,
           const CRect &relPos,
           const string &label): Attr(id, relPos, label) {};

    CLabel * clone() const override{
        return new CLabel(*this);
    }

    [[nodiscard]] ostream &print(ostream &out) const override
    {
        attrString("Label", out);
        return out;
    }
};

class CComboBox: public Attr {
public:
    CComboBox(int id,
              const CRect &relPos): Attr(id, relPos, ""), selectedIndex(0) {
        setPrintConf(2);
    };

    CComboBox * clone() const override{
        return new CComboBox(*this);
    }

    // add
    CComboBox & add(const string & box) {
        boxes.push_back(box);

        return *this;
    }

    // setSelected
    CComboBox & setSelected(int x) {
        selectedIndex = x;
        return *this;
    }

    // getSelected
    [[nodiscard]] int getSelected() const {
        return selectedIndex;
    }

    // 1 = without spaces, 2 = with |, 3 = with spaces
    [[nodiscard]] ostream &print(ostream &out) const override
    {
        attrString("ComboBox", out, false);
        for (size_t i=0; i<boxes.size(); i++) {
            if (printConf == 2) {
                out << "|  ";
            }
            else if (printConf == 3) {
                out << "   ";
            }
            if (i == selectedIndex) {
                out << "+->" << boxes[i] << "<" << endl;
            }
            else {
                out << "+- " << boxes[i] << endl;
            }
        }
        return out;
    }
private:
    vector<string> boxes;
    int selectedIndex;
};

// output operators
ostream &operator<<(ostream &out, const Attr &self) {
    return self.print(out);
}

#ifndef __PROGTEST__

template<typename _T>
string toString(const _T &x) {
    ostringstream oss;
    oss << x;
    return oss.str();
}

int main(void) {
    assert (sizeof(CButton) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
    assert (sizeof(CInput) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
    assert (sizeof(CLabel) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
    CWindow a(0, "Sample window", CRect(10, 10, 600, 480));
    // 10 + 0.1 * 600
    a.add(CButton(1, CRect(0.1, 0.8, 0.3, 0.1), "Ok")).add(CButton(2, CRect(0.6, 0.8, 0.3, 0.1), "Cancel"));
    a.add(CLabel(10, CRect(0.1, 0.1, 0.2, 0.1), "Username:"));
    a.add(CInput(11, CRect(0.4, 0.1, 0.5, 0.1), "chucknorris"));
    a.add(CComboBox(20, CRect(0.1, 0.3, 0.8, 0.1)).add("Karate").add("Judo").add("Box").add("Progtest"));
    assert (toString(a) ==
            "[0] Window \"Sample window\" (10,10,600,480)\n"
            "+- [1] Button \"Ok\" (70,394,180,48)\n"
            "+- [2] Button \"Cancel\" (370,394,180,48)\n"
            "+- [10] Label \"Username:\" (70,58,120,48)\n"
            "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
            "+- [20] ComboBox (70,154,480,48)\n"
            "   +->Karate<\n"
            "   +- Judo\n"
            "   +- Box\n"
            "   +- Progtest\n");
    CWindow b = a;
    assert (toString(*b.search(20)) ==
            "[20] ComboBox (70,154,480,48)\n"
            "+->Karate<\n"
            "+- Judo\n"
            "+- Box\n"
            "+- Progtest\n");
    assert (dynamic_cast<CComboBox &> ( *b.search(20)).getSelected() == 0);
    dynamic_cast<CComboBox &> ( *b.search(20)).setSelected(3);
    assert (dynamic_cast<CInput &> ( *b.search(11)).getValue() == "chucknorris");
    dynamic_cast<CInput &> ( *b.search(11)).setValue("chucknorris@fit.cvut.cz");
    b.add(CComboBox(21, CRect(0.1, 0.5, 0.8, 0.1)).add("PA2").add("OSY").add("Both"));
    assert (toString(b) ==
            "[0] Window \"Sample window\" (10,10,600,480)\n"
            "+- [1] Button \"Ok\" (70,394,180,48)\n"
            "+- [2] Button \"Cancel\" (370,394,180,48)\n"
            "+- [10] Label \"Username:\" (70,58,120,48)\n"
            "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
            "+- [20] ComboBox (70,154,480,48)\n"
            "|  +- Karate\n"
            "|  +- Judo\n"
            "|  +- Box\n"
            "|  +->Progtest<\n"
            "+- [21] ComboBox (70,250,480,48)\n"
            "   +->PA2<\n"
            "   +- OSY\n"
            "   +- Both\n");
    assert (toString(a) ==
            "[0] Window \"Sample window\" (10,10,600,480)\n"
            "+- [1] Button \"Ok\" (70,394,180,48)\n"
            "+- [2] Button \"Cancel\" (370,394,180,48)\n"
            "+- [10] Label \"Username:\" (70,58,120,48)\n"
            "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
            "+- [20] ComboBox (70,154,480,48)\n"
            "   +->Karate<\n"
            "   +- Judo\n"
            "   +- Box\n"
            "   +- Progtest\n");
    b.setPosition(CRect(20, 30, 640, 520));
    assert (toString(b) ==
            "[0] Window \"Sample window\" (20,30,640,520)\n"
            "+- [1] Button \"Ok\" (84,446,192,52)\n"
            "+- [2] Button \"Cancel\" (404,446,192,52)\n"
            "+- [10] Label \"Username:\" (84,82,128,52)\n"
            "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
            "+- [20] ComboBox (84,186,512,52)\n"
            "|  +- Karate\n"
            "|  +- Judo\n"
            "|  +- Box\n"
            "|  +->Progtest<\n"
            "+- [21] ComboBox (84,290,512,52)\n"
            "   +->PA2<\n"
            "   +- OSY\n"
            "   +- Both\n");
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
