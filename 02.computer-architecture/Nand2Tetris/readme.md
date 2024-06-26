# index

A. hardware layer

1. boolean logic gate :white_check_mark:
2. boolean arithmetic :white_check_mark:
3. sequential logic :white_check_mark:
4. machine code :white_check_mark:
5. Von Neumann architecture :white_check_mark:

B. software layer

6. assembler :white_check_mark:
7. virtual machine - stack arthmetic
8. virtual machine - program control
9. compiler I - syntax analysis
10. compiler II - code generation
11. operating system
12. high level language


# summary

![](images/2023-04-17-19-56-27.png)
![](images/2023-04-17-19-56-43.png)
![](images/nand2tetris-index.jpg)

# chapters

## 1장. boolean logic gate

0,1 boolean input 두개가 gate로 들어온다.

AND, NOT gate의 조합으로 nand, or, xor, multiplexor demultiplexor, bus, multibit, 다입력 (not, and, or, multiplexor) 을 만든다.


### 실습

![](images/2023-04-18-17-05-05.png)

1. HardwareSimulator.sh 실행 (commandline > sh projects/tools/HardwareSimulator.sh)
2. file -> load chip 에서 and, not, or, xor 등의 chip을 로드하면, 왼쪽 하단 HDL에 PARTS 부분에 어떤 칩의 조합으로 해당 칩이 만들어 졌는지 뜬다.
3. file -> load script로 해당 칩의 test file을 로드. 오른쪽 화면에 테스트 케이스 별로 input, output이 뜬다.
4. 왼쪽 상단 >, >> 버튼 클릭해서 테스트 실행

### 0. 드모르간의 법칙

![](images/2023-04-18-17-57-15.png)


### 1. NOT's HDL

![](images/2023-04-18-17-32-13.png)

![](images/2023-04-18-18-18-08.png)

![](images/2023-04-18-18-36-32.png)

implement NOT gate using NAND gate



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

![](images/2023-04-18-18-17-47.png)

![](images/2023-04-18-18-38-56.png)

1 nand gate + 1 not gate

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

![](images/2023-04-18-18-18-26.png)

![](images/2023-04-18-18-38-05.png)

2 not gates + 1 nand gates

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

![](images/2023-04-18-18-18-36.png)

![](images/2023-04-18-18-39-48.png)




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

![](images/2023-04-18-18-41-55.png)

![](images/2023-04-18-18-18-49.png)

it takes 3 inputs(selector, x, y)

if selector is 0, output is first input. (regardless of second input)\
if selector is 1, output is second input. (regardless of first input)


"if"를 mux로 만든다.


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

![](images/2023-04-18-18-42-31.png)

![](images/2023-04-18-18-21-30.png)

demux taks 1 input and a 'selector'\
and it routes the data to different output, depending on the selector


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

```
0000 0000 0000 0011
0000 0000 0000 0101
ADD
```

2진수의 덧셈, 뺄셈 구현하기.


### 8. Half-Adder's HDL

![](images/2023-04-18-18-45-15.png)

'half' adder라고 불리는 이유는, carry에 대해 암것도 안하기 떄문.

![](images/2023-04-18-18-45-39.png)


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

'half' adder가 carry에 대해 암것도 안하기 때문에, full adder에서 carry 처리한다.

![](images/2023-04-18-18-50-09.png)

![](images/2023-04-18-18-50-17.png)



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


### 10. Inc16 (incrementor)'s HDL

input + 1 해주는 놈.

int a = 10;
a++;

할 때, 쓰이는 놈인가?


```
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/02/Inc16.hdl

/**
 * 16-bit incrementer:
 * out = in + 1 (arithmetic addition)
 */

CHIP Inc16 {
    IN in[16];
    OUT out[16];

    PARTS:
    HalfAdder (a=in[0], b=true, sum=out[0], carry=carry1);
    HalfAdder (a=in[1], b=carry1 , sum=out[1], carry=carry2);
    HalfAdder (a=in[2], b=carry2 , sum=out[2], carry=carry3);
    HalfAdder (a=in[3], b=carry3 , sum=out[3], carry=carry4);
    HalfAdder (a=in[4], b=carry4 , sum=out[4], carry=carry5);
    HalfAdder (a=in[5], b=carry5 , sum=out[5], carry=carry6);
    HalfAdder (a=in[6], b=carry6 , sum=out[6], carry=carry7);
    HalfAdder (a=in[7], b=carry7 , sum=out[7], carry=carry8);
    HalfAdder (a=in[8], b=carry8 , sum=out[8], carry=carry9);
    HalfAdder (a=in[9], b=carry9 , sum=out[9], carry=carry10);
    HalfAdder (a=in[10], b=carry10 , sum=out[10], carry=carry11);
    HalfAdder (a=in[11], b=carry11 , sum=out[11], carry=carry12);
    HalfAdder (a=in[12], b=carry12 , sum=out[12], carry=carry13);
    HalfAdder (a=in[13], b=carry13 , sum=out[13], carry=carry14);
    HalfAdder (a=in[14], b=carry14 , sum=out[14], carry=carry15);
    HalfAdder (a=in[15], b=carry15 , sum=out[15], carry=carry16);
}
```

![](images/2023-04-18-21-26-29.png)

incrementor + program counter


### 11.ALU's HDL

![](images/2023-04-18-19-01-41.png)



flags = {zx, nx, zy, ny, f, no}

