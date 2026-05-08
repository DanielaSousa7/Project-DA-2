cmake_minimum_required(VERSION 4.1)
project(Project_DA_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(Project_DA_2 main.cpp
        Parser.h Parser.cpp
        Batch.h Batch.cpp
        Menu.h Menu.cpp
        structs.h
        WebBuilder.h
        WebBuilder.cpp
        algorithms/basic/Basic.h
        algorithms/basic/Basic.cpp
        algorithms/spilling/spilling.h
        "algorithms/spilling/Spilling .cpp"
        algorithms/splitting/Splitting.h
        algorithms/splitting/Splitting.cpp
        algorithms/free/Free.h
        algorithms/free/Free.cpp
        Allocation.h
        Allocation.cpp)
