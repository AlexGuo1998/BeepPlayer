#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#ifdef _WIN32

#include <Windows.h>

#else
#ifdef __linux__

#include <sys/io.h>
#include <unistd.h>
#include <signal.h>
#define HIDE_CURSOR_ESCAPE "\x1B[?25l"
#define SHOW_CURSOR_ESCAPE "\x1B[?25h"
#define DEF_COLOR "\x1B[0m"

#else

#error Unsupported platform 

#endif
#endif

#include "decode.h"
#include "play.h"

bool playing = false, stopped = false;

#ifdef _WIN32
BOOL CtrlHandler(DWORD fdwCtrlType) {
	playing = false;
	while (!stopped);
	fprintf(stderr, "Stopped!\n");
	return FALSE;
}
#else
void CtrlHandler(int signal) {
	outb(0xfc, 0x61);
	fprintf(stderr, SHOW_CURSOR_ESCAPE);
	fprintf(stderr, DEF_COLOR);
	if (signal != 18) {
		playing = false;
		fprintf(stderr, "\nStopped!\n");
		exit(1);
	}
	//TODO when resume, hide cursor
}
#endif

void printHelp(void) {
	fprintf(stderr,
		"BeepPlayer - A program to play music through PC speaker\n\n"
		"Usage: beepplayer [options] file\n"
		"-h\t\tshow help\n"
#ifdef _WIN32
		"-p\t\tplay with inpout32\n"
#endif
		"\n"
		"Project website: <https://github.com/AlexGuo1998/BeepPlayer>\n"
	);
}

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		fprintf(stderr, "Please open a file!\ntype: \"beepplayer -h\" for help\n");
		return 1;
	}
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0) {
			printHelp();
			return 0;
		}
#ifdef _WIN32
		else if (strcmp(argv[i], "-p") == 0) {
			//use inpout32
			g_BeepMode = 1;
		}
#endif
	}

	char* notestr;

	{
		FILE *pf = fopen(argv[argc - 1], "rb");
		if (pf == NULL) {
			fprintf(stderr, "Can't open file \"%s\"!\n", argv[argc - 1]);
			return 1;
		}

		fseek(pf, 0, SEEK_END);
		int filelen;
		filelen = ftell(pf);
		notestr = (char*)malloc(filelen + 1);
		if (notestr == NULL) {
			fprintf(stderr, "Run out of memory!\n");
			return 1;
		}
		rewind(pf);
		size_t ret = fread(notestr, 1, filelen, pf);
		notestr[ret] = '\0';
		fclose(pf);
	}

	note_t *notelist;
	char *lyric;

	decodenote(notestr, &notelist, &lyric);
	free(notestr);

#ifdef _WIN32

	WORD offcolor = 0x0007, oncolor = 0x800F;

	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//console handle

	CONSOLE_CURSOR_INFO cursorinfo = {1, 0}, oldcursorinfo;
	GetConsoleCursorInfo(hConsole, &oldcursorinfo);//save old
	SetConsoleCursorInfo(hConsole, &cursorinfo);//hide cursor
	CONSOLE_SCREEN_BUFFER_INFO bufferinfo;
	GetConsoleScreenBufferInfo(hConsole, &bufferinfo);

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);

	if (g_BeepMode == 1) {
		if (loadInpout32() != 0) {
			unloadInpout32();
			fprintf(stderr, "Can't load " INPOUT32_DLLNAME "!\n");
			return 1;
		}
	}
#else

	iopl(3);//enable IO
	//TODO: don't enable io when output to wave
	//TODO: sigaction

	int signalsToCatch[] = {1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 14, 15, 17, 18, 21, 22, 24, 25, 26, 27, 29, 30};

	for (size_t i = 0; i < sizeof(signalsToCatch) / sizeof(int); i++) {
		signal(signalsToCatch[i], CtrlHandler);
	}

	fputs(HIDE_CURSOR_ESCAPE, stderr);
	char *offcolor = "\x1B[0m";
	char *oncolor = "\x1B[1;4m";

#endif
	playing = true; stopped = false;

	size_t i = 0;
	while (notelist[i].time != 0 && playing) {
		if (lyric[notelist[i].lyric] == '\n') {//first of the line
#ifdef _WIN32
			SetConsoleTextAttribute(hConsole, offcolor);//print with offcolor
#else
			fputs(offcolor, stderr);
#endif
			size_t j = i;
			do {
				if (notelist[j].lyric == 2) {//don't print \n on first line
					fprintf(stderr, "%s", &lyric[notelist[j].lyric + 1]);
				} else {
					fprintf(stderr, "%s", &lyric[notelist[j].lyric]);
				}
				j++;
			} while (lyric[notelist[j].lyric] != '\n');//until nextline
#ifdef _WIN32
			SetConsoleTextAttribute(hConsole, oncolor);//oncolor
#else
			fputs(oncolor, stderr);
#endif
			fprintf(stderr, "\r%s", &lyric[notelist[i].lyric + 1]);//go first row '\r', then print
		} else {
#ifdef _WIN32
			SetConsoleTextAttribute(hConsole, oncolor);
#else
			fputs(oncolor, stderr);
#endif
			fprintf(stderr, "%s", &lyric[notelist[i].lyric]);
		}
		//change to defcolor for possible resize
#ifdef _WIN32
		SetConsoleTextAttribute(hConsole, bufferinfo.wAttributes);
#else
		fputs(DEF_COLOR, stderr);
#endif

		if (notelist[i].height > 0) {
			callBeep(440 * pow(2, ((float)(notelist[i].height - 34) / 12)), notelist[i].time * 10 * (100 - notelist[i].staccato));
			callSleep(notelist[i].time * 10 * notelist[i].staccato);//staccato
		} else {
			callSleep(notelist[i].time * 1000);
			//height=0 -> pause
		}
		i++;
	}

	playing = false; stopped = true;

	fprintf(stderr, "\n");
	
	//restore cursor
#ifdef _WIN32
	SetConsoleCursorInfo(hConsole, &oldcursorinfo);
#else
	fputs(SHOW_CURSOR_ESCAPE, stderr);
#endif

	//free mem
	free(notelist);
	free(lyric);

	//unload inpout32
#ifdef _WIN32
	if (g_BeepMode == 1) {
		unloadInpout32();
	}
#endif
	
	return 0;
}
