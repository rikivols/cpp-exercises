
# Counter

This exercises focuses on work with time.

## Description

Recently, the plebs have primarily been entertained by counting down the time, whether it's to the end of isolation, quarantine, emergency state, or a term of office. For these tasks, it has been shown that the standard C++ library lacks a class for working with time in a user-friendly form. No more seconds to the end!
Your task is to implement the CTime class, which will represent time. The implemented class must meet the following interface:


## Interface
* **Implicit constructor** Initializes an empty object.
* **Constructor with 2 int type parameters** representing hours and minutes Initializes the object with the passed parameters and sets seconds to 0. In case of invalid parameters, it throws an std::invalid_argument exception, which is part of the standard library, declared in the stdexcept header file.
* **Constructor with 3 int type parameters** representing hours, minutes, and seconds Initializes the object with the passed parameters. In case of invalid parameters, it throws an std::invalid_argument exception, which is part of the standard library, declared in the stdexcept header file.
* **Copy constructor, assignment operator, destructor** Will be implemented if required by the internal structures of your class.
* **operator+, operator+=** Allows adding a specified number of seconds (int) to the given time.
* **operator-, operator-=** Allows subtracting a specified number of seconds (int) from the given time.
* **operator-** Allows subtracting two CTime objects from each other. The result is the minimum positive number of seconds needed to add to one of the given times to obtain the other time.
* **operator++, operator--** Allows adding (or subtracting) a second to the given time.
* **Relational operators (<, <=, >, >=, ==, !=)** Allows comparing two times with each other.
* **operator<<** Allows outputting an object to a C++ stream (output format HH:MM:SS, single-digit hours are padded with a space).

Submit the source file containing your implementation of the CTime class. In the submitted file, keep the header files, the CTimeTester class, and the main function in a block of conditional compilation.

In this example, a prescription for the required class interface is not provided. From the textual description, usage examples below, and knowledge of operator overloading, you should be able to devise this interface.

The test script expects that the class will use member variables m_Hour, m_Minute, and m_Second and checks their correct values. It also expects that the CTimeTester class will have friend access. If you change this part, your code will not compile.

## Hints:

* Accept hours from 0 to 23, minutes, and seconds from 0 to 59 as valid.
* Apply the previous point also in calculations. If a calculation results in overflow/underflow of time into the next/previous day, consider the result modulo 24 hours.
* If your program does not compile, ensure that you have correctly overloaded the operators. Check the const qualifiers in particular.
* Implement the output operator correctly -- do not send data to cout, send it to the passed output stream. Do not add line breaks or other white spaces after outputting numbers to the stream.
* The use of the class assumes many overloaded operators. Design the interface so that you do not have an unnecessarily long implementation. Use C++ features (implicit parameters); a sensible design can save a lot of work.
* The task does not require adding more member variables (attributes); if you add some or modify the existing ones, the class design test will fail.
* The testing environment does not work exactly like the assert function. If the basic test according to the example does not pass, you likely implement some operators incorrectly (e.g., ++ or --).

## More info:
Check skeletonToFill for information about the structure of the functions, and inputs/outputs 