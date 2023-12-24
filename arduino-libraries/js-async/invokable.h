#pragma once

#ifndef __INVOKABLE_H__
#define __INVOKABLE_H__

class IInvokable
{
protected:
    bool m_finished;
    IInvokable() {}
public:
    virtual void Invoke(void* t_param = nullptr) = 0;
    bool IsFinished() { return m_finished; };
};

#endif