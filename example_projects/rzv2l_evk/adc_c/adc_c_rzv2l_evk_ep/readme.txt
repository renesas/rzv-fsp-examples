/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
1. Project Overview:
    The example project demonstrates the function of r_adc_c driver.
    The example project demonstrates the typical use of the ADC HAL module APIs.
    The project initializes the ADC in single scan mode in accordance with FSP configuration.    
    Result and ADC status is displayed on the JLink RTT Viewer.
	
2. Hardware and Software Requirement:
    External Hardware : External Variable Power Supply Unit (Rheostats 10K).
    Software          : JLink RTT Viewer v7.96e

3. Hardware settings:
    Boot Mode:
    Boot mode 3 (1.8-V Single, Quad, or Octal serial flash memory)
	
    Board:
    RZ/V2L  Evaluation Kit (SMARC)

    Hardware Connection:

    (1) Set for DIP switches and jumpers as follow.

    <<Setting for Module board>>
    - SW1-1 : OFF 
      SW1-2 : Don't care

    <<Setting for Carrier board>>
    - SW1    : Don't care
      SW2    : Don't care
      SW3    : Don't care
      SW4    : Don't care
      SW5    : Don't care
      SW6    : Don't care
      SW7    : Don't care
      SW8    : Don't care
      SW11-1 : OFF
      SW11-2 : OFF
      SW11-3 : OFF
    - SW11-4 : ON

    - CN4    : Jumper connects 1-3
               Jumper connects 2-4

    (2) Connect the J-Link Base and the connector on the SMRAC Module Board with JTAG cable.
	    When connecting the conversion adapter "J-Link 9-pin Cortex-M Adapter" made by SEGGER will be required.
    (3) Connect CN1 CH0 (ADC_CH0) on SMARC Module Board and External Variable Power Supply Unit (Rheostats 10K).

NOTE:
1. Segger RTT block address is required to connect J-Link RTTViewer. 
   The RTT block address can be confirmed by searching "_SEGGER_RTT" in .map file 
   which is located in Debug folder and generated when building.