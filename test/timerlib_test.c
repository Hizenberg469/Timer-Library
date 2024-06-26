#include <stdio.h>
#include "timerlib.h"

static void
user_defined_app_cb(Timer_t* timer, void* user_data) {

	printf("User data = %s\n", (char*)user_data);

}

int
main(int argc, char* argv[]) {

	char* name = "Junet";

	Timer_t* timer = setup_timer(user_defined_app_cb, 1000, 1000, 0, name, false);
	start_timer(timer);

	printf("1. Pause Timer\n");
	printf("2. Resume Timer\n");
	printf("3. Restart timer\n");
	printf("4. Reschedule timer\n");
	printf("5. Delete timer\n");
	printf("6. Cancel Timer\n");
	printf("7. Get Remaining Time\n");
	printf("8. Print Timer State\n");

	int choice;
	choice = 0;

	while (1) {
		scanf("%d", &choice);

		switch (choice) {

		case 1:
			//pause_timer(timer);
			break;
		case 2:
			//resume_timer(timer);
			break;
		case 3:
			//restart_timer(timer);
			break;
		case 4:
			/*reschedule_timer(timer,
				timer->exp_time,
				timer->sec_exp_time);*/
			break;
		case 5:
			//delete_timer(timer);
			break;
		case 6:
			//cancel_timer(timer);
			break;
		case 7:
			//printf("Rem Time = %lu\n", timer_get_time_remaining_in_mill_sec(timer));
			break;
		case 8:
			//print_timer(timer);
			break;
		default:;
		}
	}
	return 0;
}