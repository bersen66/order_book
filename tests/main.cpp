#include <random>
#include <algorithm>

#include <orders/order_book.hpp>

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

OrderBook ob;
void InsertBenchmark()
{
	ob.Insert(RandomOrder());
}

void EraseBenchmark()
{
	static OrderId id = 0;
	ob.Erase(id++);
}

void UpdateBenchmark()
{
	static OrderId id = 0;
	ob.Update(id++, RandomOrder());
}

void TopBenchmark()
{
	ob.Top(10);
}

void TestBasic()
{
	OrderBook o;
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
	auto it = std::partition(result.begin(), result.end(), ask_part);
	std::sort(result.begin(), it, ask_cmp);
	std::sort(it, result.end(), bid_cmp);

	return result;
}

void TestTop()
{
	auto expected = GenerateCorrectTop(10);
	OrderBook o;
	ASSERT(o.Empty(), "Must be empty");
	for (const auto& order: expected)
	{
		auto id = o.Insert(order);
		ASSERT(o.Contains(id), "OrderBook must contain order");
		ASSERT_EQUAL(order, o.Get(id));
	}
	ASSERT_EQUAL(o.Size(), expected.size());
	ASSERT_EQUAL(o.Top(), expected);
}

int main()
{
	std::cerr << "Benchmarks:" << std::endl;
	RUN_BENCHMARK(RandomOrder)
	RUN_BENCHMARK(InsertBenchmark);
	RUN_BENCHMARK(UpdateBenchmark);
	RUN_BENCHMARK(TopBenchmark);
	RUN_BENCHMARK(EraseBenchmark);
	std::cerr << std::endl;

	std::cerr << "Tests:" << std::endl;

	TestRunner tr{};
	RUN_TEST(tr, TestBasic);
	RUN_TEST(tr, TestTop);
	return 0;
}