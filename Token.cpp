//Token.cpp

#include <iostream>
#include "Token(2).h"
using namespace std;

ostream & operator << (ostream & os, Token & t) {
	os << "<";
	switch (t.getType()) {
		case OPEN:
			os << "OPEN," << t.getChar(); break;
		case CLOSE:
			os << "CLOSE," << t.getChar(); break;
		case OPERATOR:
			os << "OPER," << t.getChar(); break;
		case VARIABLE:
			os << "VARI," << t.getChar(); break;
		case VALUE:
			os << "VALU," << t.getNumber(); break;
		case END:
			os << "END" ; break;
		default: os << "UNKNOWN";
	}
	os << ">";
	return os;
}		
