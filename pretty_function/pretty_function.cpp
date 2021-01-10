#include <iostream>
#include <string>

using namespace std;

template <typename T>
T adder (T last)
{
	cout << "Pretty function: " << __PRETTY_FUNCTION__ << endl;
	cout << "Function: " << __FUNCTION__ << endl;
	cout << "Func: " << __func__ << endl;

	return last;
}

// variadic templates are one where a bunch of arguments (depending on how the function was called),
// can be bunched into a singular argument. In this example, it is handy to implement a recrusive
// function that will parse one element at a time.
template <typename T, typename... A>
T adder (T first, A... args)
{
	cout << "Pretty function: " << __PRETTY_FUNCTION__ << endl;
	cout << "Function: " << __FUNCTION__ << endl;
	cout << "Func: " << __func__ << endl;

	return first + adder (args...);
}

int main (int argc, char* argv[])
{
	// __PRETTY_FUNCTION__ will print the function name, along with the arguments, and their type
	// __FUNCTION__ and __func__ will print the function name only.
	cout << "Pretty function: " << __PRETTY_FUNCTION__ << endl;
	cout << "Function: " << __FUNCTION__ << endl;
	cout << "Func: " << __func__ << endl;

	cout << "Sum of int using adder:" << endl << adder (1,2,3,4,5) << endl;

	cout << "Sum of float using adder:" << endl << adder (1.23, 3.14, 2.87, 2.98, 1.732) << endl;

	cout << "Concat of string using adder:" << endl << adder ((string) "My ",
														(string) "name ", 
														(string) "is ",
														(string) "Vignesh",
														(string) "\n");
	
	return 0;
}
