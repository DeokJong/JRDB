add_library(storage storage/StorageEngine.cpp "storage/StorageEngine.cpp" "storage/TableSchema.h")
add_library(query query/SQLParser.cpp "storage/StorageEngine.cpp" "storage/TableSchema.h")
add_library(transaction transaction/TransactionManager.cpp "storage/StorageEngine.cpp" "storage/TableSchema.h")
add_library(index index/BTree.cpp "storage/StorageEngine.cpp" "storage/TableSchema.h")
add_library(network network/Protocol.cpp network/ServerConnector.cpp "storage/StorageEngine.cpp" "storage/TableSchema.h")

target_include_directories(storage PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/storage)
target_include_directories(query PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/query)
target_include_directories(transaction PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/transaction)
target_include_directories(index PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/index)
target_include_directories(network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/network)
