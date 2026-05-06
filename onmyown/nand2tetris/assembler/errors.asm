=A;JLE   // missing dest
D=;JGE   // missing comp
D=A;     // missing jump
@        // missing symbol/constant
()       // missing label
(LABAS)
// COMMAND C
D;=A     // invalid command
ADM=A    // invalid dest
D=A+2    // invalid comp
D=A;JE   // invalid jump
// COMMAND L
(LABAS)  // label redefinition
(LA@BAS) // invalid label
// COMMAND A
@hey^1   // invalid symbol
@-1203   // invalid constant
@40000   // constant out of range
