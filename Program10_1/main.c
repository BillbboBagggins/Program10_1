//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /  ` |    |  | |  \ |__  /__`
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------

#include "sam.h"
#include "buttons.h"
#include "speaker.h"
#include "pwm.h"
#include "timer.h"
#include "led.h"
#include "counter.h"
#include "event.h"
#include "spi.h"

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
volatile uint8_t this_tone = 0;


void init_notes(void) { sim_notes[0] = DSHARP4; 
						sim_notes[1] = B3; 
						sim_notes[2] = GSHARP4; 
						sim_notes[3] = GSHARP3;}

volatile uint32_t millis;
volatile uint8_t buttonValue;

//leds
volatile uint16_t red;
volatile uint16_t blue;
volatile uint16_t green;

volatile uint8_t ledNum = 1;
volatile uint8_t check_blankFlag = 0;

//-----------------------------------------------------------------------------
//      __   __   __  ___  __  ___      __   ___  __
//     |__) |__) /  \  |  /  \  |  \ / |__) |__  /__`
//     |    |  \ \__/  |  \__/  |   |  |    |___ .__/
//
//-----------------------------------------------------------------------------
uint32_t main_return_millis();
void led_always_on();
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
	pwm_init();
	speaker_init();
	init_notes();
	timer_init();
	counter_init();
	spi_init();
	event_init();
	timer_enable();
	counter_enable();

	
    /* Replace with your application code */
    while (1) 
    {
		pwm_enable();
		main_new_time = main_return_millis();
		led_always_on();
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
				pwm_disable();
				break;
			case 1://S0
				state_now = s0_pressed;//turn on the right led, play the right note
				//this_tone = sim_notes[0];
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
		switch(state_now)
		{
			
			case 0:
				pwm_disable();
				this_tone = 0;
				break;
			case s0_pressed://green led
				
				this_tone = sim_notes[3];
				break;
			
			case s1_pressed://red led
				
				this_tone = DSHARP4;
				break;
		
			case s2_pressed://yellow led
				
				this_tone = B3;
				break;
		
			case s3_pressed:// blue led
				
				this_tone = GSHARP3;//sim_notes[2];
				break;
		}
		pwm_set(this_tone);
		
		
    }
}

//-----------------------------------------------------------------------------
//      __   __              ___  ___
//     |__) |__) | \  /  /\   |  |__
//     |    |  \ |  \/  /~~\  |  |___
//
//-----------------------------------------------------------------------------
void button_state_machineLights()
{
	check_blankFlag = counter_get_blankFlag();//check for blank
	if (!check_blankFlag) return;
	counter_clear_blank_flag();
	
}
//-----------------------------------------------------------------------------

void led_always_on()
{
	check_blankFlag = counter_get_blankFlag();//check for blank
	if (!check_blankFlag) return;
	counter_clear_blank_flag();
	red = 500;
	green = 500;
	blue = 500;
	//pwm_disable();
	
		
	switch(buttonValue)
	{
		
			case 0:
				led_write(1, 0, 100, 0);
				led_write(2, 100, 0, 0);
				led_write(3, 200, 200, 0);
				led_write(4, 0, 0, 100);
				break;
			case 1://green led
				green = 1500;
				red = 0;
				blue = 0;
				led_write(1, red, green, blue);
				break;
			
			case 2://red led
				red = 1500;
				green = 0;
				blue = 0;
				led_write(2, red, green, blue);
				
				break;
		
			case 3://yellow led
				red = 1500;
				green = 1500;
				blue = 0;
				led_write(3, red, green, blue);
				
				this_tone = B3;
				break;
		
			case 4:// blue led
				red = 0;
				green = 0;
				blue = 1500;
				led_write(4, red, green, blue);
				
				this_tone = GSHARP3;//sim_notes[2];
				break;
	}
	
	
	
	
	
	spi_write();
}


//-----------------------------------------------------------------------------

uint32_t main_return_millis()
{
	__disable_irq();
	uint32_t return_value = millis;
	__enable_irq();
	return return_value;
}

//-----------------------------------------------------------------------------

void playPause(uint32_t playTime,uint32_t pauseTime)
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