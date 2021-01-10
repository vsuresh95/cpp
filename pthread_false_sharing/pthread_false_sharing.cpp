#include <iostream>
#include <benchmark/benchmark.h>
#include <atomic>
#include <thread>
#include <gperftools/profiler.h>

using namespace std;

#define ITERATIONS 100000

// to be called for single threaded execution
void increment_atomic (atomic<int>& a)
{
	a++;
}

// to be called for multi threaded execution
void increment_atomic_thread (atomic<int>& a)
{
	for (int i = 0; i < ITERATIONS; i++)
	{
		a++;
	}
}

struct alignas(64) aligned_struct
{
	atomic<int> a;
};

// creating google benchmark function that takes the usual
// State& as the input
static void falseSharingBench (benchmark::State& s)
{
	int test_option = s.range(0);

	atomic<int> a, b, c, d;
	struct aligned_struct s_a, s_b, s_c, s_d; 

    ProfilerStart("cpu_profile.temp");

	while (s.KeepRunning())
	{
		switch (test_option)
		{
			// single threaded, alternatively incrementing
			// a, b, c, d
			case 0:
			for (int i = 0; i < ITERATIONS; i++)
			{
				increment_atomic(a);
				increment_atomic(b);
				increment_atomic(c);
				increment_atomic(d);
			}
			break;

			// multi threaded execution, with a, b, c, d within the 
			// same cache line
			case 1:
			{
				thread t1 ( [&]() { increment_atomic_thread(a); } );
				thread t2 ( [&]() { increment_atomic_thread(b); } );
				thread t3 ( [&]() { increment_atomic_thread(c); } );
				thread t4 ( [&]() { increment_atomic_thread(d); } );

				t1.join();
				t2.join();
				t3.join();
				t4.join();
			}
			break;

			// multi threaded execution, with a, b, c, d on different 
			// cache lines
			case 2:
			{
				thread t5 ( [&]() { increment_atomic_thread(s_a.a); } );
				thread t6 ( [&]() { increment_atomic_thread(s_b.a); } );
				thread t7 ( [&]() { increment_atomic_thread(s_c.a); } );
				thread t8 ( [&]() { increment_atomic_thread(s_d.a); } );

				t5.join();
				t6.join();
				t7.join();
				t8.join();
			}
			break;

			default:
			cout << "Wrong choice" << endl;
			break; 
		}
	}

    ProfilerStop(); 
}

// benchmark with run for limited range of inputs
BENCHMARK(falseSharingBench)->DenseRange(0,2); 

BENCHMARK_MAIN();