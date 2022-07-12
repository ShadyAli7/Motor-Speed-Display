/*----------------------------------------------------------------------------------------------------------------------------
 *
 * [Module]: LCD
 *
 * [File Name]: lcd.c
 *
 * [Description]: Source file for LCD driver
 *
 * [Author]:  Shady Ali
 *
------------------------------------------------------------------------------------------------------------------------------*/
#include "lcd.h"
/*------------------------------------------------------------------------------------------------------------------------------
 * [Function Name]: LCD_init
 * [Description]: intializing the LCD screen
 * [Args]: none
 * [Returns]: no return
 -----------------------------------------------------------------------------------------------------------------------------*/
void LCD_init(void){
	LCD_CTRL_PORT_DIR |=(1<<E)|(1<<RS)|(1<<RW);  /*setting control pins as output*/
	#if(DATA_BITS_MODE==4)
		#ifdef UPPER_PORT_PINS
		LCD_DATA_PORT_DIR |= 0xf0;
		#else
		LCD_DATA_PORT_DIR |=0x0f;
		#endif
		LCD_sendCommand(FOUR_BITS_DATA_MODE);
		LCD_sendCommand(TWO_LINE_LCD_FOUR_BIT_MODE);
	#elif(DATA_BITS_MODE==8)
		LCD_DATA_PORT_DIR = 0xff;
		LCD_sendCommand(TWO_LINE_LCD_EIGHT_BIT_MODE);
	#endif
	LCD_sendCommand(CURSOR_OFF);
	LCD_sendCommand(CLEAR_COMMAND);
}

/*------------------------------------------------------------------------------------------------------------------------------
 * [Function Name]: LCD_sendCommand
 * [Description]: function to send the command to lcd
 * [Args]: uint8 command:
 * 				the ASCII for the nedded command
 * [Returns]: no return
 -----------------------------------------------------------------------------------------------------------------------------*/
void LCD_sendCommand(uint8 command){
	CLEAR_BIT(LCD_CTRL_PORT, RS); /* instruction mode RS=0*/
	CLEAR_BIT(LCD_CTRL_PORT, RW); /* write mode RW=0*/
	_delay_ms(1); /* delay for processing Tas = 50ns */
	SET_BIT(LCD_CTRL_PORT, E); /*Enable LCD E=1*/
	_delay_ms(1); /*delay for Tdsw= 190ns*/
#if(DATA_BITS_MODE==4) /*LCD is connected in four bit mode*/
	/* sending the most significant 4 bits of the command*/
#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT=(LCD_DATA_PORT & 0x0f)|(command & 0xf0);
#else
	LCD_DATA_PORT= (LCD_DATA_PORT & 0xf0)|((command & 0xf0)>>4);
#endif
	_delay_ms(1);  /* delay for processing Tdsw = 100ns */
	CLEAR_BIT(LCD_CTRL_PORT,E); /* disable LCD E=0 */
	_delay_ms(1);  /* delay for processing Th = 13ns */
	/* starting to sent the seconed half of command*/
	SET_BIT(LCD_CTRL_PORT,E);  /* Enable LCD E=1 */
	_delay_ms(1);   /* delay for processing Tpw - Tdws = 190ns */
	/* sending the least significant 4 bits of the command*/
#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT= (LCD_DATA_PORT & 0x0f)|((command & 0x0f)<<4);
#else
	LCD_DATA_PORT= (LCD_DATA_PORT & 0xf0)|(command & 0x0f);
#endif
	_delay_ms(1);  /* delay for processing Tdsw = 100ns */
	CLEAR_BIT(LCD_CTRL_PORT,E);  /* disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */
#elif(DATA_BITS_MODE==8) /*LCD is connected in 8 bit mode*/
	LCD_DATA_PORT = command; /* sending the required command to the data bus D0 -> D7*/
	_delay_ms(1);  /* delay for processing Tdsw = 100ns */
	CLEAR_BIT(LCD_CTRL_PORT,E); /* disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */
#endif
}
/*-----------------------------------------------------------------------------------------------------------------------------
 * [Function Name]: LCD_displayCharacter
 * [Description]: displaying one character on the LCD screen by sending it through the data bus
 * [Args]: uint8 data:
 * 				the required data to be displayed
 * [Returns]: none
 ----------------------------------------------------------------------------------------------------------------------------*/

 void LCD_displayCharacter(uint8 data ){
	 SET_BIT(LCD_CTRL_PORT,RS); /* selecting the data register RS=1*/
	 CLEAR_BIT(LCD_CTRL_PORT,RW); /* write mode RW=0*/
	 _delay_ms(1); /* delay for processing Tas = 50ns */
	SET_BIT(LCD_CTRL_PORT, E); /*Enable LCD E=1*/
	 _delay_ms(1); /*delay for Tdsw= 190ns*/
#if(DATA_BITS_MODE==4) /*LCD is connected in four bit mode*/
	/* sending the most significant 4 bits of the command*/
#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT=(LCD_DATA_PORT & 0x0f)|(data & 0xf0);
#else
	LCD_DATA_PORT= (LCD_DATA_PORT & 0xf0)|((data & 0xf0)>>4);
#endif
	_delay_ms(1);  /* delay for processing Tdsw = 100ns */
	CLEAR_BIT(LCD_CTRL_PORT,E); /* disable LCD E=0 */
	_delay_ms(1);  /* delay for processing Th = 13ns */
	/* starting to sent the seconed half of command*/
	SET_BIT(LCD_CTRL_PORT,E);  /* Enable LCD E=1 */
	_delay_ms(1);   /* delay for processing Tpw - Tdws = 190ns */
	/* sending the least significant 4 bits of the command*/
#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT= (LCD_DATA_PORT & 0x0f)|((data & 0x0f)<<4);
#else
	LCD_DATA_PORT= (LCD_DATA_PORT & 0xf0)|(data & 0x0f);
#endif
	_delay_ms(1);  /* delay for processing Tdsw = 100ns */
	CLEAR_BIT(LCD_CTRL_PORT,E);  /* disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */
#elif(DATA_BITS_MODE==8) /*LCD is connected in 8 bit mode*/
	LCD_DATA_PORT = data; /* sending the required command to the data bus D0 -> D7*/
	_delay_ms(1);  /* delay for processing Tdsw = 100ns */
	CLEAR_BIT(LCD_CTRL_PORT,E); /* disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */
#endif
 }
 /*------------------------------------------------------------------------------------------------------------------------------
  * [Function Name]: LCD_displayString
  * [Description]: displaying string on the LCD screen by sending it through the data bus
  * [Args]: const char *str:
  * 				the required string to be displayed on the screen
  * [Returns]: this return shall indictes
  -----------------------------------------------------------------------------------------------------------------------------*/
