#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <iterator>
#include <functional>

using namespace std;

class CDate {
public:
    //---------------------------------------------------------------------------------------------
    CDate(int y,
          int m,
          int d)
            : m_Year(y),
              m_Month(m),
              m_Day(d) {
    }

    //---------------------------------------------------------------------------------------------
    int compare(const CDate &x) const {
        if (m_Year != x.m_Year)
            return m_Year - x.m_Year;
        if (m_Month != x.m_Month)
            return m_Month - x.m_Month;
        return m_Day - x.m_Day;
    }

    //---------------------------------------------------------------------------------------------
    int year(void) const {
        return m_Year;
    }

    //---------------------------------------------------------------------------------------------
    int month(void) const {
        return m_Month;
    }

    //---------------------------------------------------------------------------------------------
    int day(void) const {
        return m_Day;
    }

    //---------------------------------------------------------------------------------------------
    friend ostream &operator<<(ostream &os,
                               const CDate &x) {
        char oldFill = os.fill();
        return os << setfill('0') << setw(4) << x.m_Year << "-"
                  << setw(2) << static_cast<int> ( x.m_Month ) << "-"
                  << setw(2) << static_cast<int> ( x.m_Day )
                  << setfill(oldFill);
    }
    //---------------------------------------------------------------------------------------------
private:
    int16_t m_Year;
    int8_t m_Month;
    int8_t m_Day;
};


string simplifyString(const string &name) {

    char * res = new char[name.size()+1];
    bool wasFirstCharacter = false;
    bool wasLastSpace = false;
    size_t resIndex = 0;
    for (char c : name) {
        if (c == ' ') {
            if (wasFirstCharacter && !wasLastSpace) {
                res[resIndex] = c;
                resIndex++;
            }
            wasLastSpace = true;
        }
        else {
            wasFirstCharacter = true;
            wasLastSpace = false;
            res[resIndex] = char(tolower(c));
            resIndex++;
        }
    }

    res[resIndex] = '\0';

    if (resIndex > 0 && res[resIndex-1] == ' ') {
        res[resIndex-1] = '\0';
    }
    string finalRes = res;
    delete [] res;

    return finalRes;
}

class CInvoice {
public:
    CInvoice() : Cdate(CDate(0, 0, 0)) {};
    CInvoice(const CDate &date,
             const string &seller,
             const string &buyer,
             unsigned int amount,
             double vat): Cdate(date), Cseller(seller), Cbuyer(buyer),
                          simplifiedSeller(simplifyString(seller)), similifiedBuyer(simplifyString(buyer)),
                          Camount(amount), Cvat(vat), isIssued(false), isAccepted(false), position(0) {};

    CDate date(void) const {
        return Cdate;
    };

    string buyer(void) const {
        return Cbuyer;
    };

    string seller(void) const {
        return Cseller;
    };

    const string & getsimplifiedBuyer() const {
        return similifiedBuyer;
    };

    const string & getsimplifiedSeller() const {
        return simplifiedSeller;
    };

    void setBuyer(const string & newBuyer) {
        Cbuyer = newBuyer;
    }

    void setSeller(const string & newSeller) {
        Cseller = newSeller;
    }

    unsigned int amount(void) const {
        return Camount;
    };

    double vat(void) const {
        return Cvat;
    };

    void setIsIssued(bool isIssuedInp) {
        isIssued = isIssuedInp;
    }

    void setPosition(size_t positionInp) {
        position = positionInp;
    }

    void setIsAccepted(bool isAcceptedInp) {
        isAccepted = isAcceptedInp;
    }

    bool getIsIssued() const {
        return isIssued;
    }

    size_t getPosition() const {
        return position;
    }

    bool getIsAccepted() const {
        return isAccepted;
    }

    string getInvoiceHash() const {

        return to_string(Cdate.year()) + to_string(Cdate.month()) + to_string(Cdate.day()) +
               simplifyString(Cseller) + simplifyString(Cbuyer) + to_string(Camount) + to_string(Cvat);
    }

    friend ostream& operator<< (ostream& os, const CInvoice & inv) {
        os << inv.date() << ", " << inv.seller() << ", " << inv.buyer() << ", " << inv.amount() << ", " << inv.vat();
        os << ", is issued:" << inv.getIsIssued() << ", is accepted:" << inv.getIsAccepted() << ", position: " << inv.getPosition();
        return os;
    }

private:
    // todo
    CDate Cdate;
    string Cseller;
    string Cbuyer;
    string simplifiedSeller;
    string similifiedBuyer;
    unsigned int Camount;
    double Cvat;
    bool isIssued;
    bool isAccepted;
    size_t position;
};

