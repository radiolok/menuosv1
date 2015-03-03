/*
 * taskmanager.cpp
 *
 * Created: 02.01.2013 18:30:26
 *  Author: Artem
 */ 


#include "MTask.h"

#include "buttons.h"

#define TIMETICK (6) //0.01мс

MTask::MTask()
{
	ActiveApp = 0;
	TCCR5A |= (1<<WGM50);//PWM 8-bit with 0xFF on TOP
	TCCR5B |= (1<<WGM52);
	TCNT5 = TIMETICK;//1мс
	for (uint8_t slot = 0; slot < PSLOTS; slot++)
	{//clear all slots
		Release(slot);
	}
}

MTask::~MTask(){

}

void MTask::Start(){
	
	HwStart();
	
	while(1)
	{//SwRoutine
		Buttons.Search(timemillis);
		for (uint8_t slot = 0; slot < PSLOTS; slot++)
		{
			if ((App[slot].ptr != 0)&&(App[slot].time != 0))
			{
				if (App[slot].tick == 0)
				{
					App[slot].tick = App[slot].time;
					(*App[slot].ptr)();//non-interrupt work
				}
			}
		}	
	}
}


void MTask::HwStart()
{
	timemillis=0;//reset time counter
	TCCR5B |= (1<<CS51) | (1<<CS50);//prescaler at 64.
	TIMSK5 |= (1<<TOIE5);
}

void MTask::Add(uint8_t slot, void (*_f)(), uint32_t periodic)
{
	if (PSLOTS > slot){
		App[slot].ptr = _f;
		App[slot].time = periodic;
		App[slot].tick = periodic;
	}
}

void MTask::Release(uint8_t slot)
{
	if (PSLOTS > slot){
		App[slot].ptr = 0;
		App[slot].time = 0;
		App[slot].tick = 0;
	}
}

void MTask::HwStop()//Останавливает работу диспетчера
{
	TCCR5B &= ~((1<<CS51) | (1<<CS50));//прескалер на 64.
	TIMSK5 &= ~(1<<TOIE5);
}

uint32_t MTask::Millis(){
	return timemillis;
	
}

void MTask::Search(){
	for (uint8_t slot = 0; slot < PSLOTS; slot++)
	{//count times
		if (App[slot].ptr != 0){//If App is defined
			if(App[slot].tick != 0){//And time is not yet
				App[slot].tick--;//Count to zero
			}
		}
	} 
}


ISR(TIMER5_OVF_vect) {
	TCNT5 = TIMETICK;//update tick
	timemillis++;
	Task.Search();
 }  

