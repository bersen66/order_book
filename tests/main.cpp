#include <orders/order_book.hpp>
#include <iostream>


OrderBook Job()
{
	OrderBook result;

	return result;
}

int main()
{
	OrderBook ob = Job();

	ob.Insert(Order{.type=Order::Type::ASK, .price=120, .amount=10});
	ob.Insert(Order{.type=Order::Type::ASK, .price=1200, .amount=10});
	ob.Insert(Order{.type=Order::Type::ASK, .price=1201, .amount=10});
	auto id = ob.Insert(Order{.type=Order::Type::BID, .price=1200, .amount=10});

	ob.Erase(id);
	//ob.Update(id, Order{.type=Order::Type::ASK, .price=12032130, .amount=12321} );
	for (const auto& [price, list]: ob.Asks())
	{
		for (const auto& bid: list)
		{
			std::cout << bid << std::endl;
		}
		std::cout << std::endl;
	}

	return 0;
}