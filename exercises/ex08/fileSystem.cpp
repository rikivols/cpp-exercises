
#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include <memory>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cctype>
#include <cstdint>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */

class Data {
public:
    Data(const string & hashInp="", size_t fileSizeInp=0, const string & pathInp=""): hash(hashInp), fileSize(fileSizeInp), path(pathInp) {}

    virtual size_t Size() const{
        return 0;
    };

    void print() {
        cout << "hash: " << hash << " size: " << fileSize << "path: " << path << endl;
    }

    // 1 = file, 2 = link, 3 = dir
    int getType() const {
        if (!path.empty()) {
            return 1;
        }
        if (!hash.empty()) {
            return 2;
        }
        return 3;
    }

    string getHash() const {
        return hash;
    }

    string getPath() const {
        return path;
    }

    string & getHash() {
        return hash;
    }

    string & getPath()  {
        return path;
    }

    size_t getFileSize() const {
        return fileSize;
    }

protected:
    string hash;
    size_t fileSize;
    string path;
    map<string, Data *> datas;
};

class CFile: public Data
{
public:
    // constructor
    CFile() {
        Data();
    }

    CFile(const string &hashInp, size_t fileSizeInp) {
        hash = hashInp;
        fileSize = fileSizeInp;
//        Data(hash, fileSize, "");
    }

    CFile(const CFile &file) {
        hash = file.hash;
        fileSize = file.fileSize;
    }

    CFile(const Data &file) {
        hash = file.getHash();
        fileSize = file.getFileSize();
    }

    // Size
    virtual size_t Size() const {
        return fileSize;
    }

    // Change
    CFile &Change(const string &hashInp, size_t fileSizeInp) {
        hash = hashInp;
        fileSize = fileSizeInp;

        return *this;
    }

    string getHash() const {
        return hash;
    }

    size_t getSize() const {
        return fileSize;
    }
};

class CLink: public Data
{
public:
    CLink() {
        Data();
    }

    CLink(const string &pathInp) {
        path = pathInp;
        fileSize = path.size()+1;
    }

    CLink(const CLink &link) {
        path = link.path;
        fileSize = link.fileSize;
    }

    CLink(const Data &link) {
        path = link.getPath();
        fileSize = link.getFileSize();
    }

    // Size
    virtual size_t Size() const {
        return path.size() + 1;
    }

    // Change
    CLink & Change(const string &newPath) {
        path = newPath;

        return *this;
    };

    string getPath() const {
        return path;
    }

    friend ostream& operator<<(ostream& os, const CLink &link) {
        os << "-> " << link.path << endl;
        return os;
    }
};

class CDirectory: public Data
{
public:

    CDirectory(CDirectory & dir){
        datas = dir.datas;
    }
//
//    CDirectory(Data & dir) {
////        datas = dir.datas;
//    }
//    // Size
    ~CDirectory() {

}

    virtual size_t Size() const {
        size_t totalSize = 0;
        for (auto const &[key, val]: datas) {
            totalSize += key.size() + val->Size();
        }

        return totalSize;
    }

    CDirectory & Change(const string & fileName, const CFile &file) {
        datas[fileName] = new CFile(file);

        return *this;
    }

    CDirectory & Change(const string & fileName, const CLink &link) {
        datas[fileName] = new CLink(link);

        return *this;
    }

    CDirectory & Change(const string & fileName, const  CDirectory &dir) {
        datas[fileName] = new CDirectory(dir);

        return *this;
    }

    CDirectory & Change(const string & fileName, nullptr_t n) {
        datas.erase(fileName);

        return *this;
    }

    // Get
    auto & Get(const string & fileName) {
        if (datas.find(fileName) == datas.end()) {
            throw out_of_range("");
        }

        return *datas[fileName];
    }

    auto & Get(const string & fileName) const {
        if (datas.find(fileName) == datas.end()) {
            throw out_of_range("");
        }
        return *datas.at(fileName);
    }


    // operator<<
    friend ostream& operator<<(ostream& os, const CDirectory &dir) {
        for (auto const& [key, val] : dir.datas) {
            os << val->Size() << '\t' << key;
            switch(val->getType()) {
                case 1:
                    os << " -> " << val->getPath();
                    break;
                case 2:
                    os << " " << val->getHash();
                    break;
                case 3:
                    os << "/";

            }

            os<< '\n';
        }

        return os;
    };


private:
    map<string, Data *> datas;
};

#ifndef __PROGTEST__
int main ()
{
    CDirectory root;
    CDirectory goot;
    stringstream sout;

    goot.Change("folder", CDirectory()
            .Change("fileA.txt", CFile("", 0).Change("skjdajdakljdljadkjwaljdlaw=", 1713))
            .Change("fileB.txt", CFile("kadwjkwajdwhoiwhduqwdqwuhd=", 71313))
            .Change("fileC.txt", CFile("aihdqhdqudqdiuwqhdquwdqhdi=", 8193)));

    root.Change("file.txt", CFile("jhwadkhawkdhajwdhawhdaw=", 1623))
        .Change("file.ln", CLink("").Change("file.txt"))
        .Change("folder", CDirectory()
            .Change("fileA.txt", CFile("", 0).Change("skjdajdakljdljadkjwaljdlaw=", 1713))
            .Change("fileB.txt", CFile("kadwjkwajdwhoiwhduqwdqwuhd=", 71313))
            .Change("fileC.txt", CFile("aihdqhdqudqdiuwqhdquwdqhdi=", 8193)));

    sout.str( "" );
    sout << root;
//    cout << sout.str();
    assert( sout.str() ==
        "9\tfile.ln -> file.txt\n"
        "1623\tfile.txt jhwadkhawkdhajwdhawhdaw=\n"
        "81246\tfolder/\n" );
    assert( root.Size() == 82899 );

    string filename = "folder";
    const CDirectory & inner = dynamic_cast<const CDirectory &>( root.Get( filename ) );

    sout.str( "" );
    sout << inner;
    assert( sout.str() ==
        "1713\tfileA.txt skjdajdakljdljadkjwaljdlaw=\n"
        "71313\tfileB.txt kadwjkwajdwhoiwhduqwdqwuhd=\n"
        "8193\tfileC.txt aihdqhdqudqdiuwqhdquwdqhdi=\n" );
    assert( inner.Size() == 81246 );

    root.Change( "file.txt", goot.Get( "folder" ) );

    return 0;
}
#endif /* __PROGTEST__ */
