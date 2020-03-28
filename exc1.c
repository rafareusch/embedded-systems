#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
 
typedef void *(*state_func)();

int open_act = 0, close_act = 0, leaving_act=0;
int req = 0, c;

int kbhit(void);

void *begin();
void *middle();
void *end();
void *firstDoorOpen();
void *firstDoorClose();
void *secondDoorOpen();
void *secondDoorClose();


//1 open first door
//2 open second door


void *begin(){
	fprintf(stderr, "BEGIN");
	open_act = 0; close_act = 0; leaving_act =0;
	if (req == 1)	return firstDoorOpen;
	return begin;
}

void *firstDoorOpen(){
	fprintf(stderr, "FIRSTDOOROPEN");
	open_act = 1; close_act = 0;
	return firstDoorClose;
}

void *firstDoorClose(){
    fprintf(stderr, "FIRSTDOORCLOSE");
    open_act=0; close_act=1;req=0;
	if(leaving_act == 1) return begin;
    return middle;
}

void *middle(){
	fprintf(stderr, "MIDDLE");
	open_act=0;close_act=0;
	if(req == 1){
		leaving_act = 1;
		return firstDoorOpen;
	}
	if(req == 2) return secondDoorOpen;
	return middle;
}

void *secondDoorOpen(){
	fprintf(stderr, "SECONDDOOROPEN");
	open_act=1;close_act=0;
	return secondDoorClose;
}

void *secondDoorClose(){
	fprintf(stderr, "SECONDDOORCLOSE");
	open_act=0;close_act=1;req=0;
	if(leaving_act ==1){
		leaving_act=0;
		return middle;
	}
    return end;
}

void *end(){
	fprintf(stderr, "END");
	open_act=0; close_act=0;
	if(req == 2){
		leaving_act=1;
		return secondDoorOpen;
	}
	return end;
}

int main(){
	state_func curr_state = begin;

	while(1){
		curr_state = (state_func)(*curr_state)();
		fprintf(stderr, "  User actions: (open_act: %d, close_act: %d, req: %d)\n", open_act, close_act, req);
		sleep(1);
		if (kbhit() && !open_act && !close_act){
			c = getchar();
			if (c >= '0' && c <= '5')
				req = c - 48;
		}
	}
	return 0;
}
