#include <Arduino.h>

volatile long unsigned total_isr_count;

// time between interrupts
// resolution of the timer is 4us so make tthis a multiple of 4
#define ISR_INTERVAL_us 256

//******************************************************************************
// calculated parameters which should not need user adjustment

// Number of timer ticks between interrupts
#define OCR0A_INCR (ISR_INTERVAL_us / 4)

/*
 * TIMER0_COMPA_vect
 * ISR triggered by timer0.  In order to get interrupts at a rate greater than
 * 1 ms (default for timer0), but not require an additional timer and not
 * disturb the timer0 overflow rate (because that is used by default libraries),
 * it makes use of the timer compare interrupt and adjusts the comparison value
 * within the ISR.
 */

ISR(TIMER0_COMPA_vect) {

    total_isr_count++;

    // set the time of the next interrupt
    OCR0A = (OCR0A + OCR0A_INCR) & 0xff;
}

void setup() {
    Serial.begin(9600);

    // set up a recuring timer interrupt
    cli();  // disable interrupts

    // Turn off PWM modes of timer0 because such modes prevent the
    // immediate re-assignment of compare values to OCR0A
    TCCR0A &= ~((1 << WGM01) | (1 << WGM00));
    TCCR0B &= ~(1 << WGM02);

    OCR0A = 1;              // Set the timer0 compare value.
                            // The ISR will adjust this value to to potentially
                            // produce multiple interrupts during the normal
                            // 0 -> FF count sequence which takes 1024us
    TIMSK0 |= _BV(OCIE0A);  // Enable the timer compare interrupt

    sei();  // enable interrupts
}

void loop() {
    Serial.print("Count: ");
    Serial.print(total_isr_count);
    Serial.print("\n");
    Serial.print("Wall Time: ");
    Serial.print(micros());
    Serial.print("\n\n");
    delay(10000);
}
