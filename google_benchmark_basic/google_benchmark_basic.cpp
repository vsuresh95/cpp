#include <iostream>
#include <benchmark/benchmark.h>
#include <gperftools/profiler.h>
#include <vector>
#include <limits>

using namespace std;

static void cacheBench (benchmark::State& s)
{
    // this is the optional input that you can pass
    // via the command line.
    int bytes = 1 << s.range(0);

    // cout << "Bytes = " << bytes << endl;

    // the number of iterations of the benchmark that
    // will run. the '2' is there because the same
    // area of memory is used for storing the random values
    // and their indices.
    int count = bytes / sizeof(int) / 2;

    // cout << "Count = " << count << endl;

    // initialize the two vectors with random values
    vector<int> random_val;
    vector<int> random_index;

    for(int i = 0; i < count; i++)
    {
        random_val.push_back(rand() % numeric_limits<int>::max());
        random_index.push_back(rand() % count);
    }

    ProfilerStart("cpu_profile.temp");

    // the actual benchmark loop that checks whether
    // there are iterations left or not. If the benchmark has not
    // yet started, then this function starts it.
    while(s.KeepRunning())
    {
        long sum = 0;
        for(int i : random_index)
        {
            sum += random_val[i];
        }

        // to make sure there are no compiler optimizations here
        benchmark::DoNotOptimize(sum);
    }

    ProfilerStop();

    // it is called after a benchmark and sets the number of bytes processed
    s.SetBytesProcessed(long(s.iterations()) * long(bytes));

    s.SetLabel(to_string(bytes/1024) + "kB");
}

BENCHMARK(cacheBench)->DenseRange(12,28)->ReportAggregatesOnly(true);

BENCHMARK_MAIN ();