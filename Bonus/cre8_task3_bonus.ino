#include <avr/io.h>
#include <avr/interrupt.h>
volatile int k0 = 0,flag=0,k2=0;
ISR(INT0_vect)
{
 // If interrupts come faster than 200ms, assume it's a bounce and ignore
 if (k2>7808) 
 {
  flag^=1;
  PORTB^=0xFF;;
  k2=0;
 }
}

ISR(TIMER2_OVF_vect)
{
  ++k2;
}
ISR(TIMER0_OVF_vect)
{
  ++k0;
}
void usart_write(String data)
{
  for (int i = 0; data[i] != '\0'; i++)
  {
    while (!(UCSR0A & (1 << UDRE0)))
      ;
    UDR0 = data[i];
  }
}

void dlay(float t)
{
  TCCR0B = (1 << CS02) | (1 << CS00);
  k0 = 0;
  while (k0 <= 10)
    ;
  TCCR0B = 0;
}
void init()
{
  DDRB = 0xFF;
  PORTB=0;
  PORTD=0b00000100;
  EICRA = 1<<ISC01;
  EIMSK = 1<<INT0;
  TIMSK0 = 1 << TOIE0;
  TIMSK2 = 1<<TOIE2;
  TCCR2B = 1<<CS21;
  ADMUX = (1 << REFS0); /* Vref: Avcc, ADC channel: 0 */
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  UCSR0A = 0X00;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
  UBRR0 = 103;

}

/*
  ADC Read function
*/
uint16_t ADC_Read(unsigned char channel)
{
  ADMUX = (1 << REFS0) | (channel & 0x07); /* set input channel to read */
  ADCSRA |= (1 << ADSC);                   /* Start ADC conversion */
  while (!(ADCSRA & (1 << ADIF)))
    ;                    /* Wait until end of conversion by polling ADC interrupt flag */
  ADCSRA |= (1 << ADIF); /* Clear interrupt flag */
  return ADC;            /* Return ADC word */
}
int main()
{
  init();
  sei();
  int x, y, z;
  while (1)
  {
    x = y = z = 0;

    x = ADC_Read(0);
    y = ADC_Read(1);
    z = ADC_Read(2);
    usart_write("\nx= ");
    usart_write((String)x);
    usart_write("\ty= ");
    usart_write((String)y);
    usart_write("\tz= ");
    usart_write((String)z);
    dlay(0.01);
    if (x>=260 && x<=285 && y<=385 && y>=315 && z<=385 && z>=315 ) {            // 270, 400, 350
      if(flag)
      usart_write("\nIDLE@@");
      else
      {
        usart_write("\nIDLE");
      }
      
    }
    else if (x>=285 && x<=345 && y<=420 && y>=380 && z<=385 && z>=315 ) {     // 330, 445, 400
      if(flag)
      usart_write("\n4");    //fwd
      else
      {
        usart_write("\n0");
      }
    }
    else if (x<=315 && x>=280 && y<=330 && y>=290 && z<=385 && z>=315) {                         // 340, 340, 400
      if(flag)
      usart_write("\n5");  //back
      else
      {
        usart_write("\n1");
      }
    }
    else if (x<=330 && x>=300 && y<=365 && y>=335 && z>=290 && z<=315)                         // 360, 400, 340
    {
      if(flag)
      usart_write("\n6"); //left
      else
      {
        usart_write("\n2"); 
      }
    }
    else if (x<=310 && x>=280 && y<=370 && y>=345 && z<=425 && z>=395)                       // 330, 380, 450
    {
      if(flag)
      usart_write("\n7"); //right
      else
      {
        usart_write("\n3");
      }
    }

  }
}
