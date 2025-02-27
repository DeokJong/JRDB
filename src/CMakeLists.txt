# 프로젝트 내 모든 cpp 파일을 한 번에 찾아 추가
file(GLOB_RECURSE ALL_CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# 전체 라이브러리를 하나로 통합 (필요시 개별적으로 분리 가능)
add_library(jrdb_lib ${ALL_CPP_SOURCES})

# include 디렉터리를 자동으로 추가
target_include_directories(jrdb_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
