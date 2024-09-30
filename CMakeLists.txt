cmake_minimum_required(VERSION 3.10)
project(GraphProject)

set(CMAKE_CXX_STANDARD 14)

# Añade el ejecutable principal
add_executable(GraphMain
    src/main.cpp
    src/graph.cpp
)

file(COPY ${CMAKE_SOURCE_DIR}/graphs DESTINATION ${CMAKE_BINARY_DIR})
message(STATUS "Copiando carpeta graphs de ${CMAKE_SOURCE_DIR}/graphs a ${CMAKE_BINARY_DIR}/graphs")
# Incluye el directorio src para los headers
target_include_directories(GraphMain PRIVATE src)

# Configuración para Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

# Añade el ejecutable de pruebas
add_executable(GraphTest
    test/graph_test.cpp
    src/graph.cpp
)

# Incluye el directorio src para los headers en GraphTest
target_include_directories(GraphTest PRIVATE src)

target_link_libraries(GraphTest gtest_main)

include(GoogleTest)
gtest_discover_tests(GraphTest)