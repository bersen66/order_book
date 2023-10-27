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

Сборка осуществлялась на ```g++``` в ```Release``` режиме.

## Для UnlimitedOrderBook (среднее время на 100'000 итераций):

В ходе замера каждая операция исполнялась 100'000 раз, и высчитывалось время на
каждой. Затем бралось среднее значение. Именно оно и является результатом.

Решение, использующее бинарное дерево и связный список показало очень хорошие
результаты.

```
UNLIMITED ORDER BOOK: (TASK-1)
Benchmarks:
Avg RandomOrder time: 57 nanoseconds        < 1 микросекунды
Avg bench::InsertBenchmark time: 305 nanoseconds  < 1 микросекунды 
Avg bench::UpdateBenchmark time: 279 nanoseconds  < 1 микросекунды
Avg bench::TopBenchmark time: 85 nanoseconds  < 1 микросекунды
Avg bench::EraseBenchmark time: 202 nanoseconds  < 1 микросекунды

Tests:
TestBasic<OrderBookType> OK. Test time: 3619764 ns.
TestTop<OrderBookType> OK. Test time: 33803 ns.
```

Каждая из операций исполняется меньше чем за половину микросекунду (10^(-6)),
согласно бенчмаркам. Стоит отметить, что в измеряемых функциях (```bench::```)
используется функция ```RandomOrder```, поэтому, результаты работы методов еще
выше.

Тесты исполняются дольше, так как в ходе исполнения тестов выполняются ASSERT-ы,
которые влияют на скорость исполнения. Мы не получаем чистое время работы
методов, так как мы замеряем время работы
всего теста.

## Для LimitedOrderBook (среднее время на 1000 итераций):

```
UNLIMITED ORDER BOOK: (TASK-3)
Benchmarks:
Avg RandomOrder time: 55 nanoseconds
Avg bench::InsertBenchmark time: 11977 nanoseconds
Avg bench::UpdateBenchmark time: 25901 nanoseconds
Avg bench::TopBenchmark time: 88 nanoseconds
Avg bench::EraseBenchmark time: 264 nanoseconds

Tests:
TestBasic<OrderBookType> OK. Test time: 50655 ns.
TestTop<OrderBookType> OK. Test time: 33563 ns.
```

В решении упоминалось, что подобный алгоритм не является самым эффективным.

При добавлении большого количества заявок, превосходящим лимит стакана, могут
начаться реаллокации, влияющие на производительность.
