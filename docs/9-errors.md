---
title: Errors
description: Explanations of error messages in Engram
filename: 9-errors.md
---

# What is an Error?
Errors happen when Engram tries to parse a sentence but doesn't know how. So, it stops the program and lets you know what happened. If you put some words or sentences in the wrong place for instance, Engram will give you an error message. There are three main types of errors in Engram: syntax errors, compilation errors, and runtime errors. Each of these will be explained in this section.

# Syntax Errors
When you click the **Run** button, Engram will quickly read through your entire program to make sure all of your sentences are phrased, spelled, and capitalized correctly. If some word or sentence is incorrect, Engram will display a syntax error along with the line number of the mistake:
```
Syntax error encountered on line 6!
```

It may be helpful to think of syntax errors as typos. If you are having a hard time figuring out what was spelled wrong, try referring back to this tutorial or asking a friend to look over your code.

# Compilation Errors
These errors happen when every sentence is spelled and phrased correctly, but Engram sees something that doesn't make sense. Compilation errors happen before your program starts executing, and stop the execution of the program. When you get a compilation error, Engram will let you know how you can fix it. The 4 types of compilation errors are explained below.

## Function Name Mismatch
In the section on functions we mentioned that the function name at the beginning must match the name at the end, with correct capitalization and spelling. If even one letter is different, you will get a function mismatch error like below:
```
Error: Function EvenOrOdd does not match its end specifier.
We expected to receive EvenOrOdd on line 3 but got evenorOdd.
```

Notice that the function name at the beginning is spelled `EvenOrOdd`, but the function name at the end is spelled `evenorOdd`. The letter capitalization is different, which causes this error.

## Function Redeclaration
This error occurs when you write two or more functions with the same name. For example, if you have multiple functions called `func`, then you will get this error message:
```
Error: Function func was declared more than once.
To fix this error, either rename the repeat definition or delete it entirely.
```

## Function Not Found
If you call a function that doesn't exist, you will get this error. You must make sure that when you call a function, you use the exact same spelling and capitalization.
```
Error: Function EvenOrOdd, which was called in line 9, was not found.
Please make sure that this function exists.
```

## Variable Not Initialized
When you try to use a variable that doesn't exist, this error will occur. You must make sure that when you refer to a variable in your sentences, you use the exact same spelling and capitalization. This error usually happens when you do some math on a variable that has not been declared beforehand.
```
Error: Variable height was not initialized before it was used on line 4.
Please ensure that this variable is declared beforehand.
```

# Runtime Errors
Runtime errors happen when Engram is executing a program. Your code could be entirely correct, but user input or a lack of error checking will cause these errors. All of these errors except for "Input not a whole number" will cause Engram to immediately stop executing your program.

## Divide/Mod by 0
Neither mathematicians or computers know how to divide a number by 0. If you try to input 6/0 in your calculator for instance, you will not be given an answer. Engram too cannot divide or modulo a number by 0. If you divide a number by 0, this error message will pop up:
```
Error: Divide by 0. Exiting program.
```

Similarly, computers (or people in general) do not know how to mod by 0. If you say `Set x equal to 6 modulo 0.`, you will get this error:
```
Error: Modulo by 0. Exiting program.
```

If you plan to divide some number by a user-inputted number, be sure to check whether that number is 0 first. If the number is 0, you can output an error message.

## Integer Overflow
Computers can only count so high. If you input a number larger than 2147483647 or smaller than -2147483648 to the **Input Field**, you will get this error:
```
Error: The integer you entered was too large for the program to recognize. Exiting program.
```

## Input Not a Whole Number (Warning)
This error is different from all others, because it does not cause Engram to stop executing your program. If you accidentally type something other than a whole number into the **Input Field**, you will get this error:
```
Warning: Please make sure that you typed in a number, with no letters or punctation marks. Please enter a new number:
```

Here are some examples of inputs that will cause this warning:
- `65e`
- `four`
- `2.50`
- `--12`
- `10 + 7`

# Thank You!
You have reached the end of the Engram online tutorial - you should now know Engram like the back of your hand. Engram is a very simple programming language, and you will find that other programming languages have many more features. For example, variables in Engram can only be whole numbers. In most other languages, variables can be numbers, decimals, characters, and more. Thank you for reading, and best of luck on your future programming endeavors!

## Previous Section
[Functions](8-functions.md)

## Return to Homepage
[Homepage](1-info.md)
