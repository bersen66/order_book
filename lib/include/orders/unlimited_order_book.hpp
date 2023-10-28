#pragma once

#include <orders/order.hpp>
#include <orders/iorder_book.hpp>
#include <map>
#include <list>
#include <unordered_map>
#include <vector>

class UnlimitedOrderBook final : public IOrderBook {
private:
	using OrderList = std::list<Order>;
	using OrderListIter = std::list<Order>::iterator;
public:

	UnlimitedOrderBook();

	UnlimitedOrderBook(UnlimitedOrderBook&& other) = default;

	UnlimitedOrderBook(const UnlimitedOrderBook&) = delete;

	UnlimitedOrderBook& operator=(const UnlimitedOrderBook&) = delete;

	OrderId Insert(const Order& o) override;

	void Update(OrderId id, const Order& updated) override;

	const Order& Get(OrderId id) const override;

	void Erase(OrderId id) override;

	std::vector<Order> Top(int n) const override;

	[[nodiscard]] bool Empty() const noexcept override;

	std::size_t Size() const noexcept override;

	bool Contains(OrderId id) const override;

	const std::map<Currency, OrderList, std::greater<>>& Bids() const;

	const std::map<Currency, OrderList, std::less<>>& Asks() const;

private:
	inline OrderListIter InsertIntoTable(const Order& o);

	inline void EraseFromMap(OrderListIter it);

	template<typename K,
			typename V,
			typename Cmp,
			typename Allocator,
			template<typename, typename, typename, typename> class Map>
	void DoErase(Map<K, V, Cmp, Allocator>& map, Currency prev_price, OrderListIter it)
	{
		map[prev_price].erase(it);
		if (map[prev_price].empty())
		{
			map.erase(prev_price);
		}
	}

private:
	std::map<Currency, OrderList, std::greater<>> bid_;
	std::map<Currency, OrderList, std::less<>> ask_;
	std::unordered_map<OrderId, OrderListIter> resolver_;
};




