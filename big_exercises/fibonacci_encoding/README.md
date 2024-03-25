
# Fibonacci's encoding

# Description
The task is to create C/C++ functions capable of encoding a file from UTF-8 encoding to Fibonacci code and vice versa.

Characters are stored as numbers - indexes into the code table, most commonly UNICODE today. There are many characters in UNICODE - 1048576+65536. If they were stored directly as binary numbers, 4 bytes would always be needed to represent one character. However, most characters have lower codes, for example, values less than 128 (65536). UTF-8 encoding takes advantage of this by storing characters more compactly in 1 to 4 bytes depending on the encoded value.

UTF-8 encoding reduces the overall volume of stored data for common text files but is not optimal. When designing UTF-8 encoding, the method chosen was to always end the codes of individual characters at a whole byte boundary. This is practical for fast loading but is not optimal in terms of length.

For memory-efficient storage of whole numbers, other codes have been developed, such as Fibonacci code, which is based on the Fibonacci sequence. Recall that the Fibonacci sequence is a sequence of whole numbers where each subsequent number is the sum of the two preceding ones. The sequence looks like 1, 1, 2, 3, 5, 8, ... In Fibonacci code, numbers are stored as bit sequences, with a set bit indicating the presence of the corresponding element of the Fibonacci sequence. For example, the number 9 can be written as:

```
                            1  1  2  3  5  8  
9 = 1 + 8                   1  0  0  0  0  1
9 = 1 + 8                   0  1  0  0  0  1
9 = 1 + 3 + 5               1  0  0  1  1
9 = 1 + 1 + 2 + 5           1  1  1  0  1
```
Only those notations are selected where no number from the Fibonacci sequence is repeated more than once (thus, the notation 1 + 1 + 2 + 5 would not be used). With this restriction, we do not need to consider the two initial ones in the Fibonacci sequence, as only one of them could be used anyway. Moreover, notations where consecutive members of the Fibonacci sequence appear are not allowed (thus, the notation 1 + 3 + 5 will also be impermissible, 3 and 5 are neighbors). A pair of neighboring elements in the Fibonacci sequence can always be replaced by their sum, that is, the next element in the sequence.

With these restrictions, the Fibonacci code becomes unique. For the first few whole numbers, it looks like this:

```
                            1  2  3  5  8
1 = 1                       1
2 = 2                       0  1
3 = 3                       0  0  1
4 = 1 + 3                   1  0  1
5 = 5                       0  0  0  1
6 = 1 + 5                   1  0  0  1
7 = 2 + 5                   0  1  0  1
8 = 8                       0  0  0  0  1
9 = 1 + 8                   1  0  0  0  1
10 = 2 + 8                  0  1  0  0  1
11 = 3 + 8                  0  0  1  0  1
12 = 1 + 3 + 8              1  0  1  0  1
```
Thus, whole numbers can be stored using the Fibonacci code. Notice that smaller numbers are encoded with shorter bit sequences, while larger numbers are encoded with longer sequences. The problem, however, is recognizing where the code of one number ends and the code of another begins. For binary notation with a fixed number of bits, this is trivial - the bit sequences are divided into groups of 8/16/32/64 bits according to the fixed size of the data type. Here, however, we would like to use a variable number of bits to save space. Imagine a situation where the input is a sequence of bits 00101. This can be interpreted as one number (11) or as two numbers 3 and 2, if we divided the bit sequence into groups 001 01

## More info:
Check skeletonToFill for infomation about the structure of the functions, and example/ for example inputs and outputs 