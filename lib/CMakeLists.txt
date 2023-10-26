include_directories(include)

set(
    SOURCES
    src/order.cpp
    src/unlimited_order_book.cpp
    src/limited_order_book.cpp
)

add_library(orders ${SOURCES})

target_include_directories(
    orders
    INTERFACE $<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include>
)

