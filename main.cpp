#include <numeric>

#include <benchmark/benchmark.h>

#define BENCH_MAIN
#include <bench.h>

BENCHMARK_MAIN();

namespace bench_impl
{
	internal_input generate_input()
	{
		internal_input i(10000);
		std::iota(begin(i), end(i), 0);
		return i;
	}
}
