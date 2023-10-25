#pragma once


#include <orders/order.hpp>
#include <map>
#include <list>
#include <unordered_map>
#include <vector>

class OrderBook {
private:
	using OrderList = std::list<Order>;
	using OrderListIter = std::list<Order>::iterator;
public:

	OrderBook();

	OrderBook(OrderBook&& other) = default;

	OrderBook(const OrderBook&) = delete;

	OrderBook& operator=(const OrderBook&) = delete;

	OrderId Insert(const Order& o);

	void Update(OrderId id, const Order& updated);

	const Order& Get(OrderId id) const;

	void Erase(OrderId id);

	std::vector<Order> Top(int n = 10) const;

	const std::map<Currency, OrderList, std::greater<Currency>>& Bids() const;

	const std::map<Currency, OrderList, std::less<Currency>>& Asks() const;

	[[nodiscard]] bool Empty() const noexcept;

	std::size_t Size() const noexcept;

	bool Contains(OrderId id) const;

private:
	inline OrderListIter InsertIntoTable(const Order& o);

	inline void EraseFromMap(OrderListIter it);

private:
	std::map<Currency, OrderList, std::greater<Currency>> bid_;
	std::map<Currency, OrderList, std::less<Currency>> ask_;
	std::unordered_map<OrderId, OrderListIter> resolver_;
};




