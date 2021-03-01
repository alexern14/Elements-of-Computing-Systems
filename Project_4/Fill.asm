// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

(RESTART)
    @SCREEN     // A = SCREEN
    D=A         // D = SCREEN
    @0          // A = RAM[0]
    M=D	        // RAM[0] = SCREEN 

(KEYCHECK)
    @KBD        // A = KBD
    D=M         // D = KBD
    @BLACK      // A = BLACK
    D;JGT	    // If KBD > 0, go to BLACK
    @WHITE      // A = WHITE
    D;JEQ	    // If KBD = 0, go to WHITE
    @KEYCHECK   // A = KEYCHECK
    0;JMP       // got to KEYCHECK

(WHITE)
    @1          // A = RAM[1]
    M=0	        // RAM[1] = 0
    @CHANGE     // A = CHANGE
    0;JMP       // go to CHANGE

(BLACK)
    @1          // A = RAM[1]
    M=-1	    // RAM[1] = -1
    @CHANGE     // A = CHANGE
    0;JMP       // go to CHANGE

(CHANGE)
    @1	        // A = RAM[1]
    D=M	        // D = RAM[1]

    @0          // A = RAM[0]
    A=M	        
    M=D	        // RAM[0] = RAM[1]

    @0          // A = RAM[0]
    D=M+1	    // D = RAM[0] + 1
    @KBD        // A = KBD
    D=A-D	    // D = KBD - RAM[0] + 1

    @0          // A = RAM[0]
    M=M+1	    // RAM[0] = RAM[0] + 1
    A=M     

    @CHANGE     // A = CHANGE
    D;JGT	    // if (KBD - RAM[0] + 1) > 0, go to CHANGE
    
    @RESTART    // A = RESTART
    0;JMP       // go to RESTART

    // No END loop because it is an infinitely running loop
