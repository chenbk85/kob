//
// Created by 李寅斌 on 2018/11/29.
//

#include <gtest/gtest.h>
#include <kob/profile/prof.h>
#include <kob/ksl/this_thread.h>
#include <string>
#include <iostream>

TEST(ksl, this_thread)
{
   bool im = ksl::this_thread::is_main_thread();
   EXPECT_EQ(im, true);
   EXPECT_EQ(ksl::this_thread::thread_number(), 1);
}