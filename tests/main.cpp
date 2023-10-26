#include <random>
#include <algorithm>

#include <orders/orders.hpp>

#include "profiler.hpp"
#include "test_utils.hpp"
#include "test_runner.hpp"


template<typename OrderBookType>
void TestTop()
{
	int n = 10;
	auto expected = GenerateCorrectTop(n);
	OrderBookType o;
	ASSERT(o.Empty(), "Must be empty");
	for (const auto& order: expected)
	{
		OrderId id = o.Insert(order);
		ASSERT(o.Contains(id), "OrderBook must contain order");
		ASSERT_EQUAL(order, o.Get(id));
	}
	ASSERT_EQUAL(o.Size(), expected.size());
	const auto& top = o.Top(n);
	for (const auto& order: expected)
	{
		auto it = std::find(top.begin(), top.end(), order);
		ASSERT(it != top.end(), "Top must contain expected value");
	}

}

template<typename OrderBookType>
void TestBasic()
{
	OrderBookType o;
	ASSERT(o.Empty(), "Must be empty");
	auto order = RandomOrder();
	OrderId id = o.Insert(order);
	ASSERT(o.Contains(id), "Must contain order");
	ASSERT_EQUAL(o.Get(id), order);
	order = RandomOrder();
	o.Update(id, order);
	ASSERT_EQUAL(o.Get(id), order);
	o.Erase(id);
	ASSERT(!o.Contains(id), "Must contain order");
	ASSERT(o.Empty(), "Must be empty");
}


namespace bench
{
	OrderBookPtr global_bench;

	template<typename OrderBookType>
	void SetUpGlobalBench() {
		global_bench = MakeOrderBook<OrderBookType>();
	}

	void InsertBenchmark()
	{
		global_bench->Insert(RandomOrder());
	}

	void EraseBenchmark()
	{
		static OrderId id = 0;
		global_bench->Erase(id++);
	}

	void UpdateBenchmark()
	{
		static OrderId id = 0;
		global_bench->Update(id++, RandomOrder());
	}

	void TopBenchmark()
	{
		global_bench->Top(10);
	}

} // namespace bench

template<typename OrderBookType>
class Tester {
public:
	Tester() {
		bench::SetUpGlobalBench<OrderBookType>();
	}

	void Test()
	{
		RUN_TEST(tr_, TestBasic<OrderBookType>);
		RUN_TEST(tr_, TestTop<OrderBookType>);
	}

	void Benchmark()
	{
		RUN_BENCHMARK(RandomOrder)
		bench::global_bench = MakeOrderBook<OrderBookType>();
		RUN_BENCHMARK(bench::InsertBenchmark);
		RUN_BENCHMARK(bench::UpdateBenchmark);
		RUN_BENCHMARK(bench::TopBenchmark);
		RUN_BENCHMARK(bench::EraseBenchmark);
		std::cerr << std::endl;
	}

	void Run()
	{
		std::cerr << "Benchmarks:" << std::endl;
		Benchmark();
		std::cerr << "Tests:" << std::endl;
		Test();
		std::cerr << std::endl;
	}

private:
	TestRunner tr_;
};

int main()
{
//	{
//		std::cerr << "UNLIMITED ORDER BOOK: (TASK-1)" << std::endl;
//		Tester<OrderBook> t;
//		t.Run();
//	}

	{
		std::cerr << "LIMITED ORDER BOOK: (TASK-3)" << std::endl;
		Tester<LimitedOrderBook> t;
		t.Run();
	}
	return 0;
}