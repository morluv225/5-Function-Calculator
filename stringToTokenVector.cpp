// stringToTokenVector.cpp
//
// author: Barbara Kreaseck for CPTG 244 course use
// updated: Nov 18, 2013
//
// Assumes white space to be the following ascii values:
//   10 linefeed?
//   13 carriage return?
//   32 space
//    9 tab
// Assumes operators are the following characters:
//   + - * / 
// Assumes only parentheses ( ) used for grouping
//
// Handles integers and floating point numbers in fixed and E-notation
//
// ------------------------------------------------

#include "stringToTokenVector.h"

vector<Token> stringToTokenVector(string str) {

  char * cstr;
  cstr = new char [str.size()+1];
  strcpy(cstr,str.c_str());

  return stringToTokenVector(cstr);
}

vector<Token> stringToTokenVector(char * cstr) {

  vector<Token> result;
  char ch;

  if (strlen(cstr) <= 0 ) {
    return result;
  }

  int there = 0;
  int here = 0;
  // remove spaces and tabs
  while (cstr[here] != '\0') {
    ch = cstr[here];
    if (ch == 10 || ch == 13 || ch == 32 || ch == 9) {
      here++;
    } else {
      cstr[there] = cstr[here];
      there++;
      here++;
    }
  }

  cstr[there] = '\0'; // put back end-of-string marker

  int length = strlen(cstr);

  int passed = 0;
  int curr = 0;
  Token tok;

  // walk resulting string, convert to queue of tokens
  while (cstr[curr] != '\0' && passed < length) {
    switch (cstr[curr]) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
	case '=':
      // operator: fill token and append to queue
      tok = Token(OPERATOR,cstr[curr]);
      result.push_back(tok);
      curr++;
      passed++;
      break;
    case '(':
    case '[':
    case '{':
      // grouping symbol: open parenthesis
      tok = Token(OPEN,cstr[curr]);
      result.push_back(tok);
      curr++;
      passed++;
      break;
    case ')':
    case '}':
    case ']':
      // grouping symbol: close parenthesis
      tok = Token(CLOSE,cstr[curr]);
      result.push_back(tok);
      curr++;
      passed++;
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
      {
	// number: fill token and append to queue
	tok = Token(VALUE,(double)atof(&(cstr[curr])));
	result.push_back(tok);
	
	// find location of next character after value:
	bool firstDot = false;
	bool eFound = false;
	bool secondDot = false;
	bool done = false;
	curr++;
	passed++;
	while (!done && cstr[curr] != '\0' && passed != length) {
	  ch = cstr[curr];
	  if (ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
	      ch == '%' || ch == '=' || ch == '[' || ch == '{' || ch == '}' || 
	      ch == ']' ||
	      ch == '(' || ch == ')' ) {
	    done = true; // found next character; 
	  } 
	  else if (ch >= '0' && ch <= '9') {
	    // skip digits
	    curr++;
	    passed++;
	  }
	  else if (ch == '.') {
	    if (!firstDot && !eFound) {
	      // found first dot before e... skip
	      firstDot = true;
	      curr++;
	      passed++;
	    } else {
	      // cannot have double dots , nor dots in exponent ... error
	      cout << "found double dot in a value at character " << curr
		   << " ... exiting\n";
	      exit(1);
	    }
	  } 
	  else if (ch == 'e' || ch == 'E') {
	    if (!eFound) {
	      // ok to have one exponent
	      eFound = true;
	      curr++;
	      passed++;
	    } else {
	      // cannot have multiple E's in one value
	      cout << "found double E in a value at character " << curr
		   << " ... exiting\n";
	      exit(2);
	    }
	  } 
	  else if (ch == '\0') {
	    done = true;
	  } else {
	    // found unexpected character
	    cout << "found unexpected character '" << ch << "' in value at character " << curr
		 << " ... exiting\n";
	    exit(3);
	  }
	} // end of while
      }
      break;
    default:
    	if (cstr[curr] >= 'A' && cstr[curr] <= 'Z') {
    		tok = Token(VARIABLE,cstr[curr]);
      		result.push_back(tok);
     		curr++;
     		passed++;
   		} else {

      		cout << "found non-operator, non-value character: " << cstr[curr]
	  			 << " ... exiting\n";
     		 exit(4);
     	}
      break;
    } // end of switch statement
  } // end of while statement

  // if you want an END token at the end of your expression, uncomment the following lines
  // tok = Token(END);
  // result.push_back(tok);

  return result;
}