void LCD_displayString(const char *str){
	uint8 i=0; /*local variable for the increment*/
	while(str[i] != '\0') /* loop to go through each character of the string*/
	{
		LCD_displayCharacter(str[i]); /* displaying the character*/
		i++; /* increment the counter*/
	}
}
/*------------------------------------------------------------------------------------------------------------------------------
 * [Function Name]: LCD_goToRowColumn
 * [Description]: move the cursor to specific location
 * [Args]: uint8 row: the row index
 * 			uint8 col: the column index
 * [Returns]: none
 -----------------------------------------------------------------------------------------------------------------------------*/

void LCD_goToRowColumn(uint8 row, uint8 col){
	uint8 address=0; /* intializing the address variable*/
	/* calculations of required address*/
	switch(row)
	{
	case 0: address=col; /* first row of LCD*/
			break;
	case 1: address=col+0x40;  /* second row of LCD*/
			break;
	case 2: address=col+0x10;  /* third row of LCD*/
			break;
	case 3: address=col+0x50;  /* fourth row of LCD*/
			break;
	}
	/* to write to a specific address in the LCD
	 * we need to apply the corresponding command 0b10000000+Address */
	LCD_sendCommand(address | SET_CURSOR_LOCATION);
}
/*------------------------------------------------------------------------------------------------------------------------------
 * [Function Name]: LCD_displayStringRowColumn
 * [Description]: desplaying string on a specific location on the screen
 * [Args]:  uint8 row: the row index
 * 			uint8 col: the column index
 * 			const char *str: the required string to be displayed
 * [Returns]: none
 -----------------------------------------------------------------------------------------------------------------------------*/

void LCD_displayStringRowColumn( uint8 row, uint8 col,const char *str){
	LCD_goToRowColumn(row,col); /* go to the required location on the screen*/
	LCD_displayString(str); /* display the required string*/
}

/*------------------------------------------------------------------------------------------------------------------------------
 * [Function Name]: LCD_clearScreen
 * [Description]: sending command to clear the screen
 * [Args]: void
 * [Returns]: void
 -----------------------------------------------------------------------------------------------------------------------------*/

void LCD_clearScreen(void){
	LCD_sendCommand(CLEAR_COMMAND); /*clear display*/
}
/*------------------------------------------------------------------------------------------------------------------------------
 * [Function Name]: LCD_intgerToString
 * [Description]:   changing an intger number to string to be displayed on the screen
 * [Args]: int data :
 * 				the intger number
 * [Returns]: void
 -----------------------------------------------------------------------------------------------------------------------------*/

void LCD_intgerToString(int data){
	char buff[16]; /*string to hold the ASCII*/
	itoa(data,buff,10);
	LCD_displayString(buff);
}
