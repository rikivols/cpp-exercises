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
    CRect(double x, double y, double w, double h) : m_X(x), m_Y(y), m_W(w), m_H(h) {}

    friend ostream &operator<<(ostream &os, const CRect &x) {
        return os << '(' << x.m_X << ',' << x.m_Y << ',' << x.m_W << ',' << x.m_H << ')';
    }

    double m_X;
    double m_Y;
    double m_W;
    double m_H;
};


/**
 * Calculates absolute position of an element based on the absolute position of a window/panel it's in and 
 * relative position of the element itself.
 *
 * @param absolutePosition absolute position of a container that the element is located in
 * @param relativePosition relative position of an element to the container
 * @return newly calculated absolute position of the element
 */
CRect calculateAbsolutePosition(const CRect &absolutePosition, const CRect &relativePosition) {
    return {absolutePosition.m_X + relativePosition.m_X * absolutePosition.m_W,
            absolutePosition.m_Y + relativePosition.m_Y * absolutePosition.m_H,
            relativePosition.m_W * absolutePosition.m_W, relativePosition.m_H * absolutePosition.m_H};
}

/**
 * Adds space separators for the elements, so they appear in nested form for the print. We also make a distinction
 * between spaces and pipes. That part is controlled by isPipe vector.
 *
 * @param out output stream that's being modified by the function 
 * @param isPipe boolean vector that decides whether the given position is a pipe or space
 * @param ignoreLast parameter used for ignoring the last isPipe value. The last isPipe value is reserved
 *                   and used only for the combobox elements
 * @return output stream containing the element's spaces
 */
ostream &printElementPrefix(ostream &out, const vector<bool> &isPipe, bool ignoreLast) {
    for (size_t i = 0; i < isPipe.size(); i++) {
        if (ignoreLast && i == isPipe.size() - 1) {
            continue;
        }
        if (isPipe[i]) {
            out << "|  ";
        } else {
            out << "   ";
        }
    }

    return out;
}


/**
 * Abstract class of an element describing what methods each of the element has to have.
 */
class CElementBase {
public:
    virtual ~CElementBase() noexcept = default;

    virtual shared_ptr<CElementBase> clone() const = 0;

    virtual ostream &print(ostream &out, const vector<bool> &isPipe, bool ignorePrefix) const = 0;

    virtual const vector<shared_ptr<CElementBase>> &getElements() const = 0;

    virtual int getId() const = 0;

    virtual void setAbsolutePos(const CRect &poss) = 0;

    virtual const CRect &getOriginalPos() const = 0;
};


/**
 * Represents 1 element, like window, button, combo box...
 */
class CElement : public CElementBase {
public:
    CElement(int id, const CRect &pos, string txt) : m_Id(id),
                                                     m_OriginalPos(pos), m_AbsolutePos(pos), m_Txt(std::move(txt)) {}

    CElement(const CElement &element) : m_Id(element.m_Id), m_OriginalPos(element.m_OriginalPos),
                                        m_AbsolutePos(element.m_AbsolutePos), m_Txt(element.m_Txt) {}

    /**
     * Swaps current element values with newly created element object. Used for operator=
     */
    void swapElement(CElement &element) {
        swap(m_Id, element.m_Id);
        swap(m_OriginalPos, element.m_OriginalPos);
        swap(m_AbsolutePos, element.m_AbsolutePos);
        swap(m_Txt, element.m_Txt);
    }

    ~CElement() noexcept override = default;

    /**
     * Used to get the elements of panel or window. We need to return an empty vector (in case it's not overridden)
     * to signal that an element has no child elements.
     * 
     * The function uses a static variable, to not lose the reference. It also reduces its capacity to 0 in case
     * that it would cause any memory issues.
     */
    const vector<shared_ptr<CElementBase>> &getElements() const override {
        static vector<shared_ptr<CElementBase>> tempVector;
        tempVector.shrink_to_fit();
        return tempVector;
    };

    [[nodiscard]] shared_ptr<CElementBase> clone() const override {
        return make_shared<CElement>(*this);
    }

    /**
     * Returns an output stream containing characters to print the element (if overriden).
     * 
     * @param out output stream that's being modified by the function
     * @param isPipe boolean vector that decides whether the given position is a pipe or space
     * @param ignorePrefix whether to ignore the '+- ' that comes before an element
     * @throws logic_error we can't print a raw element, it has to be overriden
     */
    ostream &print(ostream &out, const vector<bool> &isPipe, bool ignorePrefix) const override {
        throw logic_error("Can't print a raw element");
    }

    int getId() const override {
        return m_Id;
    }

    void setAbsolutePos(const CRect &absolutePos) override {
        m_AbsolutePos = absolutePos;
    }

    const CRect &getOriginalPos() const override {
        return m_OriginalPos;
    }

