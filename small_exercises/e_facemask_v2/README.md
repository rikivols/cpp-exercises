
# E-masks v2

This exercises focuses on work with linked lists and cstrings.

## Description

Thanks to the reimplementation of the e-Mask, the situation with the spread of the epidemic has slightly improved, but there's still a long way to go before it's under control. Therefore, it's now necessary to expand this implementation to enable various simulations, to predict future developments, and to try to respond to this development in advance with further measures. And since the certified architects of the state information systems are still busy preparing to receive their well-deserved state honors, the responsibility for this project lies with you again.

Your task is to implement 3 classes. You will need to design their interface yourself, and you will also need to adapt the interface to the anticipated use (see the attached archive). The required classes are:

## Interface
* **CTimeStamp**, this class will implement simple encapsulation of a timestamp. It must have a constructor in which it will initialize its components (year, month, day, hour, minute, second). You can rely on only valid date and time values being provided.
* **CContact** is a class that encapsulates a single contact between two users. Users are identified by their phone number, and the moment of contact is recorded in the form of a timestamp CTimeStamp.
* **CEFaceMask** is described in more detail below.

You can use the implementation of the first two classes from your solution to the previous practice task.

The last required class, CEFaceMask, must implement the following methods:

* **addContact** adds a record of a meeting between two individuals at a specified time to the database.
* **getSuperSpreaders** takes a time interval as a parameter and returns which of the monitored individuals could have infected the most other people in this interval (only direct contact is counted). There may be several such individuals, so return a list of their phone numbers. If no contact occurred in the interval, return an empty list.
For the purposes of this task, represent phone numbers as integers (in practice, this would not be a good idea, as leading zeros are significant for phone numbers).

The return value of the getSuperSpreaders method represents a list of phone numbers. The list must contain only unique numbers and must be sorted in ascending order.

Finally, due to the principle of filling the database, it sometimes happens that a contact with oneself is accidentally generated. Ignore such contact.

## Notes:

In the attached archive, you will find sample runs of the program and test inputs.
Focus on a clean design of classes. You definitely do not want to make all member variables publicly accessible. Use exercises to consult on your design.
The interval from a to b is closed, including these values.

## More info:
Check skeletonToFill for information about the structure of the functions, and inputs/outputs 