include_directories(include)

set(
    SOURCES
    src/order_book.cpp
    src/order.cpp
)

add_library(orders ${SOURCES})

target_include_directories(
    orders
    INTERFACE $<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include>
)

