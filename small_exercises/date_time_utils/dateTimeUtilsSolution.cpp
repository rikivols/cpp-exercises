
#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>

using namespace std;

class CTimeTester;


class CTime {
private:
    int m_Hour;
    int m_Minute;
    int m_Second;

public:
    // constructor, destructor
    CTime() {};

    CTime(int hours, int minutes) : m_Hour(hours), m_Minute(minutes), m_Second(0) {
        if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
            throw invalid_argument("");
        }
    };

    CTime(int hours, int minutes, int seconds) : m_Hour(hours), m_Minute(minutes), m_Second(seconds) {
        if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59 || seconds < 0 || seconds > 59) {
            throw invalid_argument("");
        }
    };

//    // arithmetic operators
    CTime operator+(int seconds) const {
        long secondsAll = timeToSeconds() + seconds;
        return secondsToTime(*this, secondsAll);
    };

//
    CTime operator+(int seconds) {
        long secondsAll = timeToSeconds() + seconds;
        return secondsToTime(*this, secondsAll);
    };

    CTime operator+=(int seconds) {
        long secondsAll = timeToSeconds() + seconds;
        secondsToTime(secondsAll);
        return *this;

    };

    CTime operator-(int seconds) const {
//        cout << "seconds before reducing: " << timeToSeconds() << endl;
        long secondsAll = timeToSeconds() - seconds;
//        cout << "seconds after reducing: " << secondsAll << endl;

        return secondsToTime(*this, secondsAll);
    };

    CTime operator-(int seconds) {
//        cout << "seconds before reducing: " << timeToSeconds() << endl;
        long secondsAll = timeToSeconds() - seconds;
//        cout << "seconds after reducing: " << secondsAll << endl;

        return secondsToTime(*this, secondsAll);
    };

    CTime operator-=(int seconds) {
        long secondsAll = timeToSeconds() - seconds;
        secondsToTime(secondsAll);
        return *this;
    };

    CTime operator++() {
        long secondsAll = timeToSeconds() + 1;
        secondsToTime(secondsAll);
        return *this;
    };

    CTime operator--() {
        long secondsAll = timeToSeconds() - 1;
        secondsToTime(secondsAll);
        return *this;
    };

    CTime operator++(int) {
        CTime oldClass = *this;
        operator++();

        return oldClass;
    };

    CTime operator--(int) {
        CTime oldClass = *this;
        operator--();

        return oldClass;
    };


    // output operator

    long timeToSeconds() const {
        return m_Hour * 60 * 60 + m_Minute * 60 + m_Second;
    }

    static CTime secondsToTime(CTime t, long seconds) {
        while (seconds < 0) {
            seconds += 86400;
        }
        seconds = int(seconds % 86400);

        int hours_remainder = seconds % (60 * 60);
        t.m_Hour = seconds / (60 * 60);
        t.m_Minute = hours_remainder / 60;
        t.m_Second = hours_remainder % 60;

        return t;
    }

    void secondsToTime(long seconds) {
        while (seconds < 0) {
            seconds += 86400;
        }
        seconds = int(seconds % 86400);

        int hours_remainder = seconds % (60 * 60);
        m_Hour = seconds / (60 * 60);
        m_Minute = hours_remainder / 60;
        m_Second = hours_remainder % 60;
    }

    int getHour() const {
        return this->m_Hour;
    }

    int getMinute() const {
        return this->m_Minute;
    }

    int getSeconds() const {
        return this->m_Second;
    }

    friend class ::CTimeTester;
};

long timeToSeconds(const CTime &t) {
    return t.getHour() * 60 * 60 + t.getMinute() * 60 + t.getSeconds();
}

int operator-(const CTime &t1, const CTime &t2) {
    long secondst1 = timeToSeconds(t1);
    long secondst2 = timeToSeconds(t2);

    int temp_res = (int) abs(secondst1 - secondst2);
    if (temp_res > 43200) {
        temp_res = 86400 - temp_res;
    }

//    cout << "seconds1: " << secondst1 << endl;
//    cout << "seconds2: " << secondst2 << endl;

    return temp_res;
}

