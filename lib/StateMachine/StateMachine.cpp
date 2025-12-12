// https://github.com/DeweyOxberger/StateMachineObject

#include "StateMachine.h"

StateMachine::StateMachine(StateMethod initialState)
{
    childMachine = nullptr;
    resetState = initialState;
    reset();
}

StateMachine::~StateMachine()
{
    done();
}

void StateMachine::reset(void)
{
    setState(resetState);
}

void StateMachine::update(void)
{
    if (childMachine)
    {
        childMachine->update();
        if (childMachine->isDone())
        {
            childMachine = nullptr;
        }
    }
    else
    {
        if (!is_done)
        {
            state();
        }
    }
}

bool StateMachine::isDone(void)
{
    return is_done;
}

void StateMachine::done(void)
{
    is_done = true;
}

void StateMachine::setChild(StateMachine * child)
{
    childMachine = child;
    childMachine->reset();
}

void StateMachine::setState(StateMethod nextState)
{
    state = nextState;
    is_done = false;
}