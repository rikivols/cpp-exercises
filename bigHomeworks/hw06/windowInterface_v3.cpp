#ifndef __PROGTEST__

#include <cstdlib>
#include <cctype>
#include <cassert>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <string>
#include <memory>

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

/**
 * Calculates absolute position of an element based on the absolute position of a window/panel it's in and 
 * relative position of the element itself.
 * @param absolutePosition absolute position of a container that the element is located in
 * @param relativePosition relative position of an element to the container
 * @return Newly calculated absolute position of the element.
 */
CRect calculateAbsolutePosition(const CRect & absolutePosition, const CRect & relativePosition) {
    return {absolutePosition.m_X + relativePosition.m_X * absolutePosition.m_W,
            absolutePosition.m_Y + relativePosition.m_Y * absolutePosition.m_H,
            relativePosition.m_W * absolutePosition.m_W, relativePosition.m_H * absolutePosition.m_H};
}

/**
 * Prints space separators for the elements, so they appear in nested form. We also make a distinction
 * between spaces and pipes. That part is controlled by isPipe vector
 * @param out output stream that's being modified by the function 
 * @param isPipe boolean vector that decides whether the given position is a pipe or space
 * @param ignoreLast parameter used for ignoring the last isPipe value. The last isPipe value is reserved
 *                   and used only for the combobox elements
 * @return Output stream containing the element's spaces.
 */
ostream & printElementPrefix(ostream & out, const vector<bool> & isPipe, bool ignoreLast=false) {
    for(size_t i=0; i<isPipe.size(); i++) {
        if (ignoreLast && i == isPipe.size() -1) {
            continue;
        }
        if (isPipe[i]) {
            out << "|  ";
        }
        else {
            out << "   ";
        }
    }

    return out;
}


class CAttributeBase {
public:
    virtual ~CAttributeBase() noexcept = default;
    virtual shared_ptr<CAttributeBase> clone() const = 0;
    virtual ostream &print(ostream &out, vector<bool> isPipe, bool ignorePrefix) const = 0;
    virtual const vector<shared_ptr<CAttributeBase>> &getGroupAttributes() const = 0;
    virtual int getId() const = 0;
    virtual void setParentPos(const CRect & poss) = 0;
    virtual const CRect & getPos() const = 0;
};


class CAttribute: public CAttributeBase {
public:
    CAttribute(int id, const CRect &pos, string txt): m_Id(id),
                                                      M_Pos(pos), m_ParentPos(pos), txt(std::move(txt)){};

    CAttribute(const CAttribute & attr): m_Id(attr.m_Id), M_Pos(attr.M_Pos), m_ParentPos(attr.M_Pos), txt(attr.txt){};

    void swapAttribute(CAttribute & attr) {
        swap(m_Id, attr.m_Id);
        swap(M_Pos, attr.M_Pos);
        swap(m_ParentPos, attr.m_ParentPos);
        swap(txt, attr.txt);
    }

    ~CAttribute() noexcept override = default;

    const vector<shared_ptr<CAttributeBase>> &getGroupAttributes() const override {
        static const vector<shared_ptr<CAttributeBase>> tempVector;
        return tempVector;
    };

    [[nodiscard]] shared_ptr<CAttributeBase> clone() const override{
        return make_shared<CAttribute>(*this);
    }

    ostream &print(ostream &out, vector<bool> isPipe, bool ignorePrefix) const override {
        printAttribute("Panel", out, isPipe, false, true);
        return out;
    };

    void printAttribute(const string & attrName, ostream &out, const vector<bool> & isPipe, bool ignorePrefix=false, bool convertAbs=true) const {
        printElementPrefix(out, isPipe, true);
        if (!ignorePrefix) {
            out << "+- ";
        }
        out << "[" << m_Id << "] " << attrName << " ";
        if (!txt.empty()) {
            out << "\"" << txt << "\" ";
        }
        out << (convertAbs ? calculateAbsolutePosition(m_ParentPos, M_Pos) : M_Pos) << endl;
    }

    int getId() const override{
        return m_Id;
    }

    void setParentPos(const CRect & parentPos) override {
        m_ParentPos = parentPos;
    }

    const CRect & getPos() const override {
        return M_Pos;
    }

protected:
    int m_Id;
    CRect M_Pos;
    CRect m_ParentPos;
    string txt;
};

class CAttributeContainer: public CAttribute {
public:
    CAttributeContainer(int id,
                       const string &title,
                       const CRect &relPos): CAttribute(id, relPos, title) {};

    CAttributeContainer(const CAttributeContainer & cw): CAttribute(cw) {
        copyVec(cw);
    }

