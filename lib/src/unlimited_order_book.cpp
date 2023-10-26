#include <algorithm>
#include "orders/unlimited_order_book.hpp"

UnlimitedOrderBook::UnlimitedOrderBook()
{
	resolver_.reserve(812);
}

OrderId UnlimitedOrderBook::Insert(const Order& o)
{
	OrderId id = GenerateID(o);
	resolver_[id] = InsertIntoTable(o);
	return id;
}

void UnlimitedOrderBook::Update(OrderId id, const Order& updated)
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

const Order& UnlimitedOrderBook::Get(OrderId id) const
{
	return *resolver_.at(id);
}

void UnlimitedOrderBook::Erase(OrderId id)
{
	EraseFromMap(resolver_.at(id));
	resolver_.erase(id);
}

inline UnlimitedOrderBook::OrderListIter
UnlimitedOrderBook::InsertIntoTable(const Order& o)
{
	OrderList& orders = (o.type == Order::Type::BID ? bid_[o.price]
	                                                : ask_[o.price]);
	orders.push_back(o);
	return std::prev(orders.end());
}

void UnlimitedOrderBook::EraseFromMap(UnlimitedOrderBook::OrderListIter it)
{
	Currency prev_price = it->price;

	if (it->type == Order::Type::BID)
	{
		DoErase(bid_, prev_price, it);
	}
	else
	{
		DoErase(ask_, prev_price, it);
	}

}

const std::map<Currency, UnlimitedOrderBook::OrderList, std::greater<>>&
UnlimitedOrderBook::Bids() const
{
	return bid_;
}

const std::map<Currency, UnlimitedOrderBook::OrderList, std::less<>>&
UnlimitedOrderBook::Asks() const
{
	return ask_;
}

template<
		typename K,
		typename V,
		typename Cmp,
		template<typename, typename, typename> class Map>
void AppendN(const Map<K, V, Cmp>& src, int n, std::vector<Order>& result)
{
	std::uint64_t cnt = 0;
	for (const auto& [_, orders]: src)
	{
		for (const auto& o: orders)
		{
			result.push_back(o);
			cnt++;
			if (cnt == n)
			{
				return;
			}
		}
	}
}

std::vector<Order> UnlimitedOrderBook::Top(int n) const
{
	std::vector<Order> result;
	result.reserve(n);

	AppendN(ask_, n / 2, result);
	std::reverse(result.begin(), result.end());
	AppendN(bid_, n / 2, result);

	return result; // NRVO
}

bool UnlimitedOrderBook::Empty() const noexcept
{
	return bid_.empty() && ask_.empty() && resolver_.empty();
}

std::size_t UnlimitedOrderBook::Size() const noexcept
{
	return resolver_.size();
}

bool UnlimitedOrderBook::Contains(OrderId id) const
{
	return resolver_.find(id) != resolver_.end();
}


