
# Result listings

This task is to train algorithms and working with matrices.

## Description

The task is to design and implement a class template that can compile result lists based on given inputs - match outcomes between pairs of opponents.

It is assumed that we have several opponents. In one match, a pair of opponents face off, and the match outcome is described somehow (for example, by a Boolean value, a pair of scores, a pair of times, etc.). Because we want the class to be generic, the match outcome will be represented as a generic data type. Thus, several outcomes for specified pairs of opponents (opponents are uniquely identified by name - a string) will be entered into the class. Subsequently, we want to determine if a unique result list can be compiled based on the given outcomes, and if possible, calculate this list. The trick is that when compiling the outcome, we want to rely solely on information about the winner/loser from the given matches.

The result list is uniquely determined if for every contestant Xi at position i the following holds:

* Xi has defeated all the opponents Xj, i+1 ≤ j ≤ n. Xi defeated either directly or indirectly (Xi beat Xa, Xa beat Xb, ..., Xz beat Xj, i+1 ≤ a < b < ... < j ≤ n),
* Xi did not win or draw with any opponent Xk, 0 ≤ k ≤ i-1.

Take, for example, if it was specified that A beats B and B beats C. Then the correct result list is A B C. However, if it was specified that:

```
A beats B
B beats C
C beats A
or:
A beats B
A beats C
or:
A beats B
B draws with C
```

then the result list cannot be uniquely determined (we would need to add information about more matches or determine the order based on additional criteria, which we will not do in this task).
It was mentioned that a match is specified as a triple (opponent1, opponent2, outcome), where the outcome is some data type describing, for example, scores, times, or anything else. The outcome needs to be converted to information about the winner/draw/loser, which will be the responsibility of the comparator passed to the method for determining the result list. The comparator is a function, functor, or lambda expression that, for a given outcome, returns a return value:

* negative, if opponent1 lost,
* zero, if opponent1 and opponent2 drew,
* positive, if opponent1 won.

## Interface

The overall interface of the **CContest** class template will be:

* The template is parameterized by a generic parameter M_
This type describes the match outcome. It is guaranteed for the data type M_ to be movable, copyable, and destructible (CopyConstructible, MoveConstructible, CopyAssignable, MoveAssignable, and Destructible). No other properties of the type are guaranteed, specifically, it is not guaranteed that type M_ has an implicit constructor (may not be DefaultConstructible).
* **Implicit constructor** creates an empty CContest instance,
* **addMatch(contestant1, contestant2, result) method** adds the outcome between opponents contestant1 and contestant2. Opponents are specified in the form of strings. The result result is of the data type M_. The method stores information about this match in some storage within its instance. Calls to addMatch can be chained. The method throws a std::logic_error exception if the outcome of the match between contestant1 and contestant2 was already entered before.
* **isOrdered(comparator) method** decides whether the filled match outcomes lead to a unique result list or not. The return value is true if the result list is unique, false if not. The calling parameter is comparator - a function, functor, or lambda expression that will be called when obtaining the match outcome (converts M_ to a negative number / zero / positive number according to the description above). The method must not modify the CContest instance and must not throw exceptions.
* **results(comparator) method** calculates the result list. The result is an STL list (std::list) of strings with the names of contestants in order from first (overall winner) to last. If the result list cannot be compiled (is not unique), the method throws a std::logic_error exception. The parameter comparator has the same meaning as in the isOrdered method.

Submit the source code with your implementation of the CContest class template. Use the attached source code as the basis for your implementation. If you keep conditional compilation blocks in the code, such source code can be locally tested and also submitted to Progtest.

The evaluation is divided into mandatory and bonus parts. The mandatory part tests instances with a small number of opponents and matches. To successfully handle it, a basic algorithm working in time number_of_opponents x number_of_matches is sufficient. To handle the bonus test, a more efficient algorithm is needed.

For the basic solution, it might be useful to find all opponents directly or indirectly defeated. This

## More info:
Check skeletonToFill for information about the structure of the functions, and for example inputs and outputs 