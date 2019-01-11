#include <cstdlib> // EXIT_FAILURE
#include <iostream>
#include <memory>
#include <kob/log/fmtlog.h>


void async_example();
void syslog_example();

int main(int, char*[])
{
    try
    {
        // Multithreaded color console
        kob::log::logger_ptr console = kob::log::stdout_logger_mt("console", true);
        console->info("Welcome to fmtlog!");
        console->error("An info message example {}..", 1);

        // Formatting examples
        console->warn("Easy padding in numbers like {:08d}", 12);
        console->critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
        console->info("Support for floats {:03.2f}", 1.23456);
        console->info("Positional args are {1} {0}..", "too", "supported");

        console->info("{:<30}", "left aligned");
        console->info("{:>30}", "right aligned");
        console->info("{:^30}", "centered");

        kob::log::get("console")->info("loggers can be retrieved from a global registry using the fmtlog::get(logger_name) function");

        // Runtime log levels
        kob::log::set_level(kob::log::log_level::ll_info); //Set global log level to info
        console->debug("This message shold not be displayed!");
        console->set_level(kob::log::log_level::ll_debug); // Set specific logger's log level
        console->debug("This message shold be displayed..");

        // Create basic file logger (not rotated)
        kob::log::logger_ptr my_logger = kob::log::basic_logger_mt("basic_logger", "logs/basic.txt");
        my_logger->info("Some log message");


        // Create a file rotating logger with 5mb size max and 3 rotated files
        kob::log::logger_ptr rotating_logger = kob::log::rotating_logger_mt("some_logger_name", "logs/mylogfile", 1048576 * 5, 3);
        for (int i = 0; i < 10; ++i)
            rotating_logger->info("{} * {} equals {:>10}", i, i, i*i);

        // Create a daily logger - a new file is created every day on 2:30am
        kob::log::logger_ptr daily_logger = kob::log::daily_logger_mt("daily_logger", "logs/daily", 2, 30);
        daily_logger->info(123.44);

        // Customize msg format for all messages
        kob::log::set_pattern("*** [%H:%M:%S %z] [thread %t] %v ***");
        rotating_logger->info("This is another message with custom format");


        // Compile time debug or trace macros.
        // Enabled #ifdef fmtlog_DEBUG_ON or #ifdef fmtlog_TRACE_ON
        FMTLOG_TRACE(console, "Enabled only #ifdef fmtlog_TRACE_ON..{} ,{}", 1, 3.23);
        FMTLOG_DEBUG(console, "Enabled only #ifdef fmtlog_DEBUG_ON.. {} ,{}", 1, 3.23);

        // Asynchronous logging is very fast..
        // Just call fmtlog::set_async_mode(q_size) and all created loggers from now on will be asynchronous..
        async_example();

        // Release and close all loggers
        kob::log::drop_all();
    }

    catch (const kob::log::log_ex& ex)
    {
        std::cout << "Log failed: " << ex.what() << std::endl;
        return -1;
    }
    return 0;
}


void async_example()
{
    size_t q_size = 4096; //queue size must be power of 2
    kob::log::set_async_mode(q_size);
    kob::log::logger_ptr async_file = kob::log::daily_logger_st("async_file_logger", "logs/async_log.txt");
    for (int i = 0; i < 100; ++i)
        async_file->info("Async message #{}", i);
}


