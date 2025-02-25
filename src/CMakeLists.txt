add_library(storage 
    storage/StorageEngine.cpp 
    storage/tableSchema/TableSchema.cpp
)

add_library(query query/SQLParser.cpp)
target_link_libraries(query PUBLIC storage)

add_library(transaction transaction/TransactionManager.cpp)
target_link_libraries(transaction PUBLIC storage)

add_library(index index/BTree.cpp)
target_link_libraries(index PUBLIC storage)

add_library(network network/Protocol.cpp network/ServerConnector.cpp)
target_link_libraries(network PUBLIC storage)

target_include_directories(storage PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/storage)

target_include_directories(query PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/query ${CMAKE_CURRENT_SOURCE_DIR}/storage)
target_include_directories(transaction PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/transaction ${CMAKE_CURRENT_SOURCE_DIR}/storage)
target_include_directories(index PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/index ${CMAKE_CURRENT_SOURCE_DIR}/storage)
target_include_directories(network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/network ${CMAKE_CURRENT_SOURCE_DIR}/storage)
