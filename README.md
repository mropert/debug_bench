# debug_bench
Micro benchmark C++ code through different optimization &amp; debug flags

## Usage

* Define the `input` and `output` types of your benchmarked function in `bench_impl.h`.
* Define the `generate_input` function for your input data set.
* Define your benchmarked function(s) in `bench.cpp`
* List your benchmarked function(s) name(s) in the `BENCHMARKS` variable in `CMakeLists.txt`
* Run CMake, build and run the `Release` target

## Dependencies

* This project requires Google Benchmark. The easiest way to provide it is to install the `benchmark` package through vcpkg.
