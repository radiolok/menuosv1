/*
 * taskmanager.h
 *
 * Created: 02.01.2013 18:30:36
 *  Author: Artem
 */ 


#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_

#include <avr/interrupt.h>
#include <inttypes.h>


extern "C" void TIMER5_OVF_vect(void)  __attribute__ ((signal));

struct MAppStruct{
	void (*ptr)();
	uint32_t time;
	uint32_t tick;
};


#define PSLOTS 8 //App slots number

class MTask{
	
public:

friend void TIMER5_OVF_vect();


/**
 *  \brief Task Manager Setup
 *  
 *  \return none
 *  
 *  \details Details
 */
MTask(void);

~MTask();

/**
 *  \brief Start soft work Task Manager
 *  NOTICE: Never returns back!
 *  
 *  \return none
 *  
 *  \details Details
 */
void Start(void);

/**
 *  \brief Start TManager
 *  
 *  \return none
 *  
 *  \details Details
 */
void HwStart(void);

/**
 *  \brief Stop TManager
 *  
 *  \return none
 *  
 *  \details Details
 */
void HwStop(void);

/**
 *  \brief Add function to slot
 *  
 *  \param [in] slot number of slot
 *  \param [in] _f function pointer
 *  \param [in] periodic call period in ms
 *  \return none
 *  
 *  \details Details
 */
void Add(uint8_t slot, void (*_f)(), uint32_t periodic);

/**
 *  \brief Release active slot
 *  
 *  \param [in] slot slot to release
 *  \return none
 *  
 *  \details Details
 */
void Release(uint8_t slot);

/**
 *  \brief Periodic function
 *  
 *  \return none
 *  
 *  \details Details
 */
void Search();

/**
 *  \brief time after start in ms
 *  
 *  \return time in ms
 *  
 *  \details Details
 */
uint32_t Millis();


uint8_t ActiveApp;

private:

	MAppStruct App[PSLOTS];
	
	uint32_t timemillis;
};

extern MTask Task;
#endif /* TASKMANAGER_H_ */