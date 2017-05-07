#define _CRT_SECURE_NO_WARNINGS

#include "decode.h"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>

#define BUFFER_COUNT 32

static void getnum(const char *strnote, unsigned int *startpointer, char *outstr) {//开始时i指向数字的第一位，结束后指向第一位不是数字的字符
	char numcount = 0;
	bool reading = true;
	do {
		switch (strnote[*startpointer]) {
		case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '0': case '.': case '-':
			outstr[numcount++] = strnote[(*startpointer)++];//numcount指向下一个写入位置
			break;
		default:
			reading = false;
		}
	} while (numcount < 15 && reading);
	outstr[numcount] = '\0';
}

static void setlyric(const char *file, unsigned int *startpointer, char **lyric, size_t *lyricSaved, size_t *lyricAllocated, size_t *lyricUsed) {
	//开始时指向*
	(*startpointer)++;
	*lyricSaved = *lyricUsed;//作废
	(*lyric)[(*lyricSaved)++] = '\n';
	while (file[*startpointer] != '*') {
		(*lyric)[(*lyricSaved)++] = (file[*startpointer] == '|' ? '\0' : file[*startpointer]);
		(*startpointer)++;
		if (*lyricSaved >= *lyricAllocated - 3) {
			void *pv = realloc(*lyric, *lyricAllocated + BUFFER_COUNT);
			if (pv == NULL) {
				fprintf(stderr, "Run out of memory!");
				exit(1);
			}
			*lyric = (char *)pv;
			(*lyricAllocated) += BUFFER_COUNT;
		}
	}
	(*lyric)[(*lyricSaved)++] = 0;
}

static size_t getlyric(const char *lyric, size_t *lyricSaved, size_t *lyricUsed) {//歌词文本用/0分隔
	size_t i, j;
	if (*lyricUsed >= *lyricSaved) {//未缓存歌词，返回\0地址
		return *lyricUsed - 1;
	} else {//返回当前歌词的位置，lyricused指向下一个\0 +1
		for (i = *lyricUsed; lyric[i] != '\0'; i++);
		j = *lyricUsed;
		*lyricUsed = i + 1;
		return j;
	}
}

static char getnoteheight(char notechr, uint8_t octave, int8_t noteheight) {
	uint8_t dheight;
	switch (notechr) {
	case 'c': case 'C':
		dheight = 1;
		break;
	case 'd': case 'D':
		dheight = 3;
		break;
	case 'e': case 'E':
		dheight = 5;
		break;
	case 'f': case 'F':
		dheight = 6;
		break;
	case 'g': case 'G':
		dheight = 8;
		break;
	case 'a': case 'A':
		dheight = 10;
		break;
	case 'b': case 'B':
		dheight = 12;
		break;
	default:
		return 0;
	}
	dheight = dheight + 12 * octave + noteheight;
	if (dheight > 84) dheight = 84;
	if (dheight < 1) dheight = 1;
	return dheight;
}

