//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /  ` |    |  | |  \ |__  /__`
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------

#include "buttons.h"
#include <sam.h>

//------------------------------------------------------------------------------
//      __   ___  ___         ___  __
//     |  \ |__  |__  | |\ | |__  /__`
//     |__/ |___ |    | | \| |___ .__/
//
//------------------------------------------------------------------------------
/*
#define BUTTON_0 (PORT_PA15)
#define BUTTON_0_GROUP (0)
#define BUTTON_0_PIN (PIN_PA14%32)

#define BUTTON_1 (PORT_PA09)
#define BUTTON_1_GROUP (0)
#define BUTTON_1_PIN (PIN_PA09%32)
*/

//#define btn_0 (PORT_PB09)
//#define btn_1 (PORT_PA04)
//#define btnActive (PORT_PA08)

//------------------------------------------------------------------------------
//     ___      __   ___  __   ___  ___  __
//      |  \ / |__) |__  |  \ |__  |__  /__`
//      |   |  |    |___ |__/ |___ |    .__/
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                __          __        ___  __
//     \  /  /\  |__) |  /\  |__) |    |__  /__`
//      \/  /~~\ |  \ | /~~\ |__) |___ |___ .__/
//
//------------------------------------------------------------------------------
volatile bool btnActive;
volatile bool PB09_high = false;
volatile bool PA04_high  = false;
volatile uint8_t which_button_pressed;
volatile bool joystick_pressed;
volatile bool old_joystick_state;
volatile uint32_t yValue_jstck;

enum {S0_pressed = 1, S1_pressed = 2, S2_pressed = 3, S3_pressed = 4};
//------------------------------------------------------------------------------
//      __   __   __  ___  __  ___      __   ___  __
//     |__) |__) /  \  |  /  \  |  \ / |__) |__  /__`
//     |    |  \ \__/  |  \__/  |   |  |    |___ .__/
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//      __        __          __
//     |__) |  | |__) |    | /  `
//     |    \__/ |__) |___ | \__,
//
//------------------------------------------------------------------------------

//=============================================================================
void buttons_init(void)
{
	
	PORT ->Group[0].DIRCLR.reg = PORT_PA04 | PORT_PA08 | PORT_PA20;//PA20 is the joystick
	PORT ->Group[1].DIRCLR.reg = (1 << 9) | PORT_PB08;
	
	PORT ->Group[0].PINCFG[4].bit.INEN = 1;
	PORT ->Group[0].PINCFG[8].bit.INEN = 1;
	PORT ->Group[1].PINCFG[9].bit.INEN = 1;
	PORT ->Group[0].PINCFG[20].bit.INEN = 1;
	//PORT ->Group[1].PINCFG[8].bit.INEN = 1;
	
	PORT->Group[0].PINCFG[4].bit.PULLEN = 1;
	PORT->Group[0].OUTSET.reg = PORT_PA04; // enable pull-up

	PORT->Group[0].PINCFG[8].bit.PULLEN = 1;
	PORT->Group[0].OUTSET.reg = PORT_PA08; // enable pull-up

	PORT->Group[1].PINCFG[9].bit.PULLEN = 1;
	PORT->Group[1].OUTSET.reg = PORT_PB09; // enable pull-up
	
	PORT->Group[0].PINCFG[20].bit.PULLEN = 1;
	PORT->Group[0].OUTSET.reg = PORT_PA20; // enable pull-up
	
	//PORT->Group[1].PINCFG[8].bit.PULLEN = 1;
	//PORT->Group[1].OUTSET.reg = PORT_PB08; // enable pull-up
	
	
	//PORT->Group[1].DIRCLR.reg = (1 << 0) | (1 << 1) | (1 << 2);
	
}

//==============================================================================
// Buttons are active low
// Button 0 is on PD2
// Button 1 is on PD3
//==============================================================================
bool returnJoyStickButtonPressed()
{
	joystick_pressed = (PORT ->Group[0].IN.reg & PORT_PA20) == 0;
	//old_joystick_state = joystick_pressed;
	return joystick_pressed;
}

//==============================================================================
bool return_old_joystick_state()//dont think I need this function. 
{
	return old_joystick_state;
}

//==============================================================================
/*uint32_t return_joystick_yValue()
{
	yValue_jstck = (PORT ->Group[1].IN.reg & PORT_PB08) == 0;
}*/
//==============================================================================
uint8_t buttons_returnButtonValues()//This does not return the joystick.
{
	which_button_pressed = 0;
	btnActive = (PORT->Group[0].IN.reg & PORT_PA08) == 0;
	PB09_high = (PORT ->Group[1].IN.reg & PORT_PB09) == 0;
	PA04_high = (PORT->Group[0].IN.reg & PORT_PA04) ==0;
	if (!btnActive)
	{
		if(!PA04_high & !PB09_high)
		{
			which_button_pressed = S3_pressed;
		}
		
		if(!PA04_high & PB09_high)
		{
			which_button_pressed = S2_pressed;
		}
		
		if(PA04_high & !PB09_high)
		{
			which_button_pressed = S1_pressed;
		}
		
		if(PA04_high & PB09_high)
		{
			which_button_pressed = S0_pressed;
		}
		
		
	}
	return which_button_pressed;
}

//------------------------------------------------------------------------------
//      __   __              ___  ___
//     |__) |__) | \  /  /\   |  |__
//     |    |  \ |  \/  /~~\  |  |___
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//      __                  __        __        __
//     /  `  /\  |    |    |__)  /\  /  ` |__/ /__`
//     \__, /~~\ |___ |___ |__) /~~\ \__, |  \ .__/
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//        __   __  , __
//     | /__` |__)  /__`   
//     | .__/ |  \  .__/
//
//------------------------------------------------------------------------------
