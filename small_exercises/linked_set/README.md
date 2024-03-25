
# Linked Set

This exercises focuses on work with linked lists and cstrings.

## Description

Your task is to implement the CLinkedSet class, which will represent the abstract data type set, implemented with a sorted linked list. Such an implementation is particularly advantageous for operations like union, intersection, or difference. You are only required to implement basic operations, following this interface:

## Interface

* **Parameterless constructor** Initializes an empty set.
* **Copy constructor** Creates a deep copy.
* **Assignment operator** Deletes the current set and creates a deep copy of the passed one.
* **Destructor** Frees allocated resources.
* **bool Insert (const char * value)** Inserts a string into the set, returns true if the insertion is successful. If the string is already in the set, the function returns false and does not change the set.
* **bool Remove (const char * value)** Removes a string from the set, returns true if the deletion is successful. If the string is not in the set, the function returns false and does not change the set.
* **bool Empty () const** Returns true if the set is empty, otherwise false.
* **size_t Size () const** Returns the number of elements in the set.
* **bool Contains (const char * value)** Returns true if the string is in the set, otherwise false.

Strings in the list are sorted in ascending order, use the strcmp function for comparison.

For the implementation, use the CNode structure, which represents an element in the linked list, and implement the Value function, which returns the string.

The attribute m_Begin must point to the first element of the linked list, and the last element must point to nullptr. The testing environment checks the structure of your linked list.

## Hints:

A solution that correctly implements a deep copy will score full points.
You can add additional attributes and methods to the CLinkedSet and CNode classes. However, the testing environment relies on m_Begin pointing to the beginning of the linked list, m_Next being used as a pointer to the following element, and the Value function returning the string at the given position.
Do not implement a linked list with a sentinel. (For an empty linked list, m_Begin = nullptr must hold. For a linked list with one element, m_Begin -> m_Next = nullptr, etc.)
If your program does not compile, make sure you are adhering to the specified interface.

## More info:
Check skeletonToFill for information about the structure of the functions, and inputs/outputs 