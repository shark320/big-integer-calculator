cmake_minimum_required(VERSION 3.5)

project(calc LANGUAGES C)
set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic -ansi")

add_executable(calc 
    src/calc.c 
    
    src/long_arithmetic/long_arithmetic.c
    src/utils/futils/futils.c
    src/utils/vector/vector.c
    src/utils/cstring/cstring.c
    src/utils/expression/expression.c
    src/calculator/calculator.c
)

target_link_libraries(calc m)
