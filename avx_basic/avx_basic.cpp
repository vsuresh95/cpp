#include <iostream>
#include <immintrin.h>
#include <chrono>
#include <cassert>

#define N 10000000
#define AVX
// #define AVX512

using namespace std;
using namespace std::chrono;

high_resolution_clock::time_point get_time()
{
	return high_resolution_clock::now();
}

void assign_m256(__m256* comp, float* elem)
{
	*comp = _mm256_set_ps(elem[0], elem[1], elem[2], elem[3], elem[4], elem[5], elem[6], elem[7]);
}

void assign_m512(__m512* comp, float* elem)
{
	*comp = _mm512_set_ps(elem[0], elem[1], elem[2], elem[3], elem[4], elem[5], elem[6], elem[7], \
							elem[8], elem[9], elem[10], elem[11], elem[12], elem[13], elem[14], elem[15]);
}

int main (int argc, char* argv[])
{
	#ifdef AVX
	__m256* _a = new __m256[N/8]; 
	__m256* _b = new __m256[N/8]; 
	__m256* _c = new __m256[N/8]; 
	#endif
	#ifdef AVX512
	__m512* _d = new __m512[N/16]; 
	__m512* _e = new __m512[N/16]; 
	__m512* _f = new __m512[N/16]; 
	#endif
	
	float* _fa = new float[N];
	float* _fb = new float[N];
	float* _fc = new float[N];

	// assign values to N elements where _f points to the start
	for (int i = 0; i < N; i++)
	{
		_fa[i] = (float) i/0.125;
		_fb[i] = (float) i/0.133;
	}

	#ifdef AVX
	// assign values to the m256 vectors
	for (int i = 0; i < N/8; i++)
	{
		assign_m256(_a + i, _fa + 8*i);
		assign_m256(_b + i, _fb + 8*i);
	}
	#endif

	#ifdef AVX512
	// assign values to the m512 vectors
	for (int i = 0; i < N/16; i++)
	{
		assign_m512(_d + i, _fa + 16*i);
		assign_m512(_e + i, _fb + 16*i);
	}
	#endif

	auto begin = get_time();

	for (int i = 0; i < N; i++)
		_fc[i] = _fa[i] + _fb[i];

	auto first = get_time();

	#ifdef AVX
	for (int i = 0; i < N/8; i++)
		_c[i] = _mm256_add_ps(_a[i], _b[i]);
	#endif
	
	auto second = get_time();

	#ifdef AVX512
	for (int i = 0; i < N/16; i++)
		_f[i] = _mm512_add_ps(_d[i], _e[i]);
	#endif
	
	auto end = get_time();

	auto float_add = duration<double, milli>(first - begin);
	auto avx_add = duration<double, milli>(second - first);
	auto avx512_add = duration<double, milli>(end - second);

	cout << "Time taken for float addition = " << float_add.count() << "ms" << endl;
	#ifdef AVX
	cout << "Time taken for avx addition = " << avx_add.count() << "ms" << endl;
	#endif
	#ifdef AVX512
	cout << "Time taken for avx512 addition = " << avx512_add.count() << "ms" << endl;
	#endif

	#ifdef AVX
	delete[] _a; 
	delete[] _b; 
	delete[] _c; 
	#endif
	#ifdef AVX512
	delete[] _d; 
	delete[] _e; 
	delete[] _f; 
	#endif 
	delete[] _fa; 
	delete[] _fb; 
	delete[] _fc; 

	return 0;
}
