#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include "window.h"

void usage(void){
	puts("Usage: <topic> <time> (minutes)");
	exit(0);
}

int main(int argc, char** argv){

	if(argc < 3){
		usage();
	}
	if(argc > 3){
		printf("Please wrap your topic in \"quotes\"\n");
		exit(0);
	}

	char* title = argv[1];
	char* end;
	float time = strtof(argv[2], &end);
	if(*end != '\0'){
		printf("Invalid time\n");
		exit(0);
	}

	init_window(title, time);
        return 0;
}