    /**
     * Prints an element, but doesn't print element's children. Prints also prefix of an element (spaces, pipes)
     * thanks to the printElementPrefix function.
     *
     * @param elementName name of the element (like Panel or ComboBox)
     * @param out output stream that's being modified by the function
     * @param isPipe boolean vector that decides whether the given position is a pipe or space
     * @param ignorePrefix whether to ignore the '+- ' that comes before an element
     */
    void printElement(const string &elementName, ostream &out, const vector<bool> &isPipe, bool ignorePrefix) const {
        printElementPrefix(out, isPipe, true);
        if (!ignorePrefix) {
            out << "+- ";
        }
        out << "[" << m_Id << "] " << elementName << " ";
        if (!m_Txt.empty()) {
            out << "\"" << m_Txt << "\" ";
        }
        out << m_AbsolutePos << endl;
    }

protected:
    int m_Id;  // id of an element, used for searching
    CRect m_OriginalPos;  // original given position, not modified by any conversions, can be relative or absolute.
    CRect m_AbsolutePos;  // absolute position of the element, converted from relative
    string m_Txt;  // text to be displayed with an element
};

/**
 * Represents window and panel, attributes that can be a parent of other attributes. Has additional variable,
 * m_Elements, which stores all its children. This class is also able to print all of those children and its
 * possible children.
 */
class CElementContainer : public CElement {
public:
    CElementContainer(int id,
                      const string &title,
                      const CRect &relPos) : CElement(id, relPos, title) {}

    CElementContainer(const CElementContainer &elementContainer) : CElement(elementContainer) {
        copyElements(elementContainer.m_Elements);
    }

    CElementContainer(CElementContainer &&elementContainer) noexcept: CElement(elementContainer) {
        swap(m_Elements, elementContainer.m_Elements);
    }

    CElementContainer &operator=(const CElementContainer &element) = delete;

    CElementContainer &operator=(CElementContainer &&element) noexcept = delete;

    const vector<shared_ptr<CElementBase>> &getElements() const override {
        return m_Elements;
    }

    /**
     * Makes a deep copy of the element's vector.
     *
     * @param elements vector of pointers to the child elements of our element container
     */
    void copyElements(const vector<shared_ptr<CElementBase>> &elements) {
        vector<shared_ptr<CElementBase>> newElements;

        for (const auto &element: elements) {
            newElements.push_back(element->clone());
        }
        swap(m_Elements, newElements);
    }

    /**
     * Print our element container and all nested elements that the container has (recursively).
     *
     * @param elementName name of the element (like Panel or ComboBox)
     * @param out output stream that's being modified by the function
     * @param isPipe boolean vector that decides whether the given position is a pipe or space
     * @param ignorePrefix whether to ignore the '+- ' that comes before an element if it has any parent elements
     * @returns nothing, just modifies the out output stream
     */
    void printElementContainer(const string &elementName, ostream &out, vector<bool> isPipe, bool ignorePrefix) const {
        printElement(elementName, out, isPipe, ignorePrefix);
        size_t counter = 0;
        isPipe.push_back(false);

        for (const auto &element: m_Elements) {
            CRect absolutePos = calculateAbsolutePosition(m_AbsolutePos, element->getOriginalPos());
            element->setAbsolutePos(absolutePos);
            // last element doesn't have the pipe (used for ComboBox)
            counter++;
            if (counter == m_Elements.size()) {
                isPipe[isPipe.size() - 1] = false;
            } else {
                isPipe[isPipe.size() - 1] = true;
            }
            // ignorePrefix is set to false, because we know that this element has parents, so it can't be first
            // prefix for all elements that have parents is present (+- )
            element->print(out, isPipe, false);
        }
    }

protected:
    // vector of pointers to the child elements of our element container
    vector<shared_ptr<CElementBase>> m_Elements;
};

class CWindow : public CElementContainer {
public:
    CWindow(int id,
            const string &title,
            const CRect &absPos) : CElementContainer(id, title, absPos) {}

    CWindow(const CWindow &cw) = default;

    CWindow &operator=(CWindow cw) {
        swapElement(cw);
        swap(m_Elements, cw.m_Elements);
        return *this;
    }

    [[nodiscard]] shared_ptr<CElementBase> clone() const override {
        return make_shared<CWindow>(*this);
    }

    [[nodiscard]] ostream &print(ostream &out, const vector<bool> &isPipe, bool ignorePrefix) const override {
        printElementContainer("Window", out, isPipe, ignorePrefix);
        return out;
    }

    /**
     * Add a new element to the window
     *
     * @param element an element that we're inserting to the window
     * @return panel object with a new window
     */
    CWindow &add(const CElement &element) {
        m_Elements.push_back(element.clone());
        return *this;
    }

