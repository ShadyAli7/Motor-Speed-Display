/*------------------------------------------------------------------------------------------------------------------------------
 *
 * [FILE NAME]:  Shady_Ali_Mini_Project2
 *
 * [AUTHOR]:   Shady Ali
 *
 * [DATE] [CREATED]:  Nov 5, 2020
 *
 * [DESCRIPTION]: using ATmega16 to control 2x16 LCD Display with 8 bits data mode and control
 * 				  DC Motor speed by potentiometer
 *
 -----------------------------------------------------------------------------------------------------------------------------*/
#include"dcmotor.h"
#include"lcd.h"
#include"adc.h"
#include"timer_0.h"

/*------------------------------------------------------------------------------------------------------------------------------
 * [Function Name]: INT1_init
 * [Description]: intializing external interrupt 1
 * [Args]: none
 * [Returns]: no return
 -----------------------------------------------------------------------------------------------------------------------------*/

void INT1_init(void){
	CLEAR_BIT(SREG,7);   			/* disable global interrupt at first */
	CLEAR_BIT(DDRD,PD3);			/* intializing Interrupt 1 pin as input */
	MCUCR=(1<<ISC11)|(1<<ISC10);	/* interrupt1 enabled by raising edge */
	GICR=(1<<INT1);					/* enable INT1 interrupt */
	SET_BIT(SREG,7);				/* enable global interrupt (I-bit) */
}

ISR(INT1_vect){
	DCMOTOR_acw();  /* reverse the motion of DC Motor to acw when INT1 occurs */
}

int main(void){
	uint8 duty_cycle=0;   /* local variable to hold the duty cycle for PWM waveform */
	INT1_init();		  /* intializing INT1 */
	ADC_init();			  /* intializing ADC module */
	LCD_init();			  /* intializing LCD screen */
	LCD_clearScreen();	  /* clear the LCD screen to get ready to write on it */
	LCD_displayString("value of ADC: "); /*printing a message on the LCD */
	DCMOTOR_cw();		  /* starting DC Motor at clock wise direction */
	while(1){
		LCD_goToRowColumn(1,7);		/* moving cursor to row: 1 , column: 7 */
		LCD_intgerToString(ADC_readChannel(0)); /* the value of potentiometer is printed on the screen */
												/* after being converted to digital by ADC */
		duty_cycle=(ADC_readChannel(0))/4;		/* the value of PWM signal duty cycle */
		TIMER0_pwmInit(duty_cycle);				/* intializing timer0 in PWM mode and giving it the duty cycle value */
	}
}

