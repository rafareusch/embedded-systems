#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
 
typedef void *(*state_func)();

int timer = 0, up_act = 0, down_act = 0, open_act = 0, timer_start = 0;
int req = 0, floor_cnt = 0, c;

int kbhit(void);
void *idle();
void *goingup();
void *goingdn();
void *dooropen();

void *idle(){
	fprintf(stderr, "IDLE ");
	timer = 0;
	up_act = 0; down_act = 0; open_act = 1; timer_start = 0;
	if (req == floor_cnt)	return idle;
	if (req > floor_cnt)	return goingup;
	if (req < floor_cnt)	return goingdn;
	return 0;
}

void *goingup(){
	fprintf(stderr, "GOINGUP ");
	up_act = 1; down_act = 0; open_act = 0; timer_start = 0;
	floor_cnt++;
	if (req > floor_cnt)	return goingup;
	if (!(req > floor_cnt))	return dooropen;
	return 0;
}

void *goingdn(){
	fprintf(stderr, "GOINGDN ");
	up_act = 0; down_act = 1; open_act = 0; timer_start = 0;
	floor_cnt--;
	if (req < floor_cnt)	return goingdn;
	if (!(req < floor_cnt))	return dooropen;
	return 0;
}

void *dooropen(){
	fprintf(stderr, "DOOROPEN ");
	timer++;
	up_act = 0; down_act = 0; open_act = 1; timer_start = 1;
	if (timer < 10)		return dooropen;
	if (!(timer < 10))	return idle;
	return 0;
}

int main(){
	state_func curr_state = idle;

	while(1){
		curr_state = (state_func)(*curr_state)();
		fprintf(stderr, "floor: %d (up: %d down %d open %d)\n", floor_cnt, up_act, down_act, open_act);
		sleep(1);
		if (kbhit() && !up_act && !down_act){
			c = getchar();
			if (c >= '0' && c <= '9')
				req = c - 48;
		}
	}
	
	return 0;
}
