#include <iostream>
#include <mkl.h>

using namespace std;

int main (int argc, char* argv[])
{
	const MKL_INT n = 10;
	const MKL_INT incx = 1;
	const MKL_INT incy = 1;

	double* _x = new double[n];
	double* _y = new double[n];

	for (int i = 0; i < n; i++)
	{
		_x[i] = i;
		_y[i] = 2*i;
	}

	double _z = cblas_ddot(n, _x, incx, _y, incy);

	cout << "Output is " << _z << endl;

	delete _x, _y;

	return 0;
}
