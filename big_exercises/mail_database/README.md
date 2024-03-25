
# Mail Database

This task is for the training of C++ dynamic arrays and C-like strings. Also, it's a training of reference counting (implementing smart pointers). 

## Description
The task is to implement the **CMailServer** class, which will simulate the operation of a simple mail server.

It is assumed that the server facilitates email sending within an organization. Each email is represented by an instance of the **CMail** class, containing the sender's address, recipient's address, and the message body (all as strings). The mail server can send messages and can determine the content of sent and received mail (outbox and inbox) for individual email addresses. The list of sent and received mail is provided for individual email addresses in the form of an iterator, with the iterator instance offering methods to navigate through the list of emails and access the content of individual emails. The CMailServer class itself must correctly handle copying, with its copies serving for archival and auditing purposes.

The task is aimed at practicing the understanding of shallow and deep object copying. It has mandatory, optional, and bonus parts. To fulfill the mandatory part of the task, it suffices to create a functional class that meets the required interface. To tackle the optional and bonus parts, consider storing data in a way that access to mailboxes is fast and that memory is not unnecessarily wasted when copying.

The task is designed to practice working with object copying. For this reason, parts of the standard C++ library, especially STL and the C++ std::string data type, are suppressed in the task. Only the C library strings (cstring) interface is available. For implementation, it might be suitable to create your own simple substitute for std::string.

## Interface description
The required CMail class has the following interface:

* **Constructor (from, to, body)** creates an email instance with the from/to/body components filled according to the parameters. You can assume that email addresses are relatively short (tens to hundreds of characters) and that the message body can be relatively long (up to several megabytes),
* **Operator ==** compares the content of two CMail instances, returning true if the instances are identical (all components from, to, and the email content match).
* **Operator <<** displays information about the email into the specified stream. The format is evident from the examples.

The required **CMailServer** class has the following interface:

* **Implicit constructor** creates an empty instance,
* **Destructor** releases resources allocated by the instance,
* **Copying constructor, operator=** create identical copies of the instance according to standard rules,
* **sendMail(mail)** sends the email passed in parameters, effectively placing it in the corresponding boxes of the sender and recipient. The email is always added to the end of the existing list. Neither the recipient nor the sender needs to be created in advance; a mailbox is automatically created after processing the first email containing the given email address,
* **outbox(email)** provides access to mail sent from the specified address. The return value is a CMailIterator instance, which allows navigating through emails sent from the email address. If an unknown email address is specified, the result is an iterator for an empty list of emails. The returned iterator must capture the state of the mailbox at the moment it was created. Therefore, if the content of the mailbox being browsed changes during the use of the iterator, this change will not reflect in the values returned by the iterator. This behavior is demonstrated in the sample run, for example, for iterator i5.
* **inbox(email)** provides access to mail received at the specified address. Otherwise, the method works similarly to the outbox method.
* **Email addresses** in the task can be any strings; when comparing them, case sensitivity (case sensitive) is distinguished - in this, the task differs from the real world, where email addresses have a prescribed formal shape and where case differences are typically not distinguished.

The required **CMailIterator** class has the following interface:

* **Operator bool** determines whether the iterator refers to a valid email (returns true), or whether it has reached past the last email in the list (thus, no more emails can be read, returns false),
* **Operator !** works the same as the previous operator but returns the opposite return value,
* **Operator** * **unary operator** * provides access to the email at the current position. The return value is an instance of CMail (or a constant reference to CMail). It is not necessary to address the situation of accessing an email beyond the end of the list - the testing environment always checks the validity of the iterator first and then possibly provides access to the referenced email,
* **Operator ++ the prefix ++ operator** moves the iterator to the next email in the list. Emails are

## More info:
Check skeletonToFill for infomation about the structure of the functions, and example/ for example inputs and outputs 