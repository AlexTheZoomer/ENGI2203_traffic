// Created 2025-02-10
// Authors: AD, LM
// Traffic Light Design Lab

#include <avr/io.h>
#include <avr/interrupt.h>

void setupTimer0() {
	TCCR0B = 0b00000011; // Set prescaler to 64
	TCCR0A = (1<<WGM01); //Set TCCR0A to CTC mode
	OCR0A = 125;
	TIMSK0 = (1 << OCIE0A); // Enabling the overflow interrupt
	sei(); // Enable global interrupts
}

ISR(TIMER0_COMPA_vect) {
	PORTD ^= (1 << PD6); // Toggle Oscilloscope on PD6
}

int main() {
	DDRD |= (1 << PD6); // Set PB0 as output
	setupTimer0();

	while(1) {
		// Do nothing, the interrupt handles the LED toggle!
	}
}
