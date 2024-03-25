#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>
using namespace std;


bool isCorrectCharacter(char c) {
    string findChars = "0123456789";

    for (char findChar : findChars) {
        if (c == findChar) {
            return true;
        }
    }

    return false;
}

bool isCorrectNumber(const string & str) {
    if (str.size() == 9 && str[0] != '0') {
        for (char num: str) {
            if (!isCorrectCharacter(num)) {
                return false;
            }
        }

        return true;
    }

    return false;
}

void findNames(const vector<vector<string> > & names, const string & word, ostream & out) {
    int foundNames = 0;

    for (long unsigned int i=0; i<names.size(); i++) {
        if (word == names[i][0] || word == names[i][1]) {
            out << names[i][0] << " " << names[i][1] << " " << names[i][2] << endl;
            foundNames++;
        }
    }

    out << "-> " << foundNames << endl;
}

bool report ( const string & fileName, ostream & out )
{
    // TODO
    fstream ifs(fileName, ios::in);
    vector<vector<string> > names;
    int wordCount;
    int inputEnd = 0;
    string line;
    string word;

    if (!ifs) {
        return false;
    }

    while (getline(ifs, line)) {
//        cout << line << endl;
        wordCount = 0;
        stringstream s(line);

        vector<string> name;

        while (s >> word) {
            if (inputEnd == 0) {
                if (wordCount < 3) {
                    name.push_back(word);
                }

                if (wordCount == 2) {
                    if (!isCorrectNumber(word)) {
                        return false;
                    }
                }
            }
            wordCount++;
        }
        if (wordCount == 1) {
            inputEnd = 1;
            findNames(names, word, out);
        }
        else if (wordCount == 3) {
            if (inputEnd == 1) {
                return false;
            }

            names.push_back(name);
        }
        else if (wordCount != 0) {
            return false;
        }
    }

  return true;
}


int main ()
{
//    ostringstream oss;
//    report( "tests/test0_in.txt", oss );

    ostringstream oss;
    oss . str ( "" );
//    assert ( report( "tests/test0_in.txt", oss ) == true );
//    assert ( oss . str () ==
//        "John Christescu 258452362\n"
//        "John Harmson 861647702\n"
//        "-> 2\n"
//        "-> 0\n"
//        "Josh Dakhov 264112084\n"
//        "Dakhov Speechley 865216101\n"
//        "-> 2\n"
//        "John Harmson 861647702\n"
//        "-> 1\n" );
//    oss . str ( "" );
//    assert ( report( "tests/test1_in.txt", oss ) == false );
    assert ( report( "tests/test2_in.txt", oss ) == false );

    return 0;
}
