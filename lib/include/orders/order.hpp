#pragma once
#include <cstdint>


// Мы не можем использовать тип float или double.
// Это связано с неточностями и потерями, обусловленными
// представлением чисел с плавающей точкой (IEEE-754) в памяти компьютера.
// Для вычисления стоимостей, переводим значения в валюте в центы.

using Currency = std::uint64_t; // in cents

using Amount = std::uint64_t;

struct Order {
	enum class Type {
		BID,
		ASK,
	};
	Type type;
	Currency price;
	Amount amount;
};