//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /  ` |    |  | |  \ |__  /__`
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------

#include "sam.h"
#include "buttons.h"
//-----------------------------------------------------------------------------
//      __   ___  ___         ___  __
//     |  \ |__  |__  | |\ | |__  /__`
//     |__/ |___ |    | | \| |___ .__/
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//     ___      __   ___  __   ___  ___  __
//      |  \ / |__) |__  |  \ |__  |__  /__`
//      |   |  |    |___ |__/ |___ |    .__/
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//                __          __        ___  __
//     \  /  /\  |__) |  /\  |__) |    |__  /__`
//      \/  /~~\ |  \ | /~~\ |__) |___ |___ .__/
//
//-----------------------------------------------------------------------------

enum {s0_pressed = 1, s1_pressed, s2_pressed, s3_pressed};
volatile uint8_t state_now = 0;
volatile uint8_t next_state = 0;	

volatile uint32_t main_new_time;
volatile uint32_t main_old_time;
volatile uint32_t delayPause_newTime;
volatile uint32_t delayPause_oldTime;

//notes
uint8_t notes_index;
const float GSHARP4= 415.305;//Hz
const float DSHARP4 = 311.127;//Hz
const float B3 = 247.942;
const float GSHARP3 = 207.652;
float sim_notes[4];
void init_notes(void) { sim_notes[0] = DSHARP4; 
						sim_notes[1] = B3; 
						sim_notes[2] = GSHARP4; 
						sim_notes[3] = GSHARP3;}

volatile uint32_t millis;
volatile uint8_t buttonValue;
//-----------------------------------------------------------------------------
//      __   __   __  ___  __  ___      __   ___  __
//     |__) |__) /  \  |  /  \  |  \ / |__) |__  /__`
//     |    |  \ \__/  |  \__/  |   |  |    |___ .__/
//
//-----------------------------------------------------------------------------
uint32_t main_return_millis();
//-----------------------------------------------------------------------------
//      __        __          __
//     |__) |  | |__) |    | /  `
//     |    \__/ |__) |___ | \__,
//
//-----------------------------------------------------------------------------

//=============================================================================
int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	SysTick_Config(48000); 
	buttons_init();
	
    /* Replace with your application code */
    while (1) 
    {
		main_new_time = main_return_millis();
		//check for buttons here.
		if((main_new_time - main_old_time) > 20)
		{
			main_old_time = main_new_time;
			buttonValue = buttons_returnButtonValues();
		}
		
		//button checker
		switch (buttonValue)
		{
			case 0:
				//keep speaker off. 
				break;
			case 1://S0
				state_now = s0_pressed;//turn on the right led, play the right note
				break;
				
			case 2://S1
				state_now = s1_pressed;//same here . . . 
				break;
				
			case 3://S2
				state_now = s2_pressed;
				
				break;
			case 4://S3
				state_now = s3_pressed;
				break;
		}
		
		
    }
}

//-----------------------------------------------------------------------------
//      __   __              ___  ___
//     |__) |__) | \  /  /\   |  |__
//     |    |  \ |  \/  /~~\  |  |___
//
//-----------------------------------------------------------------------------
uint32_t main_return_millis()
{
	__disable_irq();
	uint32_t return_value = millis;
	__enable_irq();
	return return_value;
}


void playPause(uint64_t playTime,uint64_t pauseTime)
{
	
	delayPause_newTime = main_return_millis();
	if((delayPause_newTime - delayPause_oldTime) <playTime)
	{
		pwm_enable();
		
	}
	else if ((delayPause_newTime - delayPause_oldTime) < pauseTime)
	{
		pwm_disable();
		//numTimesPlayed++;
	}
	else
	{
		delayPause_oldTime = delayPause_newTime;
		notes_index++;
	}
	//numTimesPlayed++;
}
//-----------------------------------------------------------------------------
//        __   __   __
//     | /__` |__) /__`
//     | .__/ |  \ .__/
//
//-----------------------------------------------------------------------------
void SysTick_Handler ()
{
	millis++;
}