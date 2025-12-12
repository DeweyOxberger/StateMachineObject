# State Machine Object

## Overview

This project creates a simple object-oriented state machine system. Typically, a state machine is done using an int (or an enum, if you are fancy) to track the state. Then, a switch statement uses that state to run a block of code. The blocks of code can alter the int/enum to change which block of code will run the next time. 

This is an alternative approach where you define objects that have methods for each state. Rather than using an int to decide what to run, this uses pointers to the methods. Each method can do something and change the pointer as needed.

Note: this State Machine system contains a demo and unit tests written for the Teensy 4. This system should work on any "has a decent amount of ram" system. The pointers to functions, and object used, do add some overhead.

## Details

Because this is C++ the pointers to methods get a bit funky. A macro helps sort that out. Anytime you are setting a pointer to a method, use ACTION().

```cpp
    setState(ACTION(some_state_method));
```

You inherit from StateMethod and add all the data and methods you need (as protected or private members).

```cpp
class Machine1 : public StateMachine
{
public:
    Machine1();

protected:

    uint8_t some_data;

    void initialState(void);  // init the hardware
    void turnOnPump(void);
    void isTankFull(void);
    void turnOffPump(void);
  // and so on
};
```

The constructor sets the initial state of the state machine. When you reset() a state machine this is the state it will run next.

```cpp
Machine1::Machine1() : StateMachine(ACTION(initialState))
{
  // 
}
```

In a given state method you can divert to other state methods using
```cpp
 setState(ACTION(your_next_method));
```

A state machine should call done() when it's finished.

You can invoke a "sub-state machine". Common tasks, like a time delay, can be coded as their own state machine and run using setChild(). The existing state machine will pass the execution to the child until it is complete. Then the parent will proceed.

At the top level you call the udpate() function for any state machines that are running "at the same time".

```cpp
void loop() 
{
    lights.update();
    i2c.update();
    radar.update();
    serial.update();
}
```

As you write the various state methods you should (must) think about the time each block of code will require and break the task into small (time) blocks. This allows you to create a cooperative multi-tasking system. Don't have wait loops, break waiting into a state that tests if the wait is done (and does setState()). If the wait isn't done it should just return. The next .update() will try again.

The top level can also reset() a state machine (move it back to the initial state), or check if the state machine isDone().

See src/main.cpp for an example Blinky demo alternative.

Dewey
