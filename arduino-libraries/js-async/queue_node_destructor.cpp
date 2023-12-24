#ifndef __QUEUE_NODE_DESTRUCTOR_CPP__
#define __QUEUE_NODE_DESTRUCTOR_CPP__

#include "queue_node_destructor.h"

QueueNodeDestructor::QueueNodeDestructor(Queue *t_queue) {
    this->m_queue = t_queue;
}

void QueueNodeDestructor::Invoke(void* t_param) {
    QueueNode *node = (QueueNode*)t_param;
    Queue *queue = this->m_queue;

    queue->m_deleted = node;
    node->GetNext(queue->m_deletedNext);
    node->GetPrevious(queue->m_deletedPrevious);
}

#endif
