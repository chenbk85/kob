add_executable(test_event_loop test_event_loop.cc )
target_link_libraries(test_event_loop  kob_static gtest gtest_main )

add_executable(test_event test_event.cc )
target_link_libraries(test_event  kob_static gtest gtest_main )


add_executable(event event.cc )
target_link_libraries(event  kob_static)