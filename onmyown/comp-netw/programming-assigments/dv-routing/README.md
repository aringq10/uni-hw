## [Link to authors' coding assigment](https://media.pearsoncmg.com/aw/aw_kurose_network_3/labs/lab6/lab6.html)

* I rewrote the code to the more modern C23 standard, removing implicit function calls and K&R function declarations.
* I also separated the node and simulation environment functions into two modules, `nodes.h` and `sim.h` respectively.
* I only implemented the functions that the authors intended for the students to write; and added initial and final printing of the DV tables as well as two custom functions `sharedt` and `printdt` in `node0.c`.

> *If you enter a value of -1 for TRACE upon launching the program, you'll see my debug print messages of all 4 DV tables upon initialization, before the first and second link cost changes and the final tables.
