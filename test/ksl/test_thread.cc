//
// Created by 李寅斌 on 2018/11/9.
//

#include <gtest/gtest.h>
#include <kob/profile/prof.h>
#include <kob/ksl/thread.h>
#include <kob/ksl/this_thread.h>
#include <kob/ksl/timespan.h>
#include <iostream>

class worker {
public:
    worker() : run(true) {}
    void runner()
    {
        std::cout<<__FUNCTION__<<" in"<<std::endl;
        while (run) {
            std::cout<<"abc"<<std::endl;
            std::cout<<std::string(ksl::thread::current()->get_name().data(),
                                   ksl::thread::current()->get_name().size())<<std::endl;
            ksl::this_thread::sleep_for(ksl::timespan(1000000));
        }
        std::cout<<__FUNCTION__<<" out"<<std::endl;
    }

    void stop()
    {
        run = false;
    }
    bool run;
};

TEST(thread, create)
{
    worker*  w = new worker();
    ksl::thread t;
    t.set_name("kob");
    t.start(std::bind(&worker::runner, w));
    EXPECT_EQ(ksl::this_thread::thread_number(), 2);
    int i = 0;
    while(i++ < 10) {
        ksl::this_thread::sleep_for(ksl::timespan(1000000));
        char buf[128];
        sprintf(buf, "k%d", i);
        t.set_name(buf);
    }
    w->stop();
    t.join();
    EXPECT_EQ(t.get_context()->started, true);
    EXPECT_EQ(t.get_context()->joined, true);
    delete w;
}