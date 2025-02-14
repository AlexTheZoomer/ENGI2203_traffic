// Created 2025-02-10
// Authors: AD, LM
// Traffic Light Design Lab
// Timer Code

#include <avr/io.h>
#include <avr/interrupt.h>

// Pin Definitions
#define grn PD4
#define yellow PD3
#define red PD2
#define ped_light PD5
#define reset_action PD6

// Timing Definitions
#define grn_time 6000
#define yellow_time 3000
#define red_time 8000
#define blink_delta 1000

volatile uint32_t main_timer = 0;  // Time counter

void setupTimer0() {
	TCCR0B = 0b00000011; // Set prescaler to 64
	TCCR0A = (1<<WGM01); //Set TCCR0A to CTC mode
	OCR0A = 125;
	TIMSK0 = (1 << OCIE0A); // Enabling the overflow interrupt
	sei(); // Enable global interrupts
}

ISR(TIMER0_COMPA_vect) {
	main_timer++;  // Main function timer, everything relies on this
	PORTD ^= (1 << PD6); // Toggle Oscilloscope on PD6 (ONLY NEEDED FOR TIMER OSCILLIOSCOPE READING DEBUG PIN)
}

int main(void) {
	// Setup
	DDRB |= (1 << PB0);
	DDRD |= (1 << grn) | (1 << yellow) | (1 << red) | (1 << ped_light);
	DDRD &= ~(1 << reset_action);  // Button as input
	PORTD |= (1 << reset_action);  // Enable pull-up
	
	setupTimer0();

	uint32_t start_time = 0;

	while (1) {
		if (!(PIND & (1 << reset_action))) {
			main_timer = 0;
			start_time = 0;
			PORTD &= ~((1 << grn) | (1 << yellow) | (1 << ped_light));
			PORTD |= (1 << red);
			while (!(PIND & (1 << reset_action)));
		}

		uint32_t elapsed = main_timer - start_time;

		if (elapsed < grn_time) {
			PORTD |= (1 << grn);
			PORTD &= ~(1 << yellow) & ~(1 << red);
			PORTD |= (1 << ped_light);
		}
		else if (elapsed < grn_time + yellow_time) {
			PORTD |= (1 << yellow);
			PORTD &= ~(1 << grn) & ~(1 << red);

			if ((elapsed % blink_delta) < (blink_delta / 2)) {
				PORTD |= (1 << ped_light);
				} else {
				PORTD &= ~(1 << ped_light);
			}
		}
		else if (elapsed < grn_time + yellow_time + red_time) {
			PORTD |= (1 << red);
			PORTD &= ~(1 << grn) & ~(1 << yellow) & ~(1 << ped_light);
		}
		else {
			start_time = main_timer;
		}
	}
}