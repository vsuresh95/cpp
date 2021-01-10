#include <iostream>
#include <benchmark/benchmark.h>

using namespace std;

static void naive_multiply (benchmark::State& s)
{
	int N = 1 << s.range(0);

	float* a = new float[N];
	float* b = new float[N];
	float* c = new float[N];

	for (int i = 0; i < N; i++)
	{
		a[i] = rand() % 255;
		b[i] = rand() % 255;
	}

	while (s.KeepRunning())
	{
		for (int i = 0; i < N; i++)
		{
			c[i] = a[i] * b[i];
		}

		benchmark::DoNotOptimize(c);
	} 
}

BENCHMARK(naive_multiply) -> DenseRange(20, 26) -> Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();