flags=-Wall -Wextra -Wpedantic
headers= -lSDL2 -lSDL2_ttf
source= src/main.c src/window.c src/playsound.c

main: $(source) 
	gcc -o main $(source) $(flags) $(headers) 
