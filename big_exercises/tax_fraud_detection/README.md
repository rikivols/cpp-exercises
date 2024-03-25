
# Tax Fraud Detection

This task is to train maps and simple C++ STL containers.

## Description

The task is to implement classes that manage Value Added Tax (VAT).

It is assumed that companies exchange invoices. A company issuing an invoice must pay VAT from the invoiced amount to the state. Similarly, a company that receives (and pays an invoice) can claim a VAT refund from the state (under certain conditions). The VAT refund is a weak point that can be exploited for tax evasion and fraud. Therefore, the task is to implement the core of a system that will make such tax evasion more difficult.

Issued and received invoices are recorded by the **CVATRegister** class. Companies must register the invoices they issue and receive. The register pairs matching pairs of invoices and can find unmatched invoices. An invoice can be added to the register and also canceled (e.g., in case of incorrect information). The class interface is as follows:

## Interface description
* **Implicit constructor** initializes an empty register instance.
* **registerCompany (name) method** introduces the specified company into the register. The passed name is the official name of the company, which will be used in exports from the register. The return value is an indicator of success (true)/failure (false). Failure is considered if a company with the same name already exists in the register. When comparing the company name, the register is quite tolerant:does not distinguish between lower and upper case letters,
does not consider superfluous spaces.
These rules are used when registering a new company as well as when adding/deleting invoices. For example, names "My Company", "mY CoMpAnY", " My Company " and " mY CoMPAnY " are considered the same company, but "My Company" and "MyCompany" indicate two different companies.
* **addIssued (invoice) method** adds an invoice to the register. This method is called by the company that issued the invoice. The return value is a flag of success (true)/failure (false). It is considered an error if the seller/buyer in the invoice are not registered, the seller and buyer are the same company, or if the same invoice was already entered using addIssued (two invoices must differ by at least one of: seller/buyer/date/amount/VAT).
* **addAccepted (invoice) method** adds an invoice to the register, this method is called by the company that received (buyer) the invoice. Otherwise, the method behaves the same as addIssued.
* **delIssued (invoice) method** removes an invoice from the register. This method is called by the company that issued and previously registered the invoice. The return value is an indicator of success (true)/failure (false). It is considered an error if the identical invoice was not previously registered by addIssued.
* **delAccepted (invoice) method** removes an invoice from the register. This method is called by the company that received and previously registered the invoice. The return value is an indicator of success (true)/failure (false). It is considered an error if the identical invoice was not previously registered by addAccepted.
* **unmatched (company, sortOpt) method** finds all invoices related to the specified company that have not been matched (i.e., were registered only by addIssued or only by addAccepted). The method returns a list of these invoices, sorted according to the criteria specified by sortOpt. Invoices returned by this method will have the "official" name of the company, i.e., the name that was given when the company was registered using the registerCompany method. This official name will also be used for sorting.

The **CInvoice** class represents a single invoice. The interface must meet:

* **Constructor (date, seller, buyer, amount, vat)** initializes an invoice with the date, names of the selling and buying companies, invoiced amount, and VAT.
* **date, seller, buyer, amount, vat** access methods to read individual components of the invoice.
* **Others:** you can add other public/private methods and member variables needed for implementation to the invoice interface.

The **CSortOpt class** determines the criteria for sorting. All components of the invoice can be used for sorting. For example, creating an instance with:

` CSortOpt().addKey(CSortOpt::BY_AMOUNT, true).addKey(CSortOpt::BY_SELLER, false) `

then it sorts by the invoiced amount in ascending order (first sorting criterion) and for equal invoiced amounts, it uses sorting by the name of the seller in descending order (second sorting criterion). If the order is still not uniquely determined, the order of the invoice introduction into the register is used as a sorting criterion. The CSortOpt class interface is:

Implicit constructor initializes an empty class instance.

* **addKey (sortBy, ascending)** adds another sorting criterion sortBy, with the sorting direction given by the flag ascending (true = ascending, false = descending). Sorting can be based on:
  * **BY_DATE** - by invoice date,
  * **BY_SELLER** - by the official name of the selling company, sorted regardless of case,
  * **BY_BUYER** - by the official name of the buying company, sorted regardless of case,
  * **BY_AMOUNT** - by the invoiced amount,
  * **BY_VAT** - by the VAT amount.
  
The CDate class implements a simple date; its implementation is provided in the test environment and supplied in the attached archive for testing. Its implementation cannot be changed, and the supplied implementation must remain within a conditional compilation block.

Submit the source code with your implementation of the CVATRegister, CInvoice, and CSortOpt classes. Use the attached file with the method declaration and a set of basic tests as the foundation for your implementation. Adding other helper classes may be useful for your implementation.
This task somewhat resembles a previous homework assignment. It is expected that you will use suitable containers from STL (almost the entire library is available), and your implementation should be time and memory efficient. All insertions and deletions should be faster than linear, and sorting should be n log n (where n is the size of the returned list). It's not wise to use the vector collection for all internal structures. If you want to use C++11 containers unordered_set / unordered_map, then do not derive the hash functor as a specialization of std::hash. Declare the hash function/functor explicitly when creating an instance of unordered_set / unordered_map. (Specializing std::hash assumes reopening the std namespace, which is difficult to realize if you are enclosed in another namespace. Solutions available on the internet (stack overflow, cpp reference) implicitly assume that you do not use namespaces, and the recommended solutions are not universally applicable.)
Submit the source code that contains your implementation of the CVATRegister, CInvoice, and CSortOpt classes. Base your implementation on the provided file with the declaration of methods and a set of basic tests. Implementing additional helper classes might be beneficial for your implementation.

This assignment aims to practice implementing a system for VAT management, focusing on creating efficient, scalable solutions to handle the complexities of VAT transactions and prevent tax evasion through accurate and thorough invoice management.

## More info:
Check skeletonToFill for information about the structure of the functions, and for example inputs and outputs 