1. zx,nx, zy,ny flags are going to be applied first.
2. then, 'f' will be applied (ex. addition)
3. lastly, 'no' will be applied. 'negate output flag'



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
		////////////////////////////////////////////
		//case1) flags별로 나눠서 적용한 HDL
		// ZX and ZY flag handling
		Mux16(a=x, b[0..15]=false, sel=zx, out=xMux);
		Mux16(a=y, b[0..15]=false, sel=zy, out=yMux);


		// NX and NY flag handling
		Not16(in=xMux, out=notX);
		Mux16(a=xMux, b=notX, sel=nx, out=readyX);
		Not16(in=yMux, out=notY);
		Mux16(a=yMux, b=notY, sel=ny, out=readyY);


		// f control bit handling
		And16(a=readyX, b=readyY, out=andOut);
		And16(a=readyX, b=readyY, out=addOut);
		Mux16(a=andOut, b=addOut, sel=f, out=fOut);


		// NO flag handling
		Not16(in=fOut, out=fNotOut);
		Mux16(a=fOut, b=fNotOut, sel=no, out=out, out[15]=ng, out[0..7]=zrLow, out[8..15]=zrHigh);


		// ZR flag handling
		Or8Way(in=zrLow, out=or1Out);
		Or8Way(in=zrHigh, out=or2Out);
		Or(a=or1Out, b=or2Out, out=or3Out);
		Not(in=or3Out, out=zr);



		//////////////////////////////////////////
		//case2) 기존 nand2tetris의 implementation

        //Mux16 (a=x, b=false, sel=zx, out=maybeZeroedX);
        //Mux16 (a=y, b=false, sel=zy, out=maybeZeroedY);

        //Not16 (in=maybeZeroedX, out=negX);
        //Not16 (in=maybeZeroedY, out=negY);

        //Mux16 (a=maybeZeroedX, b=negX, sel=nx, out=transformedX);
        //Mux16 (a=maybeZeroedY, b=negY, sel=ny, out=transformedY);

        //Add16 (a=transformedX, b=transformedY, out=sum);
        //And16 (a=transformedX, b=transformedY, out=and);

        //Mux16 (a=and, b=sum, sel=f, out=output);
        //Not16 (in=output, out=negatedOutput);

        //Mux16 (a=output, b=negatedOutput, sel=no,  out=out, out[15]=ng, out[0..7]=firstHalfOut, out[8..15]=secondHalfOut);

        //Or8Way (in=firstHalfOut, out=firstOr);
        //Or8Way (in=secondHalfOut, out=secondOr);
        //Or (a=firstOr, b=secondOr, out=or16Way);
        //Not (in=or16Way, out=zr);
}

```

gate를 연역적으로 조합해서 만든 것들을
조합해서 덧셈, 뺄셈 가능한 ALU를 만든다.




## 3장. sequential logic

![](images/2023-04-18-19-13-21.png)

computer는 oscilator를 이용해 voltage가 한번 0 -> +1 -> -1 -> 0 돌아오는 시간, tick-tock을 1 clock cycle, 1 time unit으로 정한다.

시간 지나서 '기억' 하려면, input -> output인 칩이 필요.\
단, 중간에 clock cycle이 1번 돈 이후에 output을 출력하려면, clock도 필요하겠지?\
이게 flip-flop(a.k.a Latch).

![](images/2023-04-18-19-17-11.png)
![](images/2023-04-18-19-18-36.png)

input 'D' is changing, but output 'Q' does NOT change, until clock advances to next cycle.


nand2tetris의 data flip-flop은 delay, hold on a value by 1 clock cycle


---

1. data flip-flop
	- out(t) = in(t-1)
2. bit
	- out(t) = out(t-1)
3. 16bit-register
4. RAM memory(RAM8, RAM64, ...)
	- random access memory
	- device must allow random addresses to be accessed in the same amount of time, and accepts the following inputs:
		1. load: a bit that indicates whether to read or write data
		2. address: the address to access
		3. data: the data to write to address, if load is set.
	- its parameter must includes..
		1. width: the number of bits per register
		2. size: the number of registers
5. Program Counter
	- PC.hdl

with these, we can maintain values across time.


### 12. data flip flop's HDL

flip flop
![](images/2023-04-18-19-38-06.png)

DFF gate
![](images/2023-04-22-21-20-02.png)


---
step1. initial state

![](images/2023-04-22-21-11-12.png)

we do not know the output yet.

---
step2. saving a state

![](images/2023-04-22-21-11-59.png)

input (1,1) -> (0,1) 으로 바꿀 때, output(1,0)이 되는데,
이 때, input(1,1) 다시 toggle해도 output이 바뀌지 않음.
memory!

remembering 1 bit of data = flip flop

---
step3. reset memory

![](images/2023-04-22-21-16-11.png)

input을 (1,0) 하면 기억했던걸 버리고 output(0,1) 상태가 된다.



---
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

self-referencing circuit\
1 clock cycle 이후 input을 output으로 출력한다.




### 13. bit's HDL

out(t) = out(t-1)


![](images/2023-04-22-21-21-28.png)

![](images/2023-04-22-21-21-40.png)



![](images/2023-04-18-19-32-31.png)

1. 사다리꼴은 multiplexor
2. D라고 써진 칩이 data flip-flop
3. D에 뭍어있는 저 oscilator 모양은 clock

---
input to true 하고, clock을 toggle해도, output엔 변화가 없다.\
왜냐면 load bit is not set.\
if I set input and load bit to 1, output is still zero,\
if you advance the clock, then output becomes 1.


![](images/2023-04-18-19-36-15.png)

output이 1인 상태에서,\
input, load 둘다 0로 변경한 다음,\
clock cycle을 아무리 많이 advanced한다고 해도,\
output is always 1, because of that loop

---

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

'if' -> multiplexor





### 14. register's HDL

8 bit = 1 register

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

### 15. RAM8's HDL

![](images/2023-04-22-21-27-00.png)

1. in
	- input data
2. load
	- deciding whether we save it
3. address
	- telling the ram where to save the data, or which slot do we want to peek at this moment.



64bit (8 register) RAM with memory bank

---
RAM is volatile, which means if we turn off the power of the computer, everything in the ram is gone. But our hard disk or flash drive are non_volatile memories, they can store states and can keep them there forever.

---
best part about the RAM is that though we have thousands of addresses in the ram,\
but we can use O(1) time to get the value of the mem location that we want,
the beauty lies in the Muxer, and Demuxer.

if it’s a 16K RAM system, the Muxer will use only 10 digits to locate the desired location, which is really an elegant design.


---
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

	//memory bank
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
challenge:\
how to load new data into the circuits, while still allowing you to persist old value, that are already in the circuits, when new data is not being loaded?



### 16. Program Counter's HDL

![](images/2023-04-22-21-00-10.png)

counter는 시간 개념을 부여

![](images/2023-04-22-21-55-51.png)



```
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/03/a/PC.hdl

