# Bits, Bytes, and Integers

## Represents

- 负数以补码的形式表示 two's components
- 在C语言中，如果一个 *signed* 和一个 *unsigned* 做运算(<, >, ==, <=, >=)，有符号会转换为无符号

## Sign Expanding & Truncating

- Extension rule:
  - make k copies of sign bit (MSB)
- Truncation
  - Mod operation

# Float

### IEEE floating point standard

$$
(-1)^sM2^E
$$

- 32 bits
  - 1 bit s
  - 7 bit e
  - 23 bit m
- E = e - 127
- M = 1.m

### Special case

#### denormalized

- If e = 0
- E = 1 - 127
- if m = 0, represent zero!
- M = 0.m

#### special

- if e = 1111111
- if m = 0, infinity
- if m != 0, NaN (Not a Number)

# Machine-level

## Basics

### Assembly Code View

- PC: Program counter
  - address of next instruction
  - RIP
- Register file
  - Heavily used program data
- Condition codes
  - Store status information about most recent arithmetic or logical operation
  - used for conditional branching
- Memory
  - Byte addressable array
  - Code and user data
  - Stack to support procedures

### Compile and run

![image-20210206212315021](C:\Users\11790\AppData\Roaming\Typora\typora-user-images\image-20210206212315021.png)

- **.c->.s** complier

  - xxx.c and xxx.s are text

- **.o->p** linker

  - .o object and program are binary

  

## x86-64 Registers

- 16 registers
- **%rax** is used to return value
- **%rsp** is used to as stack pointer (current stack top)
- **%eax** can reference low-order 4 bytes

## Assembly Operations

- Transfer data between memory and register
  - Load and store
- Perform arithmetic function on register or memory data
- Transfer control
  - Unconditional jumps to/from procedures
  - Conditional branches
  - Indirect branches

### Moving Data

**movq Src Dest**

- Operand Types
  - Immediate: Constant integer data
    - **$0x400, $-533**
    - prefixed with **$**
  - Register: One of 16 integer registers
    - **%rax, %r13**
  - Memory: 8 consecutive bytes of memory at address given by register
    - **(%rax)**

### Complete Memory Addressing Modes

#### General Form

**D(Rb, Ri, S)    Mem[Reg[Rb] + S * Reg[Ri] + D]**

- D: Constant displacement
- Rb: base register, any of 16 integer registers
- Ri: Index register, except for **%rsp**
- S: Scale: 1, 2, 4, or 8

### Address Computation Instruction

**leaq Src, Dest**

- Src is address mode expression
- Set Dest to address denoted by expression
- e.g. **p = &x**

### Two operand instructions

| Format | Computation   |                    |
| ------ | ------------- | ------------------ |
| addq   | **Src, Dest** | Dest = Dest + Src  |
| subq   | **Src, Dest** | Dest = Dest - Src  |
| imulq  | **Src, Dest** | Dest = Dest * Src  |
| shlq   | **Src, Dest** | Dest = Dest << Src |
| sarq   | **Src, Dest** | Dest = Dest >> Src |
| shrq   | **Src, Dest** | Dest = Dest >> Src |
| xorq   | **Src, Dest** | Dest = Dest ^ Src  |
| andq   | **Src, Dest** | Dest = Dest & Src  |
| orq    | **Src, Dest** | Dest = Dest \| Src |

### One operand Instructions

| Format | Computation |                 |
| ------ | ----------- | --------------- |
| incq   | **Dest**    | Dest = Dest + 1 |
| decq   | **Dest**    | Dest = Dest - 1 |
| negq   | **Dest**    | Dest = -Dest    |
| notq   | **Dest**    | Dest = ~Dest    |

## Assembly Control

### Condition Codes

e.g. addq Src Dest                  t = a + b

|      |                            |                                                              |
| ---- | -------------------------- | ------------------------------------------------------------ |
| CF   | Carry Flag (for unsigned)  | if carry/borrow out from most significant bit (unsigned overflow) |
| ZF   | Zero Flag                  | if t == 0                                                    |
| SF   | Sign Flag (for signed)     | if t < 0 (as signed)                                         |
| OF   | Overflow Flag (for signed) | if two's complement overflow  (a,b>0, t < 0) \|\| (a, b < 0, t > 0) |

### Explicit Reading by Set Instruction

