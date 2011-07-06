/**
 * libpolosnd
 * Lightweight sound module for educational environments
 * (C) 2011 by the libpolo team.
 *     Marc S. Ressl (mressl@itba.edu.ar)
 * Released under the GPL
 */

#include <stdlib.h>
#include <string.h>

#include <portaudio.h>
#include <sndfile.h>

#include "polosnd.h"

static int isSoundInitialized = 0;

static PaStream *soundStream;
static SF_INFO soundInfo;
static SNDFILE *soundFile;

static int runSound(const void *inputBuffer,
                    void *outputBuffer,
                    unsigned long framesPerBuffer,
                    const PaStreamCallbackTimeInfo *timeInfo,
                    PaStreamCallbackFlags statusFlags,
                    void *userData)
{
	memset(outputBuffer, 0, framesPerBuffer * soundInfo.channels * sizeof(short));

	sf_read_short(soundFile, outputBuffer, framesPerBuffer * soundInfo.channels);
	
	return paContinue;
}

int openSound()
{
	if (isSoundInitialized)
		return;
	
	isSoundInitialized = (Pa_Initialize() == paNoError);
	
	return isSoundInitialized;
}

void closeSound()
{
	stopSound();
	
	if (isSoundInitialized)
	{
		Pa_Terminate();
		
		isSoundInitialized = 0;
	}
}

int playSound(char *path)
{
	PaStreamParameters outputParameters;
	
	if (!isSoundInitialized)
		return 0;
	
	stopSound();
	
	soundFile = sf_open(path, SFM_READ, &soundInfo);
	if (soundFile)
	{
		outputParameters.device = Pa_GetDefaultOutputDevice();
		outputParameters.channelCount = soundInfo.channels;
		outputParameters.sampleFormat = paInt16;
		outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
		outputParameters.hostApiSpecificStreamInfo = NULL;
		
		if (Pa_OpenStream(&soundStream,
		                  NULL,
		                  &outputParameters,
		                  soundInfo.samplerate,
		                  256,
		                  paClipOff,
		                  runSound,
		                  NULL) == paNoError)
		{
			Pa_StartStream(soundStream);
			
			return 1;
		}
	}
	
	return 0;
}

void stopSound()
{
	if (soundStream)
	{
		Pa_StopStream(soundStream);
		Pa_CloseStream(soundStream);
		
		soundStream = NULL;
	}
	
	if (soundFile)
	{
		sf_close(soundFile);
		
		soundFile = NULL;
	}
}

