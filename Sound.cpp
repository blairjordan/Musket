/*****************************************************************************
*																			 *
* File: Sound.cpp															 *
* Author: B. Jordan															 *
*																			 *
* Description: This file contains functions that use OpenGL to load multiple *
*			   WAV files and play them in a single buffer concurrently, or   *
*			   play them over multiple buffers and source points			 *
*																			 *
*****************************************************************************/

#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include "sound.h"

#include <al/alut.h>
#include <al/al.h>
#include <al/alc.h>

ALuint Buffers[NUM_BUFFERS];								// Sound data buffer
ALuint Sources[NUM_SOURCES];								// Source points of emitting sound
ALfloat SourcesPos[NUM_SOURCES][1];							// Sound position
ALfloat SourcesVel[NUM_SOURCES][1];							// Sound velocity
ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };					// Listener position
ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };					// Listener Velocity
ALfloat ListenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };	// Listener orientation
					  //  at   at   at    up   up   up

/***************************************************************************************
*																					   *
* Function: LoadALData																   *
* Parameters: -																		   *
* Description: Loads sound sources from disk using the alut utility then sends the     *
* data into openAL as a buffer. The source is also created to play that buffer.		   *
*																					   *
***************************************************************************************/
ALboolean LoadALData()
{
	// Variables to load into

	ALenum format;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	ALboolean loop;

	// Load wav data into buffers
	alGenBuffers(NUM_BUFFERS, Buffers);

	// Return on error
	if(alGetError() != AL_NO_ERROR)
		return AL_FALSE;

	alutLoadWAVFile("C:/game/Sounds/stage_music.wav", &format, &data, &size, &freq, &loop);
	alBufferData(Buffers[STAGE_MUSIC], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);


	// Bind buffers into audio sources
	alGenSources(NUM_SOURCES, Sources);

	if(alGetError() != AL_NO_ERROR)
		return AL_FALSE;

	alSourcei (Sources[STAGE_MUSIC], AL_BUFFER,   Buffers[STAGE_MUSIC]   );
	alSourcef (Sources[STAGE_MUSIC], AL_PITCH,    1.0f              );
	alSourcef (Sources[STAGE_MUSIC], AL_GAIN,     1.0f              );
	alSourcefv(Sources[STAGE_MUSIC], AL_POSITION, SourcesPos[STAGE_MUSIC]);
	alSourcefv(Sources[STAGE_MUSIC], AL_VELOCITY, SourcesVel[STAGE_MUSIC]);
	alSourcei (Sources[STAGE_MUSIC], AL_LOOPING,  AL_TRUE           );

	// Do another error check and return
	if(alGetError() != AL_NO_ERROR)
		return AL_FALSE;

	return AL_TRUE;
}


/***************************************************************************************
*																					   *
* Function: SetListenerValues														   *
* Parameters: -																		   *
* Description: Tells openAL to use listener values.								       *
*																					   *
***************************************************************************************/
void SetListenerValues()
{
	alListenerfv(AL_POSITION,    ListenerPos);
	alListenerfv(AL_VELOCITY,    ListenerVel);
	alListenerfv(AL_ORIENTATION, ListenerOri);
}

/***************************************************************************************
*																					   *
* Function: KillALData																   *
* Parameters: -																		   *
* Description: Return allocated memory for buffers and sources to the system.	       *
*																					   *
***************************************************************************************/
void KillALData()
{
	alDeleteBuffers(NUM_BUFFERS, Buffers);
	alDeleteSources(NUM_SOURCES, Sources);
	alutExit();
}

/***************************************************************************************
*																					   *
* Function: initializeAudio															   *
* Parameters: -																		   *
* Description: Checks for errors and plays sound sources.						       *
*																					   *
***************************************************************************************/
int initializeAudio()
{

	// Initialize OpenAL and clear the error bit.
	alutInit(NULL, 0);
	alGetError();

	// Load the wav data.
	if(LoadALData() == AL_FALSE)
		return 0;

	SetListenerValues();

	// Setup an exit procedure.
	atexit(KillALData);

	// Begin the music sample to play.
	alSourcePlay(Sources[STAGE_MUSIC]);

	return 0;
}