/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

1. Project Overview:
    The example project shows the operation of CAN-FD running on Renesas RZ MCUs using channel 0 and channel 1 on board. 
    On pressing any key on the Terminal Emulator, data is transmitted from one channel to other.
    On 1st transmission, Channel 0 transmits data to Channel 1. Channel 1 displays the received data.
    On 2nd transmission, Channel 1 transmits updated data to Channel 0 as ACK. Channel 0 displays the received data.
    then Channel 0 changes CAN frame to CANFD frame and updates data
    On 3rd transmission, Channel 0 transmits updated data to Channel 1. Channel 1 displays the received data.
    then Channel 1 changes CAN frame to CANFD frame and updates data to transmits back to Channel 0 as ACK.
    On 4th transmission, Channel 1 transmits updated data to Channel 0 as ACK. Channel 0 displays the received data

2. Hardware/Software Requirement:
    Software          : JLink RTT Viewer v7.96e
    External Hardware : 2 CAN transceiver modules

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
		
    (3) Connect PMOD LEDs to Pmod Type3/3A(CN3) (pin 1-6) on RZ/V2H EVK Expansion Board.
	
	(4) Connect Pmod Type1/1A(CN1) PIN1 of the RZ/V2H EVK Expansion Board and Tx Pin of CAN transceiver module 1.
        Connect Pmod Type1/1A(CN1) PIN2 of the RZ/V2H EVK Expansion Board and Rx Pin of CAN transceiver module 1.
        Connect Pmod Type1/1A(CN1) PIN3 of the RZ/V2H EVK Expansion Board and Tx Pin of CAN transceiver module 2.
        Connect Pmod Type1/1A(CN1) PIN4 of the RZ/V2H EVK Expansion Board and Rx Pin of CAN transceiver module 2.
        Share the VCC and GND of 2 CAN transceiver modules and then connect them to the corresponding VCC and GND pins on the RZ/V2H EVK Expansion Board. (CN1 PIN 12 connect to VCC, CN1 PIN 11 connect to GND)
        Connect the Can_H Pin of CAN transceiver module 1 to Can_H Pin of CAN transceiver module 2.
        Connect the Can_L Pin of CAN transceiver module 1 to Can_L Pin of CAN transceiver module 2
    
NOTE:
1. Segger RTT block address is required to connect J-Link RTTViewer. 
   The RTT block address can be confirmed by searching "_SEGGER_RTT" in .map file
   which is located in Debug folder and generated when building.