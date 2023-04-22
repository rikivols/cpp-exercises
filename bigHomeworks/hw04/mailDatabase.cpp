#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;
#endif /* __PROGTEST__ */


class Str {
public:
    Str(): strData(nullptr) {}
    Str(const char *str) : strData(new StrData(str)){};

    Str(const Str &str) : strData(str.strData) {
        strData->refCount++;
    }

    ~Str(void) noexcept {
        if (strData) {
            if (--strData->refCount == 0) {
                delete strData;
            }
        }
    };

    Str &operator=(Str x) {
        swap(strData, x.strData);
        return *this;
    };

    operator const char *(void) const {
        return strData->str;
    };

private:
    struct StrData {
        StrData(const char *strInp) : refCount(1),
                                      len(strlen(strInp)),
                                      str(new char[len+1]) {
            strcpy(str, strInp);
        }

        StrData(const StrData &src) : refCount(1),
                                      len(src.len),
                                      str(new char[len+1]) {
            strcpy(str, src.str);
        }

        ~StrData() {
            if (str)
                delete[] str;
        };

        int refCount;
        size_t len;
        char *str;
    };

    StrData *strData;
};

class CMail {
public:
    CMail() : emailFrom(), emailTo(), emailBody() {};

    CMail(const char *from,
          const char *to,
          const char *body) {
        emailFrom = Str(from);
        emailTo = Str(to);
        emailBody = Str(body);
    };

    static bool cmpUpperBound(const CMail &a, const CMail &b, bool isInbox) {
        if (isInbox) {
            return strcmp(a.emailTo, b.emailTo) >= 0;
        }
        return strcmp(a.emailFrom, b.emailFrom) >= 0;
    }

    bool operator==(const CMail &x) const {
        return strcmp(emailFrom, x.emailFrom) == 0 && strcmp(emailTo, x.emailTo) == 0 &&
               strcmp(emailBody, x.emailBody) == 0;
    };

    friend ostream &operator<<(ostream &os,
                               const CMail &m) {
        return os << "From: " << m.emailFrom << ", To: " << m.emailTo << ", Body: " << m.emailBody;
    };

    [[nodiscard]] const char * getFrom () const {
        return emailFrom;
    };

    [[nodiscard]] const char * getTo () const {
        return emailTo;
    };

    int cmpTo(const char * email) const {
        return strcmp(email, emailTo);
    }

    int cmpFrom(const char * email) const {
        return strcmp(email, emailFrom);
    }

private:
    // todo
    Str emailFrom;
    Str emailTo;
    Str emailBody;
};

class Cmails {
public:
    Cmails() {
        cap = 1;
        len = 0;
        mails = new CMail[cap];
    }

    Cmails(const Cmails & cmails): mails(new CMail[cmails.cap]), len(cmails.len), cap(cmails.cap) {
        for (size_t i=0; i<len; i++) {
            mails[i] = cmails.mails[i];
        }
    }

    Cmails(Cmails &&cmails): mails(cmails.mails), len(cmails.len), cap(cmails.cap){
        cmails.mails = nullptr;
    }

    ~Cmails() {
//        for (size_t i=0; i<cap; i++) {
//            delete [] mails[i].
//        }
        delete [] mails;
    }

    Cmails &operator=(Cmails cmails) {
        swap(mails, cmails.mails);
        swap(len, cmails.len);
        swap(cap, cmails.cap);

        return *this;
    }

    void append(const CMail &cmail) {
        if (len >= cap) {
            while (len >= cap) {
                cap += cap / 2 + 10;
            }

            CMail *tmp = new CMail[cap];
            for (size_t i = 0; i < min(len, cap); ++i)
                tmp[i] = mails[i];
            delete[] mails;
            mails = tmp;
        }
        mails[len] = cmail;
        len++;
    }

    int cmpFirstFrom(const char * email) const {
        return mails[0].cmpFrom(email);
    }

    int cmpFirstTo(const char * email) const {
        return mails[0].cmpTo(email);
    }

    size_t getLen() const {
        return len;
    }

