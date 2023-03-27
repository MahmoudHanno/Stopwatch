#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
int count_sec=0;
int seconds=0;
int seconds_tens=0;
int minutes=0;
int minutes_tens=0;
unsigned char num=0;

ISR(TIMER0_COMP_vect){
	count_sec++;
}
ISR(INT1_vect){
	TCCR0=(1<<FOC0)|(1<<WGM01)|(1<<CS02)|(1<<CS00);
}
void stopwatch(){
	while(count_sec==4){
		seconds++;
		if(seconds==10)
		{
			seconds_tens++;
			seconds=0;
		}
		if(seconds==0 && seconds_tens==6)
		{
			seconds_tens=0;
			minutes++;
		}
		if(minutes==10)
		{
			minutes_tens++;
			minutes=0;
		}
		if(minutes==0 &&minutes_tens==6)
		{
			minutes_tens=0;
		}
		count_sec=0;
}

}
void display(){

	PORTC|=(1<<PD4);
	PORTC=(PORTC&0xF0)|(seconds&0x0F);
	_delay_ms(2);
	PORTC&=~(1<<PD4);

	PORTC|=(1<<PD5);
	PORTC=(PORTC&0xF0)|(seconds_tens&0x0F);
	_delay_ms(2);
	PORTC&=~(1<<PD5);

	PORTC|=(1<<PD6);
	PORTC=(PORTC&0xF0)|(minutes&0x0F);
	_delay_ms(2);
	PORTC&=~(1<<PD6);

	PORTC|=(1<<PD7);
	PORTC=(PORTC&0xF0)|(minutes_tens&0x0F);
	_delay_ms(2);
	PORTC&=~(1<<PD7);


}
void reset(){
	if((PIND&(1<<PD4))){
		_delay_ms(500);
		if(PIND&(1<<PD4)){
			seconds=0;
			seconds_tens=0;
			minutes=0;
			minutes_tens=0;
		}else{
			TCCR0=0;
		}
	}
}
int main(){
	SREG|=(1<<7);
	DDRC|=0xFF;
	DDRD&=~(1<<PD4);
	DDRD&=~(1<<PD3);
	TCCR0|=(1<<WGM01)|(1<<CS02)|(1<<CS00);
	OCR0=250;
	TIMSK=(1<<OCIE0);
	GICR|=1<<INT1;
	MCUCR = 1<<ISC01 | 1<<ISC01;

	while(1){

		reset();
		stopwatch();
		display();

	}
	return 0;
}
