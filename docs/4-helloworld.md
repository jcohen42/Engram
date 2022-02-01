---
title: Writing Your First Program
description: Hello world!
filename: 4-helloworld.md
---

# The Basics
Before we start writing our first program, we must go over the skeleton of a program written for Engram. Similar to other programming languages, code is separated into statements, which we will call "sentences". Each sentence represents one instruction that the Editor will execute when we click the **Run** button. The sentences will be executed in order, meaning that the sentence at the top will go first, then the one below it, and so on. A notable exception to this is functions, which we will go over in [a later section](8-functions.md).

When we click the **Run** button, Engram will look for the `Begin program:` sentence fragment and begin executing code from there. With the exception of functions, all of our code will go after this sentence fragment, as we can see below:
```
Begin program:
	(our code will go here)
	(of course, we can write multiple lines)
End program.
```
It is important that all of our code is written in between the `Begin program:` and `End program.` sentence fragments. If we write some stray sentences before `Begin program:` or after `End program.`, then a syntax error will occur. A syntax error is a type of error where a word is either misspelled or put in the wrong place, causing Engram to stop the execution of the code. Think of syntax errors as typos. Syntax errors will also occur if we misspell or use the incorrect capitalization in our sentences. Can you spot the syntax errors in these examples?
- `Begin progrem:`
- `Begin Program:`
- `Beginprogram:`

If you're itching to learn more about syntax errors, or errors in general, then check out the section on [Errors](9-errors.md).

Also, notice how the sentences sandwiched between our `Begin program:` and `End program.` fragments are shifted over to the right by a few spaces. This is not necessary, but makes reading the code a bit easier. The Tab key on your keyboard makes spacing out code a lot easier!

Now that we know that we should write our code between the `program` sentence fragments, let's learn what we can put there!

# Displaying Output
For our first program, let's display a message on the **Output Viewer**. We use a special type of sentence, called an Output sentence in order to do this. This sentence begins with the word `Output` and is followed by a message surrounded by quotation marks. This is like how books use double quotes to show that someone is talking. After the second quotation mark, a period ends the sentence. Similar to English, all sentences in Engram end with periods. Let's put all of this together and put together an Output sentence:
```
Begin program:
	Output "Hello world!".
End program.
```
As you may have guessed, this program will output a message saying `Hello world!` on the **Output Viewer**. Type this code into the **Code Editor** and click run. As long as you don't have any syntax errors, you should see `Hello world!` display on the **Output Viewer**. Congratulations, you have just coded your first program!

In addition to words, we can also display numbers and variables using the Output sentence. Displaying output frequently is very important, as it helps whoever is running the code understand what is happening in the code. In the next section, we will learn how to create variables and change them using math. The Output sentence will be a great tool for us to use to see the results of using math.

# Exercise: Changing the Output
Now that we know how to write Output sentences, try changing the words between the quotation marks to say something else, like your name or your favorite video game. Once you do that, see if you can add some more Output sentences on different lines, and have them all say different things.

## Next Section
[Variables and Mathematic Operations](5-variables.md)

## Previous Section
[The Engram Editor](3-editor.md)
