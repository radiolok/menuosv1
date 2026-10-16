/*
 * MTask.cpp - cooperative task manager of MenuOS
 *
 * Created: 02.01.2013 18:30:26
 *  Author: Artem Kashkanov
 */


#include "MTask.h"

#include "buttons.h"
#include "hw/hwi.h"

MTask::MTask(){
	timemillis = 0;
	ActiveApp = 0;
}

MTask::~MTask(){
}

void MTask::Setup(){
	ActiveApp = 0;
	for (uint8_t slot = 0; slot < PSLOTS; slot++)
	{//clear all slots
		Release(slot);
	}
}

void MTask::Start(){
	HwStart();
	while(1)
	{//SwRoutine
		Poll();
		PortIdle();
	}
}

void MTask::Poll(){
	Buttons.Search(Millis());
	for (uint8_t slot = 0; slot < PSLOTS; slot++)
	{
		if ((App[slot].ptr != 0)&&(App[slot].time != 0))
		{
			uint8_t irq = PortIrqSave();
			uint8_t due = (App[slot].tick == 0);
			if (due){
				App[slot].tick = App[slot].time;
			}
			PortIrqRestore(irq);
			if (due){
				(*App[slot].ptr)();//non-interrupt work
			}
		}
	}
	HwDispUpdate();
}

void MTask::HwStart()
{
	timemillis = 0;//reset time counter
	PortTickStart();
}

void MTask::Add(uint8_t slot, void (*_f)(), uint32_t periodic)
{
	if (PSLOTS > slot){
		uint8_t irq = PortIrqSave();
		App[slot].ptr = _f;
		App[slot].time = periodic;
		App[slot].tick = periodic;
		PortIrqRestore(irq);
	}
}

void MTask::Release(uint8_t slot)
{
	if (PSLOTS > slot){
		uint8_t irq = PortIrqSave();
		App[slot].ptr = 0;
		App[slot].time = 0;
		App[slot].tick = 0;
		PortIrqRestore(irq);
	}
}

void MTask::HwStop()//Останавливает работу диспетчера
{
	PortTickStop();
}

uint32_t MTask::Millis(){
	uint8_t irq = PortIrqSave();
	uint32_t t = timemillis;
	PortIrqRestore(irq);
	return t;
}

void MTask::Search(){
	timemillis++;
	for (uint8_t slot = 0; slot < PSLOTS; slot++)
	{//count times
		if (App[slot].ptr != 0){//If App is defined
			if(App[slot].tick != 0){//And time is not yet
				App[slot].tick--;//Count to zero
			}
		}
	}
}

/* Called by the port from the 1 ms timer interrupt. */
void MTaskTick(void){
	Task.Search();
}

/* Called by the host port; same as one main loop pass. */
void MTaskPoll(void){
	Task.Poll();
}
