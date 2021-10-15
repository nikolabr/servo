#include "servo.h"

void servo_init(Servo * servo){
  *(servo->ddr_port) |= (1 << servo->pin);
}

void servo_write(Servo * servo, unsigned int val){
   *(servo->port) &= ~(1 << servo->pin); 
   *(servo->port) |= (val << servo->pin); 
   delay(500);
}
