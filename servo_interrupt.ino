#include "servo.h"
#define SERVO_CNT 3

static uint16_t timer_vals[SERVO_CNT * 2];

#define SET_TIMER_VALS(n, a, b) timer_vals[2 * n] = a; timer_vals[2 * n + 1] = b - a;

Servo servos[SERVO_CNT];
Servo * cur_servo; 

uint8_t tmp = 1;
uint8_t cnt = 0;
void setup() {

	SET_TIMER_VALS(0, 1250, 6000);
	SET_TIMER_VALS(1, 1250, 6000);
  SET_TIMER_VALS(2, 1250, 6000);

	servos[0] = SERVO_DEFINE(B, 1); 
	servo_init(servos);
	cur_servo = servos;

	servos[1] = SERVO_DEFINE(B, 2); 
	servo_init(servos + 1);

  servos[2] = SERVO_DEFINE(B, 3); 
  servo_init(servos + 2);

	//stop interrupts
	cli();

	TCCR1A = 0;// set entire TCCR1A register to 0
	TCCR1B = 0;// same for TCCR1B

	//initialize counter value to 0
	TCNT1  = 0; 

	//set compare match register for 1hz increments
	//OCR1A = 16*10^6 / (frequency * prescaler) - 1 (result must be <65536)

	//turn on CTC mode - timer counter resets to 0
	TCCR1B |= (1 << WGM12);

	//set prescaler for B to 8
	TCCR1B |= (1 << CS11);// | (1 << CS10);  

	//enable timer compare interrupt for timer TCNT1
	TIMSK1 |= (1 << OCIE1A);

	//allow interrupts
	sei();

	Serial.begin(9600);
}

ISR(TIMER1_COMPA_vect)
{
	cnt = (cnt + 1) % (SERVO_CNT * 2);
	cur_servo = servos + (cnt >> 1);
	tmp = !tmp;
  servo_write(cur_servo, tmp);
	cli();
	OCR1A = timer_vals[cnt];
	sei();
}

void loop()
{
	timer_vals[0] = (analogRead(0) << 1) | (1 << 11); // Bit black magic
	timer_vals[1] = 6000 - timer_vals[0];

	timer_vals[2] = (analogRead(1) << 1) | (1 << 11); // Bit black magic
	timer_vals[3] = 6000 - timer_vals[2];

	timer_vals[4] = (analogRead(2) << 1) | (1 << 11); // Bit black magic
  timer_vals[5] = 6000 - timer_vals[4];
}
