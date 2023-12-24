#pragma once

#ifndef __QUEUE_NODE_H__
#define __QUEUE_NODE_H__

#include "js-async.h"

class QueueNode
{
private:
    IInvokable *m_data;
    QueueNode *m_next;
    QueueNode *m_previous;
    IInvokable *m_onDestruct;

public:
    QueueNode(IInvokable *t_invokable = nullptr, IInvokable *t_onDestruct = nullptr);
    ~QueueNode();
    IInvokable *GetData();

    bool HasNext();
    bool GetNext(QueueNode *&t_next);
    bool SetNext(QueueNode *t_next);

    bool HasPrevious();
    bool GetPrevious(QueueNode *&t_previous);
    bool SetPrevious(QueueNode *t_previous);

    void SetOnDestruct(IInvokable *t_onDestruct);
};

#endif