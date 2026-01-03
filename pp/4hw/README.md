### SLList.h and SLList.c
* A library that defines the Singly Linked List and its node structures. It defines various functions for working with a singly linked list as well. It is not a type specific library, meaning the nodes can store any type of video via void pointers.

### studentList.h and studentList.c
* A wrapper library built on top of `SLList.h` for working with the `Student struct` as the list's data type.

### commandMenu.h and commandMenu.c
* A library with a single function - running a while loop and prompting the user to choose a command for it to execute. A switch statement is afterwards used to call the according functions.

### main1.c
* A unit-test program for checking whether the functions defined in `SLList.h` work properly.

### main2.c
* An interactable program that combines all 3 of the mentioned libraries to let the user modify a `StudentList struct` via various commands.

### Notes
> The main assignment was to create a function that removes all consecutive duplicate elements from a singly linked list, leaving only a single instance of such an element behind. This function is implemented in `SLList.h` and takes in a `compareFunc` for determining whether the element data matches.
