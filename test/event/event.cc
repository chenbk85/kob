//
// Created by 李寅斌 on 2018/12/3.
//

#include <kob/event/event_loop.h>
#include <kob/event/event_loop_thread_pool.h>
#include <kob/ksl/this_thread.h>
#include <kob/log/logging.h>
#include <kob/profile/prof.h>
#include <iostream>

typedef std::function<void()> Mytask;

using namespace kob::event;
void myprint()
{
    LOG_INFO(" myprintf in thread id: {}", ksl::this_thread::id());
}
kob::event::event_loop *g_loop = nullptr;

static int cnt = 0;

void my_quit(ksl::timestamp spam)
{
    if(!g_loop) {
        LOG_ERROR("no loop");
        return;
    }
    cnt++;
    std::cout<<"timer called"<<std::endl;
    (void)spam;
    if(cnt >= 10) {
        g_loop->quit();
    }

}

int main()
{

    kob::event::event_loop* loop(new event_loop);
    g_loop = loop;
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
    delete loop;

    ksl::this_thread::sleep_for(10000);

}