class CSortOpt {
public:
    static const int BY_DATE = 0;
    static const int BY_BUYER = 1;
    static const int BY_SELLER = 2;
    static const int BY_AMOUNT = 3;
    static const int BY_VAT = 4;

    CSortOpt(void) {};

    CSortOpt &addKey(int key,
                     bool ascending = true) {
        if (idx < 5 && key >= 0 && key <= 4) {
            config[idx][0] = key;
            config[idx][1] = ascending;
        }
        idx++;

        return *this;
    };

    bool ascCond(bool expression, bool isAsc) {
        if (isAsc) {
            return expression;
        }
        return !expression;
    }

    // 1 = a is bigger
    int cmp(const string& a, const string& b)
    {
        for (size_t i=0; i < a.size(); i++) {
            if (tolower(a[i]) > tolower(b[i]))
                return 1;
            if (tolower(a[i]) < tolower(b[i])) {
                return -1;
            }
        }
        if (a.size() > b.size()) {
            return 1;
        }
        if (a.size() < b.size()) {
            return -1;
        }
        return 0;
    }

    // allowing this class to be used in the sort
    bool operator ()(const CInvoice &a, const CInvoice &b) {
        // sort by conditions
        for (size_t i=0; i<idx; i++) {
            switch (config[i][0]) {
                case 0:
                    if (a.date().compare(b.date()) != 0) return ascCond(a.date().compare(b.date()) < 0, config[i][1]);
                    break;
                case 1:
                    if (cmp(a.buyer(), b.buyer()) != 0) return ascCond(cmp(a.buyer(), b.buyer()) < 0, config[i][1]);
                    break;
                case 2:
                    if (cmp(a.seller(), b.seller()) != 0) return ascCond(cmp(a.seller(), b.seller()) < 0, config[i][1]);
                    break;
                case 3:
                    if (a.amount() != b.amount()) return ascCond(a.amount() < b.amount(), config[i][1]);
                    break;
                case 4:
                    if (a.vat() != b.vat()) return ascCond(a.vat() < b.vat(), config[i][1]);
                    break;
            }
        }

        // if still equal, sort by position
        return a.getPosition() < b.getPosition();
    }

private:
    // todo
    int config[5][2];
    size_t idx = 0;
};

template <typename T>
void printInvoiceList(const T &a) {
    cout << "[" << endl;
    for (const auto &inv : a) {
        cout << "    [" << inv << "]" << " hash: '" << inv.getInvoiceHash() << "'" << endl;
    }
    cout << "]" << endl;
}

class CVATRegister {
public:
    CVATRegister(): positionCounter(0) {};

    bool registerCompany(const string &name) {
        string simpleString = simplifyString(name);
        if (companies.find(simpleString) != companies.end()) {
            return false;
        }
        companies[simpleString] = name;
        return true;
    };

    // returns an index to an invoice if one is found, else returns -1
    bool findInvoice(const CInvoice &x, string & invoiceHash) {
        auto invoiceFound = invoices.find(invoiceHash);
        if (invoiceFound == invoices.end()) {
            return false;
        }

        return true;
    }

    bool addContract(const CInvoice &x, bool isIssued) {

        if (x.getsimplifiedBuyer() == x.getsimplifiedSeller()) {
            return false;
        }
        if (companies.find(x.getsimplifiedSeller()) == companies.end()) {
            return false;
        }
        if (companies.find(x.getsimplifiedBuyer()) == companies.end()) {
            return false;
        }

        string invoiceHash = x.getInvoiceHash();
        bool invoiceFound = findInvoice(x, invoiceHash);

        if (!invoiceFound) {
            CInvoice newInvoice = x;
            isIssued ? newInvoice.setIsIssued(true) : newInvoice.setIsAccepted(true);
            newInvoice.setPosition(positionCounter);
            newInvoice.setBuyer(companies[x.getsimplifiedBuyer()]);
            newInvoice.setSeller(companies[x.getsimplifiedSeller()]);
            positionCounter++;
            invoices[invoiceHash] = newInvoice;
            companyHashes[x.getsimplifiedBuyer()].insert(invoiceHash);
            companyHashes[x.getsimplifiedSeller()].insert(invoiceHash);
            return true;
        }
        // we have a duplicate issued
        if (isIssued && invoices[invoiceHash].getIsIssued()) {
            return false;
        }
        if (!isIssued && invoices[invoiceHash].getIsAccepted()) {
            return false;
        }
        isIssued ? invoices[invoiceHash].setIsIssued(true) : invoices[invoiceHash].setIsAccepted(true);
        return true;
    }

