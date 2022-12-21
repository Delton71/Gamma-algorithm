set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address,undefined -Wall -Wextra -pedantic -Werror -Wno-self-assign-overloaded -Wno-missing-braces")
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

add_executable(test_gamma test_gamma.cpp gamma.cpp)
add_test(NAME test_gamma COMMAND test_gamma)
