cmake_minimum_required(VERSION 3.8)
project(OrderBook)

set(CMAKE_CXX_STANDARD 11)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lib)

# Если папка с проектом не является подпапкой другого проекта
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    # Добавляем юнит-тесты
    add_subdirectory(tests)
endif()