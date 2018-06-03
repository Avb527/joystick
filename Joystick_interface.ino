#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
void led_config(void)
{
  DDRB = DDRB| 0x20;
  PORTB = PORTB & 0xDF;
}

void led_on(void)
{
  PORTB = PORTB|0x20;
}

void led_off(void)
{
  PORTB = PORTB&0xDF;
}

void adc_pin_config (void)
{
 DDRC = DDRC|0xC0; //set PORTF direction as input
 PORTC = PORTC&0xC0; //set PORTF pins floating
}

void port_init(void)
{
  led_config();
  adc_pin_config();
}

void adc_init()
{
  ADCSRA = 0x00;
  ADCSRB = 0x00;    //MUX5 = 0
  ADMUX = 0x40;   //Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
  ACSR = 0x80;
  ADCSRA = 0x87;    //ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}

unsigned int ADC_Conversion(unsigned int Ch)
{
  unsigned int a;
  unsigned int b;
  if(Ch>7)
  {
    ADCSRB = 0x08;
  }
  Ch = Ch & 0x07;       
  ADMUX= 0x40| Ch;        
  ADCSRA = ADCSRA | 0x40;   //Set start conversion bit
  while((ADCSRA&0x10)==0);  //Wait for ADC conversion to complete
  a=ADCL;
  b=ADCH;
  ADCSRA = ADCSRA|0x10; //clear ADIF (ADC Interrupt Flag) by writing 1 to it
  ADCSRB = 0x00;
  return (b<<8)|a;
}

void init_devices (void)
{
 cli(); //Clears the global interrupts
 port_init();
 adc_init();
 sei(); //Enables the global interrupts
}

void setup(){
  Serial.begin(9600);
  init_devices();
}

void loop() 
{
  int x_axis=ADC_Conversion(0);
  int y_axis=ADC_Conversion(1);
  Serial.print("X_axis = ");
  Serial.print(x_axis);
  Serial.print("\t");
  Serial.print("Y_axis = ");
  Serial.print(y_axis);
  Serial.print("\n");
  if(x_axis>=1000)
  {
    led_on();
  }
  if(x_axis<100)
  {
    led_off();
  }
}

