// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
//
// This program only needs to handle arguments that satisfy
// R0 >= 0, R1 >= 0, and R0*R1 < 32768.

// Put your code here.

    @2	        // A = RAM[2]
    M=0	        // RAM[2] = 0

    @0          // A = RAM[0]
    D=M         // D = RAM[0]
    @END        // A = END
    D;JEQ	    // If RAM[0] = 0 go to END

    @1          // A = RAM[1]
    D=M         // D = RAM[1]
    @END        // A = END
    D;JEQ	    // If RAM[1] = 0 got to END

(LOOP)
    @1	        // A = RAM[1]
    D=M	        // D = RAM[1]

    @2	        // A = RAM[2]
    M=D+M	    // RAM[2] = RAM[2] + RAM[1]

    @0	        // A = RAM[0]
    M=M-1	    // RAM[0] = RAM[0] - 1

    D=M	        // D = RAM[0]
    @LOOP	    // A = LOOP
    D;JGT	    // If RAM[0] > 0, go to LOOP

(END)
    @END
    0;JMP	    
