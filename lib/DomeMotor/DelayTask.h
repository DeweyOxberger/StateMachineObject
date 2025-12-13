#ifndef DELAY_TASK_H
#define DELAY_TASK_H

#include "StateMachine.h"
#include "Arduino.h"

class delayTask : public StateMachine
{
public:
  delayTask() : StateMachine(ACTION(initialState))
  {

  }

  void setDelay(uint32_t time_mSec) { duration = time_mSec; }

protected:

  uint32_t duration;
  uint32_t start_time;

  void initialState(void) 
  { 
    start_time = millis(); 
    setState(ACTION(checkIfDone));
  }

  void checkIfDone(void) 
  { 
    uint32_t time = millis() - start_time;
    if (time >= duration)
    {
      done();
    }
  }
};


#endif  // DELAY_TASK_H