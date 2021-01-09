#include <bench.h>

#include <numeric>

namespace LIB_NAME
{
	bench_impl::output process(const bench_impl::input& v)
	{
		return std::accumulate(v.m_first, v.m_last, 0);
	}
}
