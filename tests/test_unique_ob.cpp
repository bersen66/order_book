#include "test_runner.hpp"
#include <orders/unique_order_book.hpp>
#include <algorithm>
#include <iostream>


void UOBRunBenchmarks()
{
	UniqueOrderBook uob;

	try
	{
		std::uint64_t time = 0;
		Order o = RandomOrder();
		{
			Profiler p(time);
			uob.Insert(o);
		}
		std::cerr << "Insertion time: " << time << " ns" << std::endl;

		time = 0;
		Order updated = RandomOrder();
		if (o.type == Order::Type::BID)
		{
			Profiler p(time);
			uob.ModifyBid(o.price, updated);
		}
		else
		{
			Profiler p(time);
			uob.ModifyAsk(o.price, updated);
		}
		std::cerr << "Update time: " << time << " ns" << std::endl;

		time = 0;
		if (updated.type == Order::Type::BID)
		{
			Profiler p(time);
			uob.EraseBid(updated.price);
		}
		else
		{
			Profiler p(time);
			uob.EraseAsk(updated.price);
		}
		std::cerr << "Erase time: " << time << " ns" << std::endl;


		for (int i = 0; i < 100; ++i)
		{
			uob.Insert(RandomOrder());
		}

		time = 0;
		{
			Profiler p(time);
			std::vector<Order> res = uob.Top(10);
		}

		std::cerr << "Top time: " << time << " ns" << std::endl;

	}
	catch (const std::exception& exc)
	{
		std::cerr << "Exception throwed: " << exc.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "Unknown exception throwed " << std::endl;
	}
}

void UOBTestBasic()
{
	UniqueOrderBook uob;
	ASSERT(uob.Empty(), "Must be empty");


	for (int i = 0; i < 100; ++i)
	{
		Order o = RandomBid();
		uob.Insert(o);
		ASSERT(uob.Size() == 1, "Must be inserted");
		ASSERT(uob.GetBid(o.price) == o, "Must be equal");

		Order new_order = RandomAsk();
		uob.ModifyBid(o.price, new_order);

		ASSERT_EQUAL(uob.Bids().empty(), true);
		ASSERT_EQUAL(uob.Asks().empty(), false);

		uob.EraseAsk(new_order.price);
		ASSERT_EQUAL(uob.Empty(), true);
	}

	for (int i = 0; i < 100; ++i)
	{
		Order o = RandomAsk();
		uob.Insert(o);
		ASSERT(uob.Size() == 1, "Must be inserted");
		ASSERT(uob.GetAsk(o.price) == o, "Must be equal");

		Order new_order = RandomBid();
		uob.ModifyAsk(o.price, new_order);

		ASSERT_EQUAL(uob.Bids().empty(), false);
		ASSERT_EQUAL(uob.Asks().empty(), true);

		uob.EraseBid(new_order.price);
		ASSERT_EQUAL(uob.Empty(), true);
	}

}

void UOBTestTop()
{
	int n = 10;
	auto expected = GenerateCorrectTop(n);
	UniqueOrderBook o;
	ASSERT(o.Empty(), "Must be empty");
	for (const auto& order: expected)
	{
		o.Insert(order);
	}
	ASSERT_EQUAL(o.Size(), expected.size());

	const auto& top = o.Top(n);
	for (const auto& order: expected)
	{
		auto it = std::find(top.begin(), top.end(), order);
		ASSERT(it != top.end(), "Top must contain expected value");
	}
}

void TestUniqueOb()
{
	UOBRunBenchmarks();
	TestRunner tr;
	RUN_TEST(tr, UOBTestBasic);
	RUN_TEST(tr, UOBTestTop);
}