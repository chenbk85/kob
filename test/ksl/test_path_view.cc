//
// Created by 李寅斌 on 2018/11/15.
//

#include <gtest/gtest.h>
#include <kob/profile/prof.h>
#include <kob/ksl/type_traits.h>
#include <kob/ksl/path_view.h>
#include <string>
#include <iostream>

TEST(filesystem, path_view)
{

    ksl::filesystem::path_view p1("/tmp/nofile");
    EXPECT_EQ(p1.build(), "/tmp/nofile");
    EXPECT_EQ(p1[0], "tmp");

    ksl::filesystem::path_view p2("/tmp//nofile");
    EXPECT_EQ(p2.build(), "/tmp/nofile");

    ksl::filesystem::path_view p3("///tmp/nofile");
    EXPECT_EQ(p3.build(), "/tmp/nofile");

    ksl::filesystem::path_view p4("/tmp//nofile/");
    EXPECT_EQ(p4.build(), "/tmp/nofile");

    ksl::filesystem::path_view p5("/tmp/abc/def/nofile");
    EXPECT_EQ(p5.sub_path(0), "/tmp");
    EXPECT_EQ(p5.sub_path(1), "/tmp/abc");
    EXPECT_EQ(p5.sub_path(2), "/tmp/abc/def");
    EXPECT_EQ(p5.sub_path(3), "/tmp/abc/def/nofile");
    EXPECT_EQ(p5.sub_path(4), "/tmp/abc/def/nofile");

    ksl::filesystem::path_view p6("/tmp//abc/def/nofile");
    EXPECT_EQ(p6.sub_path(0), "/tmp");
    EXPECT_EQ(p6.sub_path(1), "/tmp/abc");
    EXPECT_EQ(p6.sub_path(2), "/tmp/abc/def");
    EXPECT_EQ(p6.sub_path(3), "/tmp/abc/def/nofile");
    EXPECT_EQ(p6.sub_path(4), "/tmp/abc/def/nofile");

}