#include "orders/order_book.hpp"

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