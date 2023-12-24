#pragma once

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <inttypes.h>

#include "js-async.h"

class Queue
{
private:
    QueueNode *m_head;
    QueueNode *m_last;
    QueueNode *m_deleted;
    QueueNode *m_deletedNext;
    QueueNode *m_deletedPrevious;

    QueueNodeDestructor *m_nodeDestructor;

public:
    Queue();
    QueueNode *Add(IInvokable *t_data);
    bool Remove(QueueNode *&t_node);
    bool RemoveAt(uint32_t t_index);

    void GetHead(QueueNode *&t_head);
    void GetLast(QueueNode *&t_last);

    void ForEach(void (*t_callback)(IInvokable*, QueueNode*));

friend class QueueNodeDestructor;
};

#endif