#include "servo.h"

#define DEFINE_TIMER_VALS(a, b) static uint16_t timer_vals[2] = {a, b - a};

DEFINE_TIMER_VALS(250, 8000)
Servo servo; 
uint8_t tmp = 1;
void setup() {
    
  // put your setup code here, to run once:
  servo = SERVO_DEFINE(B, 1); 
  servo_init(&servo);

  cli();//stop interrupts

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B

  //initialize counter value to 0
  TCNT1  = 0; 

  // set compare match register for 1hz increments
  // OCR1A = 16*10^6 / (frequency * prescaler) - 1 (result must be <65536)

  // turn on CTC mode - timer counter resets to 0
  TCCR1B |= (1 << WGM12);

  // Set prescaler for B to 8
  TCCR1B |= (1 << CS11);// | (1 << CS10);  

  // enable timer compare interrupt for timer TCNT1
  TIMSK1 |= (1 << OCIE1A);

  //allow interrupts
  sei();
  Serial.begin(9600);
}

ISR(TIMER1_COMPA_vect)
{
  servo_write(&servo, tmp);
  tmp = !tmp;
  cli();
  OCR1A = timer_vals[tmp];
  sei();
}

void loop() {
  // put your main code here, to run repeatedly:
//  uint16_t tmp = analogRead(0);    // read the value from the sensor
//  tmp >>= 2;
//  tmp |= (1 << 9); // + 256
  timer_vals[0] = (analogRead(0) << 1) | (1 << 11); // Bit black magic
  timer_vals[1] = 8000 - timer_vals[0];
  
}
