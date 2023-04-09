cmake_minimum_required(VERSION 3.25)
project(SimpleServer)

set(CMAKE_CXX_STANDARD 17)

find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(SimpleServer main.cpp TwoSidesListener.cpp TwoSidesListener.h)

target_link_libraries(SimpleServer PRIVATE -lpqxx -lpq)