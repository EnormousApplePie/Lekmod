#pragma once
#include "Windows.h"
using MMRESULT=UINT;
#define TIMERR_NOERROR 0
extern "C" UINT timeBeginPeriod(UINT);
extern "C" UINT timeEndPeriod(UINT);
extern "C" DWORD timeGetTime();
