cmake_minimum_required(VERSION 3.10)
project(JRDB)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 모든 cpp 파일을 자동으로 찾음
file(GLOB_RECURSE ALL_CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# 라이브러리 생성 (기존 'jrdb' → 'jrdb_lib' 로 변경)
add_library(jrdb_lib ${ALL_CPP_SOURCES})
target_include_directories(jrdb_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# 실행 파일 생성
add_executable(jrdb src/main.cpp)  # main.cpp가 실행 파일의 진입점
target_link_libraries(jrdb PRIVATE jrdb_lib)

# # =======================
# # ✅ GoogleTest 설정 추가
# # =======================

# include(FetchContent)

# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
# )
# FetchContent_MakeAvailable(googletest)

# enable_testing()

# # =======================
# # ✅ 테스트 실행 파일 추가
# # =======================

# # test_storage
# add_executable(test_storage tests/test_storage.cpp)
# target_link_libraries(test_storage PRIVATE gtest gtest_main jrdb_lib)
# add_test(NAME StorageTest COMMAND test_storage)

# # test_query
# add_executable(test_query tests/test_query.cpp)
# target_link_libraries(test_query PRIVATE gtest gtest_main jrdb_lib)
# add_test(NAME QueryTest COMMAND test_query)

# # test_transaction
# add_executable(test_transaction tests/test_transaction.cpp)
# target_link_libraries(test_transaction PRIVATE gtest gtest_main jrdb_lib)
# add_test(NAME TransactionTest COMMAND test_transaction)

# # test_network
# add_executable(test_network tests/test_network.cpp)
# target_link_libraries(test_network PRIVATE gtest gtest_main jrdb_lib)
# add_test(NAME NetworkTest COMMAND test_network)
