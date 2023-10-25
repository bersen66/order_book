#include "test_runner.hpp"

TestRunner::~TestRunner()
{
	std::cerr.flush();
	if (tests_failed > 0) {
		std::cerr << tests_failed << " unit tests failed. Terminate" << std::endl;
		exit(1);
	}
}
