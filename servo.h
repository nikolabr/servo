#ifndef Servo_h
#define Servo_h

#define SERVO_DEFINE(a, b) {.ddr_port = &DDR##a, .port = &PORT##a, .pin = b}

#include <Arduino.h>

typedef struct {
  volatile uint8_t * ddr_port;
  volatile uint8_t * port;
  uint8_t pin;
} Servo;

void servo_init(Servo * servo);
void servo_write(Servo * servo, unsigned int val);

#endif
