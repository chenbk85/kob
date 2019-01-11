//
// Created by 李寅斌 on 2018/11/29.
//

#include <kob/event/event_loop.h>
#include <kob/event/event_loop_thread_pool.h>
#include <kob/ksl/this_thread.h>
#include <kob/log/logging.h>
#include <gtest/gtest.h>
#include <kob/profile/prof.h>
#include <iostream>

using namespace kob::event;
__thread int cnt_call = 0;
void myprint_multi()
{
    std::string n(ksl::thread::current()->get_name().data(),
                  ksl::thread::current()->get_name().size());
    LOG_INFO(" myprintf in thread : {} id: {}, cnt :{}", ksl::this_thread::id(), n, cnt_call++);
}

void myprint()
{
    LOG_INFO(" myprintf in thread id: {}, cnt :{}", ksl::this_thread::id(), cnt_call++);
}

kob::event::event_loop *g_loop = nullptr;

static int cnt = 0;

void my_quit(ksl::timestamp spam)
{
    cnt++;
    std::cout<<"timer called "<<cnt<<std::endl;
    (void)spam;
    if(cnt >= 10) {
        g_loop->quit();
    }

}

typedef std::function<void()> Mytask;

TEST(event_loop, run_task)
{
    kob::event::event_loop_ptr loop(new event_loop);
    g_loop = loop.get();
    bool r = loop->init();
    if(!r) {
        std::cout<<"init error"<<std::endl;
    }
    Mytask t = std::bind(&myprint);

    task_ptr tpr= make_task<Mytask>(std::move(t));
    task_ptr tpr2 = tpr;
    loop->queue_in_loop(std::move(tpr2));
    loop->run_task(std::move(tpr));

    kob::event::timer_callback timer1 = std::bind(&my_quit, std::placeholders::_1);

    loop->run_every(ksl::timespan(100000), timer1);

    LOG_INFO("start loop");
    loop->loop();
    loop->join();
    g_loop = nullptr ;
    loop.reset();

    kob::event::event_loop_thread_pool etp;
    etp.set_thread_num(10);
    etp.set_name("etp");
    etp.start();
    for(size_t i = 0 ; i < 100 ; i++) {
        event_loop_ptr ptr = etp.get_rr();
        if(!ptr) {
            KOB_ASSERT_MSG(false, "no loop ptr");
        }
        Mytask mt = std::bind(&myprint_multi);
        task_ptr tkp= make_task<Mytask>(std::move(mt));
        ptr->run_task(std::move(tkp));
    }


    LOG_TRACE("test end start");
    etp.stop();
    LOG_TRACE("test end");


}