/*
Copyright (c) [2012-2020] Microchip Technology Inc.  

    All rights reserved.

    You are permitted to use the accompanying software and its derivatives 
    with Microchip products. See the Microchip license agreement accompanying 
    this software, if any, for additional info regarding your rights and 
    obligations.
    
    MICROCHIP SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT 
    WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
    LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT 
    AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP OR ITS
    LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT 
    LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE 
    THEORY FOR ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT 
    LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, 
    OR OTHER SIMILAR COSTS. 
    
    To the fullest extend allowed by law, Microchip and its licensors 
    liability will not exceed the amount of fees, if any, that you paid 
    directly to Microchip to use this software. 
    
    THIRD PARTY SOFTWARE:  Notwithstanding anything to the contrary, any 
    third party software accompanying this software is subject to the terms 
    and conditions of the third party's license agreement.  To the extent 
    required by third party licenses covering such third party software, 
    the terms of such license will apply in lieu of the terms provided in 
    this notice or applicable license.  To the extent the terms of such 
    third party licenses prohibit any of the restrictions described here, 
    such restrictions will not apply to such third party software.
*/
#include "mcc_generated_files/system/system.h"
#include "util/delay.h"

/* Volatile variables to improve debug experience */
static volatile uint16_t adc_reading;
static volatile uint16_t temperature_in_K;
static volatile int16_t temperature_in_degC;

/*
    Main application
*/

int main(void)
{
    SYSTEM_Initialize();

    int8_t sigrow_offset = SIGROW.TEMPSENSE1;  /* Read signed offset from signature row */
	uint8_t sigrow_gain = SIGROW.TEMPSENSE0;    /* Read unsigned gain/slope from signature row */

    while(1)
    {
		/* Calibration compensation as explained in the data sheet */
		adc_reading = ADC0_GetConversion(ADC_MUXPOS_TEMPSENSE_gc) >> 2; /* 10-bit MSb of ADC result with 1.024V internal reference */
		uint32_t temp = adc_reading - sigrow_offset;
		temp *= sigrow_gain; /* Result might overflow 16-bit variable (10-bit + 8-bit) */
		temp += 0x80; /* Add 256/2 to get correct integer rounding on division below */
		temp >>= 8; /* Divide result by 256 to get processed temperature in Kelvin */
		temperature_in_K = temp;
		temperature_in_degC = temperature_in_K - 273;

		_delay_ms(500);
    }     
}