    CAttributeContainer &operator=(const CAttributeContainer &attr) = delete;
    CAttributeContainer &operator=(CAttributeContainer &&attr) noexcept = delete;

    const vector<shared_ptr<CAttributeBase>> &getGroupAttributes() const override {
        return attributes;
    };

    void copyVec(const CAttributeContainer & cw) {
        vector<shared_ptr<CAttributeBase>> newWindowAttributes;

        for (const auto& attr : cw.attributes) {
            newWindowAttributes.push_back(attr->clone());
        }
        swap(attributes, newWindowAttributes);
    }

    void printAttributeContainer(const string & attrName, ostream &out, vector<bool> isPipe, bool ignorePrefix, bool convertAbs) const {
        printAttribute(attrName, out, isPipe, ignorePrefix, convertAbs);
        size_t counter = 0;
        isPipe.push_back(false);
        for (const auto &attr : attributes) {
            CRect position = convertAbs ? calculateAbsolutePosition(m_ParentPos, M_Pos) : M_Pos;
            attr->setParentPos(position);
            counter++;
            if (counter == attributes.size()) {
                isPipe[isPipe.size()-1] = false;
            } else {
                isPipe[isPipe.size()-1] = true;
            }

            attr->print(out, isPipe, false);
        }
    }
protected:
    vector<shared_ptr<CAttributeBase>> attributes;
};

class CWindow: public CAttributeContainer {
public:
    CWindow(int id,
            const string &title,
            const CRect &parentPos): CAttributeContainer(id, title, parentPos) {};

    CWindow(const CWindow & cw) = default;

    CWindow & operator=(CWindow cw) {
        swapAttribute(cw);
        swap(attributes, cw.attributes);
        return *this;
    }

    [[nodiscard]] shared_ptr<CAttributeBase> clone() const override{
        return make_shared<CWindow>(*this);
    }

    // add
    CWindow & add(const CAttribute & attr) {
        attributes.push_back(attr.clone());
        return *this;
    }

    shared_ptr<CAttributeBase> recurSearch(int id, vector<shared_ptr<CAttributeBase>> & tempWindowAttribute, CRect parentPos) {

        for (const auto &attr: tempWindowAttribute) {
            if (attr->getId() == id) {
                attr->setParentPos(parentPos);
                return attr;
            }
            auto nextWindow = attr->getGroupAttributes();
            if (!nextWindow.empty()) {
                return recurSearch(id, nextWindow, calculateAbsolutePosition(parentPos, attr->getPos()));
            }
        }
        return nullptr;

    }

    // search
    shared_ptr<CAttributeBase> search(int id) {
        vector<shared_ptr<CAttributeBase>> tempWindowAttribute = attributes;

        return recurSearch(id, tempWindowAttribute, m_ParentPos);
    }

    // setPosition
    CAttribute & setPosition(const CRect & newPos) {
        M_Pos = newPos;
        return *this;
    }

    [[nodiscard]] ostream &print(ostream &out, vector<bool> isPipe, bool ignorePrefix) const override
    {
        printAttributeContainer("Window", out, isPipe, ignorePrefix, false);
        return out;
    }
};

class CPanel: public CAttributeContainer{
public:
    CPanel(int id,
           const CRect &relPos): CAttributeContainer(id, "", relPos) {};

    CPanel(const CPanel & cw) = default;

    [[nodiscard]] shared_ptr<CAttributeBase> clone() const override{
        return make_shared<CPanel>(*this);
    }

    CPanel & operator=(CPanel cw) {
        swapAttribute(cw);
        swap(attributes, cw.attributes);
        return *this;
    }

    CPanel & add(const CAttribute & attr) {
        attributes.push_back(attr.clone());
        return *this;
    }

    [[nodiscard]] ostream &print(ostream &out, vector<bool> isPipe, bool ignorePrefix) const override {
        printAttributeContainer("Panel", out, isPipe, ignorePrefix, true);
        return out;
    }
};

class CButton: public CAttribute {
public:
    CButton(int id,
            const CRect &relPos,
            const string &name): CAttribute(id, relPos, name) {};

    [[nodiscard]] shared_ptr<CAttributeBase> clone() const override{
        return make_shared<CButton>(*this);
    }

    [[nodiscard]] ostream &print(ostream &out, vector<bool> isPipe, bool ignorePrefix) const override
    {
        printAttribute("Button", out, isPipe, ignorePrefix);
        return out;
    }
};

class CInput: public CAttribute {
public:
    CInput(int id,
           const CRect &relPos,
           const string &value): CAttribute(id, relPos, value) {};

