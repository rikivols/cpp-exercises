#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <memory>

using namespace std;
#endif /* __PROGTEST__ */

// uncomment if your code implements initializer lists
#define EXTENDED_SYNTAX


class CRange {
public:
    // constructor
    CRange(long long low, long long high) : c_low(low), c_high(high) {
        if (low > high) {
            throw logic_error("");
        }
    };

    static long long lowRangeCmp(const CRange &a, const CRange &b) noexcept {
        return a.c_low < b.c_low;
    }

    static long long lowRangeCmpNum(long long a, const CRange &b) noexcept {
        return a < b.c_low;
    }

    static long long highRangeCmpUpper(const CRange &a, const CRange &b) noexcept {
        if (a.c_high == LLONG_MAX) {
            return false;
        }
        return a.c_high + 1 < b.c_low;
    }

    friend ostream &operator<<(ostream &os, const CRange &c) noexcept;

    static long long highRangeCmpUpper2(const CRange &a, const CRange &b) noexcept {
//        if (a.c_high == LLONG_MAX) {
//            return false;
//        }
        return a.c_high < b.c_low;
    }

    long long getLow() const noexcept {
        return c_low;
    }

    long long getHigh() const noexcept {
        return c_high;
    }

    void setLow(long long low) noexcept {
        c_low = low;
    }

    void setHigh(long long high) noexcept {
        c_high = high;
    }


private:
    // todo
    long long c_low;
    long long c_high;
};

class CRangeList {
public:
    // constructor
    CRangeList() {};

    CRangeList(const CRange &cRange) {
        intervals = {cRange};
    };

    CRangeList(const CRangeList &cRange) {
        Copy(cRange);
    };

    CRangeList(CRangeList &&cRange) noexcept {
        swap(intervals, cRange.intervals);
    };

    CRangeList(const initializer_list<CRange> &l) {
        for (auto const &e: l) {
            operator+=(e);
        }
    }

    CRangeList operator+(const CRangeList &cRang) const noexcept {
        if (*this == cRang) {
            return *this;
        }

        CRangeList cc(*this);
        for (auto const &c: cRang.intervals) {
            cc += (c);
        }

        return cc;
    }

    CRangeList operator+(const CRange &cRang) const noexcept {
        CRangeList c(*this);
        c += cRang;
        return c;
    }

    friend CRangeList operator+(const CRange &a, const CRange &b) noexcept;

    // += range / range list
    CRangeList &operator+=(const CRange &crange) noexcept {

        if (intervals.empty()) {
            intervals.push_back(crange);
            return *this;
        }

        if (crange.getLow() <= intervals.begin()->getLow() && crange.getHigh() >= intervals.back().getHigh()) {
            intervals.clear();
            intervals.push_back(crange);
            return *this;
        }

        auto positionLow = upper_bound(intervals.begin(), intervals.end(), crange, CRange::lowRangeCmp);

        // the whole vector is already included
        if (includesPointer(positionLow, crange)) {
            return *this;
        }

        long long iLow = positionLow - intervals.begin();
        auto positionHigh = upper_bound(positionLow, intervals.end(), crange, CRange::highRangeCmpUpper);

        bool doInsert = true;

        long long iHigh = positionHigh - intervals.begin();
        long long newHigh = crange.getHigh();

        if (iHigh > 0 && intervals[iHigh - 1].getHigh() > crange.getHigh()) {
            newHigh = intervals[iHigh - 1].getHigh();
        }

        if (iLow > 0 &&
            ((positionLow - 1)->getHigh() == LLONG_MAX || (positionLow - 1)->getHigh() + 1 >= crange.getLow())) {
            (positionLow - 1)->setHigh(newHigh);
            doInsert = false;
        }

        if (iLow < iHigh) {
            positionLow = intervals.erase(positionLow, positionHigh);
        }

        if (doInsert) {
            CRange newInterval = CRange(crange.getLow(), newHigh);
            intervals.insert(positionLow, newInterval);
        }

        return *this;
    };

    CRangeList &operator+=(const CRangeList &cRang) noexcept {
        for (auto const &c: cRang.intervals) {
            (*this) += (c);
        }

        return *this;
    }

