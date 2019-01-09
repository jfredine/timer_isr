#include <Arduino.h>

volatile long unsigned total_isr_count;

#define USE_TIMER0

#if defined(USE_TIMER2)
ISR(TIMER2_COMPA_vect) {
#else
ISR(TIMER0_COMPA_vect) {
#endif
    total_isr_count++;
#if defined(USE_TIMER2)
    OCR2A = (OCR2A + 64) & 0xff;
#else
    OCR0A = (OCR0A + 64) & 0xff;
#endif
}


void setup() {
    char buffer[15];
    Serial.begin(9600);

    cli();  // disable interrupts

    //set timer to run at 256KHz and interrupt on comparison match
#if defined(USE_TIMER2)
    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2 = 0;

    TCCR2B |= (1 << CS22);   // clock prescaling of /64
    OCR2A = 1;               // counter compare A value
    TIMSK2 |= _BV(OCIE2A);   // set the compare A output to cause an interrupt

    snprintf(buffer, sizeof(buffer), "TCCR2A = 0x%02x", TCCR2A);
    Serial.println(buffer);
    snprintf(buffer, sizeof(buffer), "TCCR2B = 0x%02x", TCCR2B);
    Serial.println(buffer);
    snprintf(buffer, sizeof(buffer), "OCR2A = 0x%02x", OCR2A);
    Serial.println(buffer);
    snprintf(buffer, sizeof(buffer), "OCR2B = 0x%02x", OCR2B);
    Serial.println(buffer);
    snprintf(buffer, sizeof(buffer), "TIMSK2 = 0x%02x", TIMSK2);
    Serial.println(buffer);
#else
    TCCR0A &= ~((1 << WGM01) | (1 << WGM00));  // Turn off PWM modes
    TCCR0B &= ~(1 << WGM02);

    OCR0A = 1;               // counter compare A value
    TIMSK0 |= _BV(OCIE0A);   // set the compare A output to cause an interrupt

    snprintf(buffer, sizeof(buffer), "TCCR0A = 0x%02x", TCCR0A);
    Serial.println(buffer);
    snprintf(buffer, sizeof(buffer), "TCCR0B = 0x%02x", TCCR0B);
    Serial.println(buffer);
    snprintf(buffer, sizeof(buffer), "OCR0A = 0x%02x", OCR0A);
    Serial.println(buffer);
    snprintf(buffer, sizeof(buffer), "OCR0B = 0x%02x", OCR0B);
    Serial.println(buffer);
    snprintf(buffer, sizeof(buffer), "TIMSK0 = 0x%02x", TIMSK0);
    Serial.println(buffer);
#endif

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