    [[nodiscard]] shared_ptr<CAttributeBase> clone() const override{
        return make_shared<CInput>(*this);
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

    [[nodiscard]] ostream &print(ostream &out, vector<bool> isPipe, bool ignorePrefix) const override
    {
        printAttribute("Input", out, isPipe, ignorePrefix);
        return out;
    }
};

class CLabel: public CAttribute {
public:
    CLabel(int id,
           const CRect &relPos,
           const string &label): CAttribute(id, relPos, label) {};

    [[nodiscard]] shared_ptr<CAttributeBase> clone() const override{
        return make_shared<CLabel>(*this);
    }

    [[nodiscard]] ostream &print(ostream &out, vector<bool> isPipe, bool ignorePrefix) const override
    {
        printAttribute("Label", out, isPipe, ignorePrefix);
        return out;
    }
};

class CComboBox: public CAttribute {
public:
    CComboBox(int id,
              const CRect &relPos): CAttribute(id, relPos, ""), selectedIndex(0) {
    };

    [[nodiscard]] shared_ptr<CAttributeBase> clone() const override{
        return make_shared<CComboBox>(*this);
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
    [[nodiscard]] ostream &print(ostream &out, vector<bool> isPipe, bool ignorePrefix) const override
    {
        printAttribute("ComboBox", out, isPipe, ignorePrefix, true);
        for (int i=0; i<(int)boxes.size(); i++) {

            printElementPrefix(out, isPipe);
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
ostream &operator<<(ostream &out, const CAttributeBase &self) {
    return self.print(out, {}, true);
}

#ifndef __PROGTEST__

template<typename T>
string toString(const T &x) {
    ostringstream oss;
    oss << x;
    return oss.str();
}

int main() {
    assert (sizeof(CButton) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
    assert (sizeof(CInput) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
    assert (sizeof(CLabel) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
    assert (sizeof(CButton) - sizeof(string) <= sizeof(CPanel) - sizeof(vector<void *>));
    assert (sizeof(CInput) - sizeof(string) <= sizeof(CPanel) - sizeof(vector<void *>));
    assert (sizeof(CLabel) - sizeof(string) <= sizeof(CPanel) - sizeof(vector<void *>));
    CWindow a(0, "Sample window", CRect(10, 10, 600, 480));
    a.add(CButton(1, CRect(0.1, 0.8, 0.3, 0.1), "Ok")).add(CButton(2, CRect(0.6, 0.8, 0.3, 0.1), "Cancel"));
    a.add(CLabel(10, CRect(0.1, 0.1, 0.2, 0.1), "Username:"));
    a.add(CInput(11, CRect(0.4, 0.1, 0.5, 0.1), "chucknorris"));
    a.add(CPanel(12, CRect(0.1, 0.3, 0.8, 0.7)).add(
            CComboBox(20, CRect(0.1, 0.3, 0.8, 0.1)).add("Karate").add("Judo").add("Box").add("Progtest")));
    cout << toString(a);
    assert (toString(a) ==
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

//    if ( b.search(20) == nullptr) {
//        cout << "NULL PTR!" << endl;
//    }
//    cout << toString(*b.search(20));
    assert (toString(*b.search(20)) ==
            "[20] ComboBox (118,254.8,384,33.6)\n"
            "+->Karate<\n"
            "+- Judo\n"
            "+- Box\n"
            "+- Progtest\n");

    assert (dynamic_cast<CComboBox &> ( *b.search(20)).getSelected() == 0);
    dynamic_cast<CComboBox &> ( *b.search(20)).setSelected(3);
    assert (dynamic_cast<CInput &> ( *b.search(11)).getValue() == "chucknorris");
    dynamic_cast<CInput &> ( *b.search(11)).setValue("chucknorris@fit.cvut.cz");
    CPanel &p = dynamic_cast<CPanel &> ( *b.search(12));
    p.add(CComboBox(21, CRect(0.1, 0.5, 0.8, 0.1)).add("PA2").add("OSY").add("Both"));
//    cout << "search res:" << toString(b) << endl;
//    cout << toString(b);
//    cout << toString(b);

    assert (toString(b) ==
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
//    cout << toString(a);

    assert (toString(a) ==
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
//    cout << "hereeeeee" << endl;
//    cout << toString(p);
    assert (toString(p) ==
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
    assert (toString(b) ==
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
//    cout << toString(p);
    assert (toString(p) ==
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
//    cout << "before final add.." << endl;
    p.add(p);
//    cout << toString(p);
    assert (toString(p) ==
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
    assert (toString(p) ==
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
    assert (toString(b) ==
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
    assert (toString(a) ==
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

    CWindow c(0, "Sample window", CRect(10, 10, 600, 480));
    c.add(CPanel(12, CRect(0.1, 0.3, 0.8, 0.7))).add(CPanel(13, CRect(0.1, 0.3, 0.8, 0.7))).search(12);

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
