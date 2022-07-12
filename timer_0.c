/*----------------------------------------------------------------------------------------------------------------------------
 *
 * [Module]: Timer_0
 *
 * [File Name]: timer_0
 *
 * [Description]: header file for timer 0 driver
 *
 * [Author]:  Shady Ali
 *
------------------------------------------------------------------------------------------------------------------------------*/
#include"timer_0.h"

#ifdef OVERFLOW_MODE
volatile uint8 g_timerFlag=0; /* global variable holds the number of overflows in timer0*/
#endif

#ifdef CTC_OUTPUT
volatile uint8 g_timerFlag=0; /* global variable holds the number of compare match in timer0*/
#endif
/**************************************************************************************************************************
 *                                                   OVERFLOW MODE														  *
 **************************************************************************************************************************/
#ifdef OVERFLOW_MODE
/*------------------------------------------------------------------------------------------------------------------------------
 * [Function Name]: TIMER0_init
 * [Description]: intializing timer 0 for over flow mode
 * [Args]: void
 * [Returns]: void
 -----------------------------------------------------------------------------------------------------------------------------*/

void TIMER0_ovfInit(void){
	CLEAR_BIT(SREG,7); /* disable interrupts*/
	/* TCCR0 register ---->      WGM01,WGM00:      (00) overflow mode
	 * 	compare output mode  	 COM01,COM00:      (00) normal port operation OC0 disconnected
	 * 	clock prescalling        CS02,CS01,CS00:   (101) clk/1024 (From prescaler)
	 */
	/* FOC0 bit is set to one if there is no PWM */
	TCCR0 =(1<<FOC0)|(1<<CS02)|(1<<CS01);
	/* TCNT0 register responsible for the start value of timer and counting */
	TCNT0= START;
	/* TIMSK register responsible for enabling the timer interrupt
	 * TOIE0 bit when it is written to one it enables the interrupts in overflow mode */
	SET_BIT(TIMSK,TOIE0);
	SET_BIT(SREG,7); /* enable the global interrupts */
}
/* Interrupt Service Routine when overflow occurs in timer0*/
ISR(TIMER0_OVF_vect){
	g_timerFlag++;  /* global variable increments by one on overflow of timer0*/
	TCNT0= START;
}
#endif
/**************************************************************************************************************************
 *                                            CTC OUTPUT COMPARE MATCH				  								      *
 **************************************************************************************************************************/
#ifdef CTC_OUTPUT
/*------------------------------------------------------------------------------------------------------------------------------
 * [Function Name]: TIMER0_ctcOutPutInit
 * [Description]: intializing timer 0 for compare mode
 * [Args]: uint8 a_compareValue : holds the compare match value
 * [Returns]: void
 -----------------------------------------------------------------------------------------------------------------------------*/

void TIMER0_ctcOutPutInit(uint8 a_compareValue){
	CLEAR_BIT(SREG,7); /* disable interrupts*/
	/* TCCR0 register ---->      WGM01,WGM00:    (10) CTC mode
	 * 	compare output mode  	 COM01,COM00:    (00) normal port operation OC0 disconnected
	 * 	clock prescalling        CS02,CS01,CS00:   (101) clk/1024 (From prescaler)
	 */
	/* FOC0 bit is set to one if there is no PWM */
	TCCR0 =(1<<FOC0)|(1<<WGM01)|(1<<CS02)|(1<<CS01);
	/* TCNT0 register responsible for the start value of timer */
	TCNT0= START;
	/* OCR0 register holds the value of comarison */
	OCR0= a_compareValue;
	/* TIMSK register responsible for enabling the timer interrupt
	 * OCIE0 bit when it is written to one it enables the interrupts in compare output mode */
	SET_BIT(TIMSK,OCIE0);
	SET_BIT(SREG,7); /* enable the global interrupts */
}
/* Interrupt Service Routine when Compare output match occurs in timer0*/
ISR(TIMER0_COMP_vect){
	g_timerFlag++; /* global variable increments by one on compare match*/
	TCNT0= START;
}
#endif
/**************************************************************************************************************************
 *                                                 CTC TOGGLE ON OC0 PIN												  *
 **************************************************************************************************************************/
#ifdef CTC

/*------------------------------------------------------------------------------------------------------------------------------
 * [Function Name]: TIMER0_ctcInit
 * [Description]: intializing timer 0 for compare mode toggle on OC0
 * [Args]: uint8 a_compareValue : holds the compare match value
 * [Returns]: void
 -----------------------------------------------------------------------------------------------------------------------------*/


void TIMER0_ctcInit(uint8 a_compareValue){
	CLEAR_BIT(SREG,7); /* disable interrupts*/
	SET_BIT(DDRB,PB3); /* intializing OC0 as output pin */
	/* TCCR0 register ---->      WGM01,WGM00:    (10) CTC mode
	 * 	compare output mode  	 COM01,COM00:    (01) Toggle OC0 on compare match
	 * 	clock prescalling        CS02,CS01,CS00:   (101) clk/1024 (From prescaler)
	 */
	/* FOC0 bit is set to one if there is no PWM */
	TCCR0 =(1<<FOC0)|(1<<WGM01)|(1<<CS02)|(1<<CS01)|(1<<COM00);
	/* TCNT0 register responsible for the start value of timer */
	TCNT0= START;
	/* OCR0 register holds the value of comarison */
	OCR0= a_compareValue;
	SET_BIT(SREG,7); /* enable the global interrupts */
}
#endif

/**************************************************************************************************************************
 *                                                   FAST PWM ON OC0 PIN												  *
 **************************************************************************************************************************/
#ifdef FAST_PWM
/*------------------------------------------------------------------------------------------------------------------------------
 * [Function Name]: TIMER0_pwmInit
 * [Description]: intializing timer 0 for fast pwm on OC0
 * [Args]: uint8 a_dutyCycle : holds the duty cycle percent
 * [Returns]: void
 -----------------------------------------------------------------------------------------------------------------------------*/

void TIMER0_pwmInit(uint8 a_dutyCycle){
	CLEAR_BIT(SREG,7); /* disable interrupts*/
	SET_BIT(DDRB,PB3); /* intializing OC0 as output pin */
	/* TCCR0 register ---->      WGM01,WGM00:    (11) CTC mode
	 * 	clock prescalling        CS02,CS01,CS00:   (010) clk/8 (From prescaler)
	 */
	/* Non inverted mode  COM01,COM00: (10) signal is high till reaching compare match then it is inverted to low
	 * Inverted mode      COM01,COM00: (11) signal is low till reaching compare match then it is inverted to high
	 */
	/* FOC0 bit is set to zero cause we are using pwm */
#ifdef INVERTED
	TCCR0 =(1<<WGM01)|(1<<WGM00)|(1<<CS02)|(1<<CS01)|(1<<COM01)|(1<<COM00); /* inverted mode*/
#else
	TCCR0 =(1<<WGM01)|(1<<WGM00)|(1<<CS01)|(1<<COM01);  /* non inverted mode */
#endif
	/* TCNT0 register responsible for the start value of timer */
	TCNT0= START;
	/* OCR0 register holds the value of comarison */
	OCR0= a_dutyCycle;  /* Percent of duty cycle = (OCR0/256) if it is inverted this the low period*/
						/* in case non inverted this the high period*/
	SET_BIT(SREG,7); /* enable the global interrupts */
}
#endif






