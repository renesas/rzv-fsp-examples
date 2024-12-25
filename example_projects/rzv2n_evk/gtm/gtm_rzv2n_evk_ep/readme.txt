/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

1. Project Overview:
    This Example Project demonstrates the functionality of GTM in periodic mode and one-shot mode. On providing any 
    input on the RTTviewer, GTM channel 2 starts in one-shot mode. GTM channel 1 starts in periodic mode when 
    GTM channel 2 expires. Timer in periodic mode expires periodically at a time period specified by user and toggles
    the LED0 on Pmod LED.

2. Hardware and Software Requirements:
    External Hardware : Pmod LED
    Software          : JLink RTT Viewer v7.96e

3. Hardware settings:
    Boot Mode:
    CA55 coldboot (SCIF boot)

    Board:
    RZ/V2N EVK

    Hardware Connection:

    (1) Set for DIP switches as follow :
		- DSW1 : 1-ON,2-OFF,3-ON,4-OFF,5-ON,6-OFF,7-ON,8-OFF.
		- Others : Default.
    (2) Connect the J-Link Base and the connector on the SMARC Module Board with JTAG cable.
        When connecting the conversion adapter "J-Link 9-pin Cortex-M Adapter" made by SEGGER will be required.
    (3) Connect PMOD Type1/1A PIN7-12 on SMARC Carrier Board and Pmod LED.

NOTE:
1. User is expected to enter data of size not exceeding above 15 bytes.
   Operation is not guaranteed for any user input value other than integer, char (i.e. float, special char) through RTT.

2. Segger RTT block address is required to connect J-Link RTTViewer.
   The RTT block address can be confirmed by searching "_SEGGER_RTT" in .map file 
   which is located in Debug folder and generated when building.