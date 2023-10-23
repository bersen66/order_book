#include <orders/order_book.hpp>
#include <iostream>

OrderBook Job() {
	OrderBook result;

	return result;
}

int main() {
	OrderBook ob = Job();
		ob.Push();
		ob.Pop();
		ob.Flush(std::cout);
	return 0;
}