    /**
     * Recursively finds element with a given id. Iterates over element's children, and detects if the element
     * has child elements on its own. If it does, searches within that child Element.
     *
     * We're also calculating the absolute position of every element we're searching inside of to get the final
     * position of the found Element
     *
     * @param id id of an element we're searching
     * @param currentElements elements we're currently iterating over
     * @param absolutePos recalculated absolute position of the element container that we're iterating over,
     *                    used to calculate the returned element's position
     * @return the found element, nullptr otherwise
     */
    shared_ptr<CElementBase> recurSearch(int id, const vector<shared_ptr<CElementBase>> &currentElements,
                                         CRect absolutePos) {
        for (const auto &element: currentElements) {
            if (element->getId() == id) {
                element->setAbsolutePos(calculateAbsolutePosition(absolutePos, element->getOriginalPos()));
                return element;
            }
            auto nextWindow = element->getElements();
            if (!nextWindow.empty()) {
                return recurSearch(id, nextWindow, calculateAbsolutePosition(absolutePos, element->getOriginalPos()));
            }
        }
        return nullptr;
    }

    /**
     * Recursively find the element with a given id.
     *
     * @param id of an element that we're searching
     * @return the found element, nullptr otherwise
     */
    shared_ptr<CElementBase> search(int id) {
        vector<shared_ptr<CElementBase>> tempElements = m_Elements;
        return recurSearch(id, tempElements, m_AbsolutePos);
    }

    /**
     * Set a new absolute position of the window.
     *
     * @param newPos new absolute position of the window
     * @return window object with a new position
     */
    CElement &setPosition(const CRect &newPos) {
        m_OriginalPos = newPos;
        m_AbsolutePos = newPos;
        return *this;
    }
};

class CPanel : public CElementContainer {
public:
    CPanel(int id,
           const CRect &relPos) : CElementContainer(id, "", relPos) {}

    CPanel(const CPanel &cw) = default;

    [[nodiscard]] shared_ptr<CElementBase> clone() const override {
        return make_shared<CPanel>(*this);
    }

    CPanel &operator=(CPanel cw) {
        swapElement(cw);
        swap(m_Elements, cw.m_Elements);
        return *this;
    }

    [[nodiscard]] ostream &print(ostream &out, const vector<bool> &isPipe, bool ignorePrefix) const override {
        printElementContainer("Panel", out, isPipe, ignorePrefix);
        return out;
    }

    /**
     * Add a new element to the panel.
     *
     * @param element an element that we're inserting to the panel
     * @return panel object with a new element
     */
    CPanel &add(const CElement &element) {
        m_Elements.push_back(element.clone());
        return *this;
    }
};

class CButton : public CElement {
public:
    CButton(int id,
            const CRect &relPos,
            const string &name) : CElement(id, relPos, name) {}

    [[nodiscard]] shared_ptr<CElementBase> clone() const override {
        return make_shared<CButton>(*this);
    }

    [[nodiscard]] ostream &print(ostream &out, const vector<bool> &isPipe, bool ignorePrefix) const override {
        printElement("Button", out, isPipe, ignorePrefix);
        return out;
    }
};

class CInput : public CElement {
public:
    CInput(int id,
           const CRect &relPos,
           const string &value) : CElement(id, relPos, value) {}

    [[nodiscard]] shared_ptr<CElementBase> clone() const override {
        return make_shared<CInput>(*this);
    }

    [[nodiscard]] ostream &print(ostream &out, const vector<bool> &isPipe, bool ignorePrefix) const override {
        printElement("Input", out, isPipe, ignorePrefix);
        return out;
    }

    /**
     * Replace a value of the input text with a new one.
     *
     * @param value text that we're inserting into the input
     * @return newly modified input
     */
    CInput &setValue(const string &value) {
        m_Txt = value;
        return *this;
    }

    [[nodiscard]] string getValue() const {
        return m_Txt;
    }
};

class CLabel : public CElement {
public:
    CLabel(int id,
           const CRect &relPos,
           const string &label) : CElement(id, relPos, label) {}

    [[nodiscard]] shared_ptr<CElementBase> clone() const override {
        return make_shared<CLabel>(*this);
    }

    [[nodiscard]] ostream &print(ostream &out, const vector<bool> &isPipe, bool ignorePrefix) const override {
        printElement("Label", out, isPipe, ignorePrefix);
        return out;
    }
};

class CComboBox : public CElement {
public:
    CComboBox(int id,
              const CRect &relPos) : CElement(id, relPos, ""), selectedIndex(0) {
    };

    [[nodiscard]] shared_ptr<CElementBase> clone() const override {
        return make_shared<CComboBox>(*this);
    }

