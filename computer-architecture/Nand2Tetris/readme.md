# summary

![](images/2023-04-17-19-56-27.png)
![](images/2023-04-17-19-56-43.png)

# index

A. hardware layer

1. boolean logic gate :white_check_mark:
2. boolean arithmetic :white_check_mark:
3. sequential logic :white_check_mark:
4. Von Neumann architecture
5. machine code

</br>

B. software layer

6. assembler
7. virtual machine - stack arthmetic
8. virtual machine - program control
9. compiler I - syntax analysis
10. compiler II - code generation
11. operating system
12. high level language


# todos

## 1장. boolean logic gate

0,1 boolean input 두개가 gate로 들어온다.

AND, NOT gate의 조합으로 nand, or, xor, multiplexor demultiplexor, bus, multibit, 다입력 (not, and, or, multiplexor) 을 만든다.


### 실습

![](images/2023-04-18-17-05-05.png)

1. HardwareSimulator.sh 실행
2. file -> load chip 에서 and, not, or, xor 등의 chip을 로드하면, 왼쪽 하단 HDL에 PARTS 부분에 어떤 칩의 조합으로 해당 칩이 만들어 졌는지 뜬다.
3. file -> load script로 해당 칩의 test file을 로드. 오른쪽 화면에 테스트 케이스 별로 input, output이 뜬다.
4. 왼쪽 상단 >, >> 버튼 클릭해서 테스트 실행

### 0. 드모르간의 법칙

![](images/2023-04-18-17-57-15.png)


### 1. NOT's HDL

![](images/2023-04-18-17-32-13.png)

```
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/01/Not.hdl

/**
 * Not gate:
 * out = not in
 */

CHIP Not {
    IN in;
    OUT out;

    PARTS:
    Nand (a=in, b=in, out=out);
	/* BUILTIN NOT;*/
}
```

Not x= x Nand x
..라는 점을 이용하여 Nand(a = in, b = in, out = out); 라고 서술한 것.

원래 연역식으로 작성하려면 먼저 not, and gate 가지고 nand를 만든 후, 쭉쭉 나가면 되는데,
이 방법보다 먼저 nand gate를 만들고 nand gate로 not, and 를 만드는게 칩 소모량이 덜 하기 때문에,
해당 프로젝트에서는 nand는 built-in으로 정의해두고, nand부터 시작함.



### 2. AND's HDL


![](images/2023-04-18-17-33-12.png)

```
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/01/And.hdl

/**
 * And gate:
 * out = 1 if (a == 1 and b == 1)
 *       0 otherwise
 */

CHIP And {
    IN a, b;
    OUT out;

    PARTS:
    Nand (a=a, b=b, out=val);
    Not (in=val, out=out);
}
```

원래 연역식으로 작성하려면 먼저 not, and gate 가지고 nand를 만든 후, 쭉쭉 나가면 되는데,
이 방법보다 먼저 nand gate를 만들고 nand gate로 not, and 를 만드는게 칩 소모량이 덜 하기 때문에,
해당 프로젝트에서는 nand는 built-in으로 정의해두고, nand부터 시작함.




### 3. NAND's HDL

![](images/2023-04-18-17-31-49.png)

```
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: tools/builtIn/Nand.hdl

/**
 * Nand gate: out = a Nand b.
 */

CHIP Nand {

    IN  a, b;
    OUT out;

    BUILTIN Nand;
}
```
원래 연역식으로 작성하려면 먼저 not, and gate 가지고 nand를 만든 후, 쭉쭉 나가면 되는데,
이 방법보다 먼저 nand gate를 만들고 nand gate로 not, and 를 만드는게 칩 소모량이 덜 하기 때문에,
해당 프로젝트에서는 nand는 built-in으로 정의해두고, nand부터 시작함.


### 4. OR's HDL

```
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/01/Or.hdl

 /**
 * Or gate:
 * out = 1 if (a == 1 or b == 1)
 *       0 otherwise
 */

CHIP Or {
    IN a, b;
    OUT out;

    PARTS:
    Not (in=a, out=nota);
    Not (in=b, out=notb);
    And (a=nota, b=notb, out=notaAndnotb);
    Not (in=notaAndnotb, out=out);
}
```

### 5. XOR's HDL
```
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/01/Xor.hdl

/**
 * Exclusive-or gate:
 * out = not (a == b)
 */

CHIP Xor {
    IN a, b;
    OUT out;

    PARTS:
    Not (in=a, out=nota);
    Not (in=b, out=notb);
    And (a=a, b=notb, out=w1);
    And (a=nota, b=b, out=w2);
    Or (a=w1, b=w2, out=out);
}
```


### 6. MUX(multiplexor) 's HDL
```
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/01/Mux.hdl

/**
 * Multiplexor:
 * out = a if sel == 0
 *       b otherwise
 */

CHIP Mux {
    IN a, b, sel;
    OUT out;

    PARTS:
    Not (in=sel, out=notsel);

    And (a=a, b=notsel, out=aAndnotsel);
    And (a=b, b=sel, out=bAndsel);

    Or (a=aAndnotsel, b=bAndsel, out=out);
}
```