/**
 * A 16-bit counter with load and reset control bits.
 * if      (reset[t] == 1) out[t+1] = 0
 * else if (load[t] == 1)  out[t+1] = in[t]
 * else if (inc[t] == 1)   out[t+1] = out[t] + 1  (integer addition)
 * else                    out[t+1] = out[t]
 */

CHIP PC {
    IN in[16],load,inc,reset;
    OUT out[16];

    PARTS:

	//case1) explained by Tea Leaves
	Register(in=resetMuxOut, load=true, out=out, out=regOut);
	Inc16(in=regOut, out=incOut);

	//if(reset[t] == 1) ...
	Mux16(a=loadMuxOut, b[0..15]=false, sel=reset, out=resetMuxOut); // The "reset" mux
	//if(load[t] == 1) ...
	Mux16(a=incMuxOut, b=in, sel=load, out=loadMuxOut) // The "load" mux
	//if(inc[t] == 1) ...
	Mux16(a=regOut, b=incOut, sel=inc, out=incMuxOut); // The "inc" mux


	//case2)
    // Compute the 3 possibilites
    // Decide what to output
    //Not16 (in=true, out=resetVal);
    //Mux16 (a=false, b=in, sel=load, out=loadVal);
    //Inc16 (in=prevOut , out=incOut);
    //Mux16 (a=false, b=incOut, sel=inc, out=incVal);

    //Mux16 (a=prevOut, b=incVal, sel=inc, out=incResult);
    //Mux16 (a=incResult, b=loadVal, sel=load, out=loadResult);
	//Mux16 (a=loadResult, b=resetVal, sel=reset, out=result);

    //Register (in=result, load=true, out=prevOut);
    //Register (in=result, load=true, out=out);
}
```

The output of the PC is actually an address. The address to a ROM location that the program is written on.



---
if -> Multiplexor(MUX)

1. if(reset[t] == 1) -> throw that away!
reset is the most important part.
reset is mux closest to the register. because if reset is true, it doesn't matter what load it is. we will throw them away.


ref: https://youtu.be/lo54MEu7u9A?list=PLu6SHDdOToSdD4-c9nZX2Qu3ZXnNFocOH&t=1090





## 4장. machine code

![](images/2023-04-18-20-35-57.png)

A instruction -> starts with 0\
C instruction -> starts with 1


A instruction
- red - first line is A instruction(because it starts with 0)
- green - A instruction says, put this number in this register.
	- ex. @2 == put number 2 in 'A' register.

C instruction
- red - next line is C instruction, because it starts with 1
- grey - bit 2nd, 3rd are un-used
- blue - bit 4th indicates whether the alu will treat the register as a direct or as an index into memory
- skyblue - bit 5~10th tells alu what computations to do. (take a look at input of alu above)
- orange - bit 11~13th tells what combinations of a,m,d registers the results of the operations to be stored in.
- pink - bit 14~16th tells the cpu under what conditions, it should jump(goto) that is to change the program counter to new destination.




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


## 실습

1. sh tools/Assembler.sh
2. load 04/mult/Mult.asm or 04/fill/Fill.asm
3. load .hack file into the sh tools/CPUEmulator.sh and run the program

### part1. Fill.asm -> Fill.hack

1. Fill.asm
```
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, the
// program clears the screen, i.e. writes "white" in every pixel.