    size_t getCap() const {
        return cap;
    }

    const CMail &operator[] (size_t i) const {
        return mails[i];
    }

private:
    CMail *mails;
    size_t len;
    size_t cap;
};

class SharedCmails {
private:
    Cmails * ptr;
    int * refCount;

    void copyOnWrite() {
        if (*refCount == 1) {
            return;
        }

        auto newPtr = new Cmails(*ptr);

        *refCount -= 1;

        ptr = newPtr;
        refCount = new int(1);
    }
public:
    SharedCmails(): ptr(new Cmails()), refCount(new int(1)) {};

    SharedCmails(const SharedCmails & sharedCmails): ptr(sharedCmails.ptr), refCount(sharedCmails.refCount) {
        *refCount += 1;
    };

    ~SharedCmails() {
        if (*refCount && (*refCount -= 1) == 0) {
            delete ptr;
            delete refCount;
        }
    }

    SharedCmails & operator=(SharedCmails sharedBox) {
        swap(ptr, sharedBox.ptr);
        swap(refCount, sharedBox.refCount);

        return *this;
    }

    void append(const CMail &cmail) {
        copyOnWrite();
        ptr->append(cmail);
    }

    const CMail &operator[](size_t i) const {
        return (*ptr)[i];
    }

    int cmpFirstFrom(const char * email) const {
        return ptr->cmpFirstFrom(email);
    }

    int cmpFirstTo(const char * email) const {
        return ptr->cmpFirstTo(email);
    }

    size_t getLen() const {
        return ptr->getLen();
    }

};

class CMailIterator {
public:
    CMailIterator(const SharedCmails &mailSrc, bool isMatch) {
        index = 0;
        if (isMatch) {
            len = mailSrc.getLen();
            sharedCmails = mailSrc;
        }
        else {
            len = 0;
        }
    }

    ~CMailIterator() {
    }

    explicit operator bool(void) const {
        if (index >= len) {
            return false;
        }
        return true;
    };

    bool operator!(void) const {
        return !this->operator bool();
    };

    const CMail &operator*(void) const {
        return sharedCmails[index];
    };

    CMailIterator &operator++(void) {
        index++;
        return *this;
    };

//    void print() {
//        cout << "\nMailIterator:" << endl;
//        for (size_t i=0; i<len; i++) {
//            cout << mails[i] << endl;
//        }
//    }

private:
    // todo
    SharedCmails sharedCmails;
    size_t len;
    size_t index;
};

bool cmpLowerBound1(const char * email, const SharedCmails &b, bool isInbox, bool & gotExactMatch) {
    if (isInbox) {
        int cmpRes = b.cmpFirstTo(email);
        if (cmpRes == 0) {
            gotExactMatch = true;
        }
        return cmpRes <= 0;
    }
    int cmpRes = b.cmpFirstFrom(email);
    if (cmpRes == 0) {
        gotExactMatch = true;
    }
    return cmpRes <= 0;
}

static bool cmpLowerBound2(const char * email, const SharedCmails &b, bool isInbox, bool & gotExactMatch) {
    if (isInbox) {
        int cmpRes = b.cmpFirstTo(email);
        if (cmpRes == 0) {
            gotExactMatch = true;
        }
        return cmpRes > 0;
    }
    int cmpRes = b.cmpFirstFrom(email);
    if (cmpRes == 0) {
        gotExactMatch = true;
    }
    return cmpRes > 0;
}


class Box {
public:
    Box(bool isInboxSrc = true) {
        len = 0;
        cap = 1;
        isInbox = isInboxSrc;
        cmails = new SharedCmails[cap];
    }

    Box(const Box &box) : cmails(new SharedCmails[box.cap]), len(box.len), cap(box.cap), isInbox(box.isInbox) {
        for (size_t i = 0; i < len; i++) {
            cmails[i] = box.cmails[i];
        }
    }

    Box(Box &&box): cmails(box.cmails), len(box.len), cap(box.cap), isInbox(box.isInbox){
        box.cmails = nullptr;
    }

