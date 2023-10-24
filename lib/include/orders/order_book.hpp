#pragma once


#include <orders/order.hpp>
#include <map>
#include <list>
#include <unordered_map>

class OrderBook {
private:
	using OrderList = std::list<Order>;
	using OrderListIter = std::list<Order>::iterator;
public:

	OrderId Insert(const Order& o);

	void Update(OrderId id, const Order& updated);

	const Order& Get(OrderId id) const;

	void Erase(OrderId id);

	const std::map<Currency, OrderList, std::greater<>>& Bids() const;

	const std::map<Currency, OrderList, std::less<>>& Asks() const;

private:
	inline OrderListIter InsertIntoTable(const Order& o);

	inline void EraseFromMap(OrderListIter it);

private:
	std::map<Currency, OrderList, std::greater<>> bid_;
	std::map<Currency, OrderList, std::less<>> ask_;
	std::unordered_map<OrderId, OrderListIter> resolver_;
};