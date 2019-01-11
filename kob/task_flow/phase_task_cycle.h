//
// Created by 李寅斌 on 2018/12/6.
//

#ifndef KOB_TASK_FLOW_PHASE_TASK_CYCLE_H_
#define KOB_TASK_FLOW_PHASE_TASK_CYCLE_H_
#include <kob/event/task.h>
#include <kob/task_flow/phase_task.h>
#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

namespace kob {
namespace task_flow{

namespace detail {
    KOB_FORCE_INLINE result_type phase_finish()
    {
        return kNextPhase;
    }


    KOB_FORCE_INLINE result_type cycle_finish()
    {
        return kOk;
    }
}
class phase_task_cycle {
public:
    explicit phase_task_cycle(const std::string &cyclename, size_t maxState);

    bool set_phase(size_t index, const phase_task_factor &ptf);

    const std::string name() const;

    void finish_phase_setting(size_t ph);

    void finish_cycle_setting(size_t ph);

    result_type run(basic_phase_task*);

private:
    result_type run_one(basic_phase_task*);
private:
    size_t                                                _max_phase;
    std::string                                           _name;
    std::vector<factor_list_ptr>                          _handler;
};

KOB_FORCE_INLINE
phase_task_cycle::phase_task_cycle(const std::string &cycleName, size_t maxState)
: _max_phase(maxState),
  _name(cycleName),
  _handler(maxState)
{

}

bool phase_task_cycle::set_phase(size_t index, const phase_task_factor &ptf)
{
   KOB_ASSERT_MSG(index < _handler.size(), "overflow");
   if (!_handler[index]) {
       _handler[index].reset(new factor_list());
   }
   _handler[index]->push_back(ptf)
}

const std::string phase_task_cycle::name() const
{
    return _name;
}

void phase_task_cycle::finish_phase_setting(size_t ph)
{
    if (!_handler[index]) {
        _handler[index].reset(new factor_list());
    }
    _handler[index]->push_back(std::bind(&detail::phase_finish));
}

void phase_task_cycle::finish_cycle_setting(size_t ph)
{
    KOB_ASSERT_MSG(_handler.size() = _max_phase, "must be same");
    _handler.push_back(factor_list_ptr(new factor_list()));
    _handler[_max_phase]->push_back(std::bind(&detail::cycle_finish));
}

} //namespace task_flow
} //namespace kob
#endif //KOB_TASK_FLOW_PHASE_TASK_CYCLE_H_
