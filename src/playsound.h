#ifdef _WIN32
#include <SDL/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

void audio_cleanup(SDL_AudioDeviceID id, Uint8* buf);
void* audio_thread(void* arg);
void play_async();
void play_async();
void play_sound();
