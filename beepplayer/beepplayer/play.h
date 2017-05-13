#pragma once

void callBeep(double freq, double time);
void callSleep(double time);

extern int g_BeepMode; //0=native 1=port IO

#ifdef _WIN32
int loadInpout32(void);
int unloadInpout32(void);

#ifdef _WIN64
#define INPOUT32_DLLNAME "inpoutx64.dll"
#else
#define INPOUT32_DLLNAME "inpout32.dll"
#endif

#endif