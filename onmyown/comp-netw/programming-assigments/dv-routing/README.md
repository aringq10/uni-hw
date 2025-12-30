## [Link to author's coding assigment](https://media.pearsoncmg.com/aw/aw_kurose_network_3/labs/lab6/lab6.html)

* I rewrote the code to the more modern C23 standard, removing implicit function calls and K&R function declarations.
* I also separated the node and simulation environment functions into two modules, `nodes.h` and `sim.h` respectively.
* I only implemented the functions that the authors intended for the students to write; and added initial and final printing of the DV tables as well as two custom functions `sharedt` and `printdt` in `node0.c`.
