/*
 * MTask.h - cooperative task manager of MenuOS
 *
 * Created: 02.01.2013 18:30:36
 *  Author: Artem Kashkanov
 */


#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_

#include <stdint.h>
#include "port/port.h"

struct MAppStruct{
	void (*ptr)();
	uint32_t time;
	volatile uint32_t tick;
};


#define PSLOTS 8 //App slots number

/**
 *  Cooperative scheduler.
 *
 *  A 1 ms hardware tick (see port.h) counts every slot down; the main loop
 *  calls a slot function when its counter reaches zero and reloads it.
 *  Slot functions run in the main loop, never in the interrupt.
 */
class MTask{

public:

MTask(void);

~MTask();

/**
 *  \brief Task Manager Setup: clear all slots
 */
void Setup(void);

/**
 *  \brief Start the tick and run the main loop
 *  NOTICE: Never returns back!
 */
void Start(void);

/**
 *  \brief One pass of the main loop: poll buttons, run due tasks
 */
void Poll(void);

/**
 *  \brief Start the hardware tick
 */
void HwStart(void);

/**
 *  \brief Stop the hardware tick
 */
void HwStop(void);

/**
 *  \brief Add function to slot
 *
 *  \param [in] slot number of slot
 *  \param [in] _f function pointer
 *  \param [in] periodic call period in ms
 */
void Add(uint8_t slot, void (*_f)(), uint32_t periodic);

/**
 *  \brief Release active slot
 *
 *  \param [in] slot slot to release
 */
void Release(uint8_t slot);

/**
 *  \brief Tick handler, called from the 1 ms interrupt
 */
void Search();

/**
 *  \brief time after start in ms
 *
 *  \return time in ms
 */
uint32_t Millis();


uint8_t ActiveApp;

private:

	MAppStruct App[PSLOTS];

	volatile uint32_t timemillis;
};

extern MTask Task;
#endif /* TASKMANAGER_H_ */
