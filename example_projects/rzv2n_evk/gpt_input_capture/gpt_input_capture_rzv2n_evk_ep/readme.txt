/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

1. Project Overview:
    The example project demonstrates typical use of GPT HAL module APIs.
    User has the provision to input value as per displayed menu through JLinkRTTViewer to select different GPT 
    supported modes (Periodic, PWM, One-Shot).
    In periodic mode, user can enter the time period within the permitted ranges to change the frequency of
    the user Pmod LED.
    In PWM mode, user can enter the duty cycle within the specified range to adjust the intensity of the user Pmod LED.
    In One-Shot mode, output will be displayed on JlinkRTTViewer.
    Any failure will also be displayed on JlinkRTTViewer.

2. Hardware and Software Requirements:
    External Hardware : Jump wire
    Software          : JLink RTT Viewer v7.96e

3. Hardware Settings:
    Boot Mode:
    CA55 coldboot (SCIF boot)

    Board:
    RZ/V2N EVK 

    Hardware Connection:

    (1) Set for DIP switches and jumpers as follow.

        +================+================================+
        | Board setting  | RZ/V2N EVK                     |
        +================+================================+
        | Module board   | DSW1-1  : ON                   |
        |                | DSW1-2  : OFF                  |
        |                | DSW1-3  : ON                   |
        |                | DSW1-4  : OFF                  |
        |                | DSW1-5  : ON                   |
        |                | DSW1-6  : OFF                  |
        |                | DSW1-7  : ON                   |
        |                | DSW1-8  : OFF                  |
        |                |                                |
        |                | DSW2-1  : OFF                  |
        |                | DSW2-2  : OFF                  |
        |                | DSW2-3  : OFF                  |
        |                | DSW2-4  : OFF                  |
        |                | DSW2-5  : OFF                  |
        |                | DSW2-6  : OFF                  |
        |                |                                |
        |                | DSW3-1  : ON                   |
        |                | DSW3-2  : ON                   |
        |                | DSW3-3  : ON                   |
        |                | DSW3-4  : ON                   |
        |                | DSW3-5  : ON                   |
        |                | DSW3-6  : ON                   |
        |                | DSW3-7  : ON                   |
        |                | DSW3-8  : ON                   |
        |                |                                |
        +----------------+--------------------------------+

    (2) Connect the J-Link Base and the connector on the SMARC Module Board with JTAG cable.
        When connecting the conversion adapter "J-Link 9-pin Cortex-M Adapter" made by SEGGER will be required.
    (3) Connect PMOD1/1A PIN 3 (GPT ch3) to PMOD2/2A pin 7 (GPT ch4)

NOTE:

1. Segger RTT block address is required to connect J-Link RTTViewer.
   The RTT block address can be confirmed by searching "_SEGGER_RTT" in .map file 
   which is located in Debug folder and generated when building.
   