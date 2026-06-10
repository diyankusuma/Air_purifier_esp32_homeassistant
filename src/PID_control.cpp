#include <Arduino.h>
#include "dust.h"
#include <PID_v1.h>
#include "PID_control.h"
#include "control.h"
#include "state.h"

double Setpoint, Input, Output;
double aggkp=4, aggki=0.2, aggkd=1;
double conskp=1, conski=0.05, conskd=0.25;

PID myPID(&Input, &Output, &Setpoint, conskp, conski, conskd, REVERSE);

void setupPID(){
  Setpoint = 20;
  myPID.SetOutputLimits(0, 150);
  myPID.SetMode(AUTOMATIC);
}

void loopPID(){
  if (pid_enable) {
  Input = pm25;
  double gap = abs(Setpoint - Input);
  
  if (gap < 5) {
    myPID.SetTunings(conskp, conski, conskd);
  }

  else {
    myPID.SetTunings(aggkp, aggki, aggkd);
  }

  myPID.Compute();
  pwm_control(Output);
  
  } 
}
