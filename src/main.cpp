#include <Arduino.h>

#include <StateMachine.h>
#include <DelayTask.h>
#include <DomeTask.h>

// Blinky done as a state machine. Also running a stepper motor randomly

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
    setChild(&delay);   // divert update() to running the delay
    setState(ACTION(ledOff));
  }
  void ledOff(void)
  {
    digitalWrite(13, LOW);  // led off
    delay.setDelay(offDelay);
    setChild(&delay);   // divert update() to running the delay
    setState(ACTION(initialState)); // after the delay do it all over again
  }
};

ledTask blinky;
domeTask dome;

void setup() {
  // put your setup code here, to run once:
  blinky.setOffDelay(500);
  blinky.setOnDelay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  blinky.update();

  dome.update();
  // doOtherStuff.update();

  // notes: at no point does blinky.update() take more than a few hundred usec to return. 
  // So long as you call it, it'll keep that LED blinking
}
