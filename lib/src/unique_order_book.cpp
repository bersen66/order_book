#include <orders/unique_order_book.hpp>
#include <algorithm>

const UniqueOrderBook::BidMap& UniqueOrderBook::Bids() const
{
	return bids_;
}

const UniqueOrderBook::AskMap& UniqueOrderBook::Asks() const
{
	return asks_;
}

void UniqueOrderBook::EraseBid(Currency price)
{
	bids_.erase(price);
}

void UniqueOrderBook::EraseAsk(Currency price)
{
	asks_.erase(price);
}

Order UniqueOrderBook::GetAsk(Currency price) const
{
	Amount amount = asks_.at(price);
	return Order{
			.type=Order::Type::ASK,
			.price=price,
			.amount=amount,
	};
}

Order UniqueOrderBook::GetBid(Currency price) const
{
	Amount amount = bids_.at(price);
	return Order{
			.type=Order::Type::BID,
			.price=price,
			.amount=amount,
	};
}

void UniqueOrderBook::ModifyAsk(Currency price, const Order& updated)
{
	DoModify<Order::Type::ASK>(price, updated, asks_);
}

void UniqueOrderBook::ModifyBid(Currency price, const Order& updated)
{
	DoModify<Order::Type::BID>(price, updated, bids_);
}

void UniqueOrderBook::Insert(const Order& o)
{
	switch (o.type)
	{
		case Order::Type::ASK:
			asks_[o.price] = o.amount;
			break;
		case Order::Type::BID:
			bids_[o.price] = o.amount;
			break;
	}
}

template<
		Order::Type type,
		typename K,
		typename V,
		typename Cmp,
		template<typename, typename, typename> class Map>
void AppendN(const Map<K, V, Cmp>& src, int n, std::vector<Order>& result)
{
	std::uint64_t cnt = 0;
	for (const auto& [price, amount]: src)
	{
		result.push_back(Order{
				.type=type,
				.price=price,
				.amount=amount,
		});
		cnt++;
		if (cnt == n)
		{
			return;
		}
	}

}

std::vector<Order> UniqueOrderBook::Top(int n) const
{
	std::vector<Order> result;
	result.reserve(n);

	AppendN<Order::Type::ASK>(asks_, n / 2, result);
	std::reverse(result.begin(), result.end());
	AppendN<Order::Type::BID>(bids_, n / 2, result);

	return result; // NRVO
}

bool UniqueOrderBook::Empty() const noexcept
{
	return asks_.empty() && bids_.empty();
}

std::size_t UniqueOrderBook::Size() const noexcept
{
	return asks_.size() + bids_.size();
}
