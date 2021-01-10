#include <iostream>
#include "add_by.h"

#ifdef __VALUE__
#include <add_by_value.hpp>
#endif

#ifdef __REFERENCE__
#include <add_by_reference.hpp>
#endif

#ifdef __POINTER__
#include <add_by_pointer.hpp>
#endif

using namespace std;

int main (int argc, char* argv[])
{
	#ifdef __VALUE__ 
	int value_first = 5;
	int value_second = 10;
	int value_third = sum(value_first, value_second);

	cout << "Sum by value = " << value_third << endl;	
	#endif

	#ifdef __REFERENCE__
	int reference_first = 5;
	int reference_second = 10;
	int reference_third = sum(&first, &second);

	cout << "Sum by reference = " << third << endl;	
	#endif

	#ifdef __POINTER__
	int* pointer_first = new int(5);
	int* pointer_second = new int(10);
	int* pointer_third = new int();
	sum(pointer_first, pointer_second, pointer_third);

	cout << "Sum by pointer = " << *pointer_third << endl;	
	#endif

	return 0;
}
