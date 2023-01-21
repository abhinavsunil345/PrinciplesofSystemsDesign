#Assignment 1 directory

This directory contains source code and other files for Assignment 1.

Use this README document to store notes about design, testing, and
questions you have while developing your assignment.

Design - initially reads char by char for header until newline. If no newline or exceeds 4097 return error.
Then goes into get and set depending on first command.
set uses read just to check if its the inputed file is a directory.

Uses a buffer of 10,000 just to speed up tests, otherwise they take too long.
