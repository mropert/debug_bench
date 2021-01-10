cmake_minimum_required(VERSION 3.18)
project(debug_bench CXX)

find_package(benchmark CONFIG REQUIRED)

add_compile_options(/MT /Zi)
add_link_options(/DEBUG)

set(HEADER_FILE ${CMAKE_BINARY_DIR}/bench.h)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR})

file(WRITE ${HEADER_FILE} "// Auto generated file, do no edit!\n#pragma once\n\n")
file(WRITE ${HEADER_FILE} "#include <bench_impl.h>\n\n")

add_executable(bench_debug main.cpp bench_impl.h ${HEADER_FILE})
target_link_libraries(bench_debug PRIVATE benchmark::benchmark benchmark::benchmark_main)

set(BENCH_SRC bench.cpp)

set(IMPL_NAMESPACE "bench_impl")
set(BENCHMARKS "bench_c" "bench_cpp")

set(OPT_FLAGS "/Od" "/O2")
set(INLINE_FLAGS "/Ob0" "/Ob1" "/Ob2")
set(RUNTIME_FLAGS "/MT" "/MTd" )
set(ITERATOR_DEBUG_LEVELS 0 1 )
foreach(OPT_FLAG IN LISTS OPT_FLAGS)
    foreach(INLINE_FLAG IN LISTS INLINE_FLAGS)
        foreach(RUNTIME_FLAG IN LISTS RUNTIME_FLAGS)
            foreach(ITERATOR_DEBUG_LEVEL IN LISTS ITERATOR_DEBUG_LEVELS)
                string(REPLACE "/" "_" OPT_NAME ${OPT_FLAG}${INLINE_FLAG}${RUNTIME_FLAG}_ITD${ITERATOR_DEBUG_LEVEL})
                set(LIB_NAME "bench${OPT_NAME}")
                set(LIB_EXPORT "BENCH${OPT_NAME}_EXPORT")
                add_library(${LIB_NAME} SHARED ${BENCH_SRC} bench_impl.h ${HEADER_FILE})
                target_compile_definitions(${LIB_NAME} PRIVATE _ITERATOR_DEBUG_LEVEL=${ITERATOR_DEBUG_LEVEL} LIB_NAME=${LIB_NAME} ${LIB_EXPORT})
                target_compile_options(${LIB_NAME} PRIVATE ${OPT_FLAG} ${INLINE_FLAG} ${RUNTIME_FLAG})
                target_link_libraries(bench_debug PRIVATE ${LIB_NAME})

                set(LIB_API "BENCH${OPT_NAME}_API")
                file(APPEND ${HEADER_FILE} "#ifdef ${LIB_EXPORT}\n#define ${LIB_API} __declspec(dllexport)\n#else\n#define ${LIB_API} __declspec(dllimport)\n#endif\n\n")

                foreach(BENCH_NAME IN LISTS BENCHMARKS)
                    file(APPEND ${HEADER_FILE} "namespace ${LIB_NAME} { ${LIB_API} ${IMPL_NAMESPACE}::output do_${BENCH_NAME}(const ${IMPL_NAMESPACE}::input&); }\n\n\n")
                endforeach()

                list(APPEND OPTS ${OPT_NAME})
            endforeach()
        endforeach()
    endforeach()
endforeach()

file(APPEND ${HEADER_FILE} "#ifdef BENCH_MAIN\n\n" )

foreach(BENCH_NAME IN LISTS BENCHMARKS)
    foreach(OPT_NAME IN LISTS OPTS)
        file(APPEND ${HEADER_FILE} "static void BM_${BENCH_NAME}_${OPT_NAME}(benchmark::State& state) {\n")
        file(APPEND ${HEADER_FILE} "\tauto input = ${IMPL_NAMESPACE}::generate_input();\n")
        file(APPEND ${HEADER_FILE} "\tfor (auto _ : state) {\n")
        file(APPEND ${HEADER_FILE} "\t\tconst auto res = bench${OPT_NAME}::do_${BENCH_NAME}(input);\n")
        file(APPEND ${HEADER_FILE} "\t\tbenchmark::DoNotOptimize(res);\n")
        file(APPEND ${HEADER_FILE} "\t}\n")
        file(APPEND ${HEADER_FILE} "}\n")
        file(APPEND ${HEADER_FILE} "BENCHMARK(BM_${BENCH_NAME}_${OPT_NAME});\n\n\n" )
    endforeach()
endforeach() 

file(APPEND ${HEADER_FILE} "#endif\n" )