### 7. DMUX(de-multiplexor) 's HDL

```
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/01/DMux.hdl

/**
 * Demultiplexor:
 * {a, b} = {in, 0} if sel == 0
 *          {0, in} if sel == 1
 */

CHIP DMux {
    IN in, sel;
    OUT a, b;

    PARTS:
    Not (in=sel, out=notsel);

    And (a=in, b=notsel, out=a);
    And (a=in, b=sel, out=b);
}
```


## 2장. boolean arithmetic

gate를 조합하여 half-adder, adder를 만들고,
2진수의 '보수' 성질을 이용하여 +,- 계산기(Arithmetic Logic Unit, ALU)를 만든다.
(곱셈, 나눗셈은 software layer에서 처리)


### 8. Half-Adder's HDL

```
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/02/HalfAdder.hdl

/**
 * Computes the sum of two bits.
 */

CHIP HalfAdder {
    IN a, b;    // 1-bit inputs
    OUT sum,    // Right bit of a + b
        carry;  // Left bit of a + b

    PARTS:
    Xor (a=a, b=b, out=sum);
    And (a=a, b=b, out=carry);
}
```

### 9. Full-Adder's HDL

```
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/02/FullAdder.hdl

/**
 * Computes the sum of three bits.
 */

CHIP FullAdder {
    IN a, b, c;  // 1-bit inputs
    OUT sum,     // Right bit of a + b + c
        carry;   // Left bit of a + b + c

    PARTS:
    HalfAdder (a=a, b=b, sum=partialSum, carry=firstCarry);
    HalfAdder (a=c, b=partialSum, sum=sum, carry=SecondCarry);
    Or (a=firstCarry, b=SecondCarry, out=carry);
}
```


### 10.ALU's HDL

```
 // This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/02/ALU.hdl

/**
 * The ALU (Arithmetic Logic Unit).
 * Computes one of the following functions:
 * x+y, x-y, y-x, 0, 1, -1, x, y, -x, -y, !x, !y,
 * x+1, y+1, x-1, y-1, x&y, x|y on two 16-bit inputs,
 * according to 6 input bits denoted zx,nx,zy,ny,f,no.
 * In addition, the ALU computes two 1-bit outputs:
 * if the ALU output == 0, zr is set to 1; otherwise zr is set to 0;
 * if the ALU output < 0, ng is set to 1; otherwise ng is set to 0.
 */

// Implementation: the ALU logic manipulates the x and y inputs
// and operates on the resulting values, as follows:
// if (zx == 1) set x = 0        // 16-bit constant
// if (nx == 1) set x = !x       // bitwise not
// if (zy == 1) set y = 0        // 16-bit constant
// if (ny == 1) set y = !y       // bitwise not
// if (f == 1)  set out = x + y  // integer 2's complement addition
// if (f == 0)  set out = x & y  // bitwise and
// if (no == 1) set out = !out   // bitwise not
// if (out == 0) set zr = 1
// if (out < 0) set ng = 1

CHIP ALU {
    IN
        x[16], y[16],  // 16-bit inputs
        zx, // zero the x input?
        nx, // negate the x input?
        zy, // zero the y input?
        ny, // negate the y input?
        f,  // compute out = x + y (if 1) or x & y (if 0)
        no; // negate the out output?

    OUT
        out[16], // 16-bit output
        zr, // 1 if (out == 0), 0 otherwise
        ng; // 1 if (out < 0),  0 otherwise

    PARTS:
        Mux16 (a=x, b=false, sel=zx, out=maybeZeroedX);
        Mux16 (a=y, b=false, sel=zy, out=maybeZeroedY);

        Not16 (in=maybeZeroedX, out=negX);
        Not16 (in=maybeZeroedY, out=negY);

        Mux16 (a=maybeZeroedX, b=negX, sel=nx, out=transformedX);
        Mux16 (a=maybeZeroedY, b=negY, sel=ny, out=transformedY);

        Add16 (a=transformedX, b=transformedY, out=sum);
        And16 (a=transformedX, b=transformedY, out=and);

        Mux16 (a=and, b=sum, sel=f, out=output);
        Not16 (in=output, out=negatedOutput);

        Mux16 (a=output, b=negatedOutput, sel=no,  out=out, out[15]=ng, out[0..7]=firstHalfOut, out[8..15]=secondHalfOut);

        Or8Way (in=firstHalfOut, out=firstOr);
        Or8Way (in=secondHalfOut, out=secondOr);
        Or (a=firstOr, b=secondOr, out=or16Way);
        Not (in=or16Way, out=zr);
}

```

gate를 연역적으로 조합해서 만든 것들을
조합해서 덧셈, 뺄셈 가능한 ALU를 만든다.




