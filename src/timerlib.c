#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <errno.h>
#include "timerlib.h"


unsigned long
timespec_to_millisec(struct timespec* time) {

	unsigned long res_time = 0;

	res_time += (unsigned long)time->tv_sec * 1000; // For seconds to be in millisecond.

	res_time += (unsigned long)time->tv_nsec / 1000000; // For nanosecond to millisecond.

	return res_time;
}

void
timer_fill_itimerspec(struct timespec* ts,
	unsigned long msec) {

	memset(ts, 0, sizeof(struct timespec));

	if (!msec) return;

	unsigned long sec = msec / 1000;
	ts->tv_sec = sec;

	unsigned long remaining_msec = msec % 1000;
	ts->tv_nsec = remaining_msec * (1000000);

}


static void
timer_callback_wrapper(union sigval arg) {

	Timer_t* timer = (Timer_t*)(arg.sival_ptr);

	timer->invocation_counter++;

	if (timer->threshold &&
		(timer->invocation_counter > timer->threshold)) {
		cancel_timer(timer);
		return;
	}

	if (timer->timer_state == TIMER_RESUMED) {
		if (timer->sec_exp_time != 0) {
			timer->timer_state = TIMER_RUNNING;
		}
	}

	(timer->cb)(timer, timer->user_arg);

	if (timer->exponential_backoff) {

		assert(timer->exp_back_off_time);
		reschedule_timer(timer,
			timer->exp_back_off_time *= 2, 0);
	}
	else if (timer->timer_state == TIMER_RESUMED) {

		reschedule_timer(timer,
			timer->exp_time, timer->sec_exp_time);

	}
}

/*Return NULL in timer creation fails, else
return a pointer to Timer object*/

Timer_t*
setup_timer(
	/* Timer Callback with user data and user size */
	void (*timer_cb)(Timer_t*, void*),
	/* First expiration timer interval in msec*/
	unsigned long exp_timer,
	/*Subsequent expiration time interval in msec*/
	unsigned long sec_exp_timer,
	/*Max no of expiration, 0 for infinite */
	uint32_t threshold,
	/* Arg to timer callback */
	void* user_arg,
	/* Is timer Exp back off */
	bool exponential_backoff) {

	Timer_t* timer = calloc(1, sizeof(Timer_t));

	timer->posix_timer = calloc(1, sizeof(timer_t));

	timer->user_arg = user_arg;
	timer->exp_time = exp_timer;
	timer->sec_exp_time = sec_exp_timer;
	timer->cb = timer_cb;
	timer->threshold = threshold;
	timer_set_state(timer, TIMER_INIT);
	timer->exponential_backoff = exponential_backoff;

	/* Check for error */
	assert(timer->cb);

	struct sigevent evp;
	memset(&evp, 0, sizeof(struct sigevent));

	evp.sigev_value.sival_ptr = (void*)(timer);

	evp.sigev_notify = SIGEV_THREAD;
	evp.sigev_notify_function = timer_callback_wrapper;

	int rc = timer_create(CLOCK_REALTIME,
		&evp,
		timer->posix_timer);

	assert(rc >= 0);

	timer_fill_itimerspec(&timer->ts.it_value, timer->exp_time);

	if (!timer->exponential_backoff) {
		timer_fill_itimerspec(&timer->ts.it_interval, timer->sec_exp_time);
		timer->exp_back_off_time = 0;
	}
	else {
		timer->exp_back_off_time = timespec_to_millisec(&timer->ts.it_value);
		timer_fill_itimerspec(&timer->ts.it_interval, 0);
	}
	return timer;
}

/*****APIs' to control timer******/

void
resurrect_timer(Timer_t* timer) {

	int rc;
	rc = timer_settime(*(timer->posix_timer), 0, &timer->ts, NULL);
	assert(rc >= 0);
}

void
start_timer(Timer_t* timer) {

	resurrect_timer(timer);
	timer_set_state(timer, TIMER_RUNNING);
}

void
delete_timer(Timer_t* timer) {

	int rc;
	rc = timer_delete(*(timer->posix_timer));
	assert(rc >= 0);
	timer->user_arg = NULL; /* User arg need to be freed by Appln */
	timer_set_state(timer, TIMER_DELETED);
	free(timer->posix_timer);
	free(timer);
}

