//
// Created by 李寅斌 on 2018/11/15.
//

#include <gtest/gtest.h>
#include <kob/profile/prof.h>
#include <kob/ksl/type_traits.h>
#include <kob/ksl/path_view.h>
#include <string>
#include <iostream>

TEST(filesystem, path_view_1)
{
    ksl::filesystem::path_view p6("/tmp/abc/def/sdf/nofile");
    EXPECT_EQ(p6.raw(), "/tmp/abc/def/sdf/nofile");
    ksl::filesystem::path_view tmp;
    tmp = p6.sub_path(0);
    EXPECT_EQ(tmp.raw(), "/tmp");
    tmp = p6.sub_path(1);
    EXPECT_EQ(tmp.raw(), "/tmp/abc");
    tmp = p6.sub_path(2);
    EXPECT_EQ(tmp.raw(), "/tmp/abc/def");
    tmp = p6.sub_path(3);
    EXPECT_EQ(tmp.raw(), "/tmp/abc/def/sdf");
    tmp = p6.sub_path(4);
    EXPECT_EQ(tmp.raw(), "/tmp/abc/def/sdf/nofile");
    tmp = p6.sub_path(5);
    EXPECT_EQ(tmp.raw(), "/tmp/abc/def/sdf/nofile");

}

TEST(filesystem, path_view_2)
{
    ksl::filesystem::path_view p6("/tmp//abc//def/sdf/nofile");
    EXPECT_EQ(p6.raw(), "/tmp/abc/def/sdf/nofile");
    ksl::filesystem::path_view tmp;

    tmp = p6.sub_path(0);
    EXPECT_EQ(tmp.raw(), "/tmp");
    tmp = p6.sub_path(1);
    EXPECT_EQ(tmp.raw(), "/tmp/abc");
    tmp = p6.sub_path(2);
    EXPECT_EQ(tmp.raw(), "/tmp/abc/def");

    tmp = p6.sub_path(4);
    EXPECT_EQ(tmp.raw(), "/tmp/abc/def/sdf/nofile");

    tmp = p6.sub_path(3);
    EXPECT_EQ(tmp.raw(), "/tmp/abc/def/sdf");


    tmp = p6.sub_path(5);
    EXPECT_EQ(tmp.raw(), "/tmp/abc/def/sdf/nofile");

}

TEST(filesystem, path_view_assign)
{
    ksl::filesystem::path_view p6;
    p6.assign("/tmp//abc//def/sdf/nofile");
    EXPECT_EQ(p6.raw(), "/tmp/abc/def/sdf/nofile");
    ksl::filesystem::path_view tmp;

    tmp = p6.sub_path(0);
    EXPECT_EQ(tmp.raw(), "/tmp");
    tmp = p6.sub_path(1);
    EXPECT_EQ(tmp.raw(), "/tmp/abc");
    tmp = p6.sub_path(2);
    EXPECT_EQ(tmp.raw(), "/tmp/abc/def");

    tmp = p6.sub_path(4);
    EXPECT_EQ(tmp.raw(), "/tmp/abc/def/sdf/nofile");

    tmp = p6.sub_path(3);
    EXPECT_EQ(tmp.raw(), "/tmp/abc/def/sdf");


    tmp = p6.sub_path(5);
    EXPECT_EQ(tmp.raw(), "/tmp/abc/def/sdf/nofile");

}