- **setX Dest** set low-order byte of destination Dest to 0 or 1
- Does not alter remaining 7 bytes of *Dest*

| SetX  | Condition        | Description               |
| ----- | ---------------- | ------------------------- |
| sete  | ZF               | Equal / Zero              |
| setne | ~ZF              | Not Equal / Not Zero      |
| sets  | SF               | Negative                  |
| setns | ~SF              | Nonnegative               |
| setg  | ~(SF ^ OF) & ~ZF | Greater (signed)          |
| setge | ~(SF ^ OF)       | Greater or Equal (signed) |
| setl  | SF ^ OF          | Less (signed)             |
| setle | (SF ^ OF) \| ZF  | Less or Equal (signed)    |
| seta  | ~CF & ~ZF        | Above (unsigned)          |
| setb  | CF               | Below (unsigned)          |

### Conditional branches

- **jX** jump to different part of code depending on condition codes
- Implicit reading of condition codes

| jX   | Condition         | Description               |
| ---- | ----------------- | ------------------------- |
| jmp  | 1                 | Unconditional             |
| je   | ZF                | Equal / Zero              |
| jne  | ~ZF               | Not Equal / Not Zero      |
| js   | SF                | Negative                  |
| jns  | ~SF               | Nonnegative               |
| jg   | ~(SF ^ OF) & ~ ZF | Greater (signed)          |
| jge  | ~ (SF ^ OF)       | Greater or Equal (signed) |
| jl   | SF ^ OF           | Less (signed)             |
| jle  | (SF ^ OF) \| ZF   | Less or equal (signed)    |
| ja   | ~CF & ~ZF         | Above (unsigned)          |
| jb   | CF                | Below (unsigned)          |

#### Thinking in assembly

- Change *if, while, switch, for* into **goto**
  - For switch, there may be **multiple case labels, fall through cases and missing cases**
  - The complier may move some assignments (which was outside of the switch statement), into the **case:** (goto)
- In *a ? b : c* expression, both b and c will be computed, only makes sense when computations are very simple

## Procedures

### Mechanisms in Procedures

- Passing Control
  - To beginning of procedure code
  - Back to return point
- Passing data
  - Procedure arguments
  - Return value
- Memory management
  - Allocate during procedure execution
  - Deallocate upon return

### Stack

- Stack bottom is fixed once setup, which has the bigger address
- Stack Pointer **%rsp** points to the stack **TOP**, which has the lower address
- **Grows** toward **lower** address

#### Instructions

- **pushq Src**
  - Fetch operand at Src
  - Decrement **%rsp** by 8
  - Write operand at address given by **%rsp**
- **popq Dest**
  - Read Value at address given by **%rsp**
  - Increment **%rsp** by 8
  - Store value at Dest

### Calling Conventions

#### Passing control

- Procedure call : **call label**
  - push return address on stack
  - jump to label
- Procedure return: **ret**
  - Pop address from stack
  - Jump to address

#### Passing data

- First 6 arguments are stored in the registers
  - **%rdi %rsi %rdx %rcx %r8 %r9**
- The other are stored on stack
- Return value is stored on **%rax**

#### Managing local data

- Stack allocated in frames
- Each function call has its own stack frame

##### Stack Frames

- Return information, local storage, temporary space
- Management
  - Space allocated when enter procedure
  - Deallocated when return
- **%rsp** always points to the top of the stack (lowest address)

## Data

### Array

- Contiguously allocated region in memory
- int *A[5] is pointer array
- int (*A)[5] is an array pointer

#### Multidimensional Arrays

- Row-Major Ordering
- 2D -> 1D

### Structures

- Structure represented as block of memory
- Fields ordered according to declaration
  - Different order may have different size of memory
- Compiler determines overall size + positions of fields
  - Machine-level program has no understanding of the structures in the source code

#### Alignment

- Aligned data with in structure
  - Primitive data type requires **B** types
  - Address must be multiple of **B**
- Motivation
  - Memory accessed by chunks of 4 or 8 butes
    - Inefficient to load or store datum that spans cache lines
    - Virtual memory trickier when datum spans 2 pages.
- Compiler
  - Inserts **gaps** in structure to ensure correct alignment of fields
- Overall structure placement
  - Each structure has alignment requirement **K**
    - K is the lagrest alignment of any element
  - Initial address & structure length must be multiples of **K**

#### Tips of saving space

- Put large data types first

