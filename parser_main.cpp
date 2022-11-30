#include "parser.h"

//main - driver function to check expression solver
int main() {
	std::string str;
	std::cout << "Enter expression: " << std::endl;
	std::getline(std::cin, str);
	std::cout << std::endl << "Solution: " << std::endl;
	std::vector<token> expression = tokenize(str);
	if (checkValid(expression)) {
		token answer = evaluate(expression);
		switch (answer.type) {
		case (1): {
			std::cout << str << " = " << answer.i << std::endl;
		}
				break;
		case (2): {
			std::cout << str << " = " << answer.f << std::endl;
		}
				break;
		}
	}
	else {
		std::cout << "error: invalid input" << std::endl;
	}
	return 0;
}

/*
Example for output from main (driver program):

Enter expression:
3*(45/10-6)*(3*(2-7*4))-48

Solution:
3*(45/10-6)*(3*(2-7*4))-48 = 303

*/