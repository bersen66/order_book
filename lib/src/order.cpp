#include <orders/order.hpp>
#include <tuple>

OrderId GenerateID(const Order& o)
{
	static uint64_t id = 0;
	return id++;
}

std::ostream& operator<<(std::ostream& out, const Order& o)
{
	out << (o.type == Order::Type::BID ? "Bid" : "Ask") << " "
	    << o.price << " "
	    << o.amount;
	return out;
}

bool operator==(const Order& lhs, const Order& rhs)
{
	return std::tie(lhs.type, lhs.price, lhs.amount)
	       == std::tie(rhs.type, rhs.price, rhs.amount);
}