 /*----------------------------------------------------------------------------------------------------------------------------
 *
 * [Module]: ADC
 *
 * [File Name]: adc.h
 *
 * [Description]: header file for ADC Driver
 *
 * [Author]:  Shady Ali
 *
------------------------------------------------------------------------------------------------------------------------------*/


#ifndef ADC_H_
#define ADC_H_

#include"std_types.h"
#include"common_macros.h"
#include"micro_config.h"

#define POLLING       /* if we want to use the ADC in polling mode*/
// #define INTERRUPT  /* to use ADC in interrupt mode just define INTERRUPT */

#ifdef INTERRUPT
extern uint16 ADC_data; /* global variable to hold the digital data of ADC  after conversion*/
#endif

/* function prototype responsible for intializing ADC*/
void ADC_init(void);

/* function prototype to get analog data from ADC channel
 * and convert it to digital
 */
#ifdef POLLING
uint16 ADC_readChannel(uint8 channel_num);
#endif

#ifdef INTERRUPT
void ADC_readChannel(uint8 channel_num);
#endif

#endif /* ADC_H_ */
