# Data Types
```
none                # none
object()            # object
1729                # int
3.1415              # float
(2.7 + 3.6i)        # complex
true                # bool
'C'                 # char
"Hello World"       # str
[1, 2, 3]           # list
(1, [1, 2], 'Q')    # tuple
{1: 2, 8.3: "text"} # dict
{1, 6, 4.2}         # set
float               # type
```
# Binary Operators
#### Arithmetic Operators
```
3 + 2.1 # res: 5.1, add
7 - 4   # res: 3  , subtract
2.5 * 8 # res: 20 , multiply
21 / 7  # res: 3.0, division
10 % 3  # res: 1  , modulus
18 // 8 # res: 2  , quotient
4 ** 3  # res: 64 , power
```
### Bitwise Operators
```
37 & 29 # res: 5 , and
89 | 2  # res: 91, or
23 ^ 37 # res: 50, xor
3 << 2  # res: 12, left shift
9 >> 1  # res: 4 , right shift
```
### Comparison Operators
```
5 == 5.0  # res: true , equal
2 != 2    # res: false, not equal
3 < 9.6   # res: true , less than
12 > 0    # res: true , greater than
7 <= 0.8  # res: false, less than or equal
28 >= 28  # res: true , greater than or equal
3 === 3.0 # res: false, exact equal
2 !== 2.0 # res: true , exact not equal
```
### Logical Operators
```
45 && false    # res: false, and
true and true  # res: true , and
"" || [1]      # res: true , or
false or false # res: false, or
none ^^ 2i     # res: true , xor
true xor true  # res: false, xor
```
### Other operators
```
"Hello" in "Hello World" # res: true , in
[1i, 2i, 3i] is complex  # res: false, is
[839, 5, -3, 2] are int  # res: true , are
```
## Unary Operators
### Arithmetic Operators
```
+"Hello"   # res: 5        , positive
-[1, 2, 3] # res: [3, 2, 1], negate
```
### Bitwise Operators
```
~45 # res: -46, not
```
### Logical Operators
```
!true     # res: false, not
not false # res: true , not
```
# Operations
### Ternary Condition
```
true ? 23 : 45      # res: 23     , notation
a = false ? "" : [] # res: a == [], assignment
```
### Subscript
```
"Hello"[1]    # res: 'l', access
(1, 2, 3)[-1] # res: 3  , inverse access
a = [1, 2, 3]
a[2] = 5      # res: a == [1, 2, 5], assignment
```
### Access
```
struct vec
{
	x = 6
	y = 4
}

vec.x      # res: 6          , access
vec.y = 91 # res: vec.y == 91, assignment
```
# Variables
### Assignment
```
a = 6              # res: a == 6   , plain assignment
var b = 49         # res: b == 49  , assignment
var char c = 'E'   # res: c == 'E' , fixed-type assignment
var local d = 0.1  # res: d == 0.1 , local assignment
var global e = 0   # res: e == 0   , global assignment
var const f = "OK" # res: f == "OK", constant assignment
var static g = 53  # res: g == 53  , static assignment
```
### Fixed Reassignment
```
a = 10
++a # res: 11 (a == 11), pre increment
a++ # res: 10 (a == 11), post increment
--a # res: 9 (a == 9)  , pre decrement
a-- # res: 10 (a == 9) , post decrement
```
### Reassignment
```
a = 10
a += 6  # res: (a == 16) , add
a -= 5  # res: (a == 5)  , subtract
a *= 3  # res: (a == 30) , multiply
a /= 4  # res: (a == 2.5), division
a %= 7  # res: (a == 3)  , modulus
a //= 6 # res: (a == 1)  , quotient
a **= 2 # res: (a == 100), power
a &= 41 # res: (a == 8)  , and
a |= 45 # res: (a == 47) , or
a ^= 31 # res: (a == 21) , xor
a <<= 3 # res: (a == 80) , left shift
a >>= 1 # res: (a == 5)  , right shift
```
# Functions
### Define
```
func f():
	print("Hello World")

func g(x, int y, z = 2.3)
{
	a = [x, y, z]
	return a
}

func (x, y):
	[x ** y, y ** x]
```
### Call
```
func f(x = 1, y = 2, z = 3):
	x * (y + z)

f()        # res: 5
f(3, 10)   # res: 39
f(z: 9, 5) # res: 14
```
# Structures
### Define
```
struct s1
{
	var int x
	var int y
}

struct s2
{
	x = 56
	y = true ? "OK" : "NO"
}

struct
{
	a = "Empty"
	b = [none]
}
```
# Statements
### if
```
if true:
	a = 39
elif false
{
	a = 47
	b = 23
}
else:
	c = object()
```
### switch
```
switch 'I'
{
case 'A':
	"GOOD"
case 'B':
	"NICE"
default:
	"WHAT???"
}
```
### for
```
for i = 2 to 10 step 3:
	none

a = []
for i = 30 to 20 step -1:
	a += i

for i = 4 to 40 {}

a = []
for i = to 100
{
	a += i
	a += i ** 2
}
```
### foreach
```
a = []
for i in "Hello":
	a += i

b = []
for i in [1, 2, 3, 4, 5]
{
	b += i
	b += i ** 2
}
```
### loop
```
for (i = 2; i < 10; i++):
	none

a = []
for (; i < 100; i *= 2)
{
	a += i
	a += i ** -1
}

for (; i++ < 1000;):
	none
```
### do-while
```
do:
	none
while true

a = []
i = 0
do
{
	a += i
	a += 3 ** i
} while i++ < 10
```
### while
```
while true:
	none

a = []
i = 0
while i++ < 10
{
	a += i
	a += 5 ** i
} 
```
### try
```
try:
	1 / 0
except 4:
	"DIV BY ZERO"
except "MemOverflowError"
{
	a = 7
	b = 0.7
}
except "'n' is not defined":
	"NOT DEFINED"
finally
{
	c = []
	d = "some text"
}
```