void
cancel_timer(Timer_t* timer) {

	TIMER_STATE_T timer_curr_state;

	timer_curr_state = timer_get_current_state(timer);

	if (timer_curr_state = TIMER_INIT ||
		timer_curr_state == TIMER_DELETED) {

		return; /* No operations */
	}

	/* Only Paused or running timer can be cancelled */

	timer_fill_itimerspec(&timer->ts.it_value, 0);
	timer_fill_itimerspec(&timer->ts.it_interval, 0);
	timer->time_remaining = 0;
	timer->invocation_counter = 0;
	resurrect_timer(timer);
	timer_set_state(timer, TIMER_CANCELLED);
}

void
pause_timer(Timer_t* timer) {

	if (timer_get_current_state(timer) == TIMER_PAUSED) {
		return;
	}

	timer->time_remaining =
		timer_get_time_remaining_in_milli_sec(timer);

	timer_fill_itimerspec(&timer->ts.it_value, 0);
	timer_fill_itimerspec(&timer->ts.it_interval, 0);

	resurrect_timer(timer);

	timer_set_state(timer, TIMER_PAUSED);
}

void
resume_timer(Timer_t* timer) {

	assert(timer_get_current_state(timer) == TIMER_PAUSED);

	timer_fill_itimerspec(&timer->ts.it_value, timer->time_remaining);
	timer_fill_itimerspec(&timer->ts.it_interval, timer->sec_exp_time);

	timer->time_remaining = 0;

	resurrect_timer(timer);
	timer_set_state(timer, TIMER_RESUMED);
}

unsigned long
timer_get_time_remaining_in_milli_sec(Timer_t* timer) {

	struct itimerspec remaining_time;

	switch (timer->timer_state) {

	case TIMER_INIT:
		break;

	case TIMER_DELETED:
		return ~0;

	case TIMER_PAUSED:
		break;

	case TIMER_CANCELLED:
		return ~0;

	case TIMER_RUNNING:
		break;

	default:;

	}

	memset(&remaining_time, 0, sizeof(struct itimerspec));

	timer_gettime(*timer->posix_timer, &remaining_time);

	return timespec_to_millisec(&remaining_time.it_value);

}

void
restart_timer(Timer_t* timer) {

	assert(timer->timer_state != TIMER_DELETED);

	cancel_timer(timer);

	timer_fill_itimerspec(&timer->ts.it_value,
		timer->exp_time);

	if (!timer->exponential_backoff) {
		timer_fill_itimerspec(&timer->ts.it_interval, timer->sec_exp_time);
	}
	else {
		timer_fill_itimerspec(&timer->ts.it_interval, 0);
	}

	timer->invocation_counter = 0;
	timer->time_remaining = 0;
	timer->exp_back_off_time = timer->exp_time;
	resurrect_timer(timer);
	timer_set_state(timer, TIMER_RUNNING);
}

void
reschedule_timer(Timer_t* timer,
	unsigned long exp_ti,
	unsigned long sec_exp_ti) {

	uint32_t invocation_counter;
	TIMER_STATE_T timer_state;

	timer_state = timer_get_current_state(timer);

	if (timer_state == TIMER_DELETED) assert(0);

	invocation_counter = timer->invocation_counter;

	if (timer_state != TIMER_CANCELLED) {
		cancel_timer(timer);
	}

	timer->invocation_counter = invocation_counter;
	timer_fill_itimerspec(&timer->ts.it_value, exp_ti);

	if (!timer->exponential_backoff) {
		timer_fill_itimerspec(&timer->ts.it_interval, sec_exp_ti);
	}
	else {
		timer_fill_itimerspec(&timer->ts.it_interval, 0);
		timer->exp_back_off_time = exp_ti;
	}

	timer->time_remaining = 0;
	resurrect_timer(timer);
	timer_set_state(timer, TIMER_RUNNING);
}

void
print_timer(Timer_t* timer) {

	printf("Counter = %u, time remaining = %lu, state = %d\n",
		timer->invocation_counter,
		timer_get_time_remaining_in_milli_sec(timer),
		timer_get_current_state(timer));
}

bool
is_timer_running(Timer_t* timer) {

	TIMER_STATE_T timer_state;

	timer_state = timer_get_current_state(timer);

	if (timer_state == TIMER_RUNNING ||
		timer_state == TIMER_RESUMED) {
		return true;
	}

	return false;
}


/*****APIs' to control timer******/