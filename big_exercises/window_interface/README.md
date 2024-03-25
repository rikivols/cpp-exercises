
# Window Interface

This task is to train inheritance and polymorphism in C++.
It's split into 2 versions, v1 and v2. v2 is an extension of v1.

## Window v1 description

The task is to design and implement a set of classes that will simulate elements in a user interface. Specifically, this includes a window (CWindow), a button (CButton), a text input (CInput), a text label (CLabel), and a combobox (CComboBox).

The task focuses on class design utilizing inheritance, polymorphism, and abstract methods. If these OOP mechanisms are used rationally, the implementation will not be overly lengthy. Conversely, a poor design will result in repetitive code and a large implementation file.

The designed classes will only simulate a GUI. They will not render anything but will instead create models of user interface elements. Thus, the classes can be used to assemble a model of user control elements in a window, manipulate their state, and display their state in the output stream (see below). Individual windows and user elements can be identified by their numerical id. If this id is unique, the element can be searched by this id. If the id is not unique, the control element will function, but it cannot be searched by this ID.

The window will respond to changes in location/size. The position and size of individual control elements are specified relative to the width/height of the parent window (thus, size/position is a decimal number in the range of 0 to 1). When the size of the window changes, the positions and sizes of the individual control elements are recalculated. During the recalculation, the corresponding width (height) is multiplied by the decimal number of the position/size, and this resulting decimal number is used for display.

The class interfaces for representing control elements are very similar. The constructor typically has 2 or 3 parameters. The first parameter is usually an integer id assigned to the control element, followed by information about the location (CRect with 4 components of decimal numbers, indicating the relative position x, y, and relative sizes w, h). The last parameter is usually a string specifying the displayed content. Furthermore, all classes implementing control elements must be able to display themselves in the output stream using the operator << and must be able to be correctly copied (deep copy). The testing environment usually requires only this basic functionality (i.e., constructor, copying, and output operator). However, your implementation will likely require further interface elements, which you will need to add to the classes.

## Window v1 Interface description
* **CButton** Represents a button. The testing environment requires only the constructor and the output operator.
* **CInput** Represents text input. The last parameter of the constructor is a string displayed in the input box. The testing environment requires a constructor, an output operator, and two additional methods:
  * **setValue(x)**, which sets the content to the string x,
  * **getValue()**, which returns the currently displayed string.
* **CLabel** Represents static text. The last parameter of the constructor is the displayed string. The testing environment requires a constructor and an output operator.
* **CComboBox** Represents a combobox. The testing environment requires a constructor, an output operator, and three additional methods:
  * **add(x)**, which adds the specified string x to the list of options,
  * **setSelected(x)**, which sets the option with index x as selected (the default selected option is index 0),
  * **getSelected()**, which determines the index of the currently selected option.
* **CWindow** Represents a window. The class has the following interface:
  * **constructor CWindow(id, title, absPos)**, which creates a window with the title. The window will have its position x, y, and size w x h set according to the given absPos parameter. Note that here dimensions are absolute.
  * **Method add(x)**, which adds a control element x.
  * **Method setPosition(absPos)**, which changes the position/size of the window. The specified values are absolute coordinates and sizes of the window after the change. The method also ensures the recalculation of the position and size of all control elements contained in the window.
  * **Method search(id)** searches for a control element by the given id. If an element with the specified id does not exist in the window, the method returns nullptr. If there are multiple control elements with the same id in the window, the method returns a reference to one of them (e.g., the first one).
  * **Operator <<** displays the content of the window in the output stream; the format is evident from the attached source code.
* **CRect** A helper class representing a rectangle. This class is implemented in the testing environment and is also included in the attached source file. Leave it in the conditional compilation block when submitting.

Submit the source code with your implementation of the CWindow, CButton, CInput, CLabel, and CComboBox classes. Include all necessary supporting declarations in the submitted file. Leave parts for header inclusion and your tests in the conditional compilation block, as indicated in the attached example.

## Window v2 description
This task is an extension of a simpler version. Additionally, it assumes the introduction of a panel element (CPanel), which will allow grouping control elements into groups and working with these groups.

In the basic version of the task, there was a window into which control elements were placed. Therefore, the position of a control element was defined relative to the window. In practice, such a simple approach is not sufficient (e.g., for windows that have splitters, which allow the user to adjust the ratio of the window area allocated to individual control elements). We want to extend our implementation to handle a similar function. Therefore, we want to create a CPanel class that will be able to contain control elements and that can be placed into a parent window (or recursively into a parent panel). The panel will act as a special control element. Its position/size will be defined relative to the parent window/panel, and the size/position of the elements contained within will be recalculated relative to the panel's size.

The testing environment for the panel requires a constructor (2 parameters as shown in the example), an add method, and an output operator. The interface of the remaining classes remains unchanged.

A correct solution to this task, which passes the mandatory tests with 100%, may be used for code review.

## More info:
Check skeletonToFill_v1 and skeletonToFill_v2 for information about the structure of the functions, and for example inputs and outputs 