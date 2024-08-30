/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
1. Project Overview:
    The example project demonstrates the function of r_adc_e driver.
    The example project demonstrates the typical use of the ADC HAL module APIs.
    The project initializes the ADC in single scan mode in accordance with FSP configuration.
    Result and ADC status is displayed on the JLink RTT Viewer.
	
2. Hardware/Software Requirement:
    Software          : JLink RTT Viewer v7.96e
    External Hardware : External Variable Power Supply Unit (Rheostats 10K).

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
		
    (3) Connect ANI000 on RZ/V2H Secure Evaluation Board and External Variable Power Supply Unit (Rheostats 10K).

NOTE:
1. Segger RTT block address is required to connect J-Link RTTViewer. 
   The RTT block address can be confirmed by searching "_SEGGER_RTT" in .map file 
   which is located in Debug folder and generated when building.