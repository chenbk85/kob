//
// Created by 李寅斌 on 2018/12/2.
//

#include <gtest/gtest.h>
#include <kob/profile/prof.h>
#include <kob/ksl/ratio.h>
#include <string>
#include <iostream>

TEST(ksl, ratio)
{
    ksl::ratio<3600> r;
    EXPECT_EQ(r.num, 3600);
    EXPECT_EQ(r.den, 1);
}