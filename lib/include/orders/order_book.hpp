#pragma once

#include <ostream> // std::ostream
#include <memory> // for std::unique_ptr


// pimpl idiom
class OrderBookBase
{
protected:
	void Push();

	void Pop();

	void Flush(std::ostream& out) const;

public:
	OrderBookBase();

	virtual ~OrderBookBase();

	OrderBookBase(const OrderBookBase&) = delete;
	OrderBookBase& operator=(const OrderBookBase&) = delete;
	OrderBookBase(OrderBookBase&& other) noexcept;
private:
	struct Impl;
	Impl* pImpl;
};

struct OrderBook : private OrderBookBase
{
	void Push()
	{
		OrderBookBase::Push();
	}

	void Pop()
	{
		OrderBookBase::Pop();
	}

	void Flush(std::ostream& out)
	{
		OrderBookBase::Flush(out);
	}
};
