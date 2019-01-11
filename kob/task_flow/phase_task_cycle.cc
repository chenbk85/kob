//
// Created by 李寅斌 on 2018/12/6.
//

#include <kob/task_flow/phase_task_cycle.h>

namespace kob {
namespace task_flow {

result_type  phase_task_cycle::run_one(basic_phase_task *task)
{
    result_type rc = kAgain;
    index_type index = task->get_index();
    state_type phase = task->get_phase();

    if(!_handler[phase]) {
        task->set_phase(phase +1);
        task->set_index(0);
    }
    rc = (*_handler[phase])[index]();

    if (rc == kNextFactor) {
        task->set_index(index + 1);
        return kAgain;
    }

    if(rc == kNextPhase) {
        task->set_phase(phase + 1);
        task->set_index(0UL);
        return kAgain;
    }

    //kError or kOk
    return rc;
}

result_type  phase_task_cycle::run(basic_phase_task *task)
{
    result_type rc;
    do {
        rc = run_one(task);
    }while (rc == kAgain);

    return rc;
}

} //namespace task_flow
} //namespace kob