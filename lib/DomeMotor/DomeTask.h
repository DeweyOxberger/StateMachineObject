#ifndef DOME_MOTOR_H
#define DOME_MOTOR_H

#include "StateMachine.h"
#include "DelayTask.h"
#include "Arduino.h"

// This state machine animates the dome on an R2D2. The dome mechanics use a 
// NEMA-17 being driven by a quiet stepper driver.
// This creates movements of random distances with random pauses between movements
// yet limits the dome rotation to about 90 degrees (no slip ring, so limit amount of rotation)


class domeTask : public StateMachine
{
public:
  domeTask() : StateMachine(ACTION(initialState))
  {
    // initialize IO
    pinMode(dirPin, OUTPUT);  // Dir
    pinMode(stepPin, OUTPUT);  // Step
    pinMode(enablePin, OUTPUT);  // ~Enable
    digitalWrite(dirPin, HIGH);
    digitalWrite(stepPin, HIGH);
    digitalWrite(enablePin, HIGH);

// ESP32
//    ledcSetup(pwmChannel, 16384, 12);  // config pwm
//    ledcAttachPin(stepPin, pwmChannel);
  }

protected:
  // ESP32
  // const uint8_t dirPin = 19;   // AUX5
  // const uint8_t stepPin = 18;  // AUX4
  // const uint8_t enablePin = 5; // AUX3
  // const uint8_t pwmChannel = 0;
  // ESP32

  // Teensy 4
  const uint8_t dirPin = 3;  
  const uint8_t stepPin = 4; 
  const uint8_t enablePin = 5; 
  // Teensy 4

  const uint8_t maxMult = 5;

  uint32_t duration;
  int32_t position = 0;
  uint16_t motorDirection = 0;

  uint32_t motorIndex;
  uint16_t waveFreq[10] =   { 100, 200, 300, 400, 500, 400, 300, 200, 100, 0 }; // has to be 0 terminated array
  uint16_t waveSteps[10] =  {   4,   8,  12,  16, 160,  16,  12,   8,   4, 0 };
  uint8_t  waveExtend[10] = {   0,   0,   0,   0,   1,   0,   0,   0,   0, 0 };
  uint32_t motorDelay[6] =  {0, 0, 5000, 15000, 20000, 30000};

  uint32_t motorMultiplier = 1;

  delayTask delay;  // child state machine

  void initialState(void)
  {
    duration = 1000; // power up delay

    setState(ACTION(initialDelay));
  }

  void initialDelay(void)
  {
    delay.setDelay(duration);
    setChild(&delay);
    setState(ACTION(setupMove));
  }

  void setupMove(void)
  {
    motorIndex = 0;
    duration = 2000 * motorMultiplier; // keep the motor cool, wait longer for longer moves
    digitalWrite(enablePin, LOW);
    if (motorDirection == 0)
    {
      digitalWrite(dirPin, LOW);
    }
    else
    {
      digitalWrite(dirPin, HIGH);
    }
    setState(ACTION(nextSpeed));
  }

  void nextSpeed(void)
  {
    if (waveFreq[motorIndex] == 0)
    {
      // no more movements in the array, finish up
      setState(ACTION(finishMove));
    }
    else
    {
      // Teensy 4
      tone(stepPin, waveFreq[motorIndex]);
      // ESP32
      // ledcWriteTone(pwmChannel, waveFreq[motorIndex]);
      uint32_t totalSteps = waveSteps[motorIndex];
      if (waveExtend[motorIndex] != 0)
      {
        totalSteps *= motorMultiplier;
      }
      if (motorDirection == 0)
      {
        position -= totalSteps;
      }
      else
      {
        position += totalSteps;
      }
      duration = totalSteps * 1000 / waveFreq[motorIndex];
      delay.setDelay(duration);  // wait for all pusles to be sent
      setChild(&delay);

      motorIndex++; // inc this at the end
    }
  }

  void finishMove(void)
  {
    // turn off tone
    // Teensy 4
    noTone(stepPin);
    // ESP32
    //ledcWriteTone(pwmChannel, 0);

    // plan next move
    if (position < 0)
    {
      motorDirection = 1;
      motorMultiplier = 3;
    }
    else if (position > 12000)
    {
      motorDirection = 0;
      motorMultiplier = 3;
    }
    else
    {
      motorDirection = random(0, 1);
      motorMultiplier = random(1, maxMult);
    }

    duration = 2000 * (maxMult - motorMultiplier) + motorDelay[random(0, 5)]; 

    digitalWrite(enablePin, HIGH); // disable motor driver, it'll cool down
    setState(ACTION(initialDelay));
  }

};

#endif // DOME_MOTOR_H