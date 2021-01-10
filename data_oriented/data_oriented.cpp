#include <iostream>
#include <benchmark/benchmark.h>
#include <vector>
#include <time.h>

using namespace std;

// a single member of the org and their attributes
struct member
{
	int member_data[20];

	void initialize ()
	{
		srand(time(NULL));

		for (auto i : member_data)
		{
			i = rand() % 256; 
		}
	}
};

// a single attribute and its value for every member of the org
struct group
{	
	vector<int> category[20];

	void resize (int members)
	{
		for(auto i : category)
		{
			i.resize(members);
		}
	}

	void initialize ()
	{
		srand(time(NULL));

		for (auto i : category)
		{
			for(auto j : i)
			{
				j = rand() % 256; 
			}
		}
	}
};

// structure of vectors
static void data_oriented_bm (benchmark::State& s)
{
	// number of members
	int size = 1 << s.range(0);

	group org;

	org.resize(size);

	org.initialize();

    while(s.KeepRunning())
    {
		for (auto i : org.category)
		{
			for (auto j : i)
			{
				j++;
			}
		}
	}
}

// range of the number of records we need
BENCHMARK(data_oriented_bm)->DenseRange(10,26); 

// vector of structures
static void object_oriented_bm (benchmark::State& s)
{
	// size of the record
	int members = 1 << s.range(0);

	vector<member> record;
	record.resize(members);

	for (auto i : record)
	{
		i.initialize();
	}

    while(s.KeepRunning())
    {
		for (auto i : record)
		{
			for (auto j : i.member_data)
			{
				j++;
			}
		}
	}
}

// range of the number of members we need
BENCHMARK(object_oriented_bm)->DenseRange(10,26); 

BENCHMARK_MAIN();