#pragma once

#include <vector>

namespace bench_impl
{
	using internal_input = std::vector<int>;

	// Do not pass STL types as input or output, as ABI may differ
	struct input
	{
		input(internal_input& i)
			: first(i.data())
			, last(i.data() + i.size())
		{}

		const int* first;
		const int* last;
	};
	using output = int;

	internal_input generate_input();
}
