#include <orders/order_book.hpp>
#include <iostream>

struct OrderBookBase::Impl
{
	void Push()
	{
		std::cout << "Push" << std::endl;
	}

	void Pop()
	{
		std::cout << "Pop" << std::endl;
	}

	void Flush(std::ostream& out)
	{
		out << "Amogus" << std::endl;
	}
};

OrderBookBase::OrderBookBase()
		: pImpl(new Impl)
{}

OrderBookBase::~OrderBookBase()
{
	delete pImpl;
}

void OrderBookBase::Push()
{
	pImpl->Push();
}

void OrderBookBase::Pop()
{
	pImpl->Pop();
}

void OrderBookBase::Flush(std::ostream& out) const
{
	pImpl->Flush(out);
}

OrderBookBase::OrderBookBase(OrderBookBase&& other)
	: pImpl(other.pImpl)
{
	other.pImpl = nullptr;
}


