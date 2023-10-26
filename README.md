# OrderBook

Библиотека реализует биржевой стакан.

Библиотека активно использует нововведения из C++17, поэтому требует поддержки
этого стандарта.

Подробное описание решения задачи и само решение
находится [здесь](https://github.com/bersen66/order_book/tree/develop/lib).

Тесты и бенчмарки
находятся [тут](https://github.com/bersen66/order_book/tree/develop/tests).

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