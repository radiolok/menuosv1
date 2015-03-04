/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#ifndef HWFILE_H_
#define HWFILE_H_

#include "hwdef.h"

#include "menudef.h"


/*===================================================================
					FILE STRUCTURE ACCESS
===================================================================*/

/**
 *  \brief Return file info
 *  
 *  \param [in] filenumber number of selected file
 *  \param [in] file filedata structure
 *  \return 0
 *  
 *  \details Details
 */
uint8_t HwFileGetInfo(uint8_t filenumber, struct filedata*);

/**
 *  \brief Return file info
 *  
 *  \param [in] filenumber number of selected file
 *  \param [in] file byte position to read
 *  \return 0
 *  
 *  \details Details
 */
uint8_t HwFileGetInfo(uint8_t filenumber, uint8_t pos);

/**
 *  \brief Read file name from memory
 *  
 *  \param [in] filenumb file number id
 *  \param [in] name char array
 *  \return 0
 *  
 *  \details Details
 */
uint8_t HwFileGetName(uint8_t filenumb, char* name);

/**
 *  \brief Get file type
 *  
 *  \param [in] filenumb file number id
 *  \return file type
 *  
 *  \details Details
 */
uint8_t HwFileGetType(uint8_t filenumb);


/*===================================================================
					CONFIG STRUCTURE ACCESS
===================================================================*/

/**
 *  \brief Read config data from structure and memory
 *  
 *  \param [in] configid config number
 *  \param [in] config config strtucture
 *  \param [in] shift dynamic config shift
 *  \return 0 if success, 1 in any error
 *  
 *  \details Details
 */
uint8_t HwConfigGetData(uint8_t configid, struct configdata* config, uint16_t shift);

/**
 *  \brief Store config value in memory
 *  
 *  \param [in] configid config number
 *  \param [in] config config structure
 *  \param [in] shift dynamic config shift
 *  \return 0 if success, 1 in any error
 *  
 *  \details Details
 */
uint8_t HwConfigSetData(uint8_t configid, struct configdata* config, uint16_t shift);
 


#endif /* HWFILE_H_ */