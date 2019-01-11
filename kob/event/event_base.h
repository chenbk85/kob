//
// Created by 李寅斌 on 2018/11/24.
//

#ifndef KOB_EVENT_EVENT_BASE_H_
#define KOB_EVENT_EVENT_BASE_H_
#include <functional>
#include <kob/ksl/timestamp.h>
#include <vector>

namespace kob {
namespace event {

    typedef std::function<void(ksl::timestamp)> event_callback;

    class channel;
    typedef std::vector<channel*> channel_list;


} //namespace event
} //namespace kob

#endif //KOB_EVENT_EVENT_BASE_H_
