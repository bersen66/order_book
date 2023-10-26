#pragma once

#include <orders/order.hpp>
#include <orders/iorder_book.hpp>

#include <unordered_map>
#include <vector>
#include <memory>
#include <algorithm>

class LimitedOrderBook final : public IOrderBook {
public:
	explicit LimitedOrderBook(std::size_t bid_limit_ = 1000,
	                          std::size_t ask_limit_ = 1000);

	LimitedOrderBook(const LimitedOrderBook&) = delete;

	LimitedOrderBook& operator=(const LimitedOrderBook&) = delete;

	LimitedOrderBook(LimitedOrderBook&&) = default;

	OrderId Insert(const Order& o) override;

	void Update(OrderId id, const Order& updated) override;

	void Erase(OrderId id) override;

	[[nodiscard]] std::vector<Order> Top(int n) const override;

	[[nodiscard]] const Order& Get(OrderId id) const override;

	[[nodiscard]] bool Contains(OrderId id) const override;

	[[nodiscard]] bool Empty() const noexcept override;

	[[nodiscard]] std::size_t Size() const noexcept override;

private:
	struct OrderWrapper {
		Order order;
		OrderId id;

		OrderWrapper(const Order& o, OrderId id)
				: order(o), id(id)
		{
		}
	};

	friend bool operator==(const OrderWrapper& lhs,
	                       const OrderWrapper& rhs);

	friend bool operator!=(const OrderWrapper& lhs,
	                       const OrderWrapper& rhs);

	std::vector<OrderWrapper>::const_iterator
	ConstIteratorToWrapper(const std::vector<OrderWrapper>& cont,
	                       OrderId id) const;

	std::vector<OrderWrapper>::iterator
	IteratorToWrapper(std::vector<OrderWrapper>& cont, OrderId id);

	std::pair<
			std::vector<OrderWrapper>::const_iterator,
			const std::vector<OrderWrapper>>
	Locate(OrderId id) const;

	std::pair<
			std::vector<OrderWrapper>::iterator,
			std::vector<OrderWrapper>>
	Locate(OrderId id);

	friend void AppendN(std::vector<Order>& dest,
	                    const std::vector<LimitedOrderBook::OrderWrapper>& src,
	                    int n);

private:
	std::vector<OrderWrapper> bid_;
	std::vector<OrderWrapper> ask_;
	std::unordered_map<OrderId, Order::Type> contained_;
};
