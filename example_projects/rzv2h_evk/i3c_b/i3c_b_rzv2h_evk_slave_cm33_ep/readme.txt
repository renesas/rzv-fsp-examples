/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

1. Project Overview:
    This example project demonstrates the typical use of the I3C Driver on Renesas RZ MCUs based on Renesas FSP.
    The I3C master on RZ/V2H demonstrates operations associated with an I3C slave running on another RZ/V2H board.
    Once initialization is successful, I3C Master device assigns slave address to I3C slave device through 
    DAA (dynamic address assignment) using I3C common command code (CCC). Once Dynamic Address Assignment is completed,
    the Master EP performs write/read operations, and displays slave device information based on user input.
    If the on-board switch is pressed on the slave side, it will initiate an IBI transfer request.
    Error and info messages will be printed on Jlink RTTViewer.

2. Hardware and Software Requirement:
    External Hardware : Pmod LED, Pmod BUTTON
    Software          : JLink RTT Viewer v7.96e

3. Hardware settings:
    Boot Mode:
    CA55 coldboot (SCIF boot)

    Board:
    RZ/V2H EVK-Ver1

    Hardware Connection:
    (1) Set for DIP switches and jumpers as follow.
        - DSW1 : 1-ON,2-OFF,3-ON,4-OFF,5-ON,6-OFF,7-ON,8-OFF.
        - Others : Default.
    (2) Connect the J-Link Base and the connector on the SMARC Module Board with JTAG cable.
        When connecting the conversion adapter "J-Link 9-pin Cortex-M Adapter" made by SEGGER will be required.
    (3) Connect PMOD Type2/2A PIN7-12 on Expansion Board and Pmod BUTTON.
    (4) Connect PMOD Type1/1A PIN1-6 on Expansion Board and Pmod LED.
    (5) Connect Pins on RZV2H_Expansion Board between Master board and Slave board as follows:
        PMOD Type6/6A (Master board)    |    PMOD Type6/6A (Slave board)
        ----------------------------    |    ---------------------------
        |1|                                         |1|
        |2|                                         |2|
        |3|<---------------------------SCL--------->|3|
        |4|<---------------------------SDA--------->|4|
        |5|<---------------------------GND--------->|5|
        |6|                                         |6|
        ----------------------------         ---------------------------

Note:
1. Segger RTT block address is required to connect J-Link RTTViewer. 
   The RTT block address can be confirmed by searching "_SEGGER_RTT" in .map file 
   which is located in Debug folder and generated when building.