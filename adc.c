 /*----------------------------------------------------------------------------------------------------------------------------
 *
 * [Module]: ADC
 *
 * [File Name]: adc.c
 *
 * [Description]: source file for ADC Driver
 *
 * [Author]:  Shady Ali
 *
------------------------------------------------------------------------------------------------------------------------------*/
#include "adc.h"

/*------------------------------------------------------------------------------------------------------------------------------
 * [Function Name]: ADC_init
 * [Description]: funtion responsible for intializing the ADC module
 * [Args]: void
 * [Returns]: void
 -----------------------------------------------------------------------------------------------------------------------------*/


#ifdef INTERRUPT
uint16 ADC_data=0; /* global variable to hold the digital data of ADC  after conversion*/
#endif
void ADC_init(void){
	/* ADMUX Register Bits Description:
	 * REFS1:0 = 00 to choose to connect external reference voltage by input this voltage through AREF pin
	 * 			 01 AVCC with external capacitor at AREF pin
	 * 			 11 Internal 2.56V Voltage Reference with external capacitor at AREF pin
	 * ADLAR   = 0 right adjusted
	 * MUX4:0  = 00000 to choose channel 0 as initialization
	 * 00000=ADC0   ----> 00111=ADC7
	 */
	ADMUX=0;
	/* ADCSRA Register Bits Description:
	 * ADEN    = 1 Enable ADC
	 * ADIE    = 0 Disable ADC Interrupt
	 * ADSC    = 1 ADC start conversion
	 * ADIF        ADC Interrupt Flag
	 * ADPS2:0 = 011 to choose ADC_Clock=F_CPU/8=1Mhz/8=125Khz --> ADC must operate in range 50-200Khz
	 */
#ifdef POLLING
	ADCSRA=(1<<ADEN)|(1<<ADPS1)|(1<<ADPS0);
#endif
#ifdef INTERRUPT
	ADCSRA =(1<<ADEN)|(1<<ADIE)|(1<<ADPS1)|(1<<ADPS0);
#endif
}

/*------------------------------------------------------------------------------------------------------------------------------
 * [Function Name]: ADC_readChannel
 * [Description]: function responsible for getting analog data from ADC channel
 * 				  and convert it to digital
 * [Args]:  uint8 channel_num:
 * 					the analog data
 * [Returns]: this return shall indictes the digital data
 -----------------------------------------------------------------------------------------------------------------------------*/
#ifdef POLLING
uint16 ADC_readChannel(uint8 channel_num){
	/* channel number must be from 0 --> 7 */
	/*inserting the channel number in the first 5 bits of ADMUX register*/
	ADMUX = (ADMUX & 0b11100000) | (channel_num & 0b00011111);
	SET_BIT(ADCSRA,ADSC); /* start conversion write '1' to ADSC */
	while(BIT_IS_CLEAR(ADCSRA,ADIF)); /* wait for conversion to complete ADIF becomes '1' */
	SET_BIT(ADCSRA,ADIF);/* clear ADIF by write '1' to it */
	return ADC; /* return the data register */
}
#endif

#ifdef INTERRUPT
void ADC_readChannel(uint8 channel_num){
	/* channel number must be from 0 --> 7 */
	/*inserting the channel number in the first 5 bits of ADMUX register*/
	ADMUX = (ADMUX & 0xE0) | (channel_num & 0x1f);
	ADCSRA = (1<<ADSC); /* start conversion write '1' to ADSC */
}
ISR(ADC_vect){
	ADC_data = ADC;
	SET_BIT(ADCSRA,ADIF);
}
#endif

