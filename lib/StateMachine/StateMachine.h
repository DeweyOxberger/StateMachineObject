#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H


#include <functional>
#define ACTION(a) [this]() { this->a(); }

//typedef void (*StateMethod)(void);

class StateMachine
{
public:
    using StateMethod = std::function<void(void)>;

    StateMachine(StateMethod firstState);
    virtual ~StateMachine();
    
    void reset(void);
    void update(void);
    bool isDone(void);

protected:
    void setState(StateMethod nextState);
    void done(void);
    void setChild(StateMachine * child);

    StateMachine * childMachine;

    bool is_done;
    StateMethod resetState;
    StateMethod state;

};

#endif // STATE_MACHINE_H
