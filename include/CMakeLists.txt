add_library(lf_queue INTERFACE)
target_include_directories(lf_queue INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_library(macros INTERFACE)
target_include_directories(macros INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_library(mem_pool INTERFACE)
target_link_libraries(mem_pool INTERFACE macros)
target_include_directories(mem_pool INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_library(thread_utils INTERFACE)

add_library(time_utils INTERFACE)

add_library(socket_utils STATIC socket_utils.h ../src/socket_utils.cpp)

add_library(logger STATIC logger.h ../src/logger.cpp)
target_link_libraries(logger PUBLIC lf_queue macros thread_utils time_utils)

add_library(tcp_socket STATIC tcp_socket.h ../src/tcp_socket.cpp)
target_link_libraries(tcp_socket PUBLIC logger socket_utils)

add_library(tcp_server STATIC tcp_server.h ../src/tcp_server.cpp)
target_link_libraries(tcp_server PUBLIC tcp_socket)

add_library(types INTERFACE)




