#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;

// we pass argc and argv[] in case we want to pass
// command line options directly to the c++ code
int main (int argc, char* argv[])
{
	for (int i = 0; i < argc; i++)
	{
		cout << "argv[" << i << "] = " << argv[i] << endl;
	}

	// bitset is a way of type-casting to bits,
	// with <N> to size-cast a bit vector of size N
	//
	// ios is a standard class (IOStream)
	//
	// fstream has various modes - app, trunc, out, in, etc.
	fstream f_read; 
	f_read.open(argv[1], fstream::in);

	// Allocate a region in memory for an array pointer
	//
	// The reason we're declaring a size_t variable is because 
	// functions like malloc expect arguments as size_t type.
	// size_t is just a type of unsigned int that is used
	// specifically for this purpose
	//
	// malloc will return void* pointer (because the data allocated
	// can point to anything), therefore there is typecast to int
	int* arr;
	int N;
	f_read >> N;
	size_t size = N * N * sizeof(int);
	arr = (int*) malloc(size);

	// Read the array from the file
	// 
	// Since we are using a int pointer, spaces will be ignored
	for (int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			f_read >> arr[i*N + j];
			cout << arr[i*N + j] << ' ';
		}
		cout << endl;
	}

	f_read.close();

	// open another file and write the same values as input
	// file but multiplied by the size of the matrix
	fstream f_write;
	f_write.open(argv[2], fstream::out);

	for (int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			f_write << N * arr[i*N + j] << ' ';
			cout << N * arr[i*N + j] << ' ';
		}

		f_write << endl;
		cout << endl;
	}

	f_write.close();

	return 0;
}
