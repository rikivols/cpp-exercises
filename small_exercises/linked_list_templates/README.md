
# Linked list and templates

This exercises focuses on work with linked list and templates.

## Description

Your task is to implement the CSet template, which will represent the abstract data type set. You are only required to implement basic operations, following this interface:

## Template
* **Parameterless constructor** Initializes an empty set.
* **Copy constructor** Creates a deep copy.
* **Assignment operator** Deletes the current set and creates a deep copy of the passed one.
* **Destructor** Frees allocated resources.
* **bool Insert(Type)** Inserts an object of type Type into the set, returns true if the insertion is successful. If the object is already in the set, the function returns false and does not change the set.
* **bool Remove(Type)** Removes an object of type Type from the set, returns true if the deletion is successful. If the object is not in the set, the function returns false and does not change the set.
* **size_t Size() const** Returns the number of elements in the set.
* **bool Contains(Type)** Returns true if an object of type Type is in the set, otherwise false.

The Type class, the template parameter, is guaranteed to have the following functionalities:
* Copy constructor,
* Assignment operator,
* operator< for comparing two elements.

You can use the CLinkedSet class from a previous practice task for the implementation.

## Hints:

If you are implementing CSet using an array, consider whether the functionalities of the Type class will be sufficient for you.
If your program does not compile, make sure you are adhering to the specified interface (not requiring anything extra from the elements).

## More info:
Check skeletonToFill for information about the structure of the functions, and inputs/outputs 