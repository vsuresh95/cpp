#include <iostream>

using namespace std;

// we pass argc and argv[] in case we want to pass
// command line options directly to the c++ code
int main (int argc, char* argv[])
{
	const char* hello_world = "Hello World!";
	cout << hello_world << endl;

	return 0;
}
