#pragma once

#include <vector>

namespace bench_impl
{
	// Do not pass STL types as input or output, as ABI may differ
	struct input
	{
		input(const std::vector<int>& v)
			: m_first(v.data())
			, m_last(v.data() + v.size())
		{}

		const int* m_first;
		const int* m_last;
	};
	using output = int;

	std::vector<int> generate_input();
}
