#include <iostream>
#include "INTERPRETER.H"

using namespace std;
int main() {
    cout << "*********************************************************" << endl;
    cout << "***                                                   ***" << endl;
    cout << "***                                                   ***" << endl;
    cout << "***              Toy Language Interpreter             ***" << endl;
    cout << "***                  CISC 3160 Projec                 ***" << endl;
    cout << "***                                                   ***" << endl;
    cout << "***              Professor: Neng-Fa Zhou              ***" << endl;
    cout << "***              Written by: Zhijie Tan               ***" << endl;
    cout << "***                  Dec.15, 2023                     ***" << endl;
    cout << "***                                                   ***" << endl;
    cout << "***                                                   ***" << endl;
    cout << "*********************************************************" << endl;

    /*
     Project requirements: 
        The following defines a simple language, in which a program consists of assignments and each variable is assumed to be of the integer type.
        For the sake of simplicity, only operators that give integer values are included. 
        Write an interpreter for the language in a language of your choice. Your interpreter should be able to do the following for a given program: 
        (1) detect syntax errors; 
        (2) report uninitialized variables; and 
        (3) perform the assignments if there is no error and print out the values of all the variables after all the assignments are done.

        Program:
	        Assignment*

        Assignment:
	        Identifier = Exp;

        Exp: 
	        Exp + Term | Exp - Term | Term

        Term:
	        Term * Fact  | Fact

        Fact:
	        ( Exp ) | - Fact | + Fact | Literal | Identifier

        Identifier:
     	     Letter [Letter | Digit]*

        Letter:
	        a|...|z|A|...|Z|_

        Literal:
	        0 | NonZeroDigit Digit*
		
        NonZeroDigit:
	        1|...|9

        Digit:
	        0|1|...|9
     */
    Interpreter interpreter;
    //string program = "x = 4 y = 5; z = x + y; q = x - y; w = x * y; r = -w;"; // This one have some syntax erroe.
    string program = "x = 4; y = 5; z = x + y; q = x - y; w = x * y; r = -w; u = (2 + 2) * 4;"; //Test all operations (+, -, *, (Unary minus -),()) 
    interpreter.interpret(program);
    return 0;
}

