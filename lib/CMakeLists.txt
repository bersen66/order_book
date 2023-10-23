include_directories(include)

set(
    SOURCES
    src/order_book.cpp
)

add_library(orderbook ${SOURCES})

target_include_directories(
    orderbook
    INTERFACE $<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include>
)

