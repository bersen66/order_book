#pragma once

#include <iostream>
#include <chrono>

class Profiler {
public:
	explicit Profiler(std::uint64_t& output);

	~Profiler();

private:
	uint64_t& result;
	std::chrono::steady_clock::time_point start;
};

template<typename BenchFunc>
void Benchmark(BenchFunc bf, std::uint64_t& result)
{
	Profiler t(result);
	bf();
}

template<typename BenchFunc, typename... Args>
void Benchmark(std::uint64_t& result, BenchFunc bf, Args&&... args)
{
	Profiler t(result);
	bf(std::forward<Args>(args)...);
}


template<typename BenchFunc>
std::uint64_t Average(BenchFunc foo, std::uint64_t iter = 100'000)
{
	std::uint64_t total_ns = 0;
	for (std::uint64_t i = 0; i < iter; i++)
	{
		std::uint64_t itertime_ns = 0;
		Benchmark(foo, itertime_ns);
		total_ns += itertime_ns;
	}
	return total_ns / iter;
}

template<typename BenchFunc, typename... Args>
std::uint64_t Average(std::uint64_t iter, BenchFunc foo, Args&&... args)
{
	std::uint64_t total_ns = 0;
	for (std::uint64_t i = 0; i < iter; i++)
	{
		std::uint64_t itertime_ns = 0;
		Benchmark(itertime_ns, foo, std::forward<Args>(args)...);
		total_ns += itertime_ns;
	}
	return total_ns / iter;
}

#define RUN_BENCHMARK(foo) {        \
    try {                           \
        std::uint64_t avg = Average(foo);   \
        std::cerr << "Avg " << #foo << " time: " << avg << " nanoseconds" << std::endl; \
    } catch(const std::exception& exc) {  \
        std::cerr << #foo << " throwed exception: " << exc.what() << std::endl; \
    } catch(...) { \
        std::cerr << #foo << "throwed unknown exception" << std::endl; \
    } \
    std::cerr.flush(); \
}