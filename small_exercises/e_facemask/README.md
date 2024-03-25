
# E-Facemask

This exercises focuses on work with time.

## Description

The COVID-19 pandemic is once again getting out of control, necessitating stricter measures. To improve the functioning of contact tracing, the state has legal direct access to the phone directories of traced citizens. Your task is to write a program that simplifies searching for contacts in these phone directories and produces the necessary reports.

## Interface description
Your task is to implement the function:

* ``bool report ( const string & fileName, ostream & out );``
  * **fileName** is a text string containing the path to the phone directory and queries,
  * **out** is the output stream where the report should be written,
  * **the** function returns false if an error occurs during the processing of the file or the phone directory contained within it, or true if the processing is successful without problems.

The input file first contains records of the phone directory. This is followed by an empty line and then a list of queries that your program must answer on the passed output stream. Each record of the phone directory is separated by a new line, in the record is the first name, last name, and telephone number in sequence. Individual items are separated by any number of white spaces (other than line breaks). Subsequent queries always contain exactly one word.

The function searches for records that match in the entire first name or last name or both. Partial matches, such as "Joh" and "John", are not accepted. Print matching records to the specified stream, the order on the output must correspond to the order in the phone directory. Display all outputs on the passed output stream (not on standard cout or error output cerr).

The format of the input and output is shown below in the function work sample. If the input is invalid, the function must detect it and return false. Consider the input incorrect if:

an attribute is missing or superfluous,
the telephone number does not have exactly nine digits,
the telephone number contains characters other than digits,
the telephone number starts with 0,
there is some further input continuing after the telephone number on the line (white spaces may follow, but nothing else),
the empty line is missing after the list of numbers.

## Notes:

In the attached archive, you will find sample runs of the program and test inputs.
The line break character (\n) is also after the last line of output (even after any error message).
If you wish, you can use std::string, std::vector, std::array, and algorithms from the standard C++ algorithm library for implementation. Otherwise, you may use dynamically allocated arrays.
To complete the task, using std::getline and/or std::stringstream might be helpful.
The task focuses on working with files, so you do not need to worry too much about time or memory demands.

## More info:
Check skeletonToFill for information about the structure of the functions, and inputs/outputs 