#ifndef _SOUND_H
#define _SOUND_H

#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <al/alut.h>

#define NUM_BUFFERS 1					// Maximum data buffers required
#define NUM_SOURCES 1					// Maximum emissions required
#define STAGE_MUSIC 0					// Buffers and sources index

int initializeAudio();

#endif