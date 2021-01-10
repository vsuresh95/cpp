#include <iostream>
#include <vector>

using namespace std;

int* return_bad_pointer(int value)
{
	int var = value;

	// here, var is a local variable whose scope exists only within return_bad_pointer, and
	// var is allocated on the stack.
	// therefore, when we return back to the calling function, var has already been popped off
	// the stack and its memory de-allocated.
	// thus, you will recieve a fault during runtime and a warning during compile
	return &var;
}

int* return_good_pointer(int value)
{
	int* var = new int;
	*var = value;

	// here, although var has been declared inside the scope of the function return_good_pointer,
	// it has been allocated memory on the heap.
	// therefore, the pointer var and its associated memory location will remain allocated
	// even after returning to the calling function.
	// 
	// however, since this is a local pointer, the programmer must pay attention to deallocate the
	// memory in the calling function when the work with the pointer is done. else, you will run out 
	// of heap memory.
	return var;
}

// we pass argc and argv[] in case we want to pass
// command line options directly to the c++ code
int main (int argc, char* argv[])
{
	const char* hello_world = "Hello World!";
	cout << hello_world << endl;
	
	// incorrect way to return a pointer from a function
	int* bad_ptr = return_bad_pointer(5);

	// cout << "Value at bad pointer = " << *bad_ptr << endl;

	// correct way to return a pointer from a function
	int* good_ptr = return_good_pointer(10);

	cout << "Value at good pointer " << good_ptr << " = " << *good_ptr << endl;

	// we create a vector of integer pointers and push back the pointers
	// that we create using return_good_pointer function
	// 
	// usually you would create a vector of integers and push back integers
	// and the vector library would take care of allocating memory, here
	// we are allocating the memory and the vector is merely a vector of pointers.
	vector<int*> vec_ptr;

	for (int i = 0; i < 5; i++)
	{
		vec_ptr.push_back(return_good_pointer(i*5 + 15));
	}

	for (int i = 0; i < vec_ptr.size(); i++)
	{
		cout << "Value at " << vec_ptr.at(i) << " element number " << i << " = " << *vec_ptr.at(i) << endl;
	}
	
	return 0;
}
