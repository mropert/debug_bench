#include <numeric>

#include <benchmark/benchmark.h>

#define BENCH_MAIN
#include <bench.h>

BENCHMARK_MAIN();


namespace bench_impl
{
	std::vector<int> generate_input()
	{
		std::vector<int> v( 10000 );
		std::iota(begin(v), end(v), 0);
		return v;
	}
}
