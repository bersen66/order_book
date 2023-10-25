#include <algorithm>
#include "orders/order_book.hpp"

OrderBook::OrderBook()
{
	resolver_.reserve(812);
}

OrderId OrderBook::Insert(const Order& o)
{
	OrderId id = GenerateID(o);
	resolver_[id] = InsertIntoTable(o);
	return id;
}

void OrderBook::Update(OrderId id, const Order& updated)
{
	OrderListIter it = resolver_.at(id);
	if (updated.price == it->price && updated.type == it->type)
	{
		it->amount = updated.amount;
		return;
	}

	EraseFromMap(it);
	resolver_[id] = InsertIntoTable(updated);
}

const Order& OrderBook::Get(OrderId id) const
{
	return *resolver_.at(id);
}

void OrderBook::Erase(OrderId id)
{
	EraseFromMap(resolver_.at(id));
	resolver_.erase(id);
}

inline OrderBook::OrderListIter OrderBook::InsertIntoTable(const Order& o)
{
	OrderList& orders = (o.type == Order::Type::BID ? bid_[o.price]
	                                                : ask_[o.price]);
	orders.push_back(o);
	return std::prev(orders.end());
}

void OrderBook::EraseFromMap(OrderBook::OrderListIter it)
{
	Currency prev_price = it->price;

	if (it->type == Order::Type::BID)
	{
		bid_[prev_price].erase(it);
		if (bid_[prev_price].empty())
		{
			bid_.erase(prev_price);
		}
	}
	else
	{
		ask_[prev_price].erase(it);
		if (ask_[prev_price].empty())
		{
			ask_.erase(prev_price);
		}
	}

}

const std::map<Currency, OrderBook::OrderList, std::greater<>>&
OrderBook::Bids() const
{
	return bid_;
}

const std::map<Currency, OrderBook::OrderList, std::less<>>&
OrderBook::Asks() const
{
	return ask_;
}

std::vector<Order> OrderBook::Top(int n) const
{
	std::vector<Order> result;
	result.reserve(n);

	int cnt = 0;
	for (const auto& [p, orders]: ask_)
	{
		for (const auto& o: orders)
		{
			result.push_back(o);
			cnt++;
			if (cnt == n / 2)
			{
				goto ENDLOOP1;
			}
		}
	}
ENDLOOP1:
	std::reverse(result.begin(), result.begin() + cnt);

	cnt = 0;
	for (const auto& [p, orders]: bid_)
	{
		for (const auto& o: orders)
		{
			result.push_back(o);
			cnt++;
			if (cnt == n / 2)
			{
				goto ENDLOOP2;
			}
		}
	}
ENDLOOP2:

	return result; // NRVO
}

bool OrderBook::Empty() const noexcept
{
	return bid_.empty() && ask_.empty() && resolver_.empty();
}

std::size_t OrderBook::Size() const noexcept
{
	return resolver_.size();
}

bool OrderBook::Contains(OrderId id) const
{
	return resolver_.find(id) != resolver_.end();
}


