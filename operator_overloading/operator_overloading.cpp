#include <iostream>

using namespace std;

struct length
{
	int feet;
	int inches;

	// in initializer list, the class variable comes first, followed by the
	// constructor argument.
	length (const int f = 0, const int i = 0) : feet(f), inches(i) {}

	float length_in_feet ()
	{
		return (feet + (float) inches/12);
	}

	float length_in_meters ()
	{
		return 0.3 * (feet + (float) inches/12);
	}

	void print_length (const char* name)
	{
		cout << "Length of " << name << " = " << feet << " feet and " << inches << " inches" << endl;
	}
};

// you can implement this with pointers and replace the '.' with '->' wherever you're referring
// to members of the struct (or class). however, you will get the below error:
// error: 'length* operator+(length*, length*)' must have an argument of class or enumerated type
length operator+ (length l1, length l2)
{
	length temp;

	temp.inches = l1.inches + l2.inches;

	if (temp.inches >= 12)
	{
		temp.feet += 1;
		temp.inches -= 12;
	}

	temp.feet += l1.feet + l2.feet;

	return temp;
}

// irrespective of what you enter, the output of fn(<var>) will 
// always be the type of <var>, as this is decided during 
// compile time where the template variable is replaced with
// the data type it is supposed to be.
// 
// for functions you can implicitly pass a variable and compiler
// will correctly replace the datatype in the template. however, for
// classes, you need to explicitly pass the data type while declaring the
// class as...
// <class_name><type> <object_name>
// similarly if you want to explicitly mention the type for a function, you
// call the function as...
// <function_name><type>(<parameters>)
template <typename T>
T template_typecast (T var)
{
	T temp;
	cout << "Enter variable" << endl;
	cin >> temp;

	return temp;
}

// we pass argc and argv[] in case we want to pass
// command line options directly to the c++ code
int main (int argc, char* argv[])
{
	const char* hello_world = "Hello World!";
	std::cout << hello_world << std::endl;

	// pass an integer to the typecasted function
	cout << template_typecast(5) << endl;

	// create two lengths and add them
	length l1(5,5);

	cout << "Length of l1 in feet = " << l1.length_in_feet() << endl;

	cout << "Length of l1 in meters = " << l1.length_in_meters() << endl;

	l1.print_length("l1");

	length l2(4,11);

	l2.print_length("l2");

	length l3 = l1 + l2;

	l3.print_length("l3");

	return 0;
}
