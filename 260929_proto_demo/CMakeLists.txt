cmake_minimum_required(VERSION 3.10)
project(proto_demo)
set(CMAKE_CXX_STANDARD 11)
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
# =====新增这一行！=====
include_directories(${PROJECT_SOURCE_DIR}/data/proto)

add_executable(demo main_proto.cpp ${PROJECT_SOURCE_DIR}/data/proto/app_config.pb.cc)
target_link_libraries(demo ${Protobuf_LIBRARIES})