    bool addIssued(const CInvoice &x) {
        return addContract(x, true);
    };

    bool addAccepted(const CInvoice &x) {
        return addContract(x, false);
    };

    bool delInvoice(const CInvoice &x, bool isIssued) {
        string invoiceHash = x.getInvoiceHash();
        bool invoiceFound = findInvoice(x, invoiceHash);

        if (!invoiceFound || (isIssued && !invoices[invoiceHash].getIsIssued()) ||
            (!isIssued && !invoices[invoiceHash].getIsAccepted())) {
            return false;
        }

        if (isIssued && invoices[invoiceHash].getIsAccepted()) {
            invoices[invoiceHash].setIsIssued(false);
        }
        else if (!isIssued && invoices[invoiceHash].getIsIssued()) {
            invoices[invoiceHash].setIsAccepted(false);
        }
        else {
            companyHashes[invoices[invoiceHash].getsimplifiedBuyer()].erase(invoiceHash);
            companyHashes[invoices[invoiceHash].getsimplifiedSeller()].erase(invoiceHash);
            invoices.erase(invoiceHash);
        }

        return true;
    }

    bool delIssued(const CInvoice &x) {
        return delInvoice(x, true);
    };

    bool delAccepted(const CInvoice &x) {
        return delInvoice(x, false);
    };

    list<CInvoice> unmatched(const string &company,
                             const CSortOpt &sortBy) const {
        vector<CInvoice> unmatchedInvoices;
        string simplifiedCompany = simplifyString(company);

        if (companyHashes.find(simplifiedCompany) == companyHashes.end()) {
            return {};
        }

        for (auto const& invoiceHash : companyHashes.at(simplifiedCompany)) {
            // we cant have a match in this vector anymore
            auto val = invoices.at(invoiceHash);
            if ((val.getIsIssued() && !val.getIsAccepted()) || (!val.getIsIssued() && val.getIsAccepted())) {
                unmatchedInvoices.push_back(val);
            }
        };

        sort(unmatchedInvoices.begin(), unmatchedInvoices.end(), sortBy);

        return {unmatchedInvoices.begin(), unmatchedInvoices.end()};
    }

//    friend ostream & operator<<(ostream &os, const CVATRegister & reg) {
//        os << "{" << endl;
//        for (auto const& [key, val] : reg.invoices) {
//            os << "  '" << key << "': " << "'";
//            cout << "  [" << endl;
//            for (const auto &inv : val) {
//                cout << "      [" << inv << "]" << endl;
//            }
//            cout << "  ]," << endl;
//        }
//        os << "{" << endl;
//        return os;
//    }

private:
    // todo
    map<string, string> companies;
    map<string, CInvoice> invoices;
    map<string, set<string>> companyHashes;
    size_t positionCounter;

};


void printInvoiceMap(const map<string, vector<CInvoice>> &invoices) {
    cout << "{" << endl;
    for (auto const& [key, val] : invoices) {
        cout << "  '" << key << "': " << "'";
        printInvoiceList(val);
    }
    cout << "{" << endl;
}

bool isInvEqual(const CInvoice & a, const CInvoice & b) {
    return (a.date().compare(b.date()) == 0 && a.seller() == b.seller() && a.buyer() == b.buyer() &&
            a.amount() == b.amount() && a.vat() == b.vat());
}

bool equalLists(const list<CInvoice> &a, const list<CInvoice> &b) {
    // todo
    if (a.size() != b.size()) {
        return false;
    }
    auto it1 = a.begin();
    auto it2 = b.begin();
    while (it1 != a.end() && it2 != b.end()) {
        if (!(isInvEqual(*it1, *it2))) {
            return false;
        }
        it1++;
        it2++;
    }

    return true;
}

