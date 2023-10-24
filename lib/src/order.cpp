#include <orders/order.hpp>



OrderId GenerateID(const Order& o) {
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
