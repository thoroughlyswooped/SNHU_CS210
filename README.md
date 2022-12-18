# SNHU_CS210
This project was to write a program in both C++ and Python that allowed for simple functionality to analyze sales of produce given a specific data set in the form of a text file.

What did you do particularly well?
I did a good job of going beyond the base code that was given for the project. Initially the project would call the python module by instantiating a new object for each C++ function call that used python and then deleting the object at the end of the function. This meant that data in the python module did not persist in the way one would expect it to. I changed this by making a class to store the python module information and instantiating it once at the begining of the program and passing a reference to the object to each C++ function. This can be seen in source.cpp.

A good place for me to improve my code would be in doing more input validation as there are places where certain input could cause problems in the program and security problems in a real-world aplication.

The code that I found most challenging to write was the conversion of string to int for analyzing the frequency of the sales of products. This is not inherently difficult, but I did not immediatly realize that the delimiter was taking space when counting indices in the parsing logic that I had written. This lead to a significant amount of time being spent debugging the parsing of the frequency file that I created using python. 

The most valuable skill that I gained from this project is integrating C++ and Python into a simple project together. Both of these languages are extremely useful and I have experience with both languages so I am excieted to use both languages together in future projects.

I made this program maintainable, readable, and adaptable by simplifying the code I was given and following naming conventions where I could.