    [[nodiscard]] ostream &print(ostream &out, const vector<bool> &isPipe, bool ignorePrefix) const override {
        printElement("ComboBox", out, isPipe, ignorePrefix);
        // print all boxes of the combo box
        for (int i = 0; i < (int) boxes.size(); i++) {
            // print spaces and pipes before the box
            printElementPrefix(out, isPipe, false);
            if (i == selectedIndex) {
                out << "+->" << boxes[i] << "<" << endl;
            } else {
                out << "+- " << boxes[i] << endl;
            }
        }
        return out;
    }

    /**
     * Add a new box to the combo box.
     *
     * @param box the box that we're adding to combobox
     * @return newly modified combo box
     */
    CComboBox &add(const string &box) {
        boxes.push_back(box);

        return *this;
    }

    CComboBox &setSelected(int x) {
        selectedIndex = x;
        return *this;
    }

    [[nodiscard]] int getSelected() const {
        return selectedIndex;
    }

private:
    vector<string> boxes;
    int selectedIndex;
};

ostream &operator<<(ostream &out, const CElementBase &self) {
    // we don't print spaces or pipes for the first element (that has no parents), so we set isPipe vector to empty
    // we also want to ignore prefix of the element, since first element has no (+- ) part
    return self.print(out, {}, true);
}


template<typename T>
string toString(const T &x) {
    ostringstream oss;
    oss << x;
    return oss.str();
}

int main() {
    // my tests at the bottom
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

    // MY TESTS
    CWindow c(0, "Sample window", CRect(10, 10, 600, 480));
    // search at the end to make sure that we returned the window from add
    c.add(CPanel(1, CRect(0.1, 0.3, 0.8, 0.7))).add(CPanel(13, CRect(0.2, 0.4, 0.8, 0.8))).search(12);
    assert (toString(c) ==
            "[0] Window \"Sample window\" (10,10,600,480)\n"
            "+- [1] Panel (70,154,480,336)\n"
            "+- [13] Panel (130,202,480,384)\n");

    // recursion hell test
    for (int i = 1; i < 11; i++) {
        CPanel &p2 = dynamic_cast<CPanel &> (*c.search(i));
        p2.add(CPanel(i + 1, CRect(0.1, 0.3, 0.8, 0.7)));
    }
    assert (toString(c) ==
            "[0] Window \"Sample window\" (10,10,600,480)\n"
            "+- [1] Panel (70,154,480,336)\n"
            "|  +- [2] Panel (118,254.8,384,235.2)\n"
            "|     +- [3] Panel (156.4,325.36,307.2,164.64)\n"
            "|        +- [4] Panel (187.12,374.752,245.76,115.248)\n"
            "|           +- [5] Panel (211.696,409.326,196.608,80.6736)\n"
            "|              +- [6] Panel (231.357,433.528,157.286,56.4715)\n"
            "|                 +- [7] Panel (247.085,450.47,125.829,39.5301)\n"
            "|                    +- [8] Panel (259.668,462.329,100.663,27.671)\n"
            "|                       +- [9] Panel (269.735,470.63,80.5306,19.3697)\n"
            "|                          +- [10] Panel (277.788,476.441,64.4245,13.5588)\n"
            "|                             +- [11] Panel (284.23,480.509,51.5396,9.49117)\n"
            "+- [13] Panel (130,202,480,384)\n");

    // add element to the third last panel, check if pipes are displayed correctly
    CPanel &p3 = dynamic_cast<CPanel &> (*c.search(9));
    p3.add(CComboBox(20, CRect(0.1, 0.3, 0.8, 0.1)).add("Progtest").add("makes").add("me").add("insane"));

    assert (toString(c) ==
            "[0] Window \"Sample window\" (10,10,600,480)\n"
            "+- [1] Panel (70,154,480,336)\n"
            "|  +- [2] Panel (118,254.8,384,235.2)\n"
            "|     +- [3] Panel (156.4,325.36,307.2,164.64)\n"
            "|        +- [4] Panel (187.12,374.752,245.76,115.248)\n"
            "|           +- [5] Panel (211.696,409.326,196.608,80.6736)\n"
            "|              +- [6] Panel (231.357,433.528,157.286,56.4715)\n"
            "|                 +- [7] Panel (247.085,450.47,125.829,39.5301)\n"
            "|                    +- [8] Panel (259.668,462.329,100.663,27.671)\n"
            "|                       +- [9] Panel (269.735,470.63,80.5306,19.3697)\n"
            "|                          +- [10] Panel (277.788,476.441,64.4245,13.5588)\n"
            "|                          |  +- [11] Panel (284.23,480.509,51.5396,9.49117)\n"
            "|                          +- [20] ComboBox (277.788,476.441,64.4245,1.93697)\n"
            "|                             +->Progtest<\n"
            "|                             +- makes\n"
            "|                             +- me\n"
            "|                             +- insane\n"
            "+- [13] Panel (130,202,480,384)\n");

    return EXIT_SUCCESS;
}
