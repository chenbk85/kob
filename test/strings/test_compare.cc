//
// Created by 李寅斌 on 2018/11/12.
//

#include <gtest/gtest.h>
#include <kob/profile/prof.h>
#include <kob/strings/compare.h>
#include <string>
#include <iostream>

TEST(strings, compare)
{
    std::string s1 = "abcDeF";
    std::string s2 = "abcdef";

    EXPECT_EQ(kob::strings::compare(s1, s2) != 0, true);
    EXPECT_EQ(kob::strings::compare_icase(s1, s2), 0);

}

TEST(strings, start_with)
{
    std::string s1 = "abcDeF";
    std::string s2 = "ABC";

    EXPECT_EQ(kob::strings::start_with(s1,s2), false);
    EXPECT_EQ(kob::strings::start_with_icase(s1,s2), true);
}

TEST(strings, end_with)
{
    std::string s1 = "abcDEF";
    std::string s2 = "def";

    EXPECT_EQ(kob::strings::end_with(s1,s2), false);
    EXPECT_EQ(kob::strings::end_with_icase(s1,s2), true);
}