    // -= range / range list
    CRangeList &operator-=(const CRange &crange) noexcept {

        if (intervals.empty()) {
            return *this;
        }

        if (crange.getLow() <= intervals.begin()->getLow() && crange.getHigh() >= intervals.back().getHigh()) {
            intervals.clear();
            return *this;
        }

        auto positionLow = upper_bound(intervals.begin(), intervals.end(), crange, CRange::lowRangeCmp);

        if (includesPointerMinus(positionLow, crange)) {
            long long toAdd = crange.getHigh() == LLONG_MAX ? LLONG_MAX : crange.getHigh() + 1;
            CRange newInterval = CRange(toAdd, (positionLow - 1)->getHigh());
            long long toAdd2 = crange.getLow() == LLONG_MIN ? LLONG_MIN : crange.getLow() - 1;
            (positionLow - 1)->setHigh(toAdd2);
            intervals.insert(positionLow, newInterval);

            return *this;
        }

        //  {<-20..20>, <40, 60>} - <-19, 61>;

        long long iLow = positionLow - intervals.begin();

        auto positionHigh = upper_bound(positionLow, intervals.end(), crange, CRange::highRangeCmpUpper2);
        long long iHigh = positionHigh - intervals.begin();
        // <-20, 20> - <-20, 0>

//        cout << "iLow: " << iLow << endl;
//        cout << "iHigh: " << iHigh << endl;

        if (iLow > 0 && intervals[iLow - 1].getHigh() >= crange.getLow()) {
            // we're inside of a interval, just cut it in two

//            if (intervals[iLow - 1].getHigh() >= crange.getHigh()) {

            if (crange.getLow() == LLONG_MIN) {
                iLow--;
                positionLow--;
            } else {

                if (intervals[iLow - 1].getHigh() <= crange.getHigh()) {
                    (positionLow - 1)->setHigh(crange.getLow() - 1);
                }
                if ((positionLow - 1)->getLow() > (positionLow - 1)->getHigh()) {
                    iLow--;
                    positionLow--;
                }
            }
        }
        // we need to cut into the interval on the right
        if (iHigh > 0 && crange.getHigh() >= intervals[iHigh - 1].getLow()) {

            if (crange.getHigh() != LLONG_MAX) {
                if (crange.getLow() <= (positionHigh - 1)->getLow()) {
                    (positionHigh - 1)->setLow(crange.getHigh() + 1);
                }

                // don't remove this whole interval
                if ((positionHigh - 1)->getLow() <= (positionHigh - 1)->getHigh()) {
                    iHigh--;
                    positionHigh--;
                }
            }
        }

//        cout << "iLow: " << iLow << endl;
//        cout << "iHigh: " << iHigh << endl;

        // we also need to delete intervals
        if (iLow < iHigh) {
            intervals.erase(positionLow, positionHigh);
        }

        return *this;
    };

    CRangeList &operator-=(const CRangeList &cRang) noexcept {
        if (*this == cRang) {
            this->intervals.clear();
            return *this;
        }
        for (auto const &c: cRang.intervals) {
            *this -= (c);
        }

        return *this;
    }

    CRangeList operator-(const CRangeList &cRang) const noexcept {
        CRangeList cc(*this);
        if (*this == cRang) {
            return cc;
        }
        for (auto const &c: cRang.intervals) {
            cc -= (c);
        }

        return cc;
    }

    CRangeList operator-(const CRange &cRang) const noexcept {
        CRangeList cc(*this);

        cc -= (cRang);
        return cc;
    }

    friend CRangeList operator-(const CRange &a, const CRange &b) noexcept;

    friend ostream &operator<<(ostream &os, const CRangeList &c) noexcept;

    vector<CRange>::iterator begin() noexcept { return intervals.begin(); }

    vector<CRange>::const_iterator cbegin() const noexcept { return intervals.cbegin(); }

    vector<CRange>::iterator end() noexcept { return intervals.end(); }

    vector<CRange>::const_iterator cend() const noexcept { return intervals.cend(); }

