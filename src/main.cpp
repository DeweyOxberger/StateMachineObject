#include <Arduino.h>

#include <StateMachine.h>

// Blinky done as a state machine.

// define a "delay" state machine. This is "non-blocking".
// You keep calling update() until it isDone()
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

// State Machine to handle blinking the LED forever
class ledTask : public StateMachine
{
public:
  ledTask() : StateMachine(ACTION(initialState))
  {
    pinMode(13, OUTPUT); 
    offDelay = 100;
    onDelay = 200;
  }

  void setOffDelay(uint32_t off_msec) { offDelay = off_msec; }
  void setOnDelay(uint32_t on_msec) { onDelay = on_msec; }
  
protected:

  uint32_t offDelay;
  uint32_t onDelay;
  delayTask delay;

  void initialState(void) 
  { 
    digitalWrite(13, HIGH);  // led on
    delay.setDelay(onDelay);
    setChild(&delay);   // kick off a delay
    setState(ACTION(ledOff));
  }
  void ledOff(void)
  {
    digitalWrite(13, LOW);
    delay.setDelay(offDelay);
    setChild(&delay);
    setState(ACTION(initialState)); // do it all over again
  }
};

ledTask blinky;

void setup() {
  // put your setup code here, to run once:
  blinky.setOffDelay(500);
  blinky.setOnDelay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  blinky.update();
  // doOtherStuff.update();

  // notes: at no point does blinky.update() take more than a few hundred usec to return. 
  // So long as you call it, it'll keep that LED blinking
}
