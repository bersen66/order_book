# OrderBook

Библиотека реализует биржевой стакан.

Библиотека активно использует нововведения из C++17, поэтому требует поддержки
этого стандарта.

Подробное описание решения задачи и само решение
находится [здесь](https://github.com/bersen66/order_book/tree/main/lib).

Тесты и бенчмарки
находятся [тут](https://github.com/bersen66/order_book/tree/main/tests).

# Installation:

```CMake
# FETCHING PART
include(FetchContent)

FetchContent_Declare(
        orders
        GIT_REPOSITORY https://github.com/bersen66/order_book.git
        GIT_TAG v1.0.0
)

FetchContent_MakeAvailable(orders)

# AFTER FETCHING YOU HAVE TO LINK LIBRARY TO YOUR EXECUTABLE
add_executable(${PROJECT_NAME} main.cpp) # YOUR EXECUTABLE
target_link_libraries(${PROJECT_NAME} orders) # JUST LINK LIBRARY
```

# Замеры скорости:

Ниже приведены замеры скорости работы методов в наносекундах(10^(-9)).

## Правки по пункту 2:

Я реализовал еще одну версию класса биржевого стакана, которая не использует
OrderId.

[Объявление класса](https://github.com/bersen66/order_book/blob/main/lib/include/orders/unique_order_book.hpp)

[Реализация класса](https://github.com/bersen66/order_book/blob/main/lib/src/unique_order_book.cpp)

Замеры скорости(GNU 13.1.0):

```c++
UNIQUE ORDER BOOK:
Insertion time: 2375 ns
Update time: 2444 ns
Erase time: 1816 ns
Top time: 1886 ns
UOBTestBasic OK. Test time: 607061 ns.
UOBTestTop OK. Test time: 14737 ns.
```

Замеры скорости(Clang14):

```c++
UNIQUE ORDER BOOK:
Insertion time: 2864 ns
Update time: 1397 ns
Erase time: 1466 ns
Top time: 1956 ns
UOBTestBasic OK. Test time: 610552 ns.
UOBTestTop OK. Test time: 13479 ns.
```

Данная реализация показывает наибольшую производительность на моей машине. Это
связано с тем, что она не учитывает дублирующиеся заявки одного типа (так как не
использует ```OrderId```). Соответственно, имеет меньший оверхед, так как
выполняет меньше работы.

Я также реализовал версию класса, учитывающую дублирующиеся заявки одного типа.
Для этого был введен механизм идентификации ```OrderId```.

## Эффективная версия - UnlimitedOrderBook


[Объявление класса](https://github.com/bersen66/order_book/blob/main/lib/include/orders/unlimited_order_book.hpp)

[Реализация класса](https://github.com/bersen66/order_book/blob/main/lib/src/unlimited_order_book.cpp)


Для UnlimitedOrderBook (среднее время на 100'000 итераций):
В ходе замера каждая операция исполнялась 100'000 раз, и высчитывалось время на
каждой. Затем бралось среднее значение. Именно оно и является результатом.

Решение, использующее бинарное дерево и связный список показало очень хорошие
результаты.

Замеры скорости (Clang14):
```
UNLIMITED ORDER BOOK:
Benchmarks:
Avg RandomOrder time: 1677 nanoseconds
Avg bench::InsertBenchmark time: 2276 nanoseconds
Avg bench::UpdateBenchmark time: 2529 nanoseconds
Avg bench::TopBenchmark time: 1783 nanoseconds
Avg bench::EraseBenchmark time: 1917 nanoseconds

Tests:
TestBasic OK. Test time: 79200 ns.
TestTop OK. Test time: 38273 ns.
```

Замеры скорости (GNU 13.1.0):
```c++
UNLIMITED ORDER BOOK:
Benchmarks:
Avg RandomOrder time: 1703 nanoseconds
Avg bench::InsertBenchmark time: 2152 nanoseconds
Avg bench::UpdateBenchmark time: 2319 nanoseconds
Avg bench::TopBenchmark time: 1773 nanoseconds
Avg bench::EraseBenchmark time: 1984 nanoseconds

Tests:
TestBasic OK. Test time: 70121 ns.
TestTop OK. Test time: 32826 ns.
```

Стоит отметить, что в измеряемых функциях (```bench::```)
используется функция ```RandomOrder```, поэтому, результаты работы методов еще
выше.

Тесты исполняются дольше, так как в ходе исполнения тестов выполняются ASSERT-ы,
которые влияют на скорость исполнения. Мы не получаем чистое время работы
методов, так как мы замеряем время работы
всего теста.

## Менее эффективная реализация - LimitedOrderBook

LimitedOrderBook реализован на паре векторов. Этим обусловлена низкая скорость
изменяющих операций. Предполагается, что количество заявок не превысит
изначально заданные лимиты в конструкторе(т.е. не будет лишних реаллокаций).

Данная реализация более дружелюбна к кэшу процессора.

Для LimitedOrderBook (среднее время на 1000 итераций):

[Объявление класса](https://github.com/bersen66/order_book/blob/main/lib/include/orders/limited_order_book.hpp)

[Реализация класса](https://github.com/bersen66/order_book/blob/main/lib/src/limited_order_book.cpp)

Замеры скорости(Clang14):
```
LIMITED ORDER BOOK:
Benchmarks:
Avg RandomOrder time: 1807 nanoseconds
Avg bench::InsertBenchmark time: 8813 nanoseconds
Avg bench::UpdateBenchmark time: 16321 nanoseconds
Avg bench::TopBenchmark time: 1788 nanoseconds
Avg bench::EraseBenchmark time: 2090 nanoseconds

Tests:
TestBasic OK. Test time: 78641 ns.
TestTop OK. Test time: 44000 ns.
```

Замеры скорости (GNU 13.0.0):

```c++
LIMITED ORDER BOOK:
Benchmarks:
Avg RandomOrder time: 1837 nanoseconds
Avg bench::InsertBenchmark time: 11489 nanoseconds
Avg bench::UpdateBenchmark time: 23774 nanoseconds
Avg bench::TopBenchmark time: 1669 nanoseconds
Avg bench::EraseBenchmark time: 2009 nanoseconds

Tests:
TestBasic OK. Test time: 71448 ns.
TestTop OK. Test time: 45257 ns.
```

