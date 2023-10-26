#include <random>
#include <algorithm>

#include <orders/orders.hpp>

#include "profiler.hpp"
#include "test_runner.hpp"


std::random_device rd;
std::mt19937 rng(rd());
std::uniform_int_distribution<std::uint64_t> uni(0, 100);

Order RandomOrder()
{
	Order result{};
	result.type = uni(rng) % 2 ? Order::Type::BID : Order::Type::ASK;
	result.price = uni(rng);
	result.amount = uni(rng);
	return result;
}

Order RandomBid()
{
	auto res = RandomOrder();
	res.type = Order::Type::BID;
	return res;
}

Order RandomAsk()
{
	auto res = RandomOrder();
	res.type = Order::Type::ASK;
	return res;
}

std::vector<Order> GenerateCorrectTop(int n = 10)
{
	std::vector<Order> result;
	result.reserve(n);

	for (int i = 0; i < n; ++i)
	{
		if (i < n / 2)
		{
			result.push_back(RandomAsk());
		}
		else
		{
			result.push_back(RandomBid());
		}

	}

	auto ask_part = [](const Order& o) -> bool {
		return o.type == Order::Type::ASK;
	};
	auto ask_cmp = [](const Order& lhs, const Order& rhs) -> bool {
		return lhs.price > rhs.price;
	};
	auto bid_cmp = [](const Order& lhs, const Order& rhs) -> bool {
		return lhs.price > rhs.price;
	};
	auto it = std::stable_partition(result.begin(), result.end(), ask_part);
	std::stable_sort(result.begin(), it, ask_cmp);
	std::stable_sort(it, result.end(), bid_cmp);

	return result;
}

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
	}

private:
	TestRunner tr_;
};

int main()
{
	Tester<OrderBook> t;
	t.Run();
	return 0;
}