// Put your code here.

  (START)
    // There are 8192 registers dealing with the screen
    // initialize numScreenRegistersToPaint to 8192
    @8192
    D=A
    @numScreenRegistersToPaint
    M=D

    // Set the value of var currentScreenRegister to the first register of the screen
    @SCREEN
    D=A
    @currentScreenRegister
    M=D

    // Set D to the value of keyboard. 0=> No press // 1=> press
    @KBD
    D=M

    // If no press => set paint color to white
    @SETWHITE
    D; JEQ

    // If press => set paint color to black
    @SETBLACK
    0; JMP

  // Set var color to 0
  (SETWHITE)
    @color
    M=0

    @PAINT
    0;JMP

  // Set var color to -1
  (SETBLACK)
    @color
    M=-1

    @PAINT
    0;JMP

  // Paint the screem
  (PAINT)
    // Set D to value of color
    @color
    D=M

    // Set the value of A to the screen register number and modify its value
    // to have the value of color (0 white; -1 black)
    @currentScreenRegister
    A=M
    M=D

    // Add one to currentScreenRegister (to paint the right pixels in the next
    // loop)
    @currentScreenRegister
    M=M+1

    // Subtract 1 from numScreenRegistersToPaint
    @numScreenRegistersToPaint
    M=M-1
    D=M

    // if numScreenRegistersToPaint is 0 => go to start program
    @START
    D; JEQ

    // if numScreenRegistersToPaint is not 0, continue painting
    @PAINT
    0; JMP

```

Fill.hack
```
0010000000000000
1110110000010000
0000000000010000
1110001100001000
0100000000000000
1110110000010000
0000000000010001
1110001100001000
0110000000000000
1111110000010000
0000000000001110
1110001100000010
0000000000010010
1110101010000111
0000000000010010
1110101010001000
0000000000010110
1110101010000111
0000000000010010
1110111010001000
0000000000010110
1110101010000111
0000000000010010
1111110000010000
0000000000010001
1111110000100000
1110001100001000
0000000000010001
1111110111001000
0000000000010000
1111110010001000
1111110000010000
0000000000000000
1110001100000010
0000000000010110
1110101010000111
```

![](images/2023-04-18-20-45-42.png)

Assembler로 .asm의 어셈블리어를 0101의 기계어로 line by line 번역하는걸 볼 수 있다.



### part 2. CPUEmulator -> run Fill.hack

![](images/2023-04-18-20-47-34.png)

Fill.hack의 machine code가 RAM 메모리 주소에 값을 바꾸는걸 line by line으로 확인 가능하다.

### multiply?

Mult.asm
```
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

// Sum R1 with it self R2 times

// Initialize i to R1
@R1
D=M
@i
M=D

// Initialize result to 0
@R2
M=0

// The loop is going to decrease i by 1 and if its greater than 0 we
// sum R1 to the result if not we go to end
(LOOP)
// If i is 0 => End the program
@i
D=M
@END
D;JEQ

// Add R0 to result
@R0
D=M
@R2
M=D+M

// Decrease the value of i
@i
M=M-1

@LOOP
0; JMP


(END)
@END
0; JMP

```

곱셈을 하려면?
먼저 initialize with 0를 하고,
i번 loop해서 R0를 결과값에 더하기를 반복한다.

결국 곱셈은 더하기 여러번.

---
실습

1. sh CPUEmulator.sh
2. load Mult.hack(recommended) or Mult.asm (also works)
3. click script on left side of red flag to load Mult.tst
4. run




## 5장. Von Neumann architecture

![](images/2023-04-17-20-58-56.png)

1. instruction decoding
	- what are all these control bits in the instruction telling us to do?
2. instruction execution
	- the ALU is going to operate on these registers/inputs, and send the results to these registers/outputs
3. fetch next instruction
	- jump, or not?

### 17. Memory

![](images/2023-04-18-20-38-56.png)

monitor의 메모리 주소는 16374~24575\
keyboard는 메모리 주소는 24576~

input과 메모리 주소를 넣어서 해당 메모리 주소의 값을 변경한다.


memory chip modules should use
1. memory we built using gates
2. maps memory to screens & keyboards (so that we can read and write from screen & keyboard)


```Memory.hdl
 * the range 0x4000-0x5FFF results in accessing the screen memory
 * map. Access to address 0x6000 results in accessing the keyboard
 * memory map. The behavior in these addresses is described in the
 * Screen and Keyboard chip specifications given in the book.
 */

CHIP Memory {
    IN in[16], load, address[15];
    OUT out[16];

    PARTS:
    // The first 16K go from 15-bit 0s(all zeros) to 01111111111111
    // The next 8K (screen) go from 100000000000000 to 101111111111111
    // The keyboard is 110000000000000
    // So basically if we get address[14..15]:
    // 11 => keyboard
    // 10 => screen
    // 00 - 01 => RAM

    DMux4Way (in=load, sel=address[13..14], a=firstRAM, b=secondRAM, c=screenLoad, d=kbdLoad);

    Or (a=firstRAM, b=secondRAM, out=ramLoad);
    RAM16K (in=in, load=ramLoad, address=address[0..13], out=ramOut);

    Screen (in=in, load=screenLoad, address=address[0..12], out=screenOut);
    Keyboard (out=kbdOut);

    Mux4Way16 (a=ramOut, b=ramOut, c=screenOut, d=kbdOut, sel=address[13..14], out=out);
}
```

features)

Only the upper 16K+8K+1 words of the Memory chip are used.
Access to address>0x6000 is invalid. Access to any address in the range 0x4000-0x5FFF results in accessing the screen memory map.

Access to address 0x6000 results in accessing the keyboard memory map.
The behavior in these addresses is described in the Screen and Keyboard chip specifications given in the book.





### 18. CPU

![](images/2023-04-18-21-07-32.png)

- input
1. inM: contains the piece of memory which is indexed by current value of a register in the cpu. depending on the state of cpu, this might be ignored.
2. instruction: instruction the cpu is going to execute
3. reset: signal whether to restart the program(reset the program counter to 0)

- output
1. outM
2. writeM: if it's 1, we will write the content of 'outM' into  memory location(addressM)
3. addressM
4. pc: indicates what the next value of program counter should be.

---
case1) core of CPU without control bits
![](images/2023-04-18-21-27-56.png)


```CPU.asm
// Note this implementation of the CPU doesn't use the given Register A
// and D so in order to test upload `CPU-external.tst`
// Also this implementation uses Mux8Way, a chip not required by any project
// but I found useful to have.

// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/05/CPU.hdl

/**
 * The Hack CPU (Central Processing unit), consisting of an ALU,
 * two registers named A and D, and a program counter named PC.
 * The CPU is designed to fetch and execute instructions written in
 * the Hack machine language. In particular, functions as follows:
 * Executes the inputted instruction according to the Hack machine
 * language specification. The D and A in the language specification
 * refer to CPU-resident registers, while M refers to the external
 * memory location addressed by A, i.e. to Memory[A]. The inM input
 * holds the value of this location. If the current instruction needs
 * to write a value to M, the value is placed in outM, the address
 * of the target location is placed in the addressM output, and the
 * writeM control bit is asserted. (When writeM==0, any value may
 * appear in outM). The outM and writeM outputs are combinational:
 * they are affected instantaneously by the execution of the current
 * instruction. The addressM and pc outputs are clocked: although they
 * are affected by the execution of the current instruction, they commit
 * to their new values only in the next time step. If reset==1 then the
 * CPU jumps to address 0 (i.e. pc is set to 0 in next time step) rather
 * than to the address resulting from executing the current instruction.
 */

CHIP CPU {

    IN  inM[16],         // M value input  (M = contents of RAM[A])
        instruction[16], // Instruction for execution
        reset;           // Signals whether to re-start the current
                         // program (reset==1) or continue executing
                         // the current program (reset==0).

    OUT outM[16],        // M value output
        writeM,          // Write to M?
        addressM[15],    // Address in data memory (of M)
        pc[15];          // address of next instruction

    PARTS:
    // 1. Divide instruction into its components
    // if A instruction => instructionType == 1
    // if C instruction => instructionType == 0
    And (a=true, b=instruction[15], out=isInstructionC);
    Not (in=isInstructionC, out=isInstructionA);

    // if C instruction the 4th bit of instruction indicates which input to the ALU
    // either input from memory or from registerA
    Or (a=instruction[12], b=false, out=inputType);

    // Set the appropriate bit values for the ALU computation
    Or (a=instruction[11], b=false, out=zx);
    Or (a=instruction[10], b=false, out=nx);
    Or (a=instruction[9], b=false, out=zy);
    Or (a=instruction[8], b=false, out=ny);
    Or (a=instruction[7], b=false, out=f);
    Or (a=instruction[6], b=false, out=no);

    // Set the appropriate values for where we need to store the output of Register
    And (a=instruction[5], b=isInstructionC, out=storeInA);
    And (a=instruction[4], b=isInstructionC, out=storeInD);
    And (a=instruction[3], b=isInstructionC, out=writeM);

    // Compute the 8 bools for different jump options
    // No jump
    Not (in=true, out=nojmp);
    // Jump
    Not (in=false, out=jmp);
    // is Zero
    And (a=true, b=zr, out=jeq);
    // is Neg
    And (a=true, b=ng, out=jlt);
    // is not Zero
    Not (in=zr, out=jne);
    // is Pos
    Not (in=jle, out=jgt);
    // is Zero or Pos
    Or (a=zr, b=jgt, out=jge);
    // is Zero or Neg
    Or (a=zr, b=ng, out=jle);

    // Compute whether we need to jump in the next instruction
    Mux8Way (a=nojmp, b=jgt, c=jeq, d=jge, e=jlt, f=jne, g=jle, h=jmp, sel=instruction[0..2], out=maybeJump);
    And (a=isInstructionC, b=maybeJump, out=shouldJump);

    // possible value to load to registerA
    Mux16 (a=outALU, b=instruction, sel=isInstructionA, out=registerAVal);
    // Only modify value of register A if isInstructionA or storeInA
    Or (a=isInstructionA, b=storeInA, out=shouldLoadA);
    Register (in=registerAVal, load=shouldLoadA, out=registerA, out[0..14]=addressM);

    // Set value of register D to output of ALU if storeInD is 1
    Register (in=outALU, load=storeInD, out=registerD);

    // if inputType is 1 then use M as the ALU input, otherwise user Register A
    // as the input
    Mux16 (a=registerA, b=inM, sel=inputType, out=inputALU);

    ALU (x=registerD, y=inputALU, zx=zx, nx=nx, zy=zy ,ny=ny ,f=f ,no=no, zr=zr, ng=ng, out=outALU);

    Or16 (a=false, b=outALU, out=outM);

    // Calculate the counter. Remember that reset has priority over load and
    // load has priority over inc, that's why we set inc to true (always the default
    // action) should be to increase the counter.
    PC (in=registerA,load=shouldJump,inc=true,reset=reset, out[0..14]=pc);

}
```

---
case2) suggested by 'Tea Leaves'

![](images/2023-04-18-21-33-52.png)


### 19. Computer



```Computer.asm
// Since our CPU implements its own A and D registers to test run with
// these files: ComputerAdd-external.tst,  ComputerMax-external.tst
// ComputerRect-external.tst


// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/05/Computer.hdl

/**
 * The HACK computer, including CPU, ROM and RAM.
 * When reset is 0, the program stored in the computer's ROM executes.
 * When reset is 1, the execution of the program restarts.
 * Thus, to start a program's execution, reset must be pushed "up" (1)
 * and "down" (0). From this point onward the user is at the mercy of
 * the software. In particular, depending on the program's code, the
 * screen may show some output and the user may be able to interact
 * with the computer via the keyboard.
 */

CHIP Computer {

    IN reset;

    PARTS:
    Memory(in=outMemory ,load=writeM ,address=addressM ,out=inMemory);
    ROM32K (address=pc, out=instruction);
    CPU (reset=reset, instruction=instruction, inM=inMemory, addressM=addressM, writeM=writeM, outM=outMemory, pc=pc);
}
```

- test scripts

1. add.hack
2. max.hack
3. rect.hack (draw a rectable)



## 6장. assembler

![](images/2023-04-18-20-35-57.png)

A instruction -> starts with 0\
C instruction -> starts with 1


A instruction
- red - first line is A instruction(because it starts with 0)
- green - A instruction says, put this number in this register.
	- ex. @2 == put number 2 in 'A' register.

C instruction
- red - next line is C instruction, because it starts with 1
- grey - bit 2nd, 3rd are un-used
- blue - bit 4th indicates whether the alu will treat the register as a direct or as an index into memory
- skyblue - bit 5~10th tells alu what computations to do. (take a look at input of alu above)
- orange - bit 11~13th tells what combinations of a,m,d registers the results of the operations to be stored in.
- pink - bit 14~16th tells the cpu under what conditions, it should jump(goto) that is to change the program counter to new destination.

---
### assembler in python

Q. 아래 assembly 언어를 어떻게 binary machine code(.hack)로 변환하는 어셈블러를 만들지?

A. needs 3 parts
1. symbol table
2. parser module
	- step1. assembly language -> token
3. code module
	- step2. token -> machine code
	- ex
		1. dest
		2. comp
		3. jump


```assembly
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/06/max/Max.asm

// Computes R2 = max(R0, R1)  (R0,R1,R2 refer to  RAM[0],RAM[1],RAM[2])

   @R0
   D=M              // D = first number
   @R1
   D=D-M            // D = first number - second number
   @OUTPUT_FIRST
   D;JGT            // if D>0 (first is greater) goto output_first
   @R1
   D=M              // D = second number
   @OUTPUT_D
   0;JMP            // goto output_d
(OUTPUT_FIRST)
   @R0
   D=M              // D = first number
(OUTPUT_D)
   @R2
   M=D              // M[2] = D (greatest number)
(INFINITE_LOOP)
   @INFINITE_LOOP
   0;JMP            // infinite loop

```

1. 먼저 @R0, @INFINITE_LOOP 같은 애들한테 숫자를 매핑해주는 token, symbol table 필요
2. symbol table에는 screen, keyboard같은 애들의 메모리 주소도 있으면 좋겠다.
3. 어셈블리어를 token으로 파싱하는 메서드 필요
4. token을 binary machine code로 파싱하는 메서드 필요





```assembler.py
# Example: python assembler.py "max/Max.asm"

import sys

if __name__ != '__main__':
    print 'Please run as a self-conatined program'

//step01 - symbol(token 정의)
symbols = {
    'R0': 0,
    'R1': 1,
    'R2': 2,
    'R3': 3,
    'R4': 4,
    'R5': 5,
    'R6': 6,
    'R7': 7,
    'R8': 8,
    'R9': 9,
    'R1O': 10,
    'R11': 11,
    'R12': 12,
    'R13': 13,
    'R14': 14,
    'R15': 15,
    'SCREEN': 16384, //모니터 draw등 할 때 메모리 주소
    'KBD': 24576, //키보드 io 할 때 메모리 주소
    'SP': 0, //memory address of pointer
    'LCL': 1, //memory address of pointer
    'ARG': 2, //memory address of pointer
    'THIS': 3, //memory address of pointer
    'THAT': 4 //memory address of pointer
}
next_address = 16 //예약어0~15 이후에 자유롭게 값을 입력할 메모리 주소. 입력하면 +1.


//removes comments (starting with "//") and whitespaces from a line.
def remove_comments_and_whitespace(line):
    without_comments = line.split('/')[0]
    return without_comments.replace(' ', '')


//reads an input assembly file and returns a list of instructions with comments and whitespaces removed.
def get_instructions(hack_filename):
    hack_text = open(hack_filename, 'r')

    instructions = []
    for line in hack_text.read().split('\r\n'):
        stripped = remove_comments_and_whitespace(line)
        if stripped:
            instructions.append(stripped)

    return instructions

//adds a new symbol to the symbol table(new memory address) or returns its existing value.
def parse_symbol(symbol):
    global next_address
    if symbol not in symbols:
        symbols[symbol] = next_address
        next_address += 1

    return symbols[symbol]

//converts an integer to a binary string of a specified bit size.
def int_to_bin(num, bit_size=16):
    bin_num = bin(num)[2:]
    return (bit_size - len(bin_num)) * '0' + bin_num

