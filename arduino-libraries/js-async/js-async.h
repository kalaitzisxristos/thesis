#pragma once

#ifndef __ASYNC_H__
#define __ASYNC_H__

class IInvokable;
class Queue;
class QueueNode;
class QueueNodeDestructor;

#include "invokable.h"
#include "queue.h"
#include "queue_node.h"
#include "queue_node_destructor.h"

class async : public IInvokable
{
protected:
    static Queue &m_tasks();
    QueueNode *m_node;
    IInvokable *m_invokable;
    void (*m_callback)();

    async() {}
    async(void (*t_callback)(), unsigned long t_time);

public:
    virtual ~async();
    static void InvokeAll();
    static unsigned long (*CurrentTime)();
    void Cancel();
};

class Timeout : public async
{
private:
    unsigned long m_initTime;
    unsigned long m_time;

public:
    Timeout(void (*t_callback)(), unsigned long t_time);
    void Invoke(void* t_param = nullptr) override;

    friend Timeout *setTimeout(void (*t_callback)(), unsigned long t_time);
};

class Interval : public async
{
private:
    unsigned long m_lastTime;
    unsigned long m_time;

public:
    Interval(void (*t_callback)(), unsigned long t_time);
    void Invoke(void* t_param = nullptr) override;

    friend Interval *setInterval(void (*t_callback)(), unsigned long t_time);
};

Timeout *setTimeout(void (*t_callback)(), unsigned long t_time);
Interval *setInterval(void (*t_callback)(), unsigned long t_time);

#endif
