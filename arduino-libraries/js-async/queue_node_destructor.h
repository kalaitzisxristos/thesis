#pragma once

#ifndef __QUEUE_NODE_DESTRUCTOR_H__
#define __QUEUE_NODE_DESTRUCTOR_H__

#include "js-async.h"

class QueueNodeDestructor : public IInvokable
{
private:
    Queue *m_queue;

public:
    QueueNodeDestructor(Queue *t_queue);
    void Invoke(void* t_param = nullptr) override;
};

#endif
