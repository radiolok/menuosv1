/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/




#include "config.h"
#include <string.h>//Работа функций копирования буфера
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "definitions.h"

uint16_t ConfigDefinition[CONFIGARRAYWEIGHT+1];//config setup and current value
uint8_t ConfigArrayParam[4];//StartX, StartY, cursor, dynamic config
int8_t CursorMax;

//low-level eeprm functions
uint16_t ConfigReadWord(uint16_t EEPROMCell){
  return eeprom_read_word ((uint16_t *) EEPROMCell);
}

void ConfigWriteWord(uint16_t EEPROMCell, uint16_t EEPROMValue){
  eeprom_write_word ((uint16_t *) EEPROMCell, EEPROMValue);
}

uint8_t ConfigReadByte(uint8_t EEPROMCell){
  return eeprom_read_byte ((unsigned char *) EEPROMCell);
}

void ConfigWriteByte(uint8_t EEPROMCell, uint8_t EEPROMValue){
  eeprom_write_byte ((unsigned char *) EEPROMCell, EEPROMValue);
}

/**
 *  \brief Setup config utility and start redact
 *  
 *  \param [in] StartX config X position
 *  \param [in] StartY Cnfig Y position
 *  \param [in] ConfigName cnfig name string
 *  \param [in] ConfigNumber 
 *  \param [in] DynConfig dynamic config flag
 *  \return Return_Description
 *  
 *  \details Details
 */
void ConfigSetup(uint8_t StartX, uint8_t StartY,char* ConfigName, uint8_t ConfigNumber,uint8_t DynConfig){
	//read data
	CursorMax=0;
	ConfigArrayParam[3]=DynConfig;
	ConfigReadLimits(ConfigNumber,DynConfig);
	BSlot(BUTTONSLOTLEFT,ConfigLeftButton);//left button
	BSlot(BUTTONSLOTRIGHT,ConfigRightButton);//rigth button
	BSlot(BUTTONSLOTUP,ConfigUpButton);//uo button
	BSlot(BUTTONSLOTDOWN,ConfigDownButton);//down button
	BSlot(BUTTONSLOTRETURN,ConfigReturnButton);//return button
	BSlot(BUTTONSLOTENTER,ConfigEnterButton);//accept button
//show info
ConfigView(StartX,StartY,ConfigName,ConfigNumber,1);
}

/**
 *  \brief Read config data from eeprom
 *  
 *  \param [in] ConfigNumber system config number
 *  \param [in] DynConfig dynamic config flag
 *  \return Return_Description
 *  
 *  \details Details
 */
void ConfigReadLimits(uint8_t ConfigNumber,uint8_t DynConfig)
{
  for (int i=0;i<CONFIGARRAYWEIGHT;i++)
    ConfigDefinition[i]=pgm_read_word(&configsLimit[ConfigNumber*CONFIGARRAYWEIGHT+i]);
	ConfigDefinition[CONFIGARRAYWEIGHT]=ConfigReadWord(ConfigDefinition[2]+DynConfig*2);
}


void ConfigView(uint8_t StartX, uint8_t StartY, char* ConfigName,uint8_t ConfigNumber,uint8_t ConfigMode){
	GLCD.SelectFont(SystemFont5x7);
	GLCD.FillRect(0,StartY,128,MENUFOTNHIGHT,WHITE);
	GLCD.CursorToXY(StartX+1,StartY+1);
	char ConfigNameCut[15];
	strlcpy(ConfigNameCut,ConfigName,sizeof(ConfigNameCut));//cut long string
	GLCD.Puts(ConfigNameCut);
	ConfigArrayParam[0]=StartX+sizeof(ConfigNameCut)*6;
	ConfigArrayParam[1]=StartY;
	if (ConfigMode){//config mode, refresh
		ConfigRefresh();
	}
	else{
		uint16_t ConfigValue=ConfigReadWord(pgm_read_word(&configsLimit[ConfigNumber*CONFIGARRAYWEIGHT+(CONFIGARRAYWEIGHT-1)]));//прочитали значение
		uint16_t ConfigMaxValue=pgm_read_word(&configsLimit[ConfigNumber*CONFIGARRAYWEIGHT+1]);//прочитали значение
		CursorMaxDefine(ConfigMaxValue,1);
		if (CursorMax>=0){//numeric, not boolean
			CursorMaxDefine(ConfigValue,0);
		GLCD.CursorToXY(StartX+1+(sizeof(ConfigNameCut)+(4-CursorMax))*MENUFONTWEIGHT, StartY+1);
			//if (ConfigValue<0)//print minus
			//	GLCD.PutChar("-");
			GLCD.PrintNumber(ConfigValue);
		}
		else{//booplean
		
			GLCD.CursorToXY(StartX+1+sizeof(ConfigNameCut)*MENUFONTWEIGHT, StartY+1);	
			if (ConfigValue)
				GLCD.Puts("Yes");
			else
				GLCD.Puts("No ");			
		}
	}
	
}
void CursorMaxDefine(uint16_t _Value,uint8_t _mode){
		if ((_Value==1) && _mode) CursorMax=-1;//boolean
		 else if (_Value<10) CursorMax=0;
			else if (_Value<100) CursorMax=1;
				else if (_Value<1000) CursorMax=2;
					else if (_Value<10000) CursorMax=3;
						else if (_Value<=65535) CursorMax=4;
}