//translates an A-instruction (an address or a symbol) into a 16-bit binary string.
def parse_a_instruction(instruction):
    address = instruction[1:]
    try:
        return int_to_bin(int(address))
    except ValueError:
        return int_to_bin(parse_symbol(address))

// translate the 'computation' parts of a C-instruction into their respective binary representations.
def get_bin_comp(comp):
    return {
        '0': '0101010',
        '1': '0111111',
        '-1': '0111010',
        'D': '0001100',
        'A': '0110000',
        '!D': '0001101',
        '!A': '0110001',
        '-D': '0001111',
        '-A': '0110011',
        'D+1': '0011111',
        'A+1': '0110111',
        'D-1': '0001110',
        'A-1': '0110010',
        'D+A': '0000010',
        'D-A': '0010011',
        'A-D': '0000111',
        'D&A': '0000000',
        'D|A': '0010101',
        'M': '1110000',
        '!M': '1110001',
        '-M': '1110011',
        'M+1': '1110111',
        'M-1': '1110010',
        'D+M': '1000010',
        'D-M': '1010011',
        'M-D': '1000111',
        'D&M': '1000000',
        'D|M': '1010101'
    }[comp]

// translate the 'destination' parts of a C-instruction into their respective binary representations.
def get_bin_dest(dest):
    dest_list = ['0', '0', '0']
    if not dest:
        return ''.join(dest_list)
    if 'A' in dest:
        dest_list[0] = '1'
    if 'D' in dest:
        dest_list[1] = '1'
    if 'M' in dest:
        dest_list[2] = '1'
    return ''.join(dest_list)


// translate the 'jump' parts of a C-instruction into their respective binary representations.
def get_bin_jmp(jmp):
    if not jmp:
        return '000'
    return {
        'JGT': '001',
        'JEQ': '010',
        'JGE': '011',
        'JLT': '100',
        'JNE': '101',
        'JLE': '110',
        'JMP': '111'
    }[jmp]


//translates a C-instruction into a 16-bit binary string.
def parse_c_instruction(instruction):
    bin_output = '111'
    dest = None
    jmp = None

    if '=' in instruction:
        [dest, instruction] = instruction.split('=')
    if ';' in instruction:
        [comp, jmp] = instruction.split(';')
    else:
        comp = instruction

    bin_output += get_bin_comp(comp)
    bin_output += get_bin_dest(dest)
    bin_output += get_bin_jmp(jmp)
    return bin_output

##################--------- DRIVER CODE  -------######################  # NOQA

try:
    hack_filename = sys.argv[1]
except IndexError:
    sys.exit('Please add a .asm file as an argument of the assembler')

# 1. Get a list of instructions from input file
instructions = get_instructions(hack_filename)

# 2. Loop through the instructions and store the loops in the symbols table
counter = 0
for idx, val in enumerate(instructions):
    if val[0] == '(' and val[len(val) - 1] == ')':
        loop = val.replace('(', '').replace(')', '')
        symbols[loop] = idx - counter
        counter += 1

# 3. Create the output by modifying the extension of the input file
filename = hack_filename.split('.asm')[0]

output_file = filename + '.hack'
output = open(output_file, 'w')


# 4. Convert each instruction in its binary representation and write it to
# the output file
for instruction in instructions:
    if instruction[0] == '(':
        continue

    elif instruction[0] == '@':
        binary_instruction = parse_a_instruction(instruction)

    else:
        binary_instruction = parse_c_instruction(instruction)

    output.write(binary_instruction + '\r\n')

```

### A-instruction vs C-instruction

in 'Hack' assembly language, the assembly instructions are divided into two types: A-instructions and C-instructions.

A-instruction (Address instruction):

A-instructions are used to set the address register, also called the A-register, to a specified value. They are represented by a line starting with the '@' symbol followed by a non-negative decimal number or a symbol, which can be a variable or a label.

For example:

```assembly
@21 // Sets the A-register to the value 21
@LOOP // Sets the A-register to the address associated with the symbol LOOP
```

In the binary representation, A-instructions have a 16-bit format where the first bit is 0, followed by a 15-bit address:

```binary-machine-code
0xxxxxxxxxxxxxxx
```

C-instruction (Computation instruction):

C-instructions are used for computation and control flow. They specify a computation to be performed on the A-register and/or the data register (D-register), an optional destination for storing the computation result, and an optional jump condition based on the computation result. C-instructions have three parts: destination (dest), computation (comp), and jump (jmp), with the following syntax:

```
dest=comp;jmp
```

For example:

```
D=A // Stores the value of the A-register in the D-register
M=D+1 // Computes D+1 and stores the result in the memory location pointed to by the A-register (M)
D;JGT // If D > 0, jumps to the instruction at the address stored in the A-register
```

In the binary representation, C-instructions have a 16-bit format where the first three bits are 1s, followed by the binary codes for comp (7 bits), dest (3 bits), and jmp (3 bits):

```assembly
111accccccdddddjjj
```

In summary, A-instructions are used to set the address register's value, while C-instructions are used for computations and control flow, specifying computation, destination, and jump conditions.


### 'A', 'D', and 'M' are mnemonics used in C-instructions

Q. what is this 'A', 'D' and 'M' from assembler.py?

```python

