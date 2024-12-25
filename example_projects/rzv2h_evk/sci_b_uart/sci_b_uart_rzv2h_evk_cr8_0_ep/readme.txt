/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

1. Project Overview:
    The example project demonstrates the typical use of the UART HAL module APIs.
    The project initializes the UART with Baud rate of 115200 bps and GTM module.
    Using a Terminal Program (like Tera Term) user can provide a value & press enter key to set the period 
    of the Pmod LED signal.
    The range of input values are displayed on the JLinkRTTViewer.
    Any failure will also be displayed using JLinkRTTViewer.
    To see user input values on Serial terminal, enable local echo option.

2. Hardware and Software Requirements:
    External Hardware : Pmod USBUART
                      : Pmod LED
    Software          : Serial Terminal/TeraTerm, JLink RTT Viewer v7.96e

3. Hardware settings:
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
    (3) Connect PMOD Type1/1A PIN1-6 on Expansion Board and Pmod LED.
        Connect Pmod USBUART and PMOD Type3/3A PIN1-6 on Expansion Board as follows.

        J2 (Pmod USBUART)       PMOD Type3/3A PIN1-6 (Expansion Board)
        ---                          -------
        |1|---RTS                    |1   7|
        |2|---RXD  --->  TXD      ---|2   8|
        |3|---TXD  --->  RXD      ---|3   9|
        |4|---CTS                    |4  10|
        |5|---GND  --->  GND pin  ---|5  11|
        |6|---VCC  --->  VCC pin  ---|6  12|
        ---                          -------

        Connect J1 on Pmod USBUART with PC.

NOTE:
1. Segger RTT block address is required to connect J-Link RTTViewer. 
   The RTT block address can be confirmed by searching "_SEGGER_RTT" in .map file
   which is located in Debug folder and generated when building.