/**
 *  \brief Refresh config data, called only after primary ConfigView and every ConfigSetup. block position cheanging
 *  
 *  \return void
 *  
 *  \details Details
 */
void ConfigRefresh(void){
	GLCD.FillRect(ConfigArrayParam[0],ConfigArrayParam[1],(GLCD.Width-ConfigArrayParam[0]-2),8,WHITE);//clear 
	GLCD.CursorToXY(ConfigArrayParam[0]+1,ConfigArrayParam[1]+1);
	CursorMaxDefine(ConfigDefinition[1],1);
	if (CursorMax>=0){	
		uint16_t DataUnsignedLog=ConfigDefinition[CONFIGARRAYWEIGHT];//delete sign
		uint8_t DataDigits[5]={0};
		for (uint8_t digits_count=0;digits_count<5;digits_count++)
		  {//separate digits with base
		  if (DataUnsignedLog>0){          
			DataDigits[digits_count] = DataUnsignedLog % 10; // copy digit to array
			DataUnsignedLog /= 10;        //next digit
			  }
			  else{
				  DataDigits[digits_count]=0;                               
				  }
			}
		uint8_t NonDigit=0;
		for (uint8_t digits_count=4;digits_count<254;digits_count--){
			if (DataDigits[digits_count]||(!digits_count))
				NonDigit=1;
			if (NonDigit){
				GLCD.PrintNumber(DataDigits[digits_count]);
			}
			else{
				GLCD.Puts(" ");.
			}
		}
		GLCD.InvertRect(ConfigArrayParam[0]+6*(4-ConfigArrayParam[2]),ConfigArrayParam[1],6,8);//Курсор, чо :)
	}
	else{
		if (ConfigDefinition[CONFIGARRAYWEIGHT])
			GLCD.Puts("Yes");
		else
			GLCD.Puts("No ");
	}


}

void ConfigUpButton(){
	if (CursorMax>=0){
	ConfigDefinition[CONFIGARRAYWEIGHT]+=UIntPow(10,ConfigArrayParam[2]);//если больше большего
	if (ConfigDefinition[1]<ConfigDefinition[CONFIGARRAYWEIGHT])
		ConfigDefinition[CONFIGARRAYWEIGHT]=ConfigDefinition[1];
	}
	else 
	ConfigDefinition[CONFIGARRAYWEIGHT]=!ConfigDefinition[CONFIGARRAYWEIGHT];		
	ConfigRefresh();
}

void ConfigDownButton(){
	if (CursorMax>=0){
	ConfigDefinition[CONFIGARRAYWEIGHT]-=UIntPow(10,ConfigArrayParam[2]);//если меньше меньшего
	if ((ConfigDefinition[1]*2<ConfigDefinition[CONFIGARRAYWEIGHT])||(ConfigDefinition[0]>ConfigDefinition[CONFIGARRAYWEIGHT]))
		ConfigDefinition[CONFIGARRAYWEIGHT]=ConfigDefinition[0];
			}
	else 
	ConfigDefinition[CONFIGARRAYWEIGHT]=!ConfigDefinition[CONFIGARRAYWEIGHT];	
	ConfigRefresh();
}
void ConfigLeftButton(){
	if (CursorMax>0)
		if (CursorMax<(++ConfigArrayParam[2])) ConfigArrayParam[2]=0;
	ConfigRefresh();
}

void ConfigRightButton(){
	if (CursorMax>0)
	if (254<(--ConfigArrayParam[2])) ConfigArrayParam[2]=CursorMax;
	ConfigRefresh();
}

/**
 *  \brief Save data to eeprom
 *  
 *  \return void
 *  
 *  \details Details
 */
void ConfigEnterButton(){
	ConfigWriteWord(ConfigDefinition[2]+ConfigArrayParam[3]*2,ConfigDefinition[CONFIGARRAYWEIGHT]);
	//GLCD.CursorToXY(ConfigArrayParam[0],ConfigArrayParam[1]);
	//GLCD.Puts("SAVED");
	//MessageBox needed
	ConfigRefresh();
}

void ConfigReturnButton(){
	ConfigEnterButton();	
	ConfigRefresh();
	TManager.WorkType=0;
}

/**
 *  \brief Power integer
 *  
 *  \param [in] _data number to power
 *  \param [in] _pow power
 *  \return number^power
 *  
 *  \details 
 */
uint16_t UIntPow(uint16_t _data, uint8_t _pow){
        uint16_t _result=1;
        if (!_pow)
			return 1;
        while (_pow){
                _result*=_data;
                _pow--;
        }
        return _result;         
}