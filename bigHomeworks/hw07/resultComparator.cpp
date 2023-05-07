#ifndef __PROGTEST__

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <iterator>
#include <stdexcept>

using namespace std;
#endif /* __PROGTEST__ */


long BIG_NUM = 2147483647;

template <typename T>
void printMatrix(vector<vector<T>> matrix) {
    cout << endl << "[";
    for (size_t i=0; i<matrix.size(); i++) {
        cout << "[";
        for (size_t j=0; j<matrix[i].size(); j++) {
            if (matrix[i][j] == BIG_NUM) {
                cout << "INF";
            }
            else {
                cout << matrix[i][j];
            }
            cout << ", ";
        }
        cout << endl;
    }
    cout << "]" << endl;
}

void printWins(const map<string, set<string>> & wins) {
    cout << endl << "{" << endl;
    for (auto & [key, val]: wins) {
        cout << key << ": " << "[";
        for (auto & v : val) {
            cout << v << ", ";
        }
        cout << "]" << endl;
    }
    cout << "}" << endl;
}

void printPairs(const map<string, int> &pairs) {
    cout << endl << "{";
    for (auto &[key, val] : pairs) {
        cout << key << ": " << val << ", ";
    }
    cout << "}" << endl;
}


string separator = ";;||\\*-*/!~";

template<typename M_>
class CContest {
public:
    // default constructor
    CContest() {};
    // destructor
    ~CContest() {};
    // addMatch ( contestant1, contestant2, result )
    CContest & addMatch(const string & cont1, const string & cont2, const M_ &result){
        if (matches.find(cont1 + separator + cont2) != matches.end()
            || matches.find(cont2 + separator + cont1) != matches.end() ) {
            throw logic_error("");
        }
        matches[cont1 + separator + cont2] = pairs(cont1, cont2, result).clone();

        return *this;
    }

    template<class T>
    void getGraphMap(map<string, set<string>> &wins, T func) const {

        for (auto & [str, pair] : matches) {
            int res = func(pair->toEval);
            string winner;
            string loser;
            if (res > 0) {
                winner = pair->left;
                loser = pair->right;
            }
            else if (res < 0){
                winner = pair->right;
                loser = pair->left;
            }
            else {
                continue;
            }
            wins[winner].insert(loser);
        }
    }

    void getStringPairs(map<string, int> &pairs, map<int, string> &reversePairs, const map<string, set<string>> &wins) const {
        size_t counter = 0;

        for (const auto & [key, val] : wins) {
            if (pairs.find(key) == pairs.end()) {
                pairs[key] = counter;
                counter++;
            }
            for (const auto & name2 : val) {
                if (pairs.find(name2) == pairs.end()) {
                    pairs[name2] = counter;
                    counter++;
                }
            }
        }

        for (const auto & [key, val] : pairs) {
            reversePairs[val] = key;
        }
    }

    void convertToMatrix(vector<vector<long>> &matrix, map<string, int> &pairs, map<string, set<string>> &wins) const {

        vector<vector<int>> matrixHelper;
        size_t vertices = pairs.size();
        matrixHelper.resize(vertices);
        for (auto &[key, num] : pairs) {
            if (wins.find(key) != wins.end()) {
                for (auto & loser : wins[key]) {
                    matrixHelper[num].push_back(pairs[loser]);
                }
            }
        }

//        printMatrix(matrixHelper);

        matrix.resize(vertices);
        for (size_t i=0; i<pairs.size(); i++) {
            matrix[i].resize(vertices);
        }

        for (size_t i=0; i<vertices; i++) {
            for (size_t j=0; j<vertices; j++) {
                matrix[i][j] = BIG_NUM;
            }
        }

        for (size_t i=0; i<vertices; i++) {
            for (int j: matrixHelper[i]) {
                matrix[i][j] = 1;
            }
        }
    }

    void populateMatrix(vector<vector<long>> &matrix, size_t vertices) const {
        for (size_t k=0; k < vertices; k++) {
            for (size_t i=0; i < vertices; i++) {
                for (size_t j=0; j < vertices; j++) {
                    if (matrix[i][k] + matrix[k][j] < matrix[i][j])
                        matrix[i][j] = matrix[i][k] + matrix[k][j];
                }
            }
        }
    }

    void populateResultVec(vector<pair<string, int>> &resultVec, map<string, int> &pairs, vector<vector<long>> &matrix, map<int, string> &reversePairs) const {
        resultVec.resize(pairs.size());
        for (size_t i=0; i<matrix.size(); i++) {
            int nonInfCounter = 0;
            for (size_t j=0; j<matrix[i].size(); j++) {
                if (matrix[i][j] != BIG_NUM) {
                    nonInfCounter++;
                }
            }
            resultVec[i].first = reversePairs[i];
            resultVec[i].second = nonInfCounter;
        }
    }

    static bool sortComp(pair<string, int>& a, pair<string, int>& b)
    {
        return a.second > b.second;
    }

