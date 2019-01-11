//
// Created by 李寅斌 on 2018/11/29.
//

#include <kob/event/event.h>
#include <gtest/gtest.h>
#include <kob/profile/prof.h>
#include <iostream>

using namespace kob::event;

TEST(event, event)
{
    poll_event pe = poll_event::none;
    pe |= poll_event::read;
    EXPECT_EQ(static_cast<int>(pe),0x001);
    EXPECT_EQ(is_event_read(pe), true);
    EXPECT_EQ(is_event_read(poll_event::read), true);
}