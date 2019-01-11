//
// Created by 李寅斌 on 2018/11/6.
//

#include <gtest/gtest.h>
#include <kob/profile/prof.h>
#include <kob/ksl/type_traits.h>
#include <kob/ksl/string_view.h>
#include <string>

TEST(string_view, build)
{
    ksl::string_view sv("12345");
    EXPECT_EQ(sv.size(), 5);
}

TEST(string_view, build_string)
{
    std::string str("12345");
    ksl::string_view sv(str);
    EXPECT_EQ(sv.size(), 5);
}

TEST(string_view, build_cstring)
{
    const char* str = "12345";
    ksl::string_view sv(str);
    EXPECT_EQ(sv.size(), 5);
}

TEST(string_view, build_other)
{
    const char* str = "12345";
    ksl::string_view sv(str);
    ksl::string_view sv1(sv);
    EXPECT_EQ(sv.size(), sv1.size());
    EXPECT_EQ(sv.data(), sv1.data());
}

TEST(string_view, remove_prefix)
{
    const char* str = "12345";
    ksl::string_view sv(str);
    ksl::string_view sv1(sv);
    sv1.remove_prefix(5);
    EXPECT_EQ(sv1.size(), sv.size() - 5);
}

TEST(string_view, rfind_sv)
{
    const char* str = "12345";
    ksl::string_view sv(str);
    ksl::string_view sv1("23");
    ksl::string_view::size_type n = sv.rfind(sv1);
    EXPECT_EQ(n, 1);
}

TEST(string_view, rfind_cstring)
{

    ksl::string_view sv("12345");
    const char* f = "23";
    ksl::string_view::size_type n = sv.rfind(f);
    EXPECT_EQ(n, 1);
}

TEST(string_view, rfind_cstring_pos)
{

ksl::string_view sv("12345");
const char* f = "23";
ksl::string_view::size_type n = sv.rfind(f, 4, 2);
EXPECT_EQ(n, 1);
}

TEST(string_view, equal)
{

ksl::string_view sv = "12345";
EXPECT_EQ(sv == "12345", true);


}