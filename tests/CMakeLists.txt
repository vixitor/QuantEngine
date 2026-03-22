add_executable(test_lfqueue test_lfqueue.cpp)
target_link_libraries(test_lfqueue PUBLIC lf_queue thread_utils macros)

add_executable(test_logger test_logger.cpp)
target_link_libraries(test_logger PUBLIC logger)

add_executable(test_mempool test_mempool.cpp)
target_link_libraries(test_mempool PUBLIC mem_pool)

add_executable(test_tcpserver test_tcpserver.cpp)
target_link_libraries(test_tcpserver PUBLIC tcp_server time_utils)

add_executable(test_thread test_thread.cpp)
target_link_libraries(test_thread PUBLIC thread_utils)
