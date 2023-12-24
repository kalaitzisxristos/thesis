#ifndef __ASYNC_CPP__
#define __ASYNC_CPP__

#include "js-async.h"

Queue &async::m_tasks()
{
    static Queue tasks = Queue();
    return tasks;
}

async::async(void (*t_callback)(), unsigned long t_time)
{
    this->m_callback = t_callback;
    this->m_finished = false;
}

async::~async()
{
    delete m_invokable;
}

void async::InvokeAll()
{
    m_tasks().ForEach([](IInvokable *t_data, QueueNode *t_node) {
        if (t_data->IsFinished())
        {
            m_tasks().Remove(t_node);
        }
        else
        {
            t_data->Invoke();
        }
    });
}

unsigned long (*async::CurrentTime)() = []() -> unsigned long { return 0; };

void async::Cancel()
{
    this->m_finished = true;
}

Timeout::Timeout(void (*t_callback)(), unsigned long t_time) : async(t_callback, t_time)
{
    this->m_initTime = CurrentTime();
    this->m_time = t_time;
}

void Timeout::Invoke(void *t_param)
{
    if (!this->m_finished)
    {
        unsigned long timeNow = CurrentTime();
        if ((unsigned long)(timeNow - this->m_initTime) > this->m_time)
        {
            this->m_callback();
            this->Cancel();
        }
    }
}

Interval::Interval(void (*t_callback)(), unsigned long t_time) : async(t_callback, t_time)
{
    this->m_lastTime = CurrentTime();
    this->m_time = t_time;
}

void Interval::Invoke(void *t_param)
{
    if (!this->m_finished)
    {
        unsigned long timeNow = CurrentTime();
        if ((unsigned long)(timeNow - this->m_lastTime) > this->m_time)
        {
            this->m_lastTime = timeNow;
            this->m_callback();
        }
    }
}

Timeout *setTimeout(void (*t_callback)(), unsigned long t_time)
{
    Timeout *t = new Timeout(t_callback, t_time);
    t->m_node = async::m_tasks().Add(t);
    return t;
}

Interval *setInterval(void (*t_callback)(), unsigned long t_time)
{
    Interval *i = new Interval(t_callback, t_time);
    i->m_node = async::m_tasks().Add(i);
    return i;
}

#endif