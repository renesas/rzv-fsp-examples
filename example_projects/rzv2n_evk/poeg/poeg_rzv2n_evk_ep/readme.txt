/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

1. Project Overview:
	This project demonstrates the basic functionalities of POEG driver on Renesas RZV MPUs based on Renesas FSP.
	RZV MPUs is used to demonstrate the various operating modes of a POEG driver.
	GPT channels are configured to generate output via the GTIOCxA and GTIOCxB pins.
	The Port Output can be disabled using the below three options
	1. Using POEG trigger pin - This mode supports disabling GPT output pins based on GTETRG input pin level
	2. Using POEG software API - This mode supports disabling GPT output pins by R_POEG_OutputDisable() software API request
	3. Using POEG trigger GPT output level - This mode supports disabling GPT output pin using POEG trigger GPT output level
	4. Reset the POEG status - This option resets the POEG0 and POEG1 status 
	The Port Output can be enabled using the POEG Reset API.
	The status of each of the mode is displayed on J-Link RTT Viewer.

2. Software Requirement:
    Software          : JLink RTT Viewer v7.96e

3. Hardware Settings:
    Bootmode:
    CA55 coldboot (SCIF boot)

    Board:
    RZ/V2N EVK

    Hardware Connection:

    (1) Set for DIP switches and jumpers as follow.
        - DSW1 : 1-ON,2-OFF,3-ON,4-OFF,5-ON,6-OFF,7-ON,8-OFF.
        - Others : Default.
    (2) Connect the J-Link Base and the connector on the SMARC Module Board with JTAG cable.
        When connecting the conversion adapter "J-Link 9-pin Cortex-M Adapter" made by SEGGER will be required.
		
	(3) Connect P57 (PMOD3_PIN7) to P80 (PMOD1_PIN1) on RZ/V2N EVK Expansion Board.
      
NOTE:
1. Segger RTT block address is required to connect J-Link RTTViewer. 
   The RTT block address can be confirmed by searching "_SEGGER_RTT" in .map file
   which is located in Debug folder and generated when building.
   