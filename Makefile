flags=-Wall -Wextra -Wpedantic
headers= -lSDL2 -lSDL2_ttf
main: main.c window.c playsound.c
	gcc -o main main.c window.c playsound.c $(flags) $(headers) 
