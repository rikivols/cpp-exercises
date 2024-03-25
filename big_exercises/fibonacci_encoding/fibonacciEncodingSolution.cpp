#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>


using namespace std;


bool debug = false;


void print(const string &s) {
    if (debug)
        cout << s;
}

char binaryToDecimal(const string &n)
{
    string num = n;

    // Stores the decimal value
    int dec_value = 0;

    // Initializing base value to 1
    int base = 1;

    int len = num.length();
    for (int i = len - 1; i >= 0; i--) {

        // If the current bit is 1
        if (num[i] == '1')
            dec_value += base;
        base = base * 2;
    }

    // Return answer
    return (char)dec_value;
}


string addPadding(const string& s) {
    return string(8 - s.size(), '0') + s;
}


string charToBinaryString(char num)
{
    vector<bool> resBool;
    string res;

    int numConv = int((unsigned char)num);
    if (numConv < 0) {
        numConv = 256 + num;
    }

    // Convert ASCII value to binary
    while (numConv > 0)
    {
        (numConv % 2) ? res.push_back('1') :
        res.push_back('0');
        numConv /= 2;
    }
    reverse(res.begin(), res.end());

    return addPadding(res);
}


long toLong(const string &s)
{
    long n = 0;

    for (char i : s) {
        n <<= 1;
        n |= i - '0';
    }

    return n;
}


// -2 = we need another part of the string
// -1 = wrongly encoded
long getUtfNum(const string & s) {
    bool twoBytesStart = s[0] == '1' && s[1] == '1' && s[2] == '0';
    bool threeBytesStart = s[0] == '1' && s[1] == '1' && s[2] == '1' && s[3] == '0';
    bool fourBytesStart = s[0] == '1' && s[1] == '1' && s[2] == '1' && s[3] == '1' && s[4] == '0';

    if (s.size() == 8) {
        if (s[0] == '0') {
            return toLong(s.substr(1, 7));
        }
        else if (twoBytesStart || threeBytesStart || fourBytesStart)
        {
            return -2;
        }
    }
    else if (s.size() == 16) {
        if (twoBytesStart) {
            if (s[8] == '1' && s[9] == '0') {
                return toLong(s.substr(3, 5) + s.substr(10, 6));
            }
        }
        else if (threeBytesStart || fourBytesStart) {
            return -2;
        }
    }
    else if (s.size() == 24) {
        if (threeBytesStart) {
            if (s[8] == '1' && s[9] == '0' && s[16] == '1' && s[17] == '0') {
                return toLong(s.substr(4, 4) + s.substr(10, 6) + s.substr(18, 6));
            }
        }
        else if (fourBytesStart) {
            return -2;
        }
    }
    else if (s.size() == 32) {
        if (fourBytesStart) {
            if (s[8] == '1' && s[9] == '0' && s[16] == '1' && s[17] == '0' && s[24] == '1' && s[25] == '0') {
                return toLong(s.substr(5, 3) + s.substr(10, 6) + s.substr(18, 6) + s.substr(26, 6));
            }
        }
    }

    return -1;
}


void generateAllFibs(vector<long> &vec, long num) {
    int i = 1;
    long curVal;
    vec.push_back(1);
    if (num <= 1) {
        return;
    }

    vec.push_back(2);

    while (true) {
        i++;
        curVal = vec[i - 1] + vec[i - 2];

        if (curVal <= num)
            vec.push_back(curVal);
        else
            break;
    }
}


void generateAllFibsUpTo(vector<long> &vec, long num) {
    long curVal;
    vec.push_back(1);
    if (num <= 1) {
        return;
    }

    vec.push_back(2);

    for (int i=2; i<num; i++) {
        curVal = vec[i - 1] + vec[i - 2];
        vec.push_back(curVal);
    }
}


template <typename T>
void printVector(vector<T> &vec) {
    for (unsigned long i=0; i<vec.size(); i++) {
        print(to_string(vec[i]) + " ");
    }
    print("\n");
}



string convertToFibonacci(long num) {
    vector<long> allFibs;
    generateAllFibs(allFibs, num);
    print("all fibs: \n");
    printVector(allFibs);

    string res = string(allFibs.size(), '0');
    long curSum = num;
    unsigned long lastIndex = allFibs.size() + 100;


    while (curSum) {
        for (long i=(long)allFibs.size()-1; i >= 0; i--) {
//            cout << "i: " << i << endl;
//            cout << "i: " << i << " last index: " << lastIndex << " curSum: " << curSum << " allFibs[i]: " << allFibs[i] << endl;
            if (i + 1 < (long)lastIndex && allFibs[i] <= curSum) {
//                cout << "happened";
                res[i] = '1';
                curSum -= allFibs[i];
                lastIndex = i;
            }
        }
    }

    return res;

}

