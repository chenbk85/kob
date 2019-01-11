//
// Created by 李寅斌 on 2018/11/20.
//

#include <gtest/gtest.h>
#include <kob/profile/prof.h>
#include <kob/ksl/type_traits.h>
#include <kob/ksl/filesystem.h>
#include <string>
#include <iostream>


TEST(filesystem, create_dir)
{
    ksl::filesystem::path_view pBin1("mytest");
    EXPECT_EQ(ksl::filesystem::exists(pBin1), false);
    EXPECT_EQ(ksl::filesystem::create_directory(pBin1),true);
    EXPECT_EQ(ksl::filesystem::exists(pBin1), true);


    ksl::filesystem::path_view pBin2("pBin2/abc/cdm");
    EXPECT_EQ(ksl::filesystem::exists(pBin2), false);
    EXPECT_EQ(ksl::filesystem::create_directories(pBin2),true);
    EXPECT_EQ(ksl::filesystem::exists(pBin2), true);

}