    ~Box() {
        delete [] cmails;
    }

    Box &operator=(Box box) {
        swap(cmails, box.cmails);
        swap(len, box.len);
        swap(cap, box.cap);
        swap(isInbox, box.isInbox);

        return *this;
    }

    void insert(size_t pos, const CMail &cmail) {
        // shift elements to the right
        for (long i = long(len); i >= long(pos) + 1; i--) {
            cmails[i] = cmails[i - 1];
        }
        cmails[pos] = SharedCmails();
        cmails[pos].append(cmail);
        len++;
    }

    void add(const CMail &cmail) {
        bool gotMatch = false;

        size_t lowerIdx = isInbox ? lower_boundNew(cmail.getTo(), gotMatch) : lower_boundNew(cmail.getFrom(), gotMatch);
        // we found the value, just insert into Cmails
        if (gotMatch) {
            cmails[lowerIdx].append(cmail);
        }
        else {
            // insert into given position
            if (len >= cap) {
                while (len >= cap) {
                    cap += cap / 2 + 10;
                }

                auto *tmp = new SharedCmails[cap];
                for (size_t i = 0; i < min(len, cap); ++i)
                    tmp[i] = cmails[i];
                delete[] cmails;
                cmails = tmp;
            }

            insert(lowerIdx, cmail);
        }
    }

    // z geeks for geeks
    // ASPON ALGORITMY STE NAM MOHLI NECHAT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    size_t lower_boundNew(const char * email, bool & gotMatch) const
    {
        size_t mid;
        size_t low = 0;
        size_t high = len;

        int cntr = 0;

        while (low < high) {
            mid = low + (high - low) / 2;
            if (cmpLowerBound1(email, cmails[mid], isInbox, gotMatch)) {
                high = mid;
            }
            else {
                low = mid + 1;
            }
            cntr++;
        }
        if(low < len && cmpLowerBound2(email, cmails[low], isInbox, gotMatch)) {
            low++;
        }
        return low;
    }

    auto findMail(const char *email, size_t & idx, bool & foundMatch) const {
        idx = lower_boundNew(email, foundMatch);
        return idx;
    }

    size_t getLen() const {
        return len;
    }

    size_t getCapacity() const {
        return cap;
    }

    const SharedCmails &operator[](size_t i) const {
        return cmails[i];
    }

private:
    SharedCmails *cmails;
    size_t len;
    size_t cap;
    bool isInbox;
};


class CMailServer {
public:
    CMailServer(void) {
        inboxes = Box(true);
        outboxes = Box(false);
    };

    CMailServer(const CMailServer &src) {
        inboxes = Box(src.inboxes);
        outboxes = Box(src.outboxes);
    };

    CMailServer(CMailServer &&src) noexcept {
        swap(inboxes, src.inboxes);
        swap(outboxes, src.outboxes);
    };

    CMailServer &operator=(const CMailServer &src) {
        if (&src == this) return *this;

        inboxes = Box(src.inboxes);
        outboxes = Box(src.outboxes);

        return *this;
    };

    CMailServer &operator=(CMailServer &&src) noexcept {
        if (&src == this) return *this;

        swap(inboxes, src.inboxes);
        swap(outboxes, src.outboxes);

        return *this;
    };

    ~CMailServer(void) {};

    void sendMail(const CMail &m) {
//        auto toAdd = CMail(m);
        inboxes.add(m);
        outboxes.add(m);
    };

    CMailIterator outbox(const char *email) const {
        size_t idx = 0;
        bool foundMatch = false;
        auto lowerInd = outboxes.findMail(email, idx, foundMatch);
        return CMailIterator(outboxes[lowerInd], foundMatch);

    };

    CMailIterator inbox(const char *email) const {
        size_t idx = 0;
        bool foundMatch = false;
        auto lowerInd = inboxes.findMail(email, idx, foundMatch);
        return CMailIterator(inboxes[lowerInd], foundMatch);
    };

