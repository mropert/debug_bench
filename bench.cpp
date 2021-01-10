#include <bench.h>

#include <numeric>

namespace LIB_NAME
{
	bench_impl::output do_bench_c(const bench_impl::input& i)
	{
		int sum = 0;
		const int* p = i.first;
		while (p != i.last)
		{
			sum += *p++;
		}
		return sum;
	}

	bench_impl::output do_bench_cpp(const bench_impl::input& i)
	{
		return std::accumulate(i.first, i.last, 0);
	}
}
