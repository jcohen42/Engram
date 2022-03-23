---
title: While Loops
description: Adding repetition to your program
filename: 7-while.md
---

# Introduction
While loops are used when we want to execute multiple sentences over and over. This is why while loops are called "loops", because they do the same thing over and over in a loop. For example, we can continuously divide a larger number by 2 until it becomes 0 or 1. The condition for this while loop would be `While x is greater than 1:`.

While blocks in Engram are formed like this:
```
While condition:
	(sentences inside the while block)
	(these will be executed repeatedly until the condition becomes false)
	(after the last sentence is executed, Engram will go back to the condition...)
	(at the top of the while loop)
End while.
```

# Conditions
Just like with if statements, while blocks use conditions in order to determine whether to execute the sentences inside of the loop. If the condition is true, Engram will execute them. Otherwise, Engram will skip those sentences entirely.

# The Body of the Loop
You can write any type of sentence inside of a while loop, including another while loop. The catch here is that after the last sentence is executed, Engram will check the condition at the beginning of the loop again. If the condition is still true, then the first sentence after the condition will be run, and then the next one, and so on...

Let's write a quick example of a while loop and unpack it afterwards:
```
Begin program:
	Output "Input a number:".
	Input number.
  
	While number is greater than 0:
		Output number.
		Set number equal to number minus 1.
	End while.
  
	Output "All done!".
End program.
```

This short program asks the user for a number. It then displays each number between that number and 1, counting down. The larger the input number, the more the while loop will run. Of course, if the user inputs a number that is not greater than 0, then the while loop will not run at all (because the first check of the condition will evaluate to false). This while loop will repeat the sentences inside of it until `number` is less than or equal to 1. We decrease `number` by one at the end of the while loop. This ensures that at some point, `number` will be smaller than 0 - meaning the condition will be evaluated to false. The while loop will then exit and Engram will output "All done!". Copy and paste this program into your Engram Editor to see it in action.

# Avoiding Infinite Loops
If you aren't careful with the condition of your while loop, you may encounter an infinite loop. This happens when the condition of the loop is *always* true, and thus the while loop never stops. For this reason, you should make sure to update one of the condition variables inside the body of the while loop so that at some point the condition will become false.

The Engram Editor has a built-in way of handling infinite loops to prevent crashes. Once a loop has run 2048 times, Engram will force the program to stop.

# Exercise: Printing Out Multiples of a Number
For this exercise, ask the user for a number and how many multiples of the number they would like to print out (think of the multiplication table). You should then make a while loop that outputs each result. For example, if the user inputs 4 as the number and 13 as the number of multiples, then your program should output the result of 4 x 0, 4 x 1, 4 x 2, all the way up to 4 x 13.

Hint: you may need an extra variable to keep track of the current multiple.

## Next Section
[Functions](8-functions.md)

## Previous Section
[If and Else Statements](6-ifelse.md)
