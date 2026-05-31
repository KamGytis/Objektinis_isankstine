cmake_minimum_required(VERSION 3.20)
project(Asociatyvus_Konteineriai)
 
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

configure_file(${CMAKE_SOURCE_DIR}/tekstas.txt ${CMAKE_BINARY_DIR}/tekstas.txt COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/url_end.txt ${CMAKE_BINARY_DIR}/url_end.txt COPYONLY)
 
add_executable(Asociatyvus_Konteineriai
    main.cpp
    cleanword.cpp
    wordfrequency.cpp
    crossreference.cpp
    urls.cpp
)
 
target_include_directories(Asociatyvus_Konteineriai PRIVATE ${CMAKE_SOURCE_DIR})