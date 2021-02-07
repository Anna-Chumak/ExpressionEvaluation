#include <iostream>
#include<string>
//this is the actual file the rest are just attempts that didn`t work

//the way string works the expression shouldn`t have any spaces there


using namespace std;
//checks if the user input is a valid one (only inlcudes numbers, operators or Parenths)
//doesn`t search for spaces as they wouldn`t be caught by string class anyways
bool validExpression(string expr) {
	try {
		for (int i = 0; i < expr.length(); i++)
		{
			if (int(expr[i]) < 40 || int(expr[i])>57) //checks if it outside of the "basic math character range"
			{
				throw -1; 
			}
			else if (int(expr[i]) == 46) { //checks if it`s a point which probably means the user wanted to input some floating number
				throw "double";
			}
		}
	}
	catch (int) {
		cout<<"The input is invalid\n";
	}
	catch (char*) {
		cout << "Sorry! This program doesn`t evaluate floating numbers\n";
	}
	catch (...) {
		cout << "Ooops! Something went wrong!\n";
	}
}

//gets the number to the left of the operator 
//(loops through characters and checks if it`s a digit until it reaches the beginning, 
//conversts the part of string to int  and returns the number)
int getLeftint(string &expr, int &index) { 
	int count = 1; //counts how many digits the number has
	if (expr[index - 1] == 41) { //checks if it`s a negative number that has a Parenth right before the operator 
		expr.erase(index-1, 1);  //deletes it
		index--;				 //index becomes one less as a char is deleted
	}

	//the loop counts how many digits the number has
	while (isdigit(expr[index - count]) && count < index && int(expr[index - count]) != 40&&int(expr[index - count])!=45 && int(expr[index - count]) != 43) {
		count++; 
	} 

	//checks if it`s negative or not
	if (int(expr[index - count]) == 40|| int(expr[index - count]) == 43) {
		count--;
	}
	else if (int(expr[index - count]) == 45&& count < index) {
		if (isdigit(expr[index - count-1])) {
			count--;
		}
		else if (expr[index - count - 1] == 40) {
			expr.erase(index - count-1, 1);
			index--;
		}
	}

	//converts to int
	int left =stoi(expr.substr(index - count, count));

	//deletes this number
	expr.erase(index - count, count);

	//adjusts index value
	index -= count;
	return left;
}

//similar to the left just a little different as it goes after the operator
int getRightint(string& expr, int& index) {
	bool negative = false;
	int count = 1;
	if (expr[index + 1] == 40) {
		expr.erase(index + 1, 1);
		if (expr[index + count] == 45) {
			count++;
		}
	}
	while (isdigit(expr[index + count]) && count < expr.length() && int(expr[index + count]) != 41) {
		count++;
	}
	if (int(expr[index + 1]) == 45 && int(expr[index + count]) == 41) {
		expr.erase(index + count, 1);
	}
	count--;
	int right = stoi(expr.substr(index + 1, count));
	expr.erase(index + 1, count);
	return right;
}

//has switch insode that does the actual operation (uses above functions to the left and right values)
//the index of the ooperator is given within the arguments
int doOperation(string &expr, int &index) {

	int left = getLeftint(expr, index);			//sets the left number
	int right = getRightint(expr, index);		//sets the right number
	int operation = expr[index];				//sets operator
	switch (operation)							//performs the operation 
	{
	case 43:
		left += right;
		break;
	case 45:
		left -= right;
		break;
	case 42:
		left *= right;
		break;
	case 44:
		left %= right;
		break;
	case 47:
		try {							//checks if it tries to divide by zeri
			if (right == 0) {
				throw 1;
			}
			left /= right;
			break;
		}
		catch(int){
			cout << "Attempted division by zero!\n";
		}
		catch (...) {
			cout << "Something went wrong\n";
		}
	default:
		break;
	}
	return left;
}

//looks for the parentheses and returns the index of the first opening one
int checkParenths(string expr, int index = 0) {
	for (int i = index; i < expr.length(); i++)
	{
		if (int(expr[index]) == 40&&int(expr[index+1])!=45) {
			return index;
		}
		index++;
	}
	return -1;
}

//looks for the operators of division or multiplication and returns the index of the first instance of such
int checksMultDiv(string expr, int index = 0) {
	for (int i = index; i < expr.length(); i++)
	{
		if (int(expr[index]) == 42 || int(expr[index]) == 47 || int(expr[index]) == 44) {
			return index;
		}
		index++;
	}
	return -1;
}

//looks for the operators of addition or subtraction and returns the index of the first instance of such
int checksAddSub(string expr, int index = 0) {
	for (int i = index; i < expr.length(); i++)
	{
		if (int(expr[index]) == 43 || int(expr[index]) == 45&& int(expr[index-1])!=40) {
			return index;
		}
		index++;
	}
	return -1;
}

//looks for the index of the operator within the parentheses
int findOperatorParenths(string expr, int index) {
	while (int(expr[index]) != 42 && int(expr[index]) != 47 && int(expr[index]) != 43 && int(expr[index]) != 45) {
		index++;
	}
	return index;
}

//the acual one that first checks is there are any Parenths, than mult/div and only then performs add and subtractio
//IMPORTANT: changes the string along the way, so if the original is neededm a copy should be sent instead.
int evaluateExpression(string expr, int index = 0) {
	int result = 0;
	validExpression(expr);
	while(expr.find('*') != std::string::npos || expr.find('/') != std::string::npos || expr.find('+') != std::string::npos || expr.find('-') != std::string::npos)
	{
		if (checkParenths(expr)<0) {			//checks if there are any parentheses that contain an expression and not just a negative number
			if (expr.find('*') == std::string::npos && expr.find('/') == std::string::npos && expr.find('%') == std::string::npos) {			//checks if there is multiplication or division
				index = checksAddSub(expr);		//gets index of "+" or '-'
			}
			else {
				index = checksMultDiv(expr);	//gets index of "*" or '/' or '%'
			}
			result = doOperation(expr, index);  //gets the result of the operation
			if (expr.length() <= 1) {			//checks if there are any other operations left
				return result;
			}
			expr.erase(index, 1);				//deletes the opeator just used
			string temp;
			if (result < 0) {					//creates string for negative number
				temp = '(' + to_string(result) + ')';
			}
			else {								//creates string for a positive number
				temp = to_string(result);
			}
			expr.insert(index, temp);			//inserts the result in the string
		}
		else {
			int indexparen = checkParenths(expr);		//gets index of parentheses
			index = findOperatorParenths(expr, indexparen);	//gets index of the operator inside parentheses
			result = (doOperation(expr,index));		//gets the result of the above operation and then basically the same as in previous section 
			if (expr.length() <= 3) {
				return result;
			}
			expr.erase(indexparen, 3);
			string temp;
			if (result < 0) {
				temp = '(' + to_string(result) + ')';
			}
			else {
				temp = to_string(result);
			}
			expr.insert(indexparen, temp);
		}
		index = 0;
	}
	return result;
}

int main() {
	string math;
	cout << "Enter the equasion\n";
	cin >> math;
	int result = 0;
	result = evaluateExpression(math);
	cout <<"The result is: " <<result << endl;



}