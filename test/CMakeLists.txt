include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# 테스트 실행 파일 추가
add_executable(test_storage test_storage.cpp)
target_link_libraries(test_storage PRIVATE gtest gtest_main storage)

# add_executable(test_query test_query.cpp)
# target_link_libraries(test_query query)

# add_executable(test_transaction test_transaction.cpp)
# target_link_libraries(test_transaction transaction)

# add_executable(test_network test_network.cpp)
# target_link_libraries(test_network network)

add_test(NAME StorageTest COMMAND test_storage)
# add_test(NAME QueryTest COMMAND test_query)
# add_test(NAME TransactionTest COMMAND test_transaction)
# add_test(NAME NetworkTest COMMAND test_network)
