#include "parser.h"

//evaluate expression by recursively finding and evaluating sub-expression with highest precedence
token evaluate(const std::vector<token>& exp) {
	//handle parentheses recursively (recursively call evaluate for deepest par until none are left)
	for (int i = 0; i < exp.size(); i++) { //finding the closing par so we can go back and find the last opening one, the deepest one
		if (exp[i].type == op_t && exp[i].op == par_close) {
			for (int j = i - 1; j >= 0; j--) {
				if (exp[j].type == op_t && exp[j].op == par_open) {
					std::vector<token> v;
					for (int k = j + 1; k < i; k++) {
						v.push_back(exp[k]); //pushing the innermost expression into a new vector
					}
					token instuff = evaluate(v); //recursively calling evaluate for innermost expression
					v.clear();
					for (int k = 0; k < j; k++) {
						v.push_back(exp[k]);
					}
					v.push_back(instuff);
					for (int k = i + 1; k < exp.size(); k++) {
						v.push_back(exp[k]);
					}
					return evaluate(v); //recursively calling evaluate
				}
			}
		}
	}
	//calculating value of expression that does not contain parentheses
	token temp;
	for (int i = 0; i < exp.size(); i++) {
		if (exp[i].type == op_t && (exp[i].op == multiply || exp[i].op == divide)) { //initially check for multiply/divide ops according to correct sequence
			if (exp[i].op == multiply) { //if op type is multiply
				if (exp[i - 1].type == float_type || exp[i + 1].type == float_type) { //checking if values are float types
					float num1 = exp[i - 1].i + exp[i - 1].f;
					float num2 = exp[i + 1].i + exp[i + 1].f;
					temp = { float_type, 0, num1 * num2, plus };
				}
				else { //if values are int types
					int num1 = exp[i - 1].i;
					int num2 = exp[i + 1].i;
					temp = { int_type, num1 * num2, 0, plus };
				}
			} 
			else { //if op type is divide (always use float for divide)
				float num1 = exp[i - 1].i + exp[i - 1].f;
				float num2 = exp[i + 1].i + exp[i + 1].f;
				temp = { float_type, 0, num1 / num2, plus };
			}
			std::vector<token> v;
			for (int j = 0; j < i - 1; j++) {
				v.push_back(exp[j]);
			}
			v.push_back(temp);
			for (int j = i + 2; j < exp.size(); j++) {
				v.push_back(exp[j]);
			}
			return evaluate(v);
		}
	}
	for (int i = 0; i < exp.size(); i++) {
		if (exp[i].type == op_t && (exp[i].op == plus || exp[i].op == minus)) {
			if (exp[i].op == plus) {
				if (exp[i - 1].type == float_type || exp[i + 1].type == float_type) {
					float num1 = exp[i - 1].i + exp[i - 1].f;
					float num2 = exp[i + 1].i + exp[i + 1].f;
					temp = { float_type, 0, num1 + num2, plus };
				}
				else {
					int num1 = exp[i - 1].i;
					int num2 = exp[i + 1].i;
					temp = { int_type, num1 + num2, 0, plus };
				}
			}
			else {
				if (exp[i - 1].type == float_type || exp[i + 1].type == float_type) {
					float num1 = exp[i - 1].i + exp[i - 1].f;
					float num2 = exp[i + 1].i + exp[i + 1].f;
					temp = { float_type, 0, num1 - num2, plus };
				}
				else { //both ints
					int num1 = exp[i - 1].i;
					int num2 = exp[i + 1].i;
					temp = { int_type, num1 - num2, 0, plus };
				}
			}
			std::vector<token> v;
			for (int j = 0; j < i - 1; j++) {
				v.push_back(exp[j]);
			}
			v.push_back(temp);
			for (int j = i + 2; j < exp.size(); j++) {
				v.push_back(exp[j]);
			}
			return evaluate(v);
		}
	}

	if (exp.size() == 0) {
		return { int_type,0,0,plus };
	}
	return exp[0];

}

//checking that char is a number
bool isNumber(const char& c) {
	return c - '0' >= 0 && c - '0' <= 9;
}

//checking that char is an operator
bool isOperator(const char& c) {
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}

