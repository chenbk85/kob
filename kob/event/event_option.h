//
// Created by 李寅斌 on 2018/11/28.
//

#ifndef KOB_EVENT_EVENT_OPTION_H_
#define KOB_EVENT_EVENT_OPTION_H_
#include <kob/profile/prof.h>
#include <kob/event/event_base.h>
#include <memory>
#include <cstdint>

namespace kob {
namespace event {


struct event_option {
    int                poll_time_ms;
    bool               pri_local_task;
};

typedef std::shared_ptr<event_option> event_option_ptr;

KOB_FORCE_INLINE event_option_ptr defualt_option()
{
    event_option_ptr ptr(new event_option());
    ptr->poll_time_ms = 500;
    ptr->pri_local_task = true;
    return ptr;
}

} //namespace event
} //namespace kob
#endif //KOB_EVENT_EVENT_OPTION_H_