    // = range / range list
    CRangeList &operator=(const CRangeList &cRang) noexcept {
        if (&cRang == this) return *this;

        Copy(cRang);

        return *this;
    }

    CRangeList &operator=(CRangeList &&cRang) noexcept {
        if (&cRang == this) return *this;

        swap(intervals, cRang.intervals);

        return *this;
    }

    CRangeList &operator=(const CRange &cRang) noexcept {
        intervals = {cRang};

        return *this;
    }

    // operator ==
    bool operator==(const CRangeList &b) const noexcept {

        if (b.intervals.size() != intervals.size()) {
            return false;
        }

        long long i = 0;
        for (auto const &cRange: b.intervals) {
            if (intervals[i].getLow() != cRange.getLow() || intervals[i].getHigh() != cRange.getHigh()) {
                return false;
            }
            i++;
        }

        return true;
    }

    // operator !=
    bool operator!=(const CRangeList &b) const noexcept {
        return !operator==(b);
    }

    template<class T>
    bool includesPointerMinus(const T &position, const CRange &cRang) const noexcept {
        auto iLow = position - intervals.begin();
        // we're inside of an interval, we need to cut it in two
        if (iLow > 0 && cRang.getLow() > (position - 1)->getLow() && cRang.getHigh() < (position - 1)->getHigh()) {
            return true;
        }

        return false;
    }

    template<class T>
    bool includesPointer(const T &position, const CRange &cRang) const noexcept {
        auto iLow = position - intervals.begin();
        // we're inside of an interval, we need to cut it in two
        if (iLow > 0 && cRang.getLow() >= (position - 1)->getLow() && cRang.getHigh() <= (position - 1)->getHigh()) {
            return true;
        }

        return false;
    }

    bool includes(const CRange &cRang) const noexcept {

        auto positionLow = upper_bound(intervals.begin(), intervals.end(), cRang, CRange::lowRangeCmp);
        auto iLow = positionLow - intervals.begin();
        // we're inside of an interval, we need to cut it in two
        if (iLow > 0 && cRang.getLow() >= (positionLow - 1)->getLow() &&
            cRang.getHigh() <= (positionLow - 1)->getHigh()) {
            return true;
        }

        return false;
    }

    bool includes(long long num) const noexcept {
        auto positionLow = upper_bound(intervals.begin(), intervals.end(), num, CRange::lowRangeCmpNum);
        auto iLow = positionLow - intervals.begin();

        // we're inside of an interval, we need to cut it in two
        if (iLow > 0 && num >= (positionLow - 1)->getLow() && num <= (positionLow - 1)->getHigh()) {
            return true;
        }

        return false;
    }

private:
    // todo
    void Copy(const CRangeList &c) noexcept {
        intervals.clear();
        for (auto const &cRange: c.intervals) {
            intervals.push_back(cRange);
        }
    }

    vector<CRange> intervals;
};

CRangeList operator+(const CRange &a, const CRange &b) noexcept {
    CRangeList c(a);
    c += b;

    return c;
}

CRangeList operator-(const CRange &a, const CRange &b) noexcept {
    CRangeList c(a);
    c -= b;

    return c;
}

//
CRangeList operator+(const CRange &a, const CRangeList &b) noexcept {
    CRangeList c(b);
    c += a;

    return c;
}

CRangeList operator-(const CRange &a, const CRangeList &b) noexcept {
    CRangeList c(b);
    c -= a;

    return c;
}

ostream &operator<<(ostream &oss, const CRangeList &c) noexcept {
    auto oldFlags = oss.flags();
    oss << dec;
    oss << "{";
    size_t cntr = 0;
    for (auto const &cRange: c.intervals) {
        if (cRange.getLow() == cRange.getHigh()) {
            oss << cRange.getLow();
        } else {
            oss << "<" << cRange.getLow() << ".." << cRange.getHigh() << ">";
        }
        if (cntr < c.intervals.size() - 1) {
            oss << ",";
        }
        cntr++;
    }
    oss << "}";
    oss.flags(oldFlags);

    return oss;
}

