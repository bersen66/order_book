#pragma once

#include <map>
#include <vector>
#include <orders/order.hpp>

class UniqueOrderBook {
public:
	using AskMap = std::map<Currency, Amount, std::less<>>;
	using BidMap = std::map<Currency, Amount, std::greater<>>;
public:
	[[nodiscard]] const BidMap& Bids() const;

	[[nodiscard]] const AskMap& Asks() const;

	void Insert(const Order& o);

	void EraseBid(Currency price);

	void EraseAsk(Currency price);

	[[nodiscard]] Order GetAsk(Currency price) const;

	[[nodiscard]] Order GetBid(Currency price) const;

	void ModifyAsk(Currency price, const Order& updated);

	void ModifyBid(Currency price, const Order& updated);

	[[nodiscard]] std::vector<Order> Top(int n = 10) const;

	[[nodiscard]] bool Empty() const noexcept;

	[[nodiscard]] std::size_t Size() const noexcept;

private:
	template<
	        Order::Type type,
			typename Key,
			typename Value,
			typename Cmp,
			template<typename, typename, typename> class Map>
	void
	DoModify(Currency price, const Order& updated, Map<Key, Value, Cmp>& map)
	{
		if (map.count(price) == 0)
		{
			return;
		}

		if (updated.type == type && price == updated.price)
		{
			map[price] = updated.price;
			return;
		}
		map.erase(price);
		Insert(updated);
	}

private:
	AskMap asks_;
	BidMap bids_;
};