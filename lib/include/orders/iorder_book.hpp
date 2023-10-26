#pragma once

#include <memory> // for std::unique_ptr
#include <vector>
#include <orders/order.hpp>

struct IOrderBook {
	virtual OrderId Insert(const Order& o) = 0;

	virtual void Update(OrderId id, const Order& updated) = 0;

	virtual void Erase(OrderId id) = 0;

	[[nodiscard]] virtual const Order& Get(OrderId id) const = 0;

	[[nodiscard]] virtual std::vector<Order> Top(int n) const = 0;

	[[nodiscard]] virtual bool Empty() const noexcept = 0;

	[[nodiscard]] virtual std::size_t Size() const noexcept = 0;

	[[nodiscard]] virtual bool Contains(OrderId id) const = 0;

	virtual ~IOrderBook() = default;
};

using OrderBookPtr = std::unique_ptr<IOrderBook>;

template<typename OrderBookT, typename... Args>
OrderBookPtr MakeOrderBook(Args&& ... args)
{
	return std::make_unique<OrderBookT>(std::forward<Args>(args)...);
}