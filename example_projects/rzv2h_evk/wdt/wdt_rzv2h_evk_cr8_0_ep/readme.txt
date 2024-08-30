/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

1. Project Overview:
  The example project demonstrates the use of WDT module with the collaboration of module IRQ, GTM, WDT and button/led. 
  The operation of this EP is as below:
	- Start GTM timer having a callback every 1 second, to refresh WDT and blink the led (on Secure Evaluation Board).
	- If the button BTN0 (on PMOD BUTTON) is pressed, IRQ is triggered, the LED is OFF and program is 
    exit.

2. Hardware Requirement:
    External Hardware : PMOD BUTTON

3. Hardware Settings:
    Bootmode:
    CA55 coldboot (SCIF boot)

    Board:
    RZ/V2H EVK-Ver1

    Hardware Connection:

    (1) Set for DIP switches and jumpers as follow.
        - DSW1 : 1-ON,2-OFF,3-ON,4-OFF,5-ON,6-OFF,7-ON,8-OFF.
        - Others : Default.
    (2) Connect the J-Link Base and the connector on the SMARC Module Board with JTAG cable.
        When connecting the conversion adapter "J-Link 9-pin Cortex-M Adapter" made by SEGGER will be required.
    (3) Connect PMOD Button to Pmod Type2/2A(CN2) (pin 7-12) on RZ/V2H EVK Expansion Board.
	
NOTE:
1. Segger RTT block address is required to connect J-Link RTTViewer. 
   The RTT block address can be confirmed by searching "_SEGGER_RTT" in .map file 
   which is located in Debug folder and generated when building.