#pragma once

#include <cstdint>
#include <ostream>


// Мы не можем использовать тип float или double.
// Это связано с неточностями и потерями, обусловленными
// представлением чисел с плавающей точкой (IEEE-754) в памяти компьютера.
// Для вычисления стоимостей, переводим значения в валюте в центы.

using Currency = std::uint64_t; // in cents

using Amount = std::uint64_t;

struct Order {
	enum class Type {
		ASK = 0,
		BID = 1,
	};
	Type type;
	Currency price;
	Amount amount;
};

using OrderId = uint64_t;

// TODO: Better id generation.
OrderId GenerateID(const Order& o);

std::ostream& operator<<(std::ostream& out, const Order& o);

bool operator==(const Order& lhs, const Order& rhs);