#ifndef __QUEUE_NODE_CPP__
#define __QUEUE_NODE_CPP__

#include "queue_node.h"

QueueNode::QueueNode(IInvokable *t_invokable, IInvokable *t_onDestruct)
{
    this->m_data = t_invokable;
    this->m_next = nullptr;
    this->m_previous = nullptr;
    this->m_onDestruct = t_onDestruct;
}

QueueNode::~QueueNode()
{
    if (this->m_onDestruct != nullptr)
    {
        this->m_onDestruct->Invoke(this);
    }

    delete m_data;
}

IInvokable *QueueNode::GetData()
{
    return this->m_data;
}

bool QueueNode::HasNext()
{
    return this->m_next != nullptr;
}

bool QueueNode::GetNext(QueueNode *&t_next)
{
    t_next = this->m_next;
    if (this->m_next == nullptr)
    {
        return false;
    }
    return true;
}

bool QueueNode::SetNext(QueueNode *t_next)
{
    if (t_next == this)
    {
        return false;
    }

    this->m_next = t_next;

    if (t_next != nullptr)
    {
        QueueNode *previous;
        t_next->GetPrevious(previous);
        if (previous != this)
        {
            t_next->SetPrevious(this);
        }
    }

    return true;
}

bool QueueNode::HasPrevious()
{
    return this->m_previous != nullptr;
}

bool QueueNode::GetPrevious(QueueNode *&t_previous)
{
    t_previous = this->m_previous;
    if (this->m_previous == nullptr)
    {
        return false;
    }
    return true;
}

bool QueueNode::SetPrevious(QueueNode *t_previous)
{
    if (t_previous == this)
    {
        return false;
    }

    this->m_previous = t_previous;

    if (t_previous != nullptr)
    {
        QueueNode *next;
        t_previous->GetNext(next);
        if (next != this)
        {
            t_previous->SetNext(this);
        }
    }

    return true;
}

void QueueNode::SetOnDestruct(IInvokable *t_onDestruct)
{
    this->m_onDestruct = t_onDestruct;
}

#endif