#include <iostream>

// compile this test with make all_debug and
// observe the mangled symbol names in the .sym
// file created. Notice how the type names are
// short-handed as c, i, f, d, etc.
int function(char var1, int var2, float var3)
{
	return 0;
}

char function(int var1, char var2, double var3)
{
	return '0';
}

// we pass argc and argv[] in case we want to pass
// command line options directly to the c++ code
int main (int argc, char* argv[])
{
	const char* hello_world = "Hello World!";
	std::cout << hello_world << std::endl;

	return 0;
}
