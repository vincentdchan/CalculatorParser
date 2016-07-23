# CalculatorParser

A simple parser for simple expression in math

Input:

```
1 + 2 * 3
```

Output: 7

Just a simple parser and caculator, it will parse the expression into trees and then calculate it.

# Implementation

Although this is just a toy for programming praticing. But this program is seriously usable.

## Parser

Recursive decent parser written by hand, Combining the knowledge of precedence parsing. 

## Execute way

At first, this program is designed to execute by iterating the abstract syntax tress(ASTs). And you can see the previous versions in the early commits. But at last I decide to use the virtual machine. The program iterator the AST to generate the liner VM codes, and VM execute the codes.



## Vritual Machine

This vm is a stack machine. Use a virtual machine to calculate a math expression is overkill. But I just want to try.



# Updates



## 2016/7/14 Updates

Now you can assign value and expression to varible such as:

Example:

```
let a = 1 + 2 * 3
let b = 1 + a
a + b
```



## 2016/7/23 Updates

Now you can add the parens to change the priority of the expression, such as



```
let a = 3
(a + 2) * 2  // output: 10

10 * (2 + 3) // output: 50
```

