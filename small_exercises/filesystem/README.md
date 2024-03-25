
# Filesystem

This exercises focuses on work with maps, pointers and recursive listing of files.

## Description

Every proper manager keeps their most important and secret files on their desktop PC at work. This is also the case for the fictional company FarmComp and its owner (Zoltán Yayan; ed. note). Your task is to create a simple file system for him.
Your task is to write a set of classes that will represent a file system:

## Interface

* **CFile** This class represents a basic file in a simplified form. Only its size and data hash (text string) are recorded.
  * **CFile::CFile(hash, filesize)** - creates an object with the given parameters
  * **CFile::Size()** - returns the file size
* **CFile::Change(hash, filesize)** - sets a new hash and size for the file, call can be chained

* **CLink** This class represents a symbolic link. Only the file path itself is recorded, it's not necessary to validate that the given path is valid.
  * **CLink::CLink(path)** - creates a link with the given parameters
  * **CLink::Size()** - returns the size of the link, which equals the number of characters in the path + 1 (terminating null)
  * **CLink::Change(path)** - sets a new path for the link, call can be chained
  
* **CDirectory** This class represents a directory of files. Always records pairs of file name and file. The directory can contain not only standard CFile files but also CLink and CDirectory.
  * **CDirectory::CDirectory()** - creates an empty directory
  * **CDirectory::Size()** - returns the folder size, which equals the sum of the file sizes (including directories recursively) and the number of characters in their names.
  * **CDirectory::Change(filename, file)** - adds or replaces a file in the directory with the file named filename, call can be chained
  * **CDirectory::Change(filename, nullptr)** - removes a file named filename from the directory, call can be chained
  * **CDirectory::Get(filename)** - returns a reference to a file named filename from the directory (requires both const and non-const variants, does not search recursively), if the file named filename does not exist in the directory, throw std::out_of_range exception
  * **operator<<(ostream &, directory)** - operator for printing (the format of the printout is file size, tab, file name, and other parameters according to the type of file, see example, files in the printout are sorted by file name)
  
The task focuses on proper object design and polymorphism. Correct design and polymorphism can save a lot of writing. An implementation without using polymorphism will not pass the class design test.

## Hints:

You can add your own methods or even entire classes to the required classes.
If your program does not compile, make sure you are adhering to the required interface and do not forget the const keyword.
The testing environment creates copies of your objects, so remember to create a copying constructor and assignment operator if your classes require them.
Use std::string for passing and storing text strings.

## More info:
Check skeletonToFill for information about the structure of the functions, and inputs/outputs 