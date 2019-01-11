//
// Created by 李寅斌 on 2018/11/7.
//


#include <gtest/gtest.h>
#include <kob/profile/prof.h>
#include <kob/ksl/clock.h>
#include <string>
#include <iostream>

TEST(clock, case1)
{
    ksl::real_time_clock rtc;
    std::cout<<rtc.micoseconds()<<std::endl;
}