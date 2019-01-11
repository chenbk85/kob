//
// Created by 李寅斌 on 2018/11/12.
//

#include <gtest/gtest.h>
#include <kob/profile/prof.h>
#include <kob/strings/double-conversion/numeric_string.h>
#include <kob/strings/numeric_parser.h>
#include <string>
#include <iostream>


TEST(string, numeric_string)
{
    double a = 1.23;
    std::string res;
    kob::strings::double_to_str(res, a);
    double r = kob::strings::str_to_double("1.23");
    std::string res1;
    kob::strings::double_to_str(res1, r);
    EXPECT_EQ(res, "1.23");
    EXPECT_EQ(res1, "1.23");

    std::string istr = "124";
    int iret;
    bool rc = kob::strings::parse_int(istr, iret);
    EXPECT_EQ(rc, true);
    EXPECT_EQ(iret, 124);

    std::string truestr = "true";
    std::string onstr = "on";

    bool bret;
    rc = kob::strings::parse_bool(truestr, bret);
    EXPECT_EQ(rc, true);
    EXPECT_EQ(bret, true);

    rc = kob::strings::parse_bool(onstr, bret);
    EXPECT_EQ(rc, true);
    EXPECT_EQ(bret, true);

    std::string falsestr = "false";
    rc = kob::strings::parse_bool(falsestr, bret);
    EXPECT_EQ(rc, true);
    EXPECT_EQ(bret, false);

    std::string offstr = "off";
    rc = kob::strings::parse_bool(offstr, bret);
    EXPECT_EQ(rc, true);
    EXPECT_EQ(bret, false);

    std::string offaastr = "offaa";
    rc = kob::strings::parse_bool(offaastr, bret);
    EXPECT_EQ(rc, false);
}