#include <random>
#include <algorithm>
#include <vector>

#include <orders/order.hpp>

std::random_device rd;
std::mt19937 rng(rd());
std::uniform_int_distribution<std::uint64_t> uni(0, 10'000);

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

std::vector<Order> GenerateCorrectTop(int n)
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