#include "profiler.hpp"

Profiler::Profiler(std::uint64_t& output)
		: result(output)
		  , start(std::chrono::steady_clock::now())
{
}

Profiler::~Profiler()
{
	using namespace std::chrono;
	auto finish = steady_clock::now();
	result = duration_cast<nanoseconds>(finish - start).count();
}