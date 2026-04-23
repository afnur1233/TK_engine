#ifndef __TK_SCHEDULER_H__
#define __TK_SCHEDULER_H__
#include <TK/TK_stdint.h>
#include <TK/TK_macro.h>

typedef TK_f64                      TK_Time;
typedef struct TK_TaskScheduler     TK_TaskScheduler;
typedef struct TK_Task              TK_Task;
typedef struct TK_TaskResult        TK_TaskResult;
typedef struct TK_SchedulingOptions TK_SchedulingOptions;

typedef
    enum TK_TaskResultType TK_TaskResultType;
    enum TK_TaskResultType
{
    TK_TaskResultType_Deschedule,
    TK_TaskResultType_Continue,
};

struct TK_Scheduler
{
    TK_Task *first;
    TK_Task *last;
};

struct TK_Task
{
    TK_Task *prev;
    TK_Task *next;
    TK_usize iteration;
};

struct TK_TaskResult
{
    TK_TaskResultType type;
    TK_Time delay;
    TK_bool success;
};

struct TK_SchedulingOptions
{
    const TK_Task *task;
};

#define TK_DescheduleTask()  ((TK_TaskResult){ .success = true, .type = TK_TaskResultType_Deschedule })
#define TK_ContinueTask(...) ((TK_TaskResult){ .success = true, .type = TK_TaskResultType_Continue, __VA_ARGS__ })
#define TK_TaskScheduler(...) (TK_TaskScheduler){ __VA_ARGS__ }
#define TK_MicroSeconds(...) ((TK_Time)((__VA_ARGS__) / 1'000'000))
#define TK_MilliSeconds(...) ((TK_Time)((__VA_ARGS__) / 1'000))
#define TK_Seconds(...) ((TK_Time)(__VA_ARGS__))

#define TK_ScheduleTask(scheduler, ...)TK_ScheduleTask_o(scheduler, (TK_SchedulingOptions){__VA_ARGS__})

TKAPI const TK_Task *TK_ScheduleTask_o( TK_TaskScheduler *scheduler, TK_SchedulingOptions options );

#endif