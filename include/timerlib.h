#ifndef __TIMER_WRAP__
#define __TIMER_WRAP__

//To use POSIX TIMER library
#define __USE_POSIX199309 1

#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


/*
* Timer Data Structure, with attributes 
* for controlling timer.
*/

typedef enum {

	TIMER_INIT,
	TIMER_RUNNING,
	TIMER_CANCELLED,
	TIMER_DELETED,
	TIMER_PAUSED,
	TIMER_RESUMED

} TIMER_STATE_T;



typedef struct Timer_ {
	
	/* Timer config */
	timer_t* posix_timer;				//Handle to Posix timer
	void* user_arg;						//Arg to passed to timer callback fun.
	unsigned long exp_time;				//expiration or fire time after timer start
	unsigned long sec_exp_time;			//expiration or fire time after first timer exp., used in periodic timer
	uint32_t threshold;					//No. of times to invoke the timer callback
	void (*cb)(struct Timer_*, void*);	//Function-pointer to timer callback
	bool exponential_backoff;			//If the timer is exponential type

	/*
	* Dynamic Attributes of timer data structure:
	* 
	* Attributes which change dynamically with exec. of
	* program.
	* 
	*/

	unsigned long time_remaining;		//Time left for paused timer for next expiration/launch
	uint32_t invocation_counter;		//No. of times callback is invoked( Useful in periodic/exponential type)
	struct itimerspec ts;
	unsigned long exp_back_off_time;
	TIMER_STATE_T timer_state;			//State of the Timer object

}Timer_t;



/*
* Assignment-1 functions.
*/

unsigned long
timespec_to_millisec(struct timespec* time);

void
timer_fill_itimerspec(struct timespec* ts,
	unsigned long msec);

/*
* Assignment-1 functions.
*/



/*
* Assignment-2 functions
*/

static inline void
timer_delete_user_data(Timer_t* timer) {

	if (timer->user_arg) {
		free(timer->user_arg);
		timer->user_arg = NULL;
	}
}

static inline TIMER_STATE_T
timer_get_current_state(Timer_t* timer) {

	return timer->timer_state;
}

static inline void
timer_set_state(Timer_t* timer, TIMER_STATE_T timer_state) {

	timer->timer_state = timer_state;

}

/*
* Assignment-2 functions
*/



/*Return NULL in timer creation fails, else
return a pointer to Timer object*/

Timer_t*
setup_timer(
	/* Timer Callback with user data and user size */
	void (*)(Timer_t*, void*),
	/* First expiration timer interval in msec*/
	unsigned long,
	/*Subsequent expiration time interval in msec*/
	unsigned long,
	/*Max no of expiration, 0 for infinite */
	uint32_t,
	/* Arg to timer callback */
	void*,
	/* Is timer Exp back off */
	bool
);

/*****APIs' to control timer******/

void
resurrect_timer(Timer_t* timer);

void
start_timer(Timer_t* timer);

unsigned long
timer_get_time_remaining_in_milli_sec(Timer_t* timer);

void
pause_timer(Timer_t* timer);


void
resume_timer(Timer_t* timer);

int
execute_timer(Timer_t* timer, TIMER_STATE_T action);

void
delete_timer(Timer_t* timer);

void
cancel_timer(Timer_t* timer);

void
restart_timer(Timer_t* timer);

void
reschedule_timer(Timer_t* timer,
	unsigned long exp_ti,
	unsigned long sec_exp_ti);

void
print_timer(Timer_t* timer);

bool
is_timer_running(Timer_t* timer);


/*****APIs' to control timer******/

#endif