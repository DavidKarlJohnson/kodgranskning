-----------------------
      HOW TO RUN
-----------------------

From the console:
1. Navigate to the root folder through the console. The root folder should be the same folder where the Makefile is located.
2. Type the command: make all  (this will compile the program)
3. Type the command: make run  (this will run the program)
4. To remove temporary files, type: make clean


From IDE:
1. The program should be run from the file 'Calculator' located in:  ../src/org/ioopm/calculator/




-----------------------
        PARSER
-----------------------

When the user gives an input that input will be recursively parsed and broken down into smaller and smaller terms, expressions and factors.
The parser will stop when all evaluations are performed, and all expressions have been reduced to either constants or variables.

Example:
The input: (10+5)*(6-3)  is given.
1. The parser will break down the multiplication into two subtrees - the left and the right one.
2. Starting with the left subtree (10+5) the parser will identify addition and look to the left and to the right of the + operator.
Both 10 and 5 are constants, which is an indicator to start evaluation. The evaluation will result in 15, and now the parser will move onto
the right subtree.
3. Subtraction (6-3) is identified in the right subtree. And to the left and to the right of the minus operator are the two constants
6 and 3. Encountering two constants means that it's time to perform the evaluation, which wll result in 3.
4. Now that both subtrees to the left and to the right of the multiplication sign are evaluated, and we've got the two constants
15 and 3, the multiplication evaluation will be performed. This will result in the constant 45, and the parsing is completed.



-----------------------
  ABSTRACT SYNTAX TREE
-----------------------

* All subclasses in the AST has the superclass 'SymbolicExpression'.

* 'Binary' consists of an operator with two subtrees on each side. 
   Example: (3+4) * 5 is of the class 'Multiplication' with the superclass 'Binary'. Its left subtree is an 'Addition'.
   Its right subtree is a 'Constant'.

* 'Unary' is the superclass of unary operations, and will only contain one subtree.
   Example: sin(3+4) is of the class 'Sin', with the superclass 'Unary'. It contains the subtree: 3+4, which is of the class 'Addition'.

* 'Atom' is used for terminating symbols. All expressions will eventually be reduced to an atom.
   Example: 42 is of the class 'Constant', with the superclass 'Atom'. 

* 'Command' are the "non-calculating" inputs the user can give the console.
   Example: "quit" is of the class 'Quit', with the superclass 'Command'. 


SymbolicExpression:
    Binary:
        * Addition
        * Subtraction
        * Multiplication
        * Division
        * Assignment - Ex: x=3+4
    Unary:
        * Sin
        * Cos
        * Exp
        * Log - NOTE: Uses base 10 (log(100) = 2)
        * Negation
    Atom:
        * Variable - Ex: x,y,z...
        * Constant - Ex: pi, e
        * NamedConstant
    Command:
        * Vars  - Print all stored variables
        * Quit  - Quit the program
        * Clear - Clear all stored variables


-----------------------
ASSIGNMENT AND VARIABLES
-----------------------

When using assignment to a variable, the combination will be stored in a hashmap which we call 'Environment'.

Example: The user inputs: 3=x
"x" will be stored as the key and 3 as the value inside the hashmap. Calling the variable x from the console will now print the value 3.
Thus, we've effectively created a "memory" of all variables. We can now use this variable in any future calculations if we'd like to.
(x+3) * 10 = y   will replace x with 3, and the left-hand-side of the assignment will evaluate to 90, which will be stored in the variable "y".
Making calls to y will now result in the value 90.

To print all current variables, use the command: "Vars"
To clear all current variables, use the command: "Clear"


-----------------------
IDENTIFIERS / KEYWORDS
-----------------------

A few keywords are reserved for issuing commands to the program. These words are:
    - quit
    - clear
    - vars
Whenever an expression is parsed and one of these keywords are found the parser will return a 'Command' object.


* Words that issue commands can not be assigned to variables. Example: Quit=x