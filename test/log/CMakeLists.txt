add_executable(log_example log_example.cc)

target_link_libraries(log_example
  kob_static
  pthread
  rt 
  )