// translate the 'computation' parts of a C-instruction into their respective binary representations.
def get_bin_comp(comp):
    return {
        '0': '0101010',
        '1': '0111111',
        '-1': '0111010',
        'D': '0001100',
        'A': '0110000',
        '!D': '0001101',
        '!A': '0110001',
        '-D': '0001111',
        '-A': '0110011',
        'D+1': '0011111',
        'A+1': '0110111',
        'D-1': '0001110',
        'A-1': '0110010',
        'D+A': '0000010',
        'D-A': '0010011',
        'A-D': '0000111',
        'D&A': '0000000',
        'D|A': '0010101',
        'M': '1110000',
        '!M': '1110001',
        '-M': '1110011',
        'M+1': '1110111',
        'M-1': '1110010',
        'D+M': '1000010',
        'D-M': '1010011',
        'M-D': '1000111',
        'D&M': '1000000',
        'D|M': '1010101'
    }[comp]

// translate the 'destination' parts of a C-instruction into their respective binary representations.
def get_bin_dest(dest):
    dest_list = ['0', '0', '0']
    if not dest:
        return ''.join(dest_list)
    if 'A' in dest:
        dest_list[0] = '1'
    if 'D' in dest:
        dest_list[1] = '1'
    if 'M' in dest:
        dest_list[2] = '1'
    return ''.join(dest_list)
```

'A', 'D', and 'M' are mnemonics used in C-instructions to represent specific registers or memory locations involved in the computation

1. 'A': Represents the A-register (Address register). The A-register stores a 16-bit value, which can be an address or a data value.
2. 'D': Represents the D-register (Data register). The D-register stores a 16-bit data value and is often used to hold intermediate results of computations.
3. 'M': Represents the memory location pointed to by the A-register. 'M' is used to read from or write to the memory address stored in the A-register.

- summary
1. A 는 주소를 가르키는 포인터. 데이터 최종적으로 저장하는 메모리 주소.
2. D는 데이터를 가르키는 포인터. alu 연산 결과값 잠시 담아놓는 곳
3. M은 A(주소를 가르키는 포인트)를 가르키는 포인터. A 주소를 read / write할 떄 쓰임

---

example)
In C-instructions, these mnemonics are used in the computation (comp), destination (dest), and jump (jmp) parts. Here are some examples of how they are used in C-instructions:

1. D=A: The value in the A-register is copied to the D-register.
2. M=D+1: The value of D-register + 1 is computed and stored in the memory location pointed to by the A-register.
3. D=D&M: The bitwise AND of the D-register and the value at the memory location pointed to by the A-register is computed and stored in the D-register.


## 7장. virtual machine - stack arthmetic
### 2 tier compiliation process

![](images/2023-04-21-22-07-35.png)

different CPU have different machine languages

device마다 high level language(source code)를 각 기기의 machine code로 번역 불가.

---

![](images/2023-04-21-22-09-39.png)

write once, run everywhere

ex. java does not translate .java directly into machine language.
1. hello.java -> java compiler -> vm code(bytecode)
2. jvm이 bytecode를 각 device에 맞게 bytecode -> assembly -> binary machine code

![](images/2023-04-22-19-58-09.png)


.vm -> .asm 로 변환해주는 virtual machine compiler 만드는 것.

### overview

- meta
	- 가상화란 하드웨어를 소프트웨어로 구현한거다.
	- .jack -> .vm -> .asm -> .hack
		1. compiler가 .jack -> .vm
		2. virtual machine이 .vm -> .asm
		3. assembler가 .asm -> .hack
		4. cpu loads .hack
- .java -> .class (byte code, vm code)
	- VM I - Stack Arithmetic
		- stack arithmetic
			- pop operand from stack
			- performs operations on them
			- push result back on the stack
		- arithmetic & logical command
			- add
			- sub
			- neg
			- eq
			- gt
			- it
			- and
			- or
			- not
		- memory access command
			- push
			- pop
		- virtual memory mapping
			- static
			- function
			- stack
				- pointer
				- local
				- argument
			- heap
				- this
				- that
	- VM II - Program Control
		- program flow command
			- label xxx
			- goto xxx
			- if-goto xxx
		- function calling command (subroutine calling)
			- explaination for understanding
				1. subroutine == procedure == function == method
				2. stack
					- LIFO on function from global stack
			- funcf n
				- (f)
				- Repeat K times
				- push 0
			- call f m
				- Push return-address
				- Push LCL
				- Push ARG
				- Push THIS
				- Push THAT
				- ARG = SP - n - 5
				- LCL = SP
				- Goto f
			- return
				- FRAME = LCL
				- RET = ^(FRAME - 5)
				- \*ARG = pop()
				- SP = ARG + 1
				- THAT = \*(FRAME - 1)
				- THIS = \*(FRAME - 2)
				- ARG = \*(FRAME - 3)
				- LCL = \*(FRAME - 4)
				- Goto RET
		- Bootstrap Code
			- SP = 256
			- call sys.init
				- call os class init functions
				- call Main.main

## 8장. virtual machine - program control

## 9장. compiler I - syntax analysis

![](images/2023-04-21-22-02-24.png)

1. Main.jack + Point.jack (source code)
2. Main.vm + Point.vm (VM code, byte code for java)
3. Points.asm (assembly code)
4. Points.hack (binary machine code)
5. cpu loads Points.hack and execute



## 10장. compiler II - code generation

## 11장. operating system

## 12장. high level language




# source

1. website: https://www.nand2tetris.org/
2. course: https://www.coursera.org/learn/build-a-computer
3. book: 밑바닥 부터 만드는 컴퓨팅 시스템

