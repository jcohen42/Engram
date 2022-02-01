---
title: Functions
description: Cutting down on code reuse
filename: 8-function.md
---

# Introduction
As your programs get longer, they inevitably get more complicated. You may find that many parts of your code are written multiple times throughout the program, which adds to the overall length. Functions exist to minimize code reuse and make the program easier to understand. Instead of typing the same code over and over, we only tell the computer what to do once and then call the function when we want to run the code.

Function must be written before the `Begin program:` sentence. In Engram, functions have a name, an argument, a body (similar to if statements and while loops), and optionally, a return statement. All of these aspects will be explained in this section.

The first building block of a function is its name. Like variable names, function names must be one word. Here are some examples of valid function names:
- Add5
- Square
- CalculateArea
- testFunction

A function in Engram looks like this:
```
Begin functionName with argument x:
	(Your code will go here)
End functionName.
```

The above function does nothing - all it has is a single comment. However, notice that the name of the function is `functionName`. This name appears **both** at the beginning and end of the function, with capitalization matching. If the function name at the beginning does not match the function name at the end, a [function name mismatch error](9-errors.md) will occur.

# Calling a Function
Before explaining how to write your own function, let's go over how to call it. When you "call" a function, Engram will find the function and execute all of the sentences it contains. Afterwards, Engram will go back to wherever the function was called from and execute the next sentence. You can call a function from the main program body (the `Begin program` area), from another function, or even from within the same function.

There are two ways to call a function in Engram, depending on whether or not the function has a return statement. If the function does not return anything, you will call it like this: `Call functionName with argument variableName.`. The value of the `variableName` will be copied to the argument of the function.  Also, the variable must be declared before passing it as an argument into the function, as an error will occur otherwise. You can also use a number in the place of the variable like so: `Call functionName with argument 12.`.

If the function has a return sentence, you will call it like this: `Call functionName with argument variableName, set return to returnVariableName.`. The `returnVariableName` mentioned here can of course have any name, but it does not need to be declared beforehand. This means that the return variable can be set for the first time after calling the function.

When calling a function, you must make sure that you type its name exactly right, with correct capitalization. For example, if a function is called `testFunction` but you call it like so: `Call TestFunction with argument x.`, you will get a [function not found error](9-errors.md). Note that the lowercase t in the function name is uppercase when the function is called.

# Function Argument
As mentioned earlier, functions in Engram must have an argument. An argument is a variable that is passed into the function when it is called. You can use that argument in any way you'd like inside the function, as you can see here:
```
Begin funkyFunction with argument x:
	(Your code will go here)
	(As you can see in the line below, you can use the argument in your sentences)
	Set y equal to x times 2.
	Output y.
	(You can write as many sentences as you want in a function!)
End funkyFunction.
```

If you call the function like this: `Call funkyFunction with argument number.`, then the value of `number` will become the value of `x`. Any changes made to `x` will *not* be made to `number`. Even though they have the same value at the beginning of the function, they exist independently of each other.

Functions in Engram can only have one argument, but as you learn other programming languages, you will find that functions can have any number of arguments.

# The Body of the Function
Just like with if statements and while loops, sentences contained within `Begin function` and `End` declarations are considered part of the function. If you happen to mess up the placement or phrasing of your sentences, you will get a syntax error.

After the final sentence of the function is executed, Engram will go back to wherever the function was called from.

# Return Sentence
Functions in Engram may have a return sentence. The return sentence *must* be the last sentence of the function, and looks like this: `Return returnVariable.`. The return variable must be initialized beforehand. The return variable will overwrite whatever variable was specified in the calling sentence, as explained below:
```
Begin AddFive with argument number:
	Set result equal to number plus 5.
	(after the return sentence, Engram will jump back to wherever the function was called)
	Return result.
End AddFive.

Begin program:
	Set x equal to 6.
	(Since x = 6, the value for argument "number" will also be 6)
	Call AddFive with argument x, set return to output.
	(Variable "output" will become the return value)
	Output output.
	(This will output "11" to the Editor)
End program.
```

In this example, we call the `AddFive` function with a variable that has a value of 6, and set the return variable to a new variable. The result is stored in variable `output`, which is then displayed in the Engram Editor.

# Putting it All Together
Now that we've gone over how to write and call functions, let's put our knowledge to the test. Below is a program that asks the user for a number three times and determines whether each one is odd or even.
```
(This program takes 3 numbers from the user and determines if each one is odd or even)
Begin program:
	Output "Input a number:".
	Input num.
      Set modTwo equal to num modulo 2.
	If modTwo is equal to 0:
		Output "The number is even.".
	Else:
		Output "The number is odd.".
	End if.
	
	Output "Input a second number:".
	Input num.
      Set modTwo equal to num modulo 2.
	If modTwo is equal to 0:
		Output "The number is even.".
	Else:
		Output "The number is odd.".
	End if.

	Output "Input a third number:".
	Input num.
      Set modTwo equal to num modulo 2.
	If modTwo is equal to 0:
		Output "The number is even.".
	Else:
		Output "The number is odd.".
	End if.
End program.
```

The program in its current form does not use functions, and reuses a lot of code. The if and else statements take up a lot of space and make the code more difficult to read. If we move the even/odd calculation into a function, then that will surely make our code more readable. Let's create a function called `EvenOrOdd` that takes a variable called `number` as its argument. This function will determine whether `number` is even or odd, and output the result.
```
(This program takes 3 numbers from the user and determines if each one is odd or even)
Begin OddOrEven with argument number:
	Set modTwo equal to number modulo 2.
	If modTwo is equal to 0:
		Output "The number is even.".
	Else:
		Output "The number is odd.".
	End if.
End OddOrEven.

Begin program:
	Output "Input a number:".
	Input num.
	Call OddOrEven with argument num.
	
	Output "Input a second number:".
	Input num.
	Call OddOrEven with argument num.

	Output "Input a third number:".
	Input num.
	Call OddOrEven with argument num.
End program.
```

Now that we moved the even/odd calculation into its own function, the code has become shorter and easier to read. Of course, this updated program does the same thing as the original version. Be sure to use functions whenever you notice repeated code!

# Exercise: Finding the Square of a Number
Try to write a program that uses a function to find the square of a number. Ask the user to input a number three times, and calculate the square of that number in a function and return it back to the program. Then output the squared number to the Editor.

Hint: a number is "squared" when it is multiplied by itself (the square of 4 = 4x4 = 16).

## Next Section
[Errors](9-errors.md)

## Previous Section
[While Loops](7-while.md)
