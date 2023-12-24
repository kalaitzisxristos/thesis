#ifndef __QUEUE_CPP__
#define __QUEUE_CPP__

#include "queue.h"

Queue::Queue()
{
    this->m_head = new QueueNode();
    this->m_last = this->m_head;
    this->m_nodeDestructor = new QueueNodeDestructor(this);
}

QueueNode *Queue::Add(IInvokable *t_data)
{
    QueueNode *node = new QueueNode(t_data, this->m_nodeDestructor);
    this->m_last->SetNext(node);
    this->m_last = node;

    return node;
}

bool Queue::Remove(QueueNode *&t_node)
{
    if (t_node == this->m_head)
    {
        return false;
    }

    bool success = true;

    QueueNode *previous;
    success &= t_node->GetPrevious(previous);

    if (t_node == this->m_last)
    {
        this->m_last = previous;
        previous->SetNext(nullptr);
    }
    else
    {
        QueueNode *next;
        success &= t_node->GetNext(next);
        previous->SetNext(next);
    }

    delete t_node;

    return success;
}

bool Queue::RemoveAt(uint32_t t_index)
{
    bool success = true;

    QueueNode *node;
    if (this->m_head->GetNext(node))
    {
        for (uint32_t i = 0; i < t_index; i++)
        {
            if (!node->GetNext(node))
            {
                return false;
            }
        }

        QueueNode *previous;
        success &= node->GetPrevious(previous);

        if (node == this->m_last)
        {
            this->m_last = previous;
            previous->SetNext(nullptr);
        }
        else
        {
            QueueNode *next;
            success &= node->GetNext(next);
            success &= previous->SetNext(next);
        }

        delete node;
    }
    else
    {
        return false;
    }

    return success;
}

void Queue::GetHead(QueueNode *&t_head)
{
    t_head = this->m_head;
}

void Queue::GetLast(QueueNode *&t_last)
{
    t_last = this->m_last;
}

void Queue::ForEach(void (*t_callback)(IInvokable*, QueueNode*))
{
    QueueNode *current;
    this->GetHead(current);
    while (current->GetNext(current))
    {
        IInvokable *data;
        data = current->GetData();
        t_callback(data, current);

        if (this->m_deleted == current)
        {
            if (this->m_deletedPrevious != nullptr)
            {
                current = this->m_deletedPrevious;
            }
            else
            {
                break;
            }
        }
    }
}

#endif
