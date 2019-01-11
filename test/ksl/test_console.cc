//
// Created by 李寅斌 on 2018/11/7.
//

#include <gtest/gtest.h>
#include <kob/profile/prof.h>
#include <kob/ksl/console.h>
#include <iostream>

TEST(console, case0)
{
    std::cout<<"hello\n";
    std::cout<<ksl::console::TextBlue<<"abc"<<ksl::console::TextDefault<<"\r\n";
}