cmake_minimum_required(VERSION 3.25)
project(Server)

set(CMAKE_CXX_STANDARD 17)

find_package(nlohmann_json REQUIRED)

add_executable(Server main.cpp TwoSidesListener.cpp TwoSidesListener.h RequestManager.h RequestManager.cpp)

target_link_libraries(Server PRIVATE -lpqxx -lpq)