void decodenote(const char *file, note_t **notes, char **lyric) {
	size_t i = 0; //file position
	size_t notecount = 0;
	uint8_t octave = 3;
	uint8_t length = 4;
	int8_t noteheight = 0;
	float tempo = 120, staccato = 10;
	note_t thisnote = {0, 0, 0, 0};
	int chord = -1;

	*notes = (note_t *)malloc(BUFFER_COUNT * sizeof(note_t));
	*lyric = (char*)malloc(BUFFER_COUNT);
	(*lyric)[0] = '\n';
	(*lyric)[1] = 0;

	size_t lyricSaved = 2;
	size_t lyricAllocated = BUFFER_COUNT;
	size_t lyricUsed = 2;


	while (file[i] != '\0') {
		//printf("%c\n", file[i]);

		switch (file[i]) {
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'p':
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'P':
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '.':
		{
			char strthislength[16];
			double thislength;

			getnum(file, &i, strthislength);

			if (sscanf(strthislength, "%lf", &thislength) <= 0 || thislength == 0) thislength = length;

			thisnote.time = (double)240 / tempo / (thislength > 0 ? thislength : -thislength);
			thisnote.height = getnoteheight(file[i], octave, noteheight);
			thisnote.lyric = getlyric(*lyric, &lyricSaved, &lyricUsed);

			i++;
			bool reading = true;
			do {
				switch (file[i]) {
				case '#':
					if (thisnote.height > 0 && thisnote.height < 84) thisnote.height++;
					i++;
					break;
				case 'b':
					if (thisnote.height > 1) thisnote.height--;
					i++;
					break;
				case '.':
					thisnote.time *= 1.5;
					i++;
					break;
				default:
					reading = false;
				}
			} while (reading);

			(*notes)[notecount] = thisnote;
			(*notes)[notecount].staccato = staccato;

			notecount++;

			if (notecount % BUFFER_COUNT == 0) {
				void *pv = realloc(*notes, (notecount + BUFFER_COUNT) * sizeof(note_t));
				if (pv == NULL) {
					fprintf(stderr, "Run out of memory!");
					exit(1);
				}
				*notes = (note_t *)pv;
			}

			break;
		}
		case 'o': case 'O'://octave
		{
			i++;
			switch (file[i]) {
			case '#':
				if (octave < 7) octave++;
				i++;
				break;
			case 'b':
				if (octave > 0) octave--;
				i++;
				break;
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
				octave = file[i] - '0';
				i++;
				break;
			default:;
			}
			break;
		}
		case 't': case 'T'://tempo
		{
			char strtempo[16];
			float newtempo;
			i++;
			getnum(file, &i, strtempo);
			//tempo = (float)atof(strtempo);
			if (sscanf(strtempo, "%f", &newtempo) > 0) {
				if (newtempo > 1000) {
					tempo = 1000;
				} else if (newtempo < 30) {
					tempo = 30;
				} else {
					tempo = newtempo;
				}
			}
			break;
		}
		case 's': case 'S':
		{
			char strstaccato[16];
			float newstaccato;
			i++;
			getnum(file, &i, strstaccato);
			
			if (sscanf(strstaccato, "%f", &newstaccato) > 0) {
				if (newstaccato >= 100) {
					staccato = 1;
				} else if (newstaccato <= 0) {
					staccato = 0;
				} else {
					staccato = newstaccato / 100;
				}
			}
			break;
		}
		case 'l': case 'L':		
		{
			char strlength[16];
			int newlength;
			i++;
			getnum(file, &i, strlength);

			if (sscanf(strlength, "%i", &newlength) > 0) {
				if (newlength >= 128) {
					length = 128;
				} else if (newlength <= 1) {
					length = 1;
				} else {
					length = (uint8_t)newlength;
				}
			}
			break;
		}
		case 'h': case 'H':
		{
			char strheight[16];
			int newheight;
			i++;
			getnum(file, &i, strheight);
			if (strheight[0] == '\0') {
				switch (file[i]) {
				case '#':
					newheight = noteheight + 1;
					i++;
					break;
				case 'b':
					newheight = noteheight - 1;
					i++;
					break;
				default:
					newheight = noteheight;
				}
			} else {
				if (sscanf(strheight, "%i", &newheight) <= 0) break;
			}

			if (newheight >= 12) {
				noteheight = 12;
			} else if (newheight <= -12) {
				noteheight = -12;
			} else {
				noteheight = (uint8_t)newheight;
			}

			break;
		}
		case '*':
		{
			setlyric(file, &i, lyric, &lyricSaved, &lyricAllocated, &lyricUsed);
			i++;
			break;
		}
		case '[':
		{
			i++;
			chord = notecount;
			break;
		}
		case ']':
		{
			i++;
			if (chord == -1) break;
			for (int n = chord; n < (signed)(notecount - 1); n++) {
				(*notes)[n].time = CHORD_LENGTH;
			}
			(*notes)[notecount - 1].time -= CHORD_LENGTH * (notecount - chord - 1);
			if ((*notes)[notecount - 1].time < CHORD_LENGTH) (*notes)[notecount - 1].time = CHORD_LENGTH;
			chord = -1;
			break;
		}
		case '/':
		{
			if (file[++i] == '/') {
				do {
					i++;
				} while (file[i] != '\n' && file[i] != '\r');
			}
			//i++;
			break;
		}
		case 'r': case 'R':
		{
			octave = 3; length = 4; noteheight = 0; tempo = 120; staccato = 10;
			i++;
			break;
		}
		default:
			i++;
		}
	}
	(*notes)[notecount].time = 0;
	(*notes)[notecount].lyric = 0;
}
