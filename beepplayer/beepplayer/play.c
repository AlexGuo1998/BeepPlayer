#include "play.h"

#include <stdint.h>
#include <stddef.h>

#ifdef _WIN32
#include <Windows.h>
#include <winsock.h>

typedef void(__stdcall *lpOut32)(uint8_t, uint8_t);
typedef uint8_t(__stdcall *lpInp32)(uint8_t);

static lpOut32 fpOut32 = NULL;
static lpInp32 fpInp32 = NULL;

static HMODULE hDll = NULL;

#else
#ifdef __linux__
#define __USE_BSD
#include <sys/io.h> 
#include <unistd.h>
#else
#error Unsupported platform 
#endif
#endif

#ifdef _WIN32
int g_BeepMode = 0;
#else
int g_BeepMode = 1;
#endif

void callBeep(double freq, double time) {
#ifdef _WIN32
	if (g_BeepMode == 0) {
		Beep((DWORD)freq, (DWORD)time);
	} else {
		//port I/O
		uint16_t freqnum = (uint16_t)(1193180 / freq);
		fpOut32(0x43, 0xB6);
		fpOut32(0x42, (freqnum & 0xFF));
		fpOut32(0x42, (freqnum >> 8));
		fpOut32(0x61, 0x03 | fpInp32(0x61));
		Sleep((DWORD)time);
		fpOut32(0x61, (fpInp32(0x61) & 0xFC));
	}
#else
	uint16_t freqnum = (uint16_t)(1193180 / freq);
	outb(0xB6, 0x43);
	outb((freqnum & 0xFF), 0x42);
	outb((freqnum >> 8), 0x42);
	outb(0x03 | inb(0x61), 0x61);
	usleep(1000 * time);
	outb(0xFC | inb(0x61); , 0x61);
#endif
}

void callSleep(double time) {
#ifdef _WIN32
	Sleep((DWORD)time);
#else
	usleep(1000 * time);
#endif
}

#ifdef _WIN32
int loadInpout32(void) {
	HMODULE hDll = LoadLibraryA(INPOUT32_DLLNAME);
	if (hDll == NULL) return 1;
	fpOut32 = (lpOut32)GetProcAddress(hDll, "Out32");
	fpInp32 = (lpInp32)GetProcAddress(hDll, "Inp32");
	if (fpOut32 == NULL || fpInp32 == NULL) return 1;
	return 0;
}

int unloadInpout32(void) {
	if (hDll) FreeLibrary(hDll);
	return 0;
}
#endif