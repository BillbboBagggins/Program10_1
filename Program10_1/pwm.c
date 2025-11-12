//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /  ` |    |  | |  \ |__  /__`
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------

#include "pwm.h"
#include "sam.h"

//------------------------------------------------------------------------------
//      __   ___  ___         ___  __
//     |  \ |__  |__  | |\ | |__  /__`
//     |__/ |___ |    | | \| |___ .__/
//
//------------------------------------------------------------------------------

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

//==============================================================================
void pwm_init()
{
	// Enable the TCC bus clock (CLK_TCCx_APB)
	PM->APBCMASK.reg |= ((PM_APBCMASK_TCC0) | (PM_APBCMASK_TCC1));

	// Configure the General Clock with the 48MHz clk
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_TCC0_TCC1) |
	GCLK_CLKCTRL_GEN_GCLK0 |
	GCLK_CLKCTRL_CLKEN;
	// Wait for the GCLK to be synchronized
	while(GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);

	// Reset the timer
	//TCC0->CTRLA.reg = TCC_CTRLA_SWRST;
	//while (TCC0->SYNCBUSY.bit.SWRST);
	TCC1->CTRLA.reg = TCC_CTRLA_SWRST;
	while (TCC1->SYNCBUSY.bit.SWRST);

	//// LUPD for TCC0
	//TCC0->CTRLBCLR.bit.LUPD = 1;
	//while(TCC0->SYNCBUSY.bit.CTRLB);

	// LUPD for TCC0
	TCC1->CTRLBCLR.bit.LUPD = 1;
	while(TCC1->SYNCBUSY.bit.CTRLB);

	//set OTMX to allow for CCB
	// check FAQ
	//TCC0->WEXCTRL.bit.OTMX = 0x1;
	TCC1->WEXCTRL.bit.OTMX = 0x0;

	// Configure TCC0 into Normal PWM mode
	//TCC0->WAVE.reg = TCC_WAVE_WAVEGEN_NPWM;   // Normal PWM mode on
	//while (TCC0->SYNCBUSY.bit.WAVE);          // Wait for synchronization
	TCC1->WAVE.reg = TCC_WAVE_WAVEGEN_NPWM;   // Normal PWM mode on
	while (TCC1->SYNCBUSY.bit.WAVE);          // Wait for synchronization
	//
	//TCC0->PER.reg = 0xFFFF;  // Set period
	//while (TCC0->SYNCBUSY.bit.PER);
	//
	//TCC0->CCB[0].reg = TCC0->PER.reg / 2;      // init to half the period
	//while (TCC0->SYNCBUSY.bit.CCB0);           // Wait for synchronization

	//TCC0->CCB[1].reg = TCC0->PER.reg / 2;      // init to half the period
	//while (TCC0->SYNCBUSY.bit.CCB0);
	//TCC0->CCB[2].reg = TCC0->PER.reg / 2;      // init to half the period
	//while (TCC0->SYNCBUSY.bit.CCB0);
	//TCC0->CCB[3].reg = TCC0->PER.reg / 2;      // init to half the period
	//while (TCC0->SYNCBUSY.bit.CCB0);

	TCC1->PER.reg = 0xFFFF;  // Set period
	while (TCC1->SYNCBUSY.bit.PER);

	TCC1->CCB[0].reg = TCC1->PER.reg / 2;      // init to half the period
	while (TCC1->SYNCBUSY.bit.CCB0);           // Wait for synchronization

	// Enable the TCC0 counter
	//TCC0->CTRLA.reg |= TCC_CTRLA_ENABLE;
	//while (TCC0->SYNCBUSY.bit.ENABLE);        // Wait for synchronization
	// Enable the TCC1 counter
	TCC1->CTRLA.reg |= TCC_CTRLA_ENABLE;
	while (TCC1->SYNCBUSY.bit.ENABLE);        // Wait for synchronization


	// Enable peripheral function E on  PA09
	PORT->Group[0].PINCFG[9].bit.PMUXEN = 1;
	PORT->Group[0].PMUX[9/2].bit.PMUXO = PORT_PMUX_PMUXO_E_Val;

	// Enable the peripheral multiplexer for the output pin
	PORT->Group[0].PINCFG[15].bit.PMUXEN = 1;
	PORT->Group[0].PMUX[15/2].bit.PMUXO = PORT_PMUX_PMUXO_F_Val;



	// Enable peripheral function F on  PA20
	//PORT->Group[0].PINCFG[20].bit.PMUXEN = 1;
	//PORT->Group[0].PMUX[10].bit.PMUXE = PORT_PMUX_PMUXE_F_Val;
}


//==============================================================================
void pwm_enable()
{
	//TCC0->CTRLA.bit.ENABLE = 1;
	//while (TCC0->SYNCBUSY.bit.ENABLE);
	TCC1->CTRLA.bit.ENABLE = 1;
	while (TCC1->SYNCBUSY.bit.ENABLE);

}
//==============================================================================
void pwm_disable()
{
	//TCC0->CTRLA.bit.ENABLE = 0;
	//while (TCC0->SYNCBUSY.bit.ENABLE);
	TCC1->CTRLA.bit.ENABLE = 0;
	while (TCC1->SYNCBUSY.bit.ENABLE);
}

//==============================================================================
//  LED is Defined as 0b00000RGB ------------ Value is Duty Cycle
//==============================================================================

//void fade_light()
//{
//	const uint32_t top = TCC0->PER.reg;
//	// Fade up
//	for (uint32_t i = 0; i <= top; i += 1) {
//		TCC0->CCB[0].reg = i;
//		while (TCC0->SYNCBUSY.bit.CCB0);
//		delay_cycles(49);
//	}
//	// Fade down
//	for (uint32_t i = top; i > 0; i -= 1) {
//		TCC0->CCB[0].reg = i;
//		while (TCC0->SYNCBUSY.bit.CCB0);
//		delay_cycles(40);
//	}
//}

//------------------------------------------------------------------------------
//      __   __              ___  ___
//     |__) |__) | \  /  /\   |  |__
//     |    |  \ |  \/  /~~\  |  |___
//
//------------------------------------------------------------------------------

void pwm_set(float freqVal)
{
	TCC1 ->PERB.reg = 48000000UL/freqVal;
	
	TCC1 ->CCB[0].reg =(uint32_t)(48000000UL/(2*freqVal));
	while (TCC1->SYNCBUSY.bit.PER);
}
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