    bool getResult(list<string> & res, vector<pair<string, int>> &resultVec, bool alsoInsert=true) const {
        sort(resultVec.begin(), resultVec.end(), sortComp);
        int curCounter = resultVec.size()-1;
        for (size_t i=0; i<resultVec.size(); i++) {
            if (resultVec[i].second != curCounter) {
                return false;
            }
            curCounter--;
            if (alsoInsert) {
                res.push_back(resultVec[i].first);
            }
        }

        return true;
    }
    template<class T>
    bool isOrdered(const T &func) const {
        map<string, set<string>> wins;
        getGraphMap(wins, func);
//        printWins(wins);

        map<string, int> pairs;
        map<int, string> reversePairs;
        getStringPairs(pairs, reversePairs, wins);
//        printPairs(pairs);

        size_t vertices = pairs.size();

        vector<vector<long>> matrix;
        convertToMatrix(matrix, pairs, wins);
//        printMatrix(matrix);

        populateMatrix(matrix, vertices);

//        printMatrix(matrix);

        vector<pair<string, int>> resultVec;
        populateResultVec(resultVec, pairs, matrix, reversePairs);

        list<string> res;
        return getResult(res, resultVec, false);
    }
    template<class T>
    list<string> results(const T &func) const {
        map<string, set<string>> wins;
        getGraphMap(wins, func);

        map<string, int> pairs;
        map<int, string> reversePairs;
        getStringPairs(pairs, reversePairs, wins);

        size_t vertices = pairs.size();

        vector<vector<long>> matrix;
        convertToMatrix(matrix, pairs, wins);

        populateMatrix(matrix, vertices);

        vector<pair<string, int>> resultVec;
        populateResultVec(resultVec, pairs, matrix, reversePairs);

        list<string> res;
        if (!getResult(res, resultVec)) {
            throw logic_error("");
        }

        return res;
    }

private:
    struct pairs {
        pairs(const string &left, const string &right, const M_ & res): left(left), right(right), toEval(res), res(0) {
        }

        pairs & getCopy(const pairs & pair) {
            left = pair.left;
            right = pair.right;
            toEval = pair.toEval;
            res = pair.res;
            return *this;
        }

        [[nodiscard]] shared_ptr<pairs> clone() const {
            return make_shared<pairs>(*this);
        }

        string left;
        string right;
        M_ toEval;
        int res;
    };
    // todo
    map<string, shared_ptr<pairs>> matches;
};

#ifndef __PROGTEST__

struct CMatch {
public:
    CMatch(int a,
           int b)
            : m_A(a),
              m_B(b) {
    }

    int m_A;
    int m_B;
};

class HigherScoreThreshold {
public:
    HigherScoreThreshold(int diffAtLeast)
            : m_DiffAtLeast(diffAtLeast) {
    }

    int operator()(const CMatch &x) const {
        return (x.m_A > x.m_B + m_DiffAtLeast) - (x.m_B > x.m_A + m_DiffAtLeast);
    }

private:
    int m_DiffAtLeast;
};

int HigherScore(const CMatch &x) {
    return (x.m_A > x.m_B) - (x.m_B > x.m_A);
}

int main(void) {
    CContest<CMatch> x;

    x.addMatch("C++", "Pascal", CMatch(10, 3))
            .addMatch("C++", "Java", CMatch(8, 1))
            .addMatch("Pascal", "Basic", CMatch(40, 0))
            .addMatch("Java", "PHP", CMatch(6, 2))
            .addMatch("Java", "Pascal", CMatch(7, 3))
            .addMatch("PHP", "Basic", CMatch(10, 0));

    assert (!x.isOrdered(HigherScore));
    try {
        list<string> res = x.results(HigherScore);
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }

    x.addMatch("PHP", "Pascal", CMatch(3, 6));

    assert (x.isOrdered(HigherScore));
    try {
        list<string> res = x.results(HigherScore);
        assert ((res == list<string>{"C++", "Java", "Pascal", "PHP", "Basic"}));
    }
    catch (...) {
        assert ("Unexpected exception!" == nullptr);
    }


    assert (!x.isOrdered(HigherScoreThreshold(3)));
    try {
        list<string> res = x.results(HigherScoreThreshold(3));
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }

    assert (x.isOrdered([](const CMatch &x) {
        return (x.m_A < x.m_B) - (x.m_B < x.m_A);
    }));
    try {
        list<string> res = x.results([](const CMatch &x) {
            return (x.m_A < x.m_B) - (x.m_B < x.m_A);
        });
        assert ((res == list<string>{"Basic", "PHP", "Pascal", "Java", "C++"}));
    }
    catch (...) {
        assert ("Unexpected exception!" == nullptr);
    }

    CContest<bool> y;

    y.addMatch("Python", "PHP", true)
            .addMatch("PHP", "Perl", true)
            .addMatch("Perl", "Bash", true)
            .addMatch("Bash", "JavaScript", true)
            .addMatch("JavaScript", "VBScript", true);

    assert (y.isOrdered([](bool v) {
        return v ? 10 : -10;
    }));
    try {
        list<string> res = y.results([](bool v) {
            return v ? 10 : -10;
        });
        assert ((res == list<string>{"Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript"}));
    }
    catch (...) {
        assert ("Unexpected exception!" == nullptr);
    }

    y.addMatch("PHP", "JavaScript", false);
    assert (!y.isOrdered([](bool v) {
        return v ? 10 : -10;
    }));
    try {
        list<string> res = y.results([](bool v) {
            return v ? 10 : -10;
        });
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }

    try {
        y.addMatch("PHP", "JavaScript", false);
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }

    try {
        y.addMatch("JavaScript", "PHP", true);
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