ostream &operator<<(ostream &oss, const CRange &c) noexcept {
    auto oldFlags = oss.flags();
    oss << dec;
    if (c.getLow() == c.getHigh()) {
        oss << c.getLow();
    } else {
        oss << "<" << c.getLow() << ".." << c.getHigh() << ">";
    }

    oss.flags(oldFlags);

    return oss;
}

#ifndef __PROGTEST__

string toString(const CRangeList &x) {
    ostringstream oss;
    oss << x;
    return oss.str();
}

int main(void) {
    CRangeList a, b;

    assert (sizeof(CRange) <= 2 * sizeof(long long));
    a = CRange(5, 10);
    a += CRange(25, 100);
    assert (toString(a) == "{<5..10>,<25..100>}");
    a += CRange(-5, 0);
    a += CRange(8, 50);
    assert (toString(a) == "{<-5..0>,<5..100>}");
    a += CRange(101, 105) + CRange(120, 150) + CRange(160, 180) + CRange(190, 210);
    assert (toString(a) == "{<-5..0>,<5..105>,<120..150>,<160..180>,<190..210>}");
    a += CRange(106, 119) + CRange(152, 158);
    assert (toString(a) == "{<-5..0>,<5..150>,<152..158>,<160..180>,<190..210>}");
    a += CRange(-3, 170);
    a += CRange(-30, 1000);
    assert (toString(a) == "{<-30..1000>}");
    b = CRange(-500, -300) + CRange(2000, 3000) + CRange(700, 1001);
    a += b;
    assert (toString(a) == "{<-500..-300>,<-30..1001>,<2000..3000>}");
    a -= CRange(-400, -400);
    assert (toString(a) == "{<-500..-401>,<-399..-300>,<-30..1001>,<2000..3000>}");
    a -= CRange(10, 20) + CRange(900, 2500) + CRange(30, 40) + CRange(10000, 20000);
    assert (toString(a) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
    try {
        a += CRange(15, 18) + CRange(10, 0) + CRange(35, 38);
        assert ("Exception not thrown" == nullptr);
    }
    catch (const std::logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown" == nullptr);
    }
    assert (toString(a) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
    b = a;
    assert (a == b);
    assert (!(a != b));
    b += CRange(2600, 2700);
    assert (toString(b) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
    assert (a == b);
    assert (!(a != b));
    b += CRange(15, 15);
    assert (toString(b) == "{<-500..-401>,<-399..-300>,<-30..9>,15,<21..29>,<41..899>,<2501..3000>}");
    assert (!(a == b));
    assert (a != b);
    assert (b.includes(15));
    assert (b.includes(2900));
    assert (b.includes(CRange(15, 15)));
    assert (b.includes(CRange(-350, -350)));
    assert (b.includes(CRange(100, 200)));
    assert (!b.includes(CRange(800, 900)));
    assert (!b.includes(CRange(-1000, -450)));
    assert (!b.includes(CRange(0, 500)));
    a += CRange(-10000, 10000) + CRange(10000000, 1000000000);
    assert (toString(a) == "{<-10000..10000>,<10000000..1000000000>}");
    b += a;
    assert (toString(b) == "{<-10000..10000>,<10000000..1000000000>}");
    b -= a;
    assert (toString(b) == "{}");
    b += CRange(0, 100) + CRange(200, 300) - CRange(150, 250) + CRange(160, 180) - CRange(170, 170);
    assert (toString(b) == "{<0..100>,<160..169>,<171..180>,<251..300>}");
    b -= CRange(10, 90) - CRange(20, 30) - CRange(40, 50) - CRange(60, 90) + CRange(70, 80);
    assert (toString(b) == "{<0..9>,<20..30>,<40..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}");
#ifdef EXTENDED_SYNTAX
    CRangeList x{{5,   20},
                 {150, 200},
                 {-9,  12},
                 {48,  93}};
    assert (toString(x) == "{<-9..20>,<48..93>,<150..200>}");
    ostringstream oss;
    oss << setfill('=') << hex << left;
    for (const auto &v: x + CRange(-100, -100))
        oss << v << endl;
    oss << setw(10) << 1024;
    assert (oss.str() == "-100\n<-9..20>\n<48..93>\n<150..200>\n400=======");
#endif /* EXTENDED_SYNTAX */
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */