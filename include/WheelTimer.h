#ifndef __WHEEL_TIMER_H
#define __WHEEL_TIMER_H

#include <pthread.h>

typdedef struct _wheel_timer_t {

	int current_clock_tic;	/*Pointer to current slot, pointed by clock tic*/
	int clock_tic_interval; /*Time interval of each clock tick*/
	int wheel_size;			/*No. of slots in wheel timer*/
	int current_cycle_no;	/*No. of rotation completed by wheel timer clock tic*/
	pthread_t wheel_thread; /*Thread where the wheel timer clock run separately*/
	/*@@@@@@@@@@@@*/
	ll_t* slots[0];			/*Array of linked list, where each index in array represent a slot*/
	/*@@@@@@@@@@@@*/

} wheel_timer_t;


#endif