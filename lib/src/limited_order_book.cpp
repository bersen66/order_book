#include <orders/limited_order_book.hpp>
#include <sstream>
#include <algorithm>


bool operator==(const LimitedOrderBook::OrderWrapper& lhs,
                const LimitedOrderBook::OrderWrapper& rhs)
{
	return lhs.id == rhs.id;
}

bool operator!=(const LimitedOrderBook::OrderWrapper& lhs,
                const LimitedOrderBook::OrderWrapper& rhs)
{
	return lhs.id != rhs.id;
}

LimitedOrderBook::LimitedOrderBook(std::size_t bid_limit,
                                   std::size_t ask_limit)
{
	bid_.reserve(bid_limit);
	ask_.reserve(ask_limit);
	contained_.reserve(ask_limit + bid_limit);
}

OrderId LimitedOrderBook::Insert(const Order& o)
{
	OrderId id = GenerateID(o);
	static constexpr auto ask_cmp =
			[](const OrderWrapper& lhs, const OrderWrapper& rhs) {
				return std::tie(lhs.order.price, lhs.order.amount)
				       < std::tie(rhs.order.price, rhs.order.amount);
			};
	static constexpr auto bid_cmp =
			[](const OrderWrapper& lhs, const OrderWrapper& rhs) {
				return std::tie(lhs.order.price, lhs.order.amount)
				       > std::tie(rhs.order.price, rhs.order.amount);
			};

	switch (o.type)
	{
		case Order::Type::ASK:
			ask_.emplace_back(o, id);
			std::sort(ask_.begin(), ask_.end(), ask_cmp);
			break;
		case Order::Type::BID:
			bid_.emplace_back(o, id);
			std::sort(bid_.begin(), bid_.end(), bid_cmp);
			break;
	}

	contained_[id] = o.type;

	return id;
}

void LimitedOrderBook::Update(OrderId id, const Order& updated)
{
	auto [it, cont] = Locate(id);
	cont.erase(it);

	static constexpr auto ask_cmp =
			[](const OrderWrapper& lhs, const OrderWrapper& rhs) {
				return std::tie(lhs.order.price, lhs.order.amount)
				       < std::tie(rhs.order.price, rhs.order.amount);
			};
	static constexpr auto bid_cmp =
			[](const OrderWrapper& lhs, const OrderWrapper& rhs) {
				return std::tie(lhs.order.price, lhs.order.amount)
				       > std::tie(rhs.order.price, rhs.order.amount);
			};

	switch (updated.type)
	{
		case Order::Type::ASK:
			ask_.emplace_back(updated, id);
			std::sort(ask_.begin(), ask_.end(), ask_cmp);
			break;
		case Order::Type::BID:
			bid_.emplace_back(updated, id);
			std::sort(bid_.begin(), bid_.end(), bid_cmp);
			break;
	}

	contained_[id] = updated.type;
}

void LimitedOrderBook::Erase(OrderId id)
{
	auto [it, cont] = Locate(id);
	cont.erase(it);
	contained_.erase(id);
}

void AppendN(std::vector<Order>& dest,
             const std::vector<LimitedOrderBook::OrderWrapper>& src,
             int n)
{
	int cnt = 0;
	for (const auto& [order, id]: src)
	{
		if (cnt == n)
		{
			return;
		}
		dest.push_back(order);
		cnt++;
	}
}

std::vector<Order> LimitedOrderBook::Top(int n) const
{

	std::vector<Order> result;
	result.reserve(n);

	AppendN(result, ask_, n / 2);
	AppendN(result, bid_, n / 2);

	return result;
}

const Order& LimitedOrderBook::Get(OrderId id) const
{
	auto [it, cont] = Locate(id);
	return it->order;
}

bool LimitedOrderBook::Contains(OrderId id) const
{
	return contained_.count(id);
}

bool LimitedOrderBook::Empty() const noexcept
{
	return contained_.empty();
}

std::size_t LimitedOrderBook::Size() const noexcept
{
	return contained_.size();
}

std::vector<LimitedOrderBook::OrderWrapper>::const_iterator
LimitedOrderBook::ConstIteratorToWrapper(
		const std::vector<LimitedOrderBook::OrderWrapper>& cont,
		OrderId id) const
{
	auto pred = [id](const LimitedOrderBook::OrderWrapper& w) {
		return id == w.id;
	};
	auto it = std::find_if(cont.begin(), cont.end(), pred);
	return it;
}

std::vector<LimitedOrderBook::OrderWrapper>::iterator
LimitedOrderBook::IteratorToWrapper(
		std::vector<LimitedOrderBook::OrderWrapper>& cont, OrderId id)
{
	auto pred = [id](const LimitedOrderBook::OrderWrapper& w) {
		return id == w.id;
	};
	auto it = std::find_if(cont.begin(), cont.end(), pred);
	return it;
}

std::pair<
		std::vector<LimitedOrderBook::OrderWrapper>::const_iterator,
		const std::vector<LimitedOrderBook::OrderWrapper>>
LimitedOrderBook::Locate(OrderId id) const
{
	if (!Contains(id))
	{
		throw std::runtime_error("Invalid id");
	}

	const auto& cont = (contained_.at(id) == Order::Type::BID) ? bid_ : ask_;
	auto it = ConstIteratorToWrapper(cont, id);
	return std::make_pair(it, cont);
}

std::pair<
		std::vector<LimitedOrderBook::OrderWrapper>::iterator,
		std::vector<LimitedOrderBook::OrderWrapper>>
LimitedOrderBook::Locate(OrderId id)
{
	if (!Contains(id))
	{
		throw std::runtime_error("Invalid id");
	}

	auto& cont = (contained_.at(id) == Order::Type::BID) ? bid_ : ask_;
	auto it = IteratorToWrapper(cont, id);
	return std::make_pair(it, cont);
}











