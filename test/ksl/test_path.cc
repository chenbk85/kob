//
// Created by 李寅斌 on 2018/11/15.
//

#include <gtest/gtest.h>
#include <kob/profile/prof.h>
#include <kob/ksl/type_traits.h>
#include <kob/ksl/filesystem.h>
#include <string>
#include <iostream>

TEST(filesystem, path)
{

    ksl::filesystem::path p;
    EXPECT_EQ(p.is_absolute(), false);
    EXPECT_EQ(p.to_string(), "");

    ksl::filesystem::path p1("/tmp/nofile/");
    EXPECT_EQ(p1.is_absolute(), true);
    EXPECT_EQ(p1.to_string(), "/tmp/nofile/");
    EXPECT_EQ(p1.make_absolute().to_string(), "/tmp/nofile/");
    ksl::filesystem::path p11("abc/tmd");
    ksl::filesystem::path p12(p1, p11);
    EXPECT_EQ(p12.to_string(), "/tmp/nofile/abc/tmd");
    ksl::filesystem::path p13(p1, "tail");
    EXPECT_EQ(p13.to_string(), "/tmp/nofile/tail");
    ksl::filesystem::path phome(ksl::filesystem::path::home());

    ksl::filesystem::path pc(p1, phome);
    EXPECT_EQ(pc.to_string(), ksl::filesystem::path::home());


    std::cout<< ksl::filesystem::path::current()<<std::endl;
    std::cout<< ksl::filesystem::path::home()<<std::endl;

    ksl::filesystem::path p2(ksl::filesystem::path_type::pt_absolute);
    EXPECT_EQ(p2.is_absolute(), true);
    EXPECT_EQ(p2.is_file(), false);
    EXPECT_EQ(p2.to_string(), ksl::filesystem::path::current());


}