    void printBoxes() {
        cout << "\n\n PRINTING BOXES:\n\n";
        cout << "---------INBOXES----------" << endl;
        for (size_t i = 0; i < inboxes.getLen(); i++) {
            cout << "[" << endl;
            for (size_t j=0; j<inboxes[i].getLen(); j++) {
                cout << inboxes[i][j] << endl;
            }
            cout << "]" << endl;
        }
        cout << "---------OUTBOXES----------" << endl;
        for (size_t i = 0; i < outboxes.getLen(); i++) {
            cout << "[" << endl;
            for (size_t j=0; j<outboxes[i].getLen(); j++) {
                cout << outboxes[i][j] << endl;
            }
            cout << "]" << endl;
        }
    }

private:
    // todo
    Box inboxes;  // sorted by to
    Box outboxes;  // sorted by from
};


#ifndef __PROGTEST__

bool matchOutput(const CMail &m,
                 const char *str) {
    ostringstream oss;
    oss << m;
    return oss.str() == str;
}

int main(void) {
    char from[100], to[100], body[1024];

    assert (CMail("john", "peter", "progtest deadline") == CMail("john", "peter", "progtest deadline"));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("john", "progtest deadline", "peter")));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("peter", "john", "progtest deadline")));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("peter", "progtest deadline", "john")));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("progtest deadline", "john", "peter")));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("progtest deadline", "peter", "john")));
    CMailServer s0;
    s0.sendMail(CMail("john", "peter", "some important mail"));
//    s0.printBoxes();

    strncpy(from, "john", sizeof(from));
    strncpy(to, "thomas", sizeof(to));
    strncpy(body, "another important mail", sizeof(body));
    s0.sendMail(CMail(from, to, body));
//    s0.printBoxes();

    strncpy(from, "john", sizeof(from));
    strncpy(to, "alice", sizeof(to));
    strncpy(body, "deadline notice", sizeof(body));
    s0.sendMail(CMail(from, to, body));
//    s0.printBoxes();

    s0.sendMail(CMail("alice", "john", "deadline confirmation"));
//    s0.printBoxes();

    s0.sendMail(CMail("peter", "alice", "PR bullshit"));
