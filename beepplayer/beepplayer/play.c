#include "play.h"

#include <stdint.h>
#include <stddef.h>

#ifdef _WIN32
#include <Windows.h>
#else
#ifdef __linux__
#define __USE_BSD
#include <sys/io.h> 
#include <unistd.h>
#else
#error Unsupported platform 
#endif
#endif

void callBeep(double freq, double time) {
#ifdef _WIN32
	Beep((DWORD)freq, (DWORD)time);
#else
	uint8_t bit;
	uint16_t freqnum = 1193180 / freq;
	
	outb(0xb6, 0x43);
	outb((freqnum & 0xff), 0x42);
	outb((freqnum >> 8), 0x42);
	bit = inb(0x61);
	outb(0x03 | bit, 0x61);
	usleep(1000 * time);
	outb(0xfc | bit, 0x61);
#endif
}

void callSleep(double time) {
#ifdef _WIN32
	Sleep((DWORD)time);
#else
	usleep(1000 * time);
#endif
}