int main(void) {

    CVATRegister r;
    assert (r.registerCompany("first Company"));
    assert (r.registerCompany("Second     Company"));
    assert (r.registerCompany("ThirdCompany, Ltd."));
    assert (r.registerCompany("Third Company, Ltd."));
    assert (!r.registerCompany("Third Company, Ltd."));
    assert (!r.registerCompany(" Third  Company,  Ltd.  "));
    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 100, 20)));
    assert (r.addIssued(CInvoice(CDate(2000, 1, 2), "FirSt Company", "Second Company ", 200, 30)));
    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 100, 30)));
    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 300, 30)));
    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "Second Company ", "First Company", 300, 30)));
    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "Third  Company,  Ltd.", "  Second    COMPANY ", 400, 34)));
    assert (!r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 300, 30)));
    assert (!r.addIssued(CInvoice(CDate(2000, 1, 4), "First Company", "First   Company", 200, 30)));
    assert (!r.addIssued(CInvoice(CDate(2000, 1, 4), "Another Company", "First   Company", 200, 30)));
    assert (equalLists(r.unmatched("First Company",
                                   CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER,
                                                                                       false).addKey(CSortOpt::BY_DATE,
                                                                                                     false)),
                       list<CInvoice>
                               {
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                20.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                30.000000)
                               }));
    assert (equalLists(r.unmatched("First Company",
                                   CSortOpt().addKey(CSortOpt::BY_DATE, true).addKey(CSortOpt::BY_SELLER, true).addKey(
                                           CSortOpt::BY_BUYER, true)),
                       list<CInvoice>
                               {
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                20.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
                                                30.000000)
                               }));
    assert (equalLists(r.unmatched("First Company",
                                   CSortOpt().addKey(CSortOpt::BY_VAT, true).addKey(CSortOpt::BY_AMOUNT, true).addKey(
                                           CSortOpt::BY_DATE, true).addKey(CSortOpt::BY_SELLER, true).addKey(
                                           CSortOpt::BY_BUYER, true)),
                       list<CInvoice>
                               {
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                20.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                30.000000)
                               }));

    assert (equalLists(r.unmatched("First Company", CSortOpt()),
                       list<CInvoice>
                               {
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                20.000000),
                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                30.000000)
                               }));

    assert (equalLists(r.unmatched("second company", CSortOpt().addKey(CSortOpt::BY_DATE, false)),
                       list<CInvoice>
                               {
                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                20.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Third Company, Ltd.", "Second     Company", 400,
                                                34.000000)
                               }));
    assert (equalLists(r.unmatched("last company", CSortOpt().addKey(CSortOpt::BY_VAT, true)),
                       list<CInvoice>
                               {
                               }));
    assert (r.addAccepted(CInvoice(CDate(2000, 1, 2), "First Company", "Second Company ", 200, 30)));
    assert (r.addAccepted(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
    assert (r.addAccepted(CInvoice(CDate(2000, 1, 1), "Second company ", "First Company", 300, 32)));
    assert (equalLists(r.unmatched("First Company",
                                   CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true).addKey(
                                           CSortOpt::BY_DATE, true)),
                       list<CInvoice>
                               {
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                20.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                32.000000)
                               }));
    assert (!r.delIssued(CInvoice(CDate(2001, 1, 1), "First Company", "Second Company ", 200, 30)));
    assert (!r.delIssued(CInvoice(CDate(2000, 1, 1), "A First Company", "Second Company ", 200, 30)));
    assert (!r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Hand", 200, 30)));
    assert (!r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company", 1200, 30)));
    assert (!r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company", 200, 130)));
    assert (r.delIssued(CInvoice(CDate(2000, 1, 2), "First Company", "Second Company", 200, 30)));
    assert (equalLists(r.unmatched("First Company",
                                   CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true).addKey(
                                           CSortOpt::BY_DATE, true)),
                       list<CInvoice>
                               {
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                20.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                32.000000)
                               }));
    assert (r.delAccepted(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
    assert (equalLists(r.unmatched("First Company",
                                   CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true).addKey(
                                           CSortOpt::BY_DATE, true)),
                       list<CInvoice>
                               {
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                20.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                32.000000)
                               }));
    assert (r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
    assert (equalLists(r.unmatched("First Company",
                                   CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true).addKey(
                                           CSortOpt::BY_DATE, true)),
                       list<CInvoice>
                               {
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                20.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                32.000000)
                               }));
    return EXIT_SUCCESS;
}
