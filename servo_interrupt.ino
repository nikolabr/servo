#include "servo.h"

Servo servo; 

void setup() {
    
  // put your setup code here, to run once:
  servo = SERVO_DEFINE(B, 1); 
  servo_init(&servo);

  cli();//stop interrupts

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B

  //initialize counter value to 0  TCNT1  = 0; 

  TCNT1  = 0; 

  // set compare match register for 1hz increments
  // OCR1A = 16*10^6 / (frequency * prescaler) - 1 (result must be <65536)

  // 50Hz 
  OCR1A = 40000;  

  // 250kHz
  OCR1B = 128;

  // turn on CTC mode - timer counter resets to 0
  TCCR1A |= (1 << WGM12);
  TCCR1B |= (1 << WGM12);

  // Set prescaler for A to 64
  TCCR1A |= (1 << CS11) | (1 << CS10);  

  // Set prescaler for B to 64
  TCCR1B |= (1 << CS11) | (1 << CS10);  

  // enable timer compare interrupt for timer TCNT1
  TIMSK1 |= (1 << OCIE1A);

  //allow interrupts
  sei();
  Serial.begin(9600);
}

ISR(TIMER1_COMPA_vect)
{
  servo_write(&servo, 1);
  uint8_t sreg = SREG;
  cli();
  TIMSK1 |= (1 << OCIE1B);
  sei();
  SREG = sreg;
}

ISR(TIMER1_COMPB_vect)
{
  servo_write(&servo, 0);
  uint8_t sreg = SREG;
  cli();
  TIMSK1 &= ~(1 << OCIE1B);
  sei();
  SREG = sreg;

}

void loop() {
  // put your main code here, to run repeatedly:
  OCR1B =  analogRead(0);    // read the value from the sensor
}
