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


#ifndef TIMER_0_H_
#define TIMER_0_H_

#include"common_macros.h"
#include"micro_config.h"
#include"std_types.h"

/*#define OVERFLOW_MODE */     /* when we want overflow mode */
/*#define CTC_OUTPUT */   /* when we use output compare match mode interrupt */
/*#define CTC  */         /* compare match toggles OC0 pin */
#define FAST_PWM      /* generates fast PWM waveform */

#define START 0            /* start of TCNT0 */


#ifdef OVERFLOW_MODE
void TIMER0_ovfInit(void);  /* intializing timer0 for overflow mode */
extern volatile uint8 g_timerFlag; /* global variable holds the number of overflows in timer0*/
#endif

#ifdef CTC_OUTPUT
void TIMER0_ctcOutPutInit(uint8 a_compareValue); /* intializing timer0 for output compare match mode */
extern volatile uint8 g_timerFlag; /* global variable holds the number of overflows in timer0*/
#endif

#ifdef CTC
void TIMER0_ctcInit(uint8 a_compareValue);
#endif

#ifdef FAST_PWM
void TIMER0_pwmInit(uint8 a_dutyCycle); /* intializing timer0 for generating fast PWM waveform */
/*#define INVERTED*/ /* in case of inverted mode in pwm if we want non inverted just remove it */
#endif



#endif /* TIMER_0_H_ */