//parses string into tokens (each containing operator or number)
std::vector<token> tokenize(const std::string& str) {
	std::vector<token> t;

	for (int i = 0; i < str.size(); i++) {
		if (str[i] != ' ') {
			if (isNumber(str[i])) { //if it is a number
				if (str[i + 1] != '.' && !isNumber(str[i + 1])) { //if it is a one digit number that isnt a float
					t.push_back({ int_type, str[i] - '0', 0, plus });
				}
				else { //dealing with the entire number
					for (int j = i; j < str.size(); j++) {
						int firstDigit = i;
						if (str[j] == '.') { //dealing with a float
							float num = 0; 
							for (int k = firstDigit; k < j; k++) {
								num += (str[k] - '0') * pow(10, j - k - 1);
							}
							int k = j + 1;
							while (isNumber(str[k])) { //while it is digits
								num += (str[k] - '0') * pow(10, -(k - j));
								k++;
							}
							t.push_back({ float_type, 0, num, plus });
							i = k - 1; // moving i to where k managed to get in the string
							j = str.size(); // exiting the j for loop to go back to the main for loop
						}
						else if (str[j] == ' ' || isOperator(str[j])) {
							int num = 0;
							for (int k = firstDigit; k < j; k++) {
								num += (str[k] - '0') * pow(10, j - k - 1);
							}
							t.push_back({ int_type, num, 0, plus });
							i = j - 1; //jumping to there because i already dealt with what happens until there
							j = str.size(); // exiting the j for loop to go back to the main for loop
						}
						else if (j == str.size() - 1 && isNumber(str[j])) { //end of string and its a number
							int num = 0;

							for (int k = firstDigit; k <= j; k++) {
								num += (str[k] - '0') * pow(10, j - k);

							}
							t.push_back({ int_type, num, 0, plus });
							i = j;
							j = str.size(); // reset j to size of str before returning to main loop
						}
					}
				}

			}
			else {
				switch (str[i]) { //dealing with operators
				case '(':
				{
					if (i != 0) {
						if (t[t.size() - 1].type != op_t) {
							t.push_back({ op_t, 0, 0, multiply });
						}
					}
					t.push_back({ op_t, 0, 0, par_open });
				}
				break;
				case ')':
				{
					t.push_back({ op_t, 0, 0, par_close });
				}
				break;
				case '+':
				{
					t.push_back({ op_t, 0, 0, plus });
				}
				break;
				case '-':
				{
					t.push_back({ op_t, 0, 0, minus });
				}
				break;
				case '*':
				{
					t.push_back({ op_t, 0, 0, multiply });
				}
				break;
				case '/':
				{
					t.push_back({ op_t, 0, 0, divide });
				}
				break;
				}
			}
		}
	}
	return t;
}

//checks if expression is valid
bool checkValid(const std::vector<token>& exp) {
	int par_open_count = 0;
	int par_close_count = 0;
	for (int i = 0; i < exp.size(); i++) {
		if (exp[i].type == op_t && exp[i].op == par_open) {
			par_open_count++;
		}
		else if (exp[i].type == op_t && exp[i].op == par_close) {
			par_close_count++;
		}
		if (i < exp.size() - 1) { //not end of vector
			if (exp[i].type == op_t && exp[i].op == par_open && exp[i + 1].type == op_t && exp[i + 1].op == par_close) {
				std::cout << "error: empty parenthases" << std::endl;
				return false; //empty parenthasies
			}
		}
		if (par_close_count == 1 && par_open_count == 0) {
			std::cout << "error: close parenthasies before open parenthases" << std::endl;
			return false; // close parenthasies before open parenthasies
		}
		if (exp[i].type == op_t && exp[i].op == par_close) { 
			if (i < exp.size() - 1) { //if this isn't end of vector
				if (exp[i + 1].type != op_t) {
					std::cout << "error: number after close parenthases" << std::endl;
					return false; //number right after close parenthasies
				}
			}
		}
		if (exp[i].type == op_t && (exp[i].op != par_close && exp[i].op != par_open)) {
			if (i < exp.size() - 1) { //if this isn't end of vector
				if (exp[i + 1].type == op_t && (exp[i + 1].op != par_close && exp[i + 1].op != par_open)) {
					std::cout << "error: two non parenthases expressions in a row" << std::endl;
					return false; //checks for 2 following non-par operators
				}
			}
		}
	}
	if (par_close_count != par_open_count) {
		std::cout << "error: unequal amount of open and close parenthases" << std::endl;
		return false; //unequal amount of open and close parenthases
	}
	return true;
}