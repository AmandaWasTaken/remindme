#include <SDL2/SDL.h>
#include <pthread.h>

void audio_cleanup(SDL_AudioDeviceID id, Uint8* buf){
	SDL_CloseAudioDevice(id);
	SDL_FreeWAV(buf);
	SDL_Quit();
}

void play_sound(){

	const char* wavname = "./assets/sound.wav";
	
	// initialize SDL audio
	SDL_Init(SDL_INIT_AUDIO);
	SDL_AudioSpec wav_spec;
	Uint32 wav_len;
	Uint8* wav_buf;

	// Load wav file
	SDL_LoadWAV(wavname, &wav_spec, &wav_buf, &wav_len);

	// Open default audio device
	// @param device name (NULL = default)
	// @param recording device (don't care for playback)
	// @param output format
	// @param output
	// @param idk and don't care
	SDL_AudioDeviceID dev_id = SDL_OpenAudioDevice(NULL, 0, 
			&wav_spec, NULL, 0);

	int success = SDL_QueueAudio(dev_id, wav_buf, wav_len);
	if(success != 0){
		fprintf(stderr, "Queue audio failed: %s\n", 
				SDL_GetError());
		exit(1);
	}

	SDL_PauseAudioDevice(dev_id, 0);
	
	// add some delay to make sure the sound has enough time to play
	SDL_Delay((wav_len/wav_spec.freq)*5000);
	audio_cleanup(dev_id, wav_buf);
	pthread_exit(NULL);
}

void* audio_thread(void* arg){
	play_sound();
	return NULL;
}

void play_async(){
	pthread_t thread;
	if(pthread_create(&thread, NULL, audio_thread, NULL) != 0){
		perror("Failed to create audio thread");
	}
	pthread_detach(thread);
	return;
}
