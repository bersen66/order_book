#pragma once

#include <cstdint>
#include <iostream>
#include <sstream>

#include "test_utils.hpp"
#include "profiler.hpp"

class TestRunner {
public:

	template<class TestFunc>
	void RunTest(TestFunc func, const std::string& test_name)
	{
		std::uint64_t test_time = 0;
		try
		{
			Benchmark(func, test_time);
			std::cerr << test_name << " OK. Test time: " << test_time << " ns."
			          << std::endl;
		} catch (std::exception& e)
		{
			++tests_failed;
			std::cerr << test_name << " fail: " << e.what() << "  Test time: "
			          << test_time << " ns." << std::endl;
		} catch (...)
		{
			++tests_failed;
			std::cerr << "Unknown exception caught" << "  Test time: "
			          << test_time << " ns." << std::endl;
		}
	}

	~TestRunner();

private:
	std::uint64_t tests_failed = 0;
};

template<class T, class U>
void AssertEqual(const T& t, const U& u, const std::string& hint = {})
{
	if (!(t == u))
	{
		std::ostringstream os;
		os << "Assertion failed: " << t << " != " << u;
		if (!hint.empty())
		{
			os << " hint: " << hint;
		}
		throw std::runtime_error(os.str());
	}
}

inline void Assert(bool b, const std::string& hint) {
	AssertEqual(b, true, hint);
}

#ifndef FILE_NAME
#define FILE_NAME __FILE__
#endif

#define ASSERT_EQUAL(x, y) {                          \
  std::ostringstream __assert_equal_private_os;       \
  __assert_equal_private_os                           \
    << #x << " != " << #y << ", "                     \
    << FILE_NAME << ":" << __LINE__;                  \
  AssertEqual(x, y, __assert_equal_private_os.str()); \
}


#define ASSERT(x, msg) { \
  std::ostringstream __assert_private_os;     \
  __assert_private_os << #x << " is false, "  \
    << FILE_NAME << ":" << __LINE__ << "\nmsg: " << msg;  \
  Assert(x, __assert_private_os.str());       \
}

#define RUN_TEST(tr, func) \
  tr.RunTest(func, #func)