add_executable(test_string_view test_string_view.cc)
target_link_libraries(test_string_view gtest gtest_main)
add_test(NAME test_string_view COMMAND test_string_view)

add_executable(test_clock test_clock.cc)
target_link_libraries(test_clock gtest gtest_main)
add_test(NAME test_clock COMMAND test_clock)

add_executable(test_thread test_thread.cc )
target_link_libraries(test_thread  kob_static gtest gtest_main )
#add_test(NAME test_thread COMMAND test_thread)

add_executable(test_path test_path.cc )
target_link_libraries(test_path  kob_static gtest gtest_main )
add_test(NAME test_path COMMAND test_path)

add_executable(test_path_view test_path_view.cc )
target_link_libraries(test_path_view  kob_static gtest gtest_main )
add_test(NAME test_path_view COMMAND test_path_view)

add_executable(test_fs test_fs.cc )
target_link_libraries(test_fs  kob_static gtest gtest_main )
#add_test(NAME test_fs COMMAND test_fs)

add_executable(test_this_thread test_this_thread.cc )
target_link_libraries(test_this_thread  kob_static gtest gtest_main )
add_test(NAME test_this_thread COMMAND test_this_thread)

add_executable(test_ratio test_ratio.cc )
target_link_libraries(test_ratio  kob_static gtest gtest_main )
add_test(NAME test_ratio COMMAND test_ratio)

add_executable(test_intrusive_list test_intrusive_list.cc )
target_link_libraries(test_intrusive_list  kob_static gtest gtest_main )
add_test(NAME test_intrusive_list COMMAND test_intrusive_list)