//    s0.printBoxes();
    CMailIterator i0 = s0.inbox("alice");
    assert (i0 && *i0 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i0, "From: john, To: alice, Body: deadline notice"));
    assert (++i0 && *i0 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i0, "From: peter, To: alice, Body: PR bullshit"));
    assert (!++i0);

    CMailIterator i1 = s0.inbox("john");
    assert (i1 && *i1 == CMail("alice", "john", "deadline confirmation"));
    assert (matchOutput(*i1, "From: alice, To: john, Body: deadline confirmation"));
    assert (!++i1);

    CMailIterator i2 = s0.outbox("john");
    assert (i2 && *i2 == CMail("john", "peter", "some important mail"));
    assert (matchOutput(*i2, "From: john, To: peter, Body: some important mail"));
    assert (++i2 && *i2 == CMail("john", "thomas", "another important mail"));
    assert (matchOutput(*i2, "From: john, To: thomas, Body: another important mail"));
    assert (++i2 && *i2 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i2, "From: john, To: alice, Body: deadline notice"));
    assert (!++i2);

    CMailIterator i3 = s0.outbox("thomas");
    assert (!i3);

    CMailIterator i4 = s0.outbox("steve");
    assert (!i4);

    CMailIterator i5 = s0.outbox("thomas");
    s0.sendMail(CMail("thomas", "boss", "daily report"));
    assert (!i5);

    CMailIterator i6 = s0.outbox("thomas");
    assert (i6 && *i6 == CMail("thomas", "boss", "daily report"));
    assert (matchOutput(*i6, "From: thomas, To: boss, Body: daily report"));
    assert (!++i6);

    CMailIterator i7 = s0.inbox("alice");
    s0.sendMail(CMail("thomas", "alice", "meeting details"));
    assert (i7 && *i7 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i7, "From: john, To: alice, Body: deadline notice"));
    assert (++i7 && *i7 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i7, "From: peter, To: alice, Body: PR bullshit"));
    assert (!++i7);

    CMailIterator i8 = s0.inbox("alice");
    assert (i8 && *i8 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i8, "From: john, To: alice, Body: deadline notice"));
    assert (++i8 && *i8 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i8, "From: peter, To: alice, Body: PR bullshit"));
    assert (++i8 && *i8 == CMail("thomas", "alice", "meeting details"));
    assert (matchOutput(*i8, "From: thomas, To: alice, Body: meeting details"));
    assert (!++i8);

    CMailServer s1(s0);
    s0.sendMail(CMail("joe", "alice", "delivery details"));
    s1.sendMail(CMail("sam", "alice", "order confirmation"));
    CMailIterator i9 = s0.inbox("alice");
    assert (i9 && *i9 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i9, "From: john, To: alice, Body: deadline notice"));
    assert (++i9 && *i9 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i9, "From: peter, To: alice, Body: PR bullshit"));
    assert (++i9 && *i9 == CMail("thomas", "alice", "meeting details"));
    assert (matchOutput(*i9, "From: thomas, To: alice, Body: meeting details"));
    assert (++i9 && *i9 == CMail("joe", "alice", "delivery details"));
    assert (matchOutput(*i9, "From: joe, To: alice, Body: delivery details"));
    assert (!++i9);

    CMailIterator i10 = s1.inbox("alice");
    assert (i10 && *i10 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i10, "From: john, To: alice, Body: deadline notice"));
    assert (++i10 && *i10 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i10, "From: peter, To: alice, Body: PR bullshit"));
    assert (++i10 && *i10 == CMail("thomas", "alice", "meeting details"));
    assert (matchOutput(*i10, "From: thomas, To: alice, Body: meeting details"));
    assert (++i10 && *i10 == CMail("sam", "alice", "order confirmation"));
    assert (matchOutput(*i10, "From: sam, To: alice, Body: order confirmation"));
    assert (!++i10);

    CMailServer s2;
    s2.sendMail(CMail("alice", "alice", "mailbox test"));
    CMailIterator i11 = s2.inbox("alice");
    assert (i11 && *i11 == CMail("alice", "alice", "mailbox test"));
    assert (matchOutput(*i11, "From: alice, To: alice, Body: mailbox test"));
    assert (!++i11);

    s2 = s0;
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s2.sendMail(CMail("paul", "alice", "invalid invoice"));
    CMailIterator i12 = s0.inbox("alice");
    assert (i12 && *i12 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i12, "From: john, To: alice, Body: deadline notice"));
    assert (++i12 && *i12 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i12, "From: peter, To: alice, Body: PR bullshit"));
    assert (++i12 && *i12 == CMail("thomas", "alice", "meeting details"));
    assert (matchOutput(*i12, "From: thomas, To: alice, Body: meeting details"));
    assert (++i12 && *i12 == CMail("joe", "alice", "delivery details"));
    assert (matchOutput(*i12, "From: joe, To: alice, Body: delivery details"));
    assert (++i12 && *i12 == CMail("steve", "alice", "newsletter"));
    assert (matchOutput(*i12, "From: steve, To: alice, Body: newsletter"));
    assert (!++i12);

    CMailIterator i13 = s2.inbox("alice");
    assert (i13 && *i13 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i13, "From: john, To: alice, Body: deadline notice"));
    assert (++i13 && *i13 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i13, "From: peter, To: alice, Body: PR bullshit"));
    assert (++i13 && *i13 == CMail("thomas", "alice", "meeting details"));
    assert (matchOutput(*i13, "From: thomas, To: alice, Body: meeting details"));
    assert (++i13 && *i13 == CMail("joe", "alice", "delivery details"));
    assert (matchOutput(*i13, "From: joe, To: alice, Body: delivery details"));
    assert (++i13 && *i13 == CMail("paul", "alice", "invalid invoice"));
    assert (matchOutput(*i13, "From: paul, To: alice, Body: invalid invoice"));
    assert (!++i13);

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
