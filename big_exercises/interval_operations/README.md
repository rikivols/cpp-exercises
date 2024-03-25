
# Operations on intervals

## Description
This task is to train the usage of copy / move constructors and overloading of operators in C++.

The task is to implement classes CRange and CRangeList, which will represent integer intervals and their lists.

We consider intervals of whole numbers. An interval can contain just one number, or several numbers. Single-element intervals are written as just one number, e.g., 42. Multi-element intervals are then enclosed in angle brackets. For example, the interval <1..5> contains the numbers 1,2,3,4,5. The class CRange implements the interval of whole numbers. Since we want to work with a large range of numbers, this class will store the bounds of the interval as long long type numbers (we want to work with negative numbers as well). The CRange interface is:

## Description of the public interface
* **Constructor (lo, hi)** initializes an instance to the interval in the range lo to hi. The constructor checks that lo ≤ hi. If the condition is not met, a std::logic_error exception will be thrown (a standard exception from the stdexcept header file).
* Others: Derive further interface based on the constructions from the attached example code; of course, you can add other methods depending on your implementation. It is only necessary to adhere to the instance size - max. 2x size of long long.

The **CRangeList** class will represent a list of such intervals. When composing intervals into a CRangeList instance, we will store intervals as compactly as possible. Thus, for example, adjacent or overlapping intervals will be merged into one longer interval. The CRangeList instance will offer an interface for adding intervals, removing intervals, testing values, and output. In the bonus test, the class will also offer additional interfaces for simplifying the use (see below). The specific required interface will be:

* **Implicit constructor** initializes an empty list of intervals.
* **Destructor, copying constructor, operator=** will be implemented if needed and if the automatically generated form is insufficient. It is recommended to use a suitable container for storing intervals and leave copying and moving to the automatically generated code.
* **includes(long long) / includes (CRange)** methods determine whether the list of intervals includes the specified value / the entire specified interval of values. If yes, they return true, if not, they return false.
* **+=** using this operator, additional intervals will be added to the list. The operator ensures that added intervals are stored compactly (merging intervals). Since we work with whole numbers, merging intervals is somewhat unusual. It is natural to merge overlapping intervals, e.g.:
  * <10..20> and <15..30>,
  * <10..30> and <15..18>, or
  * <10..20> and <20..30>,
  Intervals <10..20> and <21..30> will also merge, even though they do not touch or overlap. Together, however, they cover the entire interval <10..30>. Intervals <10..19> and <21..30> will not merge, as they do not cover the number 20.
* **-=** using this operator, additional intervals will be removed from the list. Removal can also be understood as a set difference. The operator ensures that after removal, intervals are stored compactly (removing unnecessary intervals).
* **=** using this operator, the content can be set to the list of intervals on the right side (of course, compactly stored).
* **==, != operators** compare the content of two instances. Two instances that have an identical list of intervals are considered identical.
* **<< operator** displays the list content to the specified output stream. The list will be enclosed in curly brackets, individual intervals separated by a comma. Single-element intervals will be displayed as a number, longer intervals in angle brackets. The format of the display is evident from the examples. Intervals will be displayed in ascending order.

## Additional tips
The task also offers a bonus test, which requires additional interface for easier handling of the list of intervals. When solving, you can decide whether you will implement this extended interface (thus attempting the bonus) or not:

If you will not implement the extended interface, leave the preprocessor directive EXTENDED_SYNTAX in the comment. The missing syntactic constructions will not compile and the bonus test will not award additional points.
If you will implement the extended syntax, define the preprocessor directive EXTENDED_SYNTAX (remove the comment). In this situation, the testing environment will use the extended syntax and will evaluate the fulfillment of the bonus test.
Be aware, if you define the preprocessor directive EXTENDED_SYNTAX and the necessary interface is not implemented, the compilation will end with an error.

## Bonus Interface
Interface required for the bonus syntax test:

Constructor for initialization with a list of values fills the instance with specified intervals,
Range for loop the CRangeList instance must be usable in a range for loop, iterating over individual intervals in ascending order,
The output operator must always display the interval in decimal format, if the stream is set differently, it must adjust the stream setting and then restore it.
Submit a source file that contains your implementation

## More info:
Check skeletonToFill for information about the structure of the functions, and inputs/outputs 