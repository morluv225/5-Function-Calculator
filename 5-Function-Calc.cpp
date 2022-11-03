
// Morgan Williams

#include <iostream>
#include <string>
#include "Token(2).h"
#include "stringToTokenVector.h"
#include "Map.h"
#include<stack>
using namespace std;

bool parenMatch(vector<Token> & expr);
bool isEqual(Token c1, Token c2);
int priority(Token & t); 
void infixToPostfix(vector<Token> &infix, vector<Token> &postfix);
double postfixEval(vector<Token> &expr);
double extractValue(const Token param);

Map<char, double> myMap; // My Map

int main() {
  string str = ""; 

  string choice = "quit";
  Map<char, double>::Iterator iter = myMap.begin(); // Map iterator to step through the map

  // get test expression as string (using c++ standard template library(STL) class)
  
  do // Do-while loop to allow the user to insert expressions until they enter 'quit'
  {
	cout << "Enter expression(s) to test: "; 
	getline(cin,str);
	
	// convert to an STL vector of Tokens
	vector<Token> infix;
	infix = stringToTokenVector(str);
  

	// print resulting STL vector of Tokens for verification
	cout << "Resulting vector of Tokens: ";
  
	for (int i = 0; i < infix.size(); i++) {
		cout << infix[i] << ' ';
	  }
	 cout << endl;

	if (parenMatch(infix))
		cout << "\nSuccess! Matching grouping symbols!" << endl << endl;
	else {
		 cout << "\nError. Unmatched grouping symbols.\nExiting Calculator..." << endl << endl;
		 exit(23);
	  }

	 vector<Token> postfix;  // create empty postfix vector
	 infixToPostfix(infix, postfix);  // call inToPost to fill up postfix vector from infix vector
  
	 double result = 0.0;
	 result = postfixEval(postfix);

	 cout << "The result of your entered expression is: "
		  << result << endl << endl;

	 cout << "Would you like to continue? Enter 'yes' to continue or enter 'quit' to exit!" << endl;
	 cin >> choice;
	 cin.ignore(100, '\n');

  } while (choice != "quit");

  cout << "\nHere are your entered entries: " << endl;

  for (iter = myMap.begin(); iter != myMap.end(); ++iter) // To print out all the entries in the map
  {
	  cout << (*iter).key() << " = " << (*iter).value() << endl;
  }
  
  return 0;
}

//FROM LAB 3

bool parenMatch(vector<Token>  &expr)
{
	stack<Token> cStack;

	for (int it = 0; it < expr.size(); it++)
	{
		if (expr[it].getType() == OPEN)
		{
			cStack.push(expr[it]);
		}

		if (expr[it].getType() == CLOSE)
		{
			if (cStack.empty() || !isEqual(cStack.top(), expr[it]))
				return false;
			else
				cStack.pop();
		}

	}

	return (cStack.empty());
}

bool isEqual(Token c1, Token c2)
{
	if (c1.getChar() == '(' && c2.getChar() == ')')
		return true;
	else if (c1.getChar() == '{' && c2.getChar() == '}')
		return true;
	else if (c1.getChar() == '[' && c2.getChar() == ']')
		return true;
	else
		return false;
}

// FROM LAB 4

int priority(Token & t)
{
	if (t.getType() == OPERATOR)
	{
		if (t.getChar() == '*' || t.getChar() == '/' || t.getChar() == '%')
			return 3;
		else if (t.getChar() == '+' || t.getChar() == '-')
			return 2;
		else if (t.getChar() == '=') // (MW) 
			return 1;
	}


	else if (t.getType() == OPEN)
		return 0;
	else if (t.getType() == END)
		return -1;

	return -2;

}

void infixToPostfix(vector<Token> &infix, vector<Token> &postfix)
{
	stack<Token> cStack;
	int k = 0;
	Token topToken;
	Token token;
	Token tokenOut;

	for (k = 0; k < infix.size(); k++)
	{
		token = infix[k];

		if (token.getType() == OPEN)
		{
			cStack.push(token);
		}

		else if (token.getType() == CLOSE)
		{
			if (!cStack.empty())
			{
				token = cStack.top();
				cStack.pop();
			}

			while (token.getType() != OPEN)
			{
				postfix.push_back(token);
				if (!cStack.empty())
				{
					token = cStack.top();
					cStack.pop();
				}
			}
		}

		else if (token.getType() == OPERATOR)
		{
			if (!cStack.empty())
				topToken = cStack.top();

			while (!cStack.empty() && priority(token) <= priority(topToken))
			{
				if (!cStack.empty())
				{
					tokenOut = cStack.top();
					cStack.pop();
				}

				postfix.push_back(tokenOut);

				if (!cStack.empty())
					topToken = cStack.top();

			}

			cStack.push(token);
		}

		else // Character is operand or variable or end
		{
			postfix.push_back(token);

		}
	}

	while (!cStack.empty()) // Should push all remaining stack elements to postfix and pop them from the stack so that the stack is now empty
	{
		if (!cStack.empty())
		{
			token = cStack.top();
			cStack.pop();
		}
		postfix.push_back(token);
	}

}

// FROM LAB 5

