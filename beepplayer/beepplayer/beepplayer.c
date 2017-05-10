#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <stddef.h>
#include <stdbool.h>

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
	printf("Stopped!\n");
	return FALSE;
}
#else
void CtrlHandler(int signal) {
	outb(0xfc, 0x61);
	fprintf(stderr, SHOW_CURSOR_ESCAPE);
	fprintf(stderr, DEF_COLOR);
	if (signal != 18) {
		playing = false;
		printf("\nStopped!\n");
		exit(1);
	}
	//TODO when resume, hide cursor
}
#endif

int main(int argc, char *argv[]) {
	if (argc == 1) {
		fprintf(stderr, "Please open a file!\n");
		return 1;
	}
	FILE *pf = fopen(argv[argc - 1], "rb");
	if (pf == NULL) {
		fprintf(stderr, "Can't open file \"%s\"!\n", argv[argc - 1]);
		return 1;
	}
	
	fseek(pf, 0, SEEK_END);
	char* notestr; int filelen;
	filelen = ftell(pf);
	notestr = (char*)malloc(filelen + 1);
	if (notestr == NULL) {
		printf("Run out of memory!");
		return 1;
	}
	rewind(pf);
	fread(notestr, 1, filelen, pf);
	notestr[filelen] = 0;
	fclose(pf);

	note_t *notelist;
	char *lyric;

	decodenote(notestr, &notelist, &lyric);

#ifdef _WIN32

	WORD offcolor = 0x0007, oncolor = 0x800F;

	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//�õ�����̨��׼����ľ��

	CONSOLE_CURSOR_INFO cursorinfo = {1, 0}, oldcursorinfo;
	GetConsoleCursorInfo(hConsole, &oldcursorinfo);//����ԭ���Ŀ���̨���ģʽ
	SetConsoleCursorInfo(hConsole, &cursorinfo);//���ؿ���̨��꣨Ӧ���µĿ���̨���ģʽ��
	CONSOLE_SCREEN_BUFFER_INFO bufferinfo;
	GetConsoleScreenBufferInfo(hConsole, &bufferinfo);

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);

#else

	iopl(3);//enable IO
	//TODO: don't enable io when output to wave
	//TODO: sigaction

	int signalsToCatch[] = {1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 14, 15, 17, 18, 21, 22, 24, 25, 26, 27, 29, 30};

	for (size_t i = 0; i < sizeof(signalsToCatch) / sizeof(int); i++) {
		signal(signalsToCatch[i], CtrlHandler);
	}

	fprintf(stderr, HIDE_CURSOR_ESCAPE);
	char *offcolor = "\x1B[0m";
	char *oncolor = "\x1B[1;4m";

#endif
	playing = true; stopped = false;

	size_t i = 0;
	while (notelist[i].time != 0 && playing) {
		if (lyric[notelist[i].lyric] == '\n') {//��ʾ���������һ�еĿ�ͷ
#ifdef _WIN32
			SetConsoleTextAttribute(hConsole, offcolor);//���ò���ǰ�ĸ����ɫprint
#else
			fprintf(stderr, offcolor);
#endif
			size_t j = i;
			do {
				if (notelist[j].lyric == 2) {//��һ�в���Ҫ����
					fprintf(stderr, "%s", &lyric[notelist[j].lyric + 1]);
				} else {
					fprintf(stderr, "%s", &lyric[notelist[j].lyric]);
				}
				j++;
			} while (lyric[notelist[j].lyric] != '\n');//ֱ����һ�п�ʼΪֹ
#ifdef _WIN32
			SetConsoleTextAttribute(hConsole, oncolor);//��Ϊ���ź�ĸ����ɫ
#else
			fprintf(stderr, oncolor);
#endif
			fprintf(stderr, "\r%s", &lyric[notelist[i].lyric + 1]);//�Ȼص����п�ͷ'\r'����print��һ�����
		} else {
#ifdef _WIN32
			SetConsoleTextAttribute(hConsole, oncolor);
#else
			fprintf(stderr, oncolor);
#endif
			fprintf(stderr, "%s", &lyric[notelist[i].lyric]);
		}
#ifdef _WIN32
		SetConsoleTextAttribute(hConsole, bufferinfo.wAttributes);//����Ĭ����ɫ������ı䴰�ڴ�С�������
#else
		fprintf(stderr, DEF_COLOR);
#endif

		if (notelist[i].height > 0) {
			//����Beep API���в���
			callBeep(440 * pow(2, ((float)(notelist[i].height - 34) / 12)), notelist[i].time * 10 * (100 - notelist[i].staccato));
			//Ƶ�ʵļ��㷽����440 * 2^((������ֵ - 34) / 12)
			//������ֵ�����Լ������
			callSleep(notelist[i].time * 10 * notelist[i].staccato);
		} else {
			callSleep(notelist[i].time * 1000);
			//���height=0����ʾ����ֹ��
		}
		i++;
	}

	playing = false; stopped = true;

	fprintf(stderr, "\n");
	
#ifdef _WIN32
	SetConsoleCursorInfo(hConsole, &oldcursorinfo);//��ԭ�����״
#else
	fprintf(stderr, SHOW_CURSOR_ESCAPE);
#endif

	free(notelist);//�ͷ��ڴ�
	free(lyric);

	return 0;
}
