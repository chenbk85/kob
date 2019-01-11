//
// Created by 李寅斌 on 2018/12/6.
//

#ifndef KOB_TASK_FLOW_PHASE_TASK_H_
#define KOB_TASK_FLOW_PHASE_TASK_H_
#include <kob/ksl/detail/config.h>
#include <memory>
#include <functional>
#include <vector>
#include <string>
#include <list>

namespace kob {
namespace task_flow {

typedef size_t phase_type;
typedef size_t index_type;

class basic_phase_task;
typedef std::shared_ptr<basic_phase_task> phase_task_ptr;
typedef std::list<phase_task_ptr>         graph;
typedef std::shared_ptr<graph>            graph_ptr;

class phase_task_cycle;
typedef std::shared_ptr<phase_task_cycle> phase_task_cycle_ptr;

class basic_phase_task : public std::enable_shared_from_this<basic_phase_task> {
public:
    basic_phase_task() = default;
    virtual ~basic_phase_task() = default;

    void set_phase(phase_type st)
    {
        _phase = st;
    }

    phase_type get_phase() const
    {
        return _phase;
    }

    void set_index(index_type idx)
    {
        return _index;
    }
    index_type get_index() const
    {
        return _index;
    }

    void precede(const phase_task_ptr&);

    size_t num_successors() const;

    size_t num_dependents() const;
protected:
    std::string                   _name;
    phase_type                    _phase;
    index_type                    _index;
    std::vector<phase_task_ptr>   _successors;
    graph_ptr                     _sub_graph;
    phase_task_cycle_ptr          _cycle;

};

typedef std::shared_ptr<basic_phase_task> phase_task_ptr;

typedef int  result_type;

static const result_type kOk         = 0;
static const result_type kNextFactor = 1;
static const result_type kNextPhase  = 2;
static const result_type kTimeOut    = 3;
static const result_type kAgain      = -1;
static const result_type kError      = -2;

typedef std::function<result_type(void)>        phase_task_factor;
typedef  std::vector<phase_task_factor>         factor_list;
typedef  std::shared_ptr<factor_list>           factor_list_ptr;

} //namespace task_flow
} //namespace kob

#endif //KOB_TASK_FLOW_PHASE_TASK_H_
