#include <avr/io.h> //header to the enable data flow control over pins
#define F_CPU 1000000 //telling the controller crystal frequency attached
#include <util/delay.h> //header to the enable delay function in program
#include <stdlib.h>
#define enable            5 //giving the name “enable”  to 5th pin of PORTD, since it Is connected to LCD enable pin
#define registerselection 6 //giving name “registerselection” to 6th pin of PORTD, since is connected to LCD RS pin
void send_a_command(unsigned char command);
void send_a_character(unsigned char character);
void send_a_string(char *string_of_characters);
int main(void)
{
	DDRB = 0xFF; //putting the portB and portD as output pins
	DDRC = 0; //Taking portA as input.
	DDRD = 0xFF;
	_delay_ms(50); //giving the delay of 50ms
	ADMUX |=(1<<REFS0)|(1<<REFS1); //setting reference of ADC
	ADCSRA |=(1<<ADEN)|(1<<ADATE)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
	//enabling ADC, setting free running mode
	int16_t COUNTA = 0;
	char SHOWA [3]; //displaying digital output as temperature in lcd display
	send_a_command(0x01); //Clear Screen 0x01 = 00000001
	_delay_ms(50);
	send_a_command(0x38); //telling lcd we are using 8bit command /data mode
	_delay_ms(50);
	send_a_command(0b00001111); // this command is used for lcd screen is on and courser blinking
	_delay_ms(50);
	ADCSRA |=(1<<ADSC); //starting  ADC conversion
	while(1)
	{
		COUNTA = ADC/4;
		send_a_string ("TEMPRATURE IS--"); //Display at the lcd
		send_a_command(0x80 + 0x40 + 0); // shifting cursor  to 1st  shell  of second line
		itoa(COUNTA,SHOWA,10);
		send_a_string(SHOWA);
		send_a_string ("  degree Celsius");
		send_a_command(0x80 + 0); //retuning to first line first shell
	}
}
void send_a_command(unsigned char command)
{
	PORTB = command; 
	PORTD &= ~ (1<<registerselection); //putting the 0 in RS to tell lcd we are sending command
	PORTD |= 1<<enable; //telling the lcd to receive command /data at the port
	_delay_ms(20);
	PORTD &= ~1<<enable; //telling the lcd we completed sending data
	PORTB = 0;
}
void send_a_character(unsigned char character)
{
	PORTB = character;
	PORTD |= 1<<registerselection; //telling the lcd we are sending data not commands
	PORTD |= 1<<enable; //telling the lcd to start receiving command/data
	_delay_ms(20);
	PORTD &= ~1<<enable; //telling the lcd we completed sending data/command
	PORTB = 0;
}
void send_a_string(char *string_of_characters)
{
	while(*string_of_characters > 0)
	{
		send_a_character(*string_of_characters++);
	}
}