// arithmetic operators

//CTime operator+ (const CTime & t, int seconds) {
//    long secondsAll = seconds + timeToSeconds(t);
//    return CTime::secondsToTime(CTime(t.getHour(), t.getMinute(), t.getSeconds()), secondsAll);
//}

CTime operator-(int seconds, const CTime &t) {
    long secondsAll = seconds - timeToSeconds(t);
    return CTime::secondsToTime(CTime(t.getHour(), t.getMinute(), t.getSeconds()), secondsAll);
}

CTime operator-(int seconds, CTime &t) {
    long secondsAll = seconds - timeToSeconds(t);
    return CTime::secondsToTime(CTime(t.getHour(), t.getMinute(), t.getSeconds()), secondsAll);
}


CTime operator+(int seconds, const CTime &t) {
    long secondsAll = seconds + timeToSeconds(t);
    return CTime::secondsToTime(CTime(t.getHour(), t.getMinute(), t.getSeconds()), secondsAll);
}

CTime operator+(int seconds, CTime &t) {
    long secondsAll = seconds + timeToSeconds(t);
    return CTime::secondsToTime(CTime(t.getHour(), t.getMinute(), t.getSeconds()), secondsAll);
}

//CTime operator- (int seconds, const CTime & t) {
//    long secondsAll = seconds - timeToSeconds(t);
//    return CTime::secondsToTime(CTime(t.getHour(), t.getMinute(), t.getSeconds()), secondsAll);
//}

// comparison operators
bool operator<(const CTime &t1, const CTime &t2) {
    return timeToSeconds(t1) < timeToSeconds(t2);
}

bool operator<=(const CTime &t1, const CTime &t2) {
    return timeToSeconds(t1) <= timeToSeconds(t2);
}

bool operator>(const CTime &t1, const CTime &t2) {
    return timeToSeconds(t1) > timeToSeconds(t2);
}

bool operator>=(const CTime &t1, const CTime &t2) {
    return timeToSeconds(t1) >= timeToSeconds(t2);
}

bool operator==(const CTime &t1, const CTime &t2) {
    return timeToSeconds(t1) == timeToSeconds(t2);
}

bool operator!=(const CTime &t1, const CTime &t2) {
    return timeToSeconds(t1) != timeToSeconds(t2);
}

ostream &operator<<(std::ostream &os, CTime t) {
    string hours = to_string(t.getHour());
    if (hours.size() == 1) {
        hours = " " + hours;
    }
    string minutes = to_string(t.getMinute());
    if (minutes.size() == 1) {
        minutes = "0" + minutes;
    }
    string seconds = to_string(t.getSeconds());
//    cout << "seconds: " << t.getSeconds();
    if (seconds.size() == 1) {
        seconds = "0" + seconds;
    }

    os << hours + ":" + minutes + ":" + seconds;
    return os;
}


struct CTimeTester {
    static bool test(const CTime &time, int hour, int minute, int second) {
        return time.m_Hour == hour
               && time.m_Minute == minute
               && time.m_Second == second;
    }
};

#include <cassert>
#include <sstream>

int main() {
    CTime a{12, 30};
    assert(CTimeTester::test(a, 12, 30, 0));

    CTime b{13, 30};
    assert(CTimeTester::test(b, 13, 30, 0));

    assert(b - a == 3600);

    assert(CTimeTester::test(a + 60, 12, 31, 0));
    assert(CTimeTester::test(a - 60, 12, 29, 0));

    assert(a < b);
    assert(a <= b);
    assert(a != b);
    assert(!(a > b));
    assert(!(a >= b));
    assert(!(a == b));

    while (++a != b);
    assert(a == b);

    std::ostringstream output;
    assert(static_cast<bool>( output << a ));
    assert(output.str() == "13:30:00");

    assert(a++ == b++);
    assert(a == b);

    assert(--a == --b);
    assert(a == b);

    assert(a-- == b--);
    assert(a == b);

    cout << (CTime(12, 0, 1) - CTime(0, 0, 0));

//    cout << (CTime(0, 0, 0)-);

    return 0;
}
