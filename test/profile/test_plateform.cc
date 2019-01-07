//
// Created by 李寅斌 on 2018/11/6.
//

#include <gtest/gtest.h>
#include <kob/profile/prof.h>

TEST(platform, os)
{
#if defined(KOB_COMPILER_CLANG)
    EXPECT_EQ(KOB_COMPILER_CLANG, 1);
    EXPECT_EQ(KOB_COMPILER_CPP11_ENABLED, 1);
#endif

#if defined(KOB_PLATFORM_OSX)
    EXPECT_EQ(KOB_PLATFORM_NAME, "osx");
#elif defined(KOB_PLATFORM_LINUX)
    EXPECT_EQ(KOB_PLATFORM_NAME, "linux");
#elif defined(KOB_PLATFORM_MINGW)
    EXPECT_EQ(KOB_PLATFORM_NAME, "mingw");
#else
    #error "unknown platform"
#endif
}