## 3장. sequential logic

1. data flip-flop
2. bit
3. register
4. multiple RAM memory

now we can maintain values across time.

---
\+ program counter for delayed recursion.


Q. what is delayed recursion?

As the function executes, the program counter increments to keep track of the next instruction to be executed. When the function encounters a recursive call, instead of immediately executing the call, the program counter's value and any necessary information are stored in a memory structure, such as a stack or a queue.

Delaying recursion: Instead of making the recursive call immediately, the program counter is updated with the address of the next non-recursive instruction. This delays the execution of the recursive function, allowing the system to execute other tasks or optimize the order of recursive calls.



### 11. data flip flop's HDL
```
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: tools/builtIn/DFF.hdl

/**
 * Data Flip-flop: out(t) = in(t-1)
 * where t is the current time unit, or clock cycle.
 */

CHIP DFF {

    IN  in;
    OUT out;

    BUILTIN DFF;
    CLOCKED in;
}
```

### 12. bit's HDL

```
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/03/a/Bit.hdl

/**
 * 1-bit register:
 * If load[t] == 1 then out[t+1] = in[t]
 *                 else out does not change (out[t+1] = out[t])
 */

// In clocked chips we can use multiple `out` and those multiple `out`
// Think of intermediate variables as initialized with a 0 value

CHIP Bit {
    IN in, load;
    OUT out;

    PARTS:
    Mux (a=prevResult, b=in, sel=load, out=result);
    DFF (in=result, out=prevResult);
    DFF (in=result, out=out);
}
```



### 13. register's HDL
```
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/03/a/Register.hdl

/**
 * 16-bit register:
 * If load[t] == 1 then out[t+1] = in[t]
 * else out does not change
 */

CHIP Register {
    IN in[16], load;
    OUT out[16];

    PARTS:
    Bit (in=in[0], load=load, out=out[0]);
    Bit (in=in[1], load=load, out=out[1]);
    Bit (in=in[2], load=load, out=out[2]);
    Bit (in=in[3], load=load, out=out[3]);
    Bit (in=in[4], load=load, out=out[4]);
    Bit (in=in[5], load=load, out=out[5]);
    Bit (in=in[6], load=load, out=out[6]);
    Bit (in=in[7], load=load, out=out[7]);
    Bit (in=in[8], load=load, out=out[8]);
    Bit (in=in[9], load=load, out=out[9]);
    Bit (in=in[10], load=load, out=out[10]);
    Bit (in=in[11], load=load, out=out[11]);
    Bit (in=in[12], load=load, out=out[12]);
    Bit (in=in[13], load=load, out=out[13]);
    Bit (in=in[14], load=load, out=out[14]);
    Bit (in=in[15], load=load, out=out[15]);
}
```

### 14. RAM8's HDL

```
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/03/a/RAM8.hdl

/**
 * Memory of 8 registers, each 16 bit-wide. Out holds the value
 * stored at the memory location specified by address. If load==1, then
 * the in value is loaded into the memory location specified by address
 * (the loaded value will be emitted to out from the next time step onward).
 */

CHIP RAM8 {
    IN in[16], load, address[3];
    OUT out[16];

    PARTS:
    // 1. Create 8 load values from which we can only have one being 1.
    // 2. Load all in values to the registers with their load value
    // 3. Select the value of the appropriate register
    DMux8Way (in=load, sel=address, a=loadA, b=loadB, c=loadC, d=loadD, e=loadE, f=loadF, g=loadG, h=loadH);

    Register (in=in, load=loadA, out=registerA);
    Register (in=in, load=loadB, out=registerB);
    Register (in=in, load=loadC, out=registerC);
    Register (in=in, load=loadD, out=registerD);
    Register (in=in, load=loadE, out=registerE);
    Register (in=in, load=loadF, out=registerF);
    Register (in=in, load=loadG, out=registerG);
    Register (in=in, load=loadH, out=registerH);

    Mux8Way16 (a=registerA, b=registerB, c=registerC, d=registerD, e=registerE, f=registerF, g=registerG, h=registerH, sel=address, out=out);
}
```





## 4장. Von Neumann architecture

![](images/2023-04-17-20-58-56.png)




## 5장. machine code


ex.
```assembly
(LOOP)
	@END
	D;JEQ

	@sum
	M=M+D
	D=D-1

	@LOOP
	0;JMP
```
The above code adds all consecutive integers between 0 and some number, storing the sum in a variable sum.


## 6장. assembler

assembly language -> binary machine instruction


## 7장. virtual machine - stack arthmetic


different CPU have different machine languages




## 8장. virtual machine - program control

## 9장. compiler I - syntax analysis
## 10장. compiler II - code generation

## 11장. operating system

## 12장. high level language




# source

1. website: https://www.nand2tetris.org/
2. course: https://www.coursera.org/learn/build-a-computer
3. book: 밑바닥 부터 만드는 컴퓨팅 시스템

