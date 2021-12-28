---
title: Variables and Mathematic Operations
description: Learn how to assign and manipulate variables
filename: 5-variables.md
---

# Assigning Variables
Variables are used to store information, and are very important in all programming languages (including Engram!). Every variable has a name and a value. A variable can have any name, as long as it is not a reserved keyword. The name of a variable should make it easy to understand what the value represents. For instance, we could have a variable named `age` which represents how old you are. In some other program, we could have a variable named `width` which represents the width of a rectangle in inches. Variable names can only be one word. `numberOfCats` would be a good name for a variable, but `number of cats` would give a syntax error.

The value of the variable is the whole number attached to the name. This means that whole numbers such as 1, 1337, 0, or -32 can be stored in a variable. If we have a variable named `year`, then the value associated with that variable could be 2021. To make things simpler, numbers with decimal points or fractions cannot be stored in Engram.

Now let's go over how to create variables in Engram and assign values to them. Below is an example of a variable assignment sentence:
```
Set age equal to 19.
```
Notice the sentence structure here. We are setting some variable name equal to some value. In more basic terms, the sentence structure looks like this: `Set <variable name> equal to <value>.`. In this example, we have a variable named `age`, which has a value of 19.

Since every variable has a value, we can actually set a variable equal to the value of another variable:
```
Set width equal to 5.
Set height equal to width.
(the value for both width and height is now 5)
```
This can be very useful in certain situations, like when calculating the area of a square.

# Output
If we want to output the value of a variable to the **Output Viewer**, then we can enlist the help of our new friend: the Output sentence. Outputting a variable's value is easy! All we have to do is type `Output` followed by the name of the variable. If we wanted to output the value of the `age` variable we assigned above, then we would say:
```
Output age.
```
As a short exercise, try assigning multiple variables and then outputting their values in the Engram Editor.

# Input
Input is used to get a number from the user while the program is running. When Engram sees an Input statement, the user must input a value before the program can continue. When input is requested, you can type a value in the **Input Field** and press Enter to send it to the program.

The Input sentence is simple. We use the `Input` keyword followed by the name of the variable, as you can see below:
```
Input age.
```
The value typed in the **Input Field** will then be assigned to the variable. We do not need to specifically assign the variable before we input it. However, we can overwrite a previously-assigned variable using an Input sentence.

Copy and paste this program into the Engram Editor to see how Input and Output sentences can work together:
```
Begin program:
    Output "Enter your age:".
    Input age.
    Output "Your age is:".
    Output age.
End program.
```

# Math
Once we assign variables, we can manipulate them using math to get desired results. We can use a variety of operations on variables: addition, subtraction, multiplication, division, and modulo. Only one operation can be carried out per sentence.

## Addition and Subtraction
Variables or numbers can be added or subtracted to get new values. See some examples of this below:
```
Set var0 equal to 5 plus 10.
(var0 has a value of 15)
Set var1 equal to 9 minus 2.
(var1 has a value of 7)
Set var2 equal to var1 minus var0.
(var2 has a value of -8)
```

## Multiplication and Division
Variables or numbers can be multiplied or divided to get new values. See some examples of this below:
```
Set var3 equal to 6 times 3.
(var3 has a value of 18)
Set var4 equal to 20 divided by 4.
(var4 has a value of 5)
```

## Modulo
The modulo operation returns the remainder of two numbers divided by one another. See some examples of this below:
```
Set var5 equal to 10 modulo 5.
(var5 has a value of 0)
Set var6 equal to 5 modulo 2.
(var6 has a value of 1)
```

# Exercise: Using Math on Input
Using your skills to assign and manipulate variables, try writing a program that calculates the area of a trapezoid. Copy and paste this program skeleton into the Engram Editor and add functionality for calculating the area of a trapezoid. More information on the equation can be found [here](https://www.khanacademy.org/math/geometry-home/geometry-area-perimeter/geometry-area-trap-composite/v/area-of-a-trapezoid-1).
```
Begin program:
    Output "Input lower base of the trapezoid:".
    Input a.
    Output "Input upper base of the trapezoid:".
    Input b.
    Output "Input height of the trapezoid:".
    Input h.
    (add your code here)
End program.
```
Once you have calculated the area, be sure to use an Output sentence to see the results.

Hints:
- Addition, division, and multiplication will be used.
- You will need to assign multiple new variables to get the result.
- A trapezoid with a lower base of 6, an upper base of 12, and a height of 4 has an area of 36.
- A trapezoid with a lower base of 13, an upper base of 3, and a height of 7 has an area of 50.

## Next Section
[If and Else Statements](6-ifelse.md)

## Previous Section
[Writing Your First Program](4-helloworld.md)
