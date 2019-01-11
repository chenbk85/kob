//
// Created by 李寅斌 on 2018/11/23.
//

#include <kob/event/timer_base.h>

namespace kob {
namespace event {


    std::atomic<uint64_t> timer_task::g_timer_seq;

} //namespace event
} //namespace kob