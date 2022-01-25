---
title: If and Else Statements
description: Adding behavior to your program
filename: 6-ifelse.md
---

# Introduction
If statements help us let our program make decisions while it is running. We can tell the program to do something only if something is true. If statements in programming are very similar to if statements in English. We have a condition and an outcome. For example, in English we can say: If it is cold outside, then I will wear a jacket. In this section we will delve into making our own if statements in Engram.

As a brief intorduction, if blocks in Engram are formed as such:
```
If (condition):
  (sentences inside the if block)
  (these will be executued only if the condition is true)
End if.
```

# Conditions
Conditions are the first building block of forming an if statement (and while loops, which we will cover [later](7-while.md)). We can think of a condition as the gatekeeper into the sentences we write inside of the if block. If the conditition is true, then the gatekeeper will let us in. Conditions always compare two values, whether they be variables like `length` or constant numbers like `6`. There are multiple types of conditions in Engram, which are defined below.

## Greater Than
This conditions compares two values, which will be called the left value and the right value. This condition is written as such: `If value1 is greater than value2:`. If the left value is greater than the left value, then the sentences inside the if block will be executed. If the left value is equal to or less than the right value, then those sentences will be skipped, and Engram will go to the first statement after the `End if` sentence.

Here are some examples of the `greater than` condition in action:
| Left Value | Right Value | Outcome |
| 6 | 4 | sentences inside the if block will be executed |
| 7 | 12 | sentences inside the if block will *not* be executed |
| 8 | 8 | sentences inside the if block will *not* be executed |
| -14 | 2 | sentences inside the if block will *not* be executed |

## Less Than
This condition is written like this: `If value1 is less than value2:`. `less than` is the opposite of `greater than`. If the left value is less than the left value, then the sentences inside the if block will be executed. If the left value is equal to or greater than the right value, then those sentences will be skipped, and Engram will go to the first statement after the `End if` sentence.

Here are some examples of the `less than` condition in action:
| Left Value | Right Value | Outcome |
| 12 | 19 | sentences inside the if block will be executed |
| 10 | 1 | sentences inside the if block will *not* be executed |
| 2 | 2 | sentences inside the if block will *not* be executed |
| -11 | 3 | sentences inside the if block will be executed |

## Equal To
This condition is written like this: `If value1 is equal to value2:`. For this condition, the sentences inside the if block will only be executed if the left value and the right value are equal. Otherwise, Engram will go to the first statement after the `End if` sentence.

Here are some examples of the `equal to` condition in action:
| Left Value | Right Value | Outcome |
| 5 | 5 | sentences inside the if block will be executed |
| 13 | 14 | sentences inside the if block will *not* be executed |
| -2 | 2 | sentences inside the if block will *not* be executed |

## Not Equal To
This condition is written like this: `If value1 is equal to value2:`. As you might expect, this condition will only be true when the left value and the right value are different. Otherwise, Engram will go to the first statement after the `End if` sentence.

Here are some examples of the `not equal to` condition in action:
| Left Value | Right Value | Outcome |
| 3 | 6 | sentences inside the if block will be executed |
| 1 | 1 | sentences inside the if block will *not* be executed |
| -7 | 7 | sentences inside the if block will be executed |

# Writing an if Statement
Now that we know how to set up our conditions, the rest is easy! All we have to do is write the sentences we want to execute after the condition. Note that everything after the `End if.` sentence is not considered part of the if block. 

You may find that indenting the sentences inside of the if block will make it easier to visualize which sentences will be executed if the condition if true. You can also put if statements inside of if statements, if there are multiple conditions that you need to check.

Here is an example of a program with an if block:
```
Begin program:
  Input x.
  If x is greater than 5:
    Output "Your number is greater than 5.".
    Set x equal to x times x.
  End if.
  Output x.
End program.
```

If `x` turns out to be larger than 5, then we will do some math on `x`. Even if `x` is less than or equal to 5, we will still output its value after the if block.

# Else Statements
Earlier we related the condition to a gatekeeper. We said that if the condition was false, then the gatekeeper would not let you execute the sentences inside of the if block. But what if the gatekeeper could lead us into a second gate if the condition was false?

Enter else statements. These allow us to execute a different set of sentences if a condition is false.

Below is an example of an if statement with an else statement at the end:
```
Begin program:
  Input inches.
  If inches is less than 12:
    Output inches.
  Else:
    Output "".
  End if.
End program:
```

# Exercise: Finding the Larger Number
Now that you know how to write if and else statements, try to write a program to find and output the larger of two numbers. Ask the user to input two numbers, and then use conditions to compare the numbers and output the larger one. As an extra challenge, try to find a way to also see if the two numbers are equal!

## Next Section
[While Loops](7-while.md)

## Previous Section
[Variables and Mathematic Operations](5-variables.md)
