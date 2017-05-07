#pragma once

#include <stdint.h>
#include <stddef.h>

#define CHORD_LENGTH 0.1

typedef struct note {
	int8_t height;	//0=sleep
	double time;	//in seconds
	float staccato;	//0~1
	//uint16_t lyriccoloroff, lyriccoloron;
	size_t lyric;	//offset of lyric
} note_t;

void decodenote(const char *file, note_t **notes, char **lyric);


