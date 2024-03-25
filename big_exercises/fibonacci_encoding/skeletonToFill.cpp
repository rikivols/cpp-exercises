#ifndef __PROGTEST__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;
#endif /* __PROGTEST__ */

bool utf8ToFibonacci(const char *inFile,
                     const char *outFile) {
    // todo
}

bool fibonacciToUtf8(const char *inFile,
                     const char *outFile) {
    // todo
}

#ifndef __PROGTEST__

bool identicalFiles(const char *file1,
                    const char *file2) {
    // todo
}

int main(int argc, char *argv[]) {
    assert(utf8ToFibonacci("example/src_0.utf8", "output.fib")
           && identicalFiles("output.fib", "example/dst_0.fib"));
    assert(utf8ToFibonacci("example/src_1.utf8", "output.fib")
           && identicalFiles("output.fib", "example/dst_1.fib"));
    assert(utf8ToFibonacci("example/src_2.utf8", "output.fib")
           && identicalFiles("output.fib", "example/dst_2.fib"));
    assert(utf8ToFibonacci("example/src_3.utf8", "output.fib")
           && identicalFiles("output.fib", "example/dst_3.fib"));
    assert(utf8ToFibonacci("example/src_4.utf8", "output.fib")
           && identicalFiles("output.fib", "example/dst_4.fib"));
    assert(!utf8ToFibonacci("example/src_5.utf8", "output.fib"));
    assert(fibonacciToUtf8("example/src_6.fib", "output.utf8")
           && identicalFiles("output.utf8", "example/dst_6.utf8"));
    assert(fibonacciToUtf8("example/src_7.fib", "output.utf8")
           && identicalFiles("output.utf8", "example/dst_7.utf8"));
    assert(fibonacciToUtf8("example/src_8.fib", "output.utf8")
           && identicalFiles("output.utf8", "example/dst_8.utf8"));
    assert(fibonacciToUtf8("example/src_9.fib", "output.utf8")
           && identicalFiles("output.utf8", "example/dst_9.utf8"));
    assert(fibonacciToUtf8("example/src_10.fib", "output.utf8")
           && identicalFiles("output.utf8", "example/dst_10.utf8"));
    assert(!fibonacciToUtf8("example/src_11.fib", "output.utf8"));

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