double postfixEval(vector<Token> &expr)
{
	stack<Token> dStack; // allocate a stack of tokens
	Map<char, double>::Entry myEntry; // Map that holds my entries
	
	for (int i = 0; i < expr.size(); i++)
	{
		Token token = expr[i];

		if (token.getType() == VALUE)
		{
			dStack.push(token);
		}

		else if (token.getType() == VARIABLE) // Added else-if statement to handle VARIABLE Tokens
		{
			dStack.push(token);
		}

		else if (token.getType() == OPERATOR)
		{
			if (token.getChar() == '+')
			{
				if (dStack.size() >= 2)
				{

					double right = extractValue(dStack.top()); dStack.pop();
					double left = extractValue(dStack.top()); dStack.pop();

					double result = (left + right);
					Token operand = Token(VALUE, result); // Creates a VALUE token and assigns it the value of result
					dStack.push(operand);
				}

				else
				{
					cout << "Error! Too few operands for the given operator(s)...exiting calculator." << endl;
					exit(23);
				}

			}

			else if (token.getChar() == '-')
			{
				if (dStack.size() >= 2)
				{
					double right = extractValue(dStack.top()); dStack.pop();
					double left = extractValue(dStack.top()); dStack.pop();

					double result = (left - right);
					Token operand = Token(VALUE, result); // Creates a VALUE token and assigns it the value of result
					dStack.push(operand);

				}

				else
				{
					cout << "Error! Too few operands for the given operator(s)...exiting calculator." << endl;
					exit(23);
				}

			}

			else if (token.getChar() == '*')
			{
				if (dStack.size() >= 2)
				{
					double right = extractValue(dStack.top()); dStack.pop();
					double left = extractValue(dStack.top()); dStack.pop();

					double result = (left * right);
					Token operand = Token(VALUE, result); // Creates a VALUE token and assigns it the value of result 
					dStack.push(operand);

				}
				else
				{
					cout << "Error! Too few operands for the given operator(s)...exiting calculator." << endl;
					exit(23);
				}
			}

			else if (token.getChar() == '/')
			{
				if (dStack.size() >= 2)
				{
					double right = extractValue(dStack.top()); dStack.pop();
					double left = extractValue(dStack.top()); dStack.pop();

					double result = (left / right);
					Token operand = Token(VALUE, result); // Creates a VALUE token and assigns it the value of result
					dStack.push(operand);

				}

				else
				{
					cout << "Error! Too few operands for the given operator(s)...exiting calculator." << endl;
					exit(23);
				}
			}

			else if (token.getChar() == '%') // Added modulus option
			{
				if (dStack.size() >= 2)
				{
					double right = extractValue(dStack.top()); dStack.pop();
					double left = extractValue(dStack.top()); dStack.pop();

					int result = ((int)left % (int)right);
					Token operand = Token(VALUE, (double)result); // Creates a VALUE token and assigns it the value of result
					dStack.push(operand);
				}
				else
				{
					cout << "Error! Too few operands for the given operator(s)...exiting calculator." << endl;
					exit(23);
				}
			}

			else if (token.getChar() == '=') // Added '=' Function
			{

				if (dStack.size() >= 2)
				{

					if (dStack.top().getType() == VALUE) // If right operand is a VALUE token
					{
						double right = extractValue(dStack.top()); // Extract that value
						myEntry.setValue(right); // Set that value as the value in my Map
						dStack.pop(); // Pop off the stack

					}

					else if (dStack.top().getType() == VARIABLE) // If right operand is a VARIABLE token
					{
						double right = extractValue(dStack.top()); // Extracts that value
						myEntry.setValue(right); // Sets that value as the value in my Map
						dStack.pop(); // Pop off the stack
					}

					else {

						cout << "Invalid right operand. Exiting..."; // If any other Token type is the right operand
						exit(1);
					}

					if (dStack.top().getType() == VARIABLE) // If the left operand is a VARIABLE
					{
						char left = dStack.top().getChar(); // Assigns left as the VARIABLE character
						dStack.pop(); // Pop off the stack 
						char key = left; // Assigns the value of the key
						myEntry.setKey(key); // Sets that character value as the key in my Map
						double value = myEntry.value(); // Assigns the value of the value
						myMap.put(key, value); // Puts them in my map

						Token operand = Token(VALUE, value); // Creates a VALUE token and assigns it the value of value
						dStack.push(operand); // Pushes it onto the stack
					}

					else { // If left operand is any other Token besides a VARIABLE

						cout << "Error. Invalid left operand. Exiting...";
						exit(1);
					}

				}

				else
				{
					cout << "Error! Too few operands for the given operator(s)...exiting calculator." << endl;
					exit(23);
				}
			}


			else
			{
				cout << "Error! Unknown character...exiting program." << endl; // Not being printed even when unknown character is entered
				exit(23);													  // Already coded error is being thrown instead from stringToTokenVector.cpp
			}
		}
	}

		if (dStack.size() != 1)
		{
			cout << "Error! Too many operands for the given operator(s)...exiting program." << endl;
			exit(23);
		}
		
		else {
			return extractValue(dStack.top());
		}
	
	
}

double extractValue(Token param) // Function to return the value of a given Token
{
	Map<char, double>::Iterator p = myMap.begin(); // Iterator to search for given key

	if (param.getType() == VALUE) { // If parameter is a VALUE Token, return the given number 
		return param.getNumber();
	}
	else if (param.getType() == VARIABLE) { // If parameter is a VARIABLE Token

		char vari = param.getChar(); // Retrieves the character
		p = myMap.find(vari); // Uses p iterator to find the entry with character key 'vari'

		if (p == myMap.end()) // If the entry with given key cannot be found
		{
			cout << "Error. No valid entry in Map. Exiting...";
			exit(1);
		}
		
		return (*p).value(); // Else, return the value associated with that character key
	}

	else { // If operand is any other Token type besides VALUE or VARIABLE

		cout << "Error! Invalid Token type for operand. Exiting...";
		exit(1);
	}
}




