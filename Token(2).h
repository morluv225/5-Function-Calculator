//Token.h

#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <list> // (MW)
#include <sstream> // (MW)
using namespace std;

class Token;
ostream & operator << (ostream & os, Token & t);

enum TokenType { OPEN, CLOSE, OPERATOR, VARIABLE, VALUE, END };

class Token {

public:
	Token (TokenType t, char c) : ttype(t), ch(c) { }
	Token (TokenType t, double d) : ttype(t), number(d) { }
	Token (TokenType t) : ttype(t) { }
	Token () : ttype (END), ch('?'), number(-99999999) { }
	
	TokenType getType() {return ttype;}
	char getChar() {return ch;}
	double getNumber() {return number;}
	
private:
	TokenType ttype;
	char ch;
	double number;
};



#endif // TOKEN_H