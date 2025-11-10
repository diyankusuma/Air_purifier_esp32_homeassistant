#ifndef PID_CONTROL_H
#define PID_CONTROL_H

#include <PID_v1.h>


extern double Setpoint, Input, Output;

extern double aggkp, aggki, aggkd;
extern double conskp, conski, conskd;

extern PID myPID;

void setupPID();
void loopPID();



#endif // !PID_CONTROL_H

