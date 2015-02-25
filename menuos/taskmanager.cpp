/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/
#include "taskmanager.h"

#define TIMETICK 100 //0.01мс

	
void taskmanager::TSetup()//установка таймера.
{
  DDRJ|=(1<<PJ2)|(1<<PJ0);
  TIMSK2 &= ~(1<<TOIE2);//отключили перерывание по переполнению
  TCCR2A &= ~((1<<WGM21) | (1<<WGM20));//обычный режим
  TCCR2B &= ~(1<<WGM22);
  ASSR &= ~(1<<AS2);//режим тактирования от внутреннего источника.
  TIMSK2 &= ~(1<<OCIE2A);//отключили прерывание по сравнению, мало ли...
 TIMSK2 &= ~(1<<OCIE2B);//отключили прерывание по сравнению, мало ли...  
  TCCR2B |= (1<<CS22)|((1<<CS21) | (1<<CS20));//прескалер на 1024.
  TCNT2 = TIMETICK;//как бы 10мс. (9.986 если быть точнее)
  for (uint8_t _i=0;_i<PSLOTS;_i++)
	{TPpresc[_i][0]=0;
	TPpresc[_i][1]=0;
	}
	WorkType=0;
	Forse=0;
}
/**
 *  \brief Enable TManager
 *  
 *  \return void
 *  
 *  \details Details
 */
void taskmanager::TStart()
{
  TIMSK2 |= (1<<TOIE2);
}

/**
 *  \brief Add function t slot
 *  
 *  \param [in] _pslot slot number
 *  \param [in] _f update period [10ms
 *  \return void
 *  
 *  \details Details
 */
void taskmanager::TReload(uint8_t _pslot, void (*_f)(), uint16_t _ppresc)
{
  progslot[_pslot]=_f;
  TPpresc[_pslot][0]=_ppresc;
}

/**
 *  \brief Stop TManager
 *  
 *  \return void
 *  
 *  \details Details
 */
void taskmanager::TStop()
{
  TIMSK2 &= ~(1<<TOIE2);
}

/**
 *  \brief TManager ISR function
 *  
 *  
 *  \details Details
 */
ISR(TIMER2_OVF_vect) {
  PORTJ|=(1<<PJ2);
  TCNT2 = TIMETICK;//update period
  cli();
  for (uint8_t _j=0;_j<PSLOTS;_j++)
  {//check times
    if (TManager.TPpresc[_j][0]){//if slot is active
		TManager.TPpresc[_j][1]++;//add time
		if (TManager.TPpresc[_j][0]==TManager.TPpresc[_j][1]){//if is time!
		TManager.TPpresc[_j][1]=0;//clear timer
		(*TManager.progslot[_j])();//execute program. INTERRUPT [DEPRECATED!!!!!]
		}
  }
  } 
  sei();
  PORTJ&=~(1<<PJ2);
}
