cmake_minimum_required(VERSION 3.18)
project(debug_bench CXX)

find_package(benchmark CONFIG REQUIRED)

add_compile_options("/Zi")
add_link_options("/DEBUG")

set(HEADER_FILE ${CMAKE_BINARY_DIR}/bench.h)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR})

file(WRITE ${HEADER_FILE} "// Auto generated file, do no edit!\n#pragma once\n\n")
file(WRITE ${HEADER_FILE} "#include <bench_impl.h>\n\n")

add_executable(bench_debug main.cpp bench_impl.h ${HEADER_FILE})
target_link_libraries(bench_debug PRIVATE benchmark::benchmark benchmark::benchmark_main)

set(BENCH_SRC bench.cpp)

set(BENCH_FUNCTION_DECL "bench_impl::output process(const bench_impl::input&)")

set(OPT_FLAGS "/Od" "/O2")
set(INLINE_FLAGS "/Ob0" "/Ob1" "/Ob2")
set(RUNTIME_FLAGS "/MT" "/MTd")
foreach(OPT_FLAG IN LISTS OPT_FLAGS)
    foreach(INLINE_FLAG IN LISTS INLINE_FLAGS)
        foreach(RUNTIME_FLAG IN LISTS RUNTIME_FLAGS)
            string(REPLACE "/" "_" OPT_NAME ${OPT_FLAG}${INLINE_FLAG}${RUNTIME_FLAG})
            set(LIB_NAME "bench${OPT_NAME}")
            set(LIB_EXPORT "BENCH${OPT_NAME}_EXPORT")
            add_library(${LIB_NAME} SHARED ${BENCH_SRC} bench_impl.h ${HEADER_FILE})
            target_compile_definitions(${LIB_NAME} PRIVATE LIB_NAME=${LIB_NAME} ${LIB_EXPORT})
            target_compile_options(${LIB_NAME} PRIVATE ${OPT_FLAG} ${INLINE_FLAG} ${RUNTIME_FLAG})
            target_link_libraries(bench_debug PRIVATE ${LIB_NAME})

            set(LIB_API "BENCH${OPT_NAME}_API")
            file(APPEND ${HEADER_FILE} "#ifdef ${LIB_EXPORT}\n#define ${LIB_API} __declspec(dllexport)\n#else\n#define ${LIB_API} __declspec(dllimport)\n#endif\n\n")
            file(APPEND ${HEADER_FILE} "namespace ${LIB_NAME} { ${LIB_API} ${BENCH_FUNCTION_DECL}; }\n\n\n")

            list(APPEND LIBS ${LIB_NAME})
        endforeach()
    endforeach()
endforeach()

file(APPEND ${HEADER_FILE} "#ifdef BENCH_MAIN\n\n" )

foreach(LIB_NAME IN LISTS LIBS)
    file(APPEND ${HEADER_FILE} "static void BM_${LIB_NAME}(benchmark::State& state) {\n")
    file(APPEND ${HEADER_FILE} "\tconst auto input = bench_impl::generate_input();\n")
    file(APPEND ${HEADER_FILE} "\tfor (auto _ : state) {\n")
    file(APPEND ${HEADER_FILE} "\t\tconst auto res = ${LIB_NAME}::process(input);\n")
    file(APPEND ${HEADER_FILE} "\t\tbenchmark::DoNotOptimize(res);\n")
    file(APPEND ${HEADER_FILE} "\t}\n")
    file(APPEND ${HEADER_FILE} "}\n")
    file(APPEND ${HEADER_FILE} "BENCHMARK(BM_${LIB_NAME});\n\n\n" )
endforeach()

file(APPEND ${HEADER_FILE} "#endif\n" )
