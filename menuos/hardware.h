/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/

/***
 *  This file define any links to hardware API or drivers.
 *  This file is used for RLN electro buttons and text LCD definition 
 *  
 */
 #ifndef HARDWARE_H_
 #define HARDWARE_H_
 
 #define RLN_ELECTRO_2012_SE
 
 #define BUTTONSDDR DDRG
 #define BUTTONSPORT PORTG
 #define BUTTONSPIN PING
 #define BUTTONSMASK 0x1F
 #define BSLOTS 6
 
 /**Button mask*/
 enum{
	BUTTONRETURN = 0x01,
	BUTTONLEFT = 0x02,
	BUTTONRIGHT = 0x10,
	BUTTONSUP = 0x04,
	BUTTONDOWN = 0x08,
	BUTTONENTER = 0x10
 }
 
 /**button slot number
  *  NOTICE: RIGHT and ENTER hase the same slot by default! MenuOS do not use ENTER button!
  */
 enum{
	BUTTONSLOTRETURN = 0,
	BUTTONSLOTLEFT = 1,
	BUTTONSLOTRIGHT = 2,
	BUTTONSSLOTUP = 3,
	BUTTONSLOTDOWN = 4,
	BUTTONSLOTENTER = 2	
 }
 /**
 *  \brief Return number of buttons
 *  
 *  \return Return_Description
 *  
 *  \details Details
 */
 uint8_t HwButtonsCount();  
 
 /**
 *  \brief Configure buttons port
 *  
 *  \param [in] mode not used
 *  \return buttons mask
 *  
 *  \details Details
 */
uint8_t HwButtonsSetup(uint8_t mode);


/**
 *  \brief Return pressed buttons mask
 *  
 *  \param [in] mode not used
 *  \return current buttons mask
 *  
 *  \details Details
 */
uint8_t HwButtonsState(uint8_t mode); 
 
 
uint8_t HwDispSetup(uint8_t mode);


 
 #endif
 