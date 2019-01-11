//
// Created by 李寅斌 on 2018/11/13.
//

#include <gtest/gtest.h>
#include <kob/profile/prof.h>
#include <kob/strings/tokenizer.h>
#include <string>
#include <iostream>

TEST(string, tokenizer)
{
    std::string str = "/a/b /c/";
    std::string sep = "/";
    kob::tokenizer tk(str, sep);
    EXPECT_EQ(tk.count(), 5);
    EXPECT_EQ(tk[0], "");
    EXPECT_EQ(tk[1], "a");
    EXPECT_EQ(tk[2], "b ");
    EXPECT_EQ(tk[3], "c");
    EXPECT_EQ(tk[4], "");

    kob::tokenizer tk1(str, sep, kob::token_policy::tp_trim);
    EXPECT_EQ(tk1.count(), 5);
    EXPECT_EQ(tk1[0], "");
    EXPECT_EQ(tk1[1], "a");
    EXPECT_EQ(tk1[2], "b");
    EXPECT_EQ(tk1[3], "c");
    EXPECT_EQ(tk1[4], "");

    kob::tokenizer tk2(str, sep, kob::token_policy::tp_ignore_empty);
    EXPECT_EQ(tk2.count(), 3);
    EXPECT_EQ(tk2[0], "a");
    EXPECT_EQ(tk2[1], "b ");
    EXPECT_EQ(tk2[2], "c");

    kob::tokenizer tk3(str, sep, kob::token_policy::tp_ignore_and_trim);
    EXPECT_EQ(tk3.count(), 3);
    EXPECT_EQ(tk3[0], "a");
    EXPECT_EQ(tk3[1], "b");
    EXPECT_EQ(tk3[2], "c");

    std::string str1 = "/a/b /c/a/";
    kob::tokenizer tk4(str1, sep, kob::token_policy::tp_ignore_and_trim);
    EXPECT_EQ(tk4.count(), 4);
    EXPECT_EQ(tk4.count("a"), 2);
    EXPECT_EQ(tk4[0], "a");
    EXPECT_EQ(tk4[1], "b");
    EXPECT_EQ(tk4[2], "c");
    EXPECT_EQ(tk4[3], "a");
}