// returns string that is going to be written to output and string that remains stores
string stringToWrite(const string & newString, const string & remainderString, vector<string> &binaryToWrite) {
    string combined = remainderString + newString + '1';

    for (unsigned long i=0; i<combined.size(); i++) {
        if (i % 8 == 7) {
            string final = combined.substr(i-7, 8);
            reverse(final.begin(), final.end());
            binaryToWrite.push_back(final);
        }
    }

    string remainder;

    if (combined.size() % 8 != 0) {
        remainder = combined.substr(combined.size() - (combined.size() % 8), combined.size() % 8);
    }

    return remainder;
}


bool utf8ToFibonacci( const char * inFile, const char * outFile )
{
  // todo
    ifstream ifs ( inFile, ios::in | ios::binary );
    ofstream ofs (outFile, ios::out | ios::binary);

    if ( ! ifs || !ofs )
    {
//        cout << "Cannot open " << inFile << endl;
        return false;
    }

    string curUtf;
    string remainderBinary;  // string that remained after splitting bits to 8-groups
//    cout << "------------" << endl;
//    string allWrittenBinary;
    long utfNum = -1;

    for ( char c; ifs.get(c) ; )
    {
//        cout << (int)c << endl;
        if (!ofs || !ifs) {
            return false;
        }

//        cout << "------------" << endl;
        string bitsString = charToBinaryString(c);
//        cout << "bitstring: " << bitsString << endl;
        curUtf += bitsString;

        utfNum = getUtfNum(curUtf);
//        cout << "utf num: " << utfNum << endl;

        if (utfNum == -1 || utfNum < -2 || utfNum > 1114111) {
//            cout << "WRONG UTF NUM" << endl;
            return false;
        }
        else if (utfNum != -2) {
//            cout << "OUR UTF NUM:" << utfNum << endl;
            string fibBinary = convertToFibonacci(utfNum+1);

//            cout << "fibonacci: " << fibBinary << endl;

            vector<string> binaryToWrite;
            remainderBinary = stringToWrite(fibBinary, remainderBinary, binaryToWrite);

//            cout << "Binary to write: ";
//            printVector(binaryToWrite);
//            cout << "Binary remainder: " << remainderBinary << endl;

            for (unsigned long i=0; i<binaryToWrite.size(); i++) {
//                allWrittenBinary += binToH(binaryToWrite[i]) + " ";
                ofs.put(binaryToDecimal(binaryToWrite[i]));
            }

            curUtf = "";
//            cout << "------------" << endl;
        }
    }

    // last utf8 number failed to load
    if (utfNum == -2) {
        return false;
    }

    // fill remainder with zeros
    if (!remainderBinary.empty()) {
        unsigned long remainderSize = remainderBinary.size();
        for (unsigned long i=0; i < 8 - remainderSize; i++) {
            remainderBinary += '0';
        }
        reverse(remainderBinary.begin(), remainderBinary.end());

        if (!ofs) {
            return false;
        }
        ofs.put(binaryToDecimal(remainderBinary));
//        cout << "Binary to write remaining: " << remainderBinary << endl;
//        allWrittenBinary += binToH(remainderBinary);
    }
//    cout << "ALL WRITTEN BINARY: " << allWrittenBinary << endl;
    return true;
}

long convertFibsToNum(const string &fibs) {
    vector<long> allFibs;
    generateAllFibsUpTo(allFibs, (long)fibs.size());

    long res = 0;

    for (unsigned long i=0; i<fibs.size(); i++) {
        if (fibs[i] == '1') {
            res += allFibs[i];
        }
    }

    res -= 1;

    if (res < 0 || res > 1114111) {
        return -1;
    }
    return res;
}


// '-1' - wrong
string getUtfsFromFib(const string & s, bool &stringStart, vector<long> &utfs) {
    string curStr;
    bool justStopped = false;

    for (unsigned long i=0; i<s.size(); i++) {
        if (s[i] == '1') {
            stringStart = true;
        }

        // check for the string end
        if (i != 0 && (s[i] == '1' && s[i - 1] == '1') && !justStopped) {
//            cout << "cur str: " << curStr << endl;
            long decNum = convertFibsToNum(curStr);
//            cout << "dec num: " << decNum << endl;
            utfs.push_back(decNum);
            if (decNum == -1) {
                return "-1";
            }
            curStr = "";
            stringStart = false;
            justStopped = true;
        }
        else {
            curStr += s[i];

            // overflows
            if (curStr.size() > 30) {
                return "-1";
            }
            justStopped = false;
        }
    }

    return curStr;
}

string decimalToBinary(long num) {
    vector<bool> resBool;
    string res;

    for(int i=0; num>0; i++)
    {
        resBool.push_back(num % 2);
        num = num / 2;
    }

    for (int i=resBool.size()-1; i>=0; i--) {
        if (resBool[i])
            res += '1';
        else
            res += '0';
    }

    return res;
}

string convertNumToUtfBin(long num) {
    string res;

    if (num < 0) {
        return "-1";
    }
    else if (num < 128) {
        string numInBin = decimalToBinary(num);
        res = string(8 - numInBin.size(), '0');
        res += numInBin;
    }
    else if (num < 2048) {
        string numInBin = decimalToBinary(num);
        numInBin = string(11 - numInBin.size(), '0') + numInBin;  // add zeros to the beginning
        res = "110" + numInBin.substr(0, 5) + "10" + numInBin.substr(5, numInBin.size() - 5);
    }
    else if (num < 65536) {
        string numInBin = decimalToBinary(num);
        numInBin = string(16 - numInBin.size(), '0') + numInBin;  // add zeros to the beginning
        res = "1110" + numInBin.substr(0, 4) + "10" + numInBin.substr(4, 6) + "10";
        res += numInBin.substr(10, numInBin.size() - 10);
    }
    else if (num < 1114112) {
        string numInBin = decimalToBinary(num);
        numInBin = string(21 - numInBin.size(), '0') + numInBin;  // add zeros to the beginning
        res = "11110" + numInBin.substr(0, 3) + "10" + numInBin.substr(3, 6) + "10" + numInBin.substr(9, 6) + "10";
        res += numInBin.substr(15, numInBin.size() - 15);
    }
    else {
        return "-1";
    }

    return res;
}

bool fibonacciToUtf8( const char * inFile, const char * outFile )
{
  // todo
    ifstream ifs ( inFile, ios::in | ios::binary );
    ofstream ofs (outFile, ios::out | ios::binary);

    if ( ! ifs || !ofs )
    {
        return false;
    }

    string remainderString;
    bool stringStart = false;

    for ( char c; ifs.get(c) ; ) {
        if (!ofs || !ifs) {
            return false;
        }

        string bitsString = charToBinaryString(c);
        reverse(bitsString.begin(), bitsString.end());
//        cout << "BITSTRING: " << bitsString << endl;
        vector<long> utfs;
        remainderString = getUtfsFromFib(remainderString + bitsString, stringStart, utfs);

        if (remainderString == "-1") {
            return false;
        }

        for (unsigned long i=0; i<utfs.size(); i++) {
            string binToWrite = convertNumToUtfBin(utfs[i]);
//            cout << "Bin to write: ";

            for (unsigned long j=0; j<binToWrite.size() / 8; j++) {
//                cout << binToWrite.substr(j*8, 8) << " (" << binToH(binToWrite.substr(j*8, 8)) << ") ";
                if (!ofs) {
                    return false;
                }
                ofs.put(binaryToDecimal(binToWrite.substr(j*8, 8)));
            }
//            cout << endl;
        }
    }

    // unended character
    if (stringStart) {
        return false;
    }

    return true;
}
  
bool identicalFiles( const char * file1, const char * file2 )
{
    // todo
    ifstream f1(file1, ifstream::ate | ifstream::binary);
    ifstream f2(file2, ifstream::ate | ifstream::binary);
    const ifstream::pos_type fileSize = f1.tellg();

    if (fileSize != f2.tellg()) {
        return false;
    }

    f1.seekg(0);
    f2.seekg(0);

    istreambuf_iterator<char> begin1(f1);
    istreambuf_iterator<char> begin2(f2);

    return equal(begin1, istreambuf_iterator<char>(), begin2);

}

int main ( int argc, char * argv [] )
{
//    bool resp = utf8ToFibonacci( "example/src_2.utf8", "output.fib" );
//    if (!resp) {
//        cout << "PROGRAM RETURNED FALSE!" << endl;
//    }
  assert ( utf8ToFibonacci ( "example/src_0.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_0.fib" ) );
  assert ( utf8ToFibonacci ( "example/src_1.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_1.fib" ) );
  assert ( utf8ToFibonacci ( "example/src_2.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_2.fib" ) );
  assert ( utf8ToFibonacci ( "example/src_3.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_3.fib" ) );
  assert ( utf8ToFibonacci ( "example/src_4.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_4.fib" ) );
  assert ( ! utf8ToFibonacci ( "example/src_5.utf8", "output.fib" ) );
  assert ( fibonacciToUtf8 ( "example/src_6.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_6.utf8" ) );
  assert ( fibonacciToUtf8 ( "example/src_7.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_7.utf8" ) );
  assert ( fibonacciToUtf8 ( "example/src_8.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_8.utf8" ) );
  assert ( fibonacciToUtf8 ( "example/src_9.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_9.utf8" ) );
  assert ( fibonacciToUtf8 ( "example/src_10.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_10.utf8" ) );
  assert ( ! fibonacciToUtf8 ( "example/src_11.fib", "output.utf8" ) );
//
  return EXIT_SUCCESS;
}
