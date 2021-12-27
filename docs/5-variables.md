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
(WORK IN PROGRESS)
```
Begin program:
  Output "Enter your age:".
  Input age.
  Output "Your age is:".
  Output age.
End program.
```

# Math

## Addition and Subtraction

## Multiplication and Division

## Modulo

# Exercise: Using Math on Input

# Next Section
[If and Else Statements](6-ifelse.md)

# Previous Section
[Writing Your First Program](4-helloworld.md)
