//------------------------------------------------------------------------------
//
// Nutmeg::Core::Engine
//
// copyright:	(c) 2008-2011 One More Studio
// author:		Alexey Egorov (FadeToBlack aka EvilSpirit)
// url:			http://nutmeg-engine.ru
// mail:		anandamide@mail.ru
// icq:			455-761-951
// skype:		fade_to_evil
//
//------------------------------------------------------------------------------

#include "Timer.h"

//------------------------------------------------------------------------------

#include "Settings.h"

//------------------------------------------------------------------------------

#ifdef NUTMEG_PLATFORM_BADA
	#include <FSysSystemTime.h>
#else
	#include <windows.h>
#endif

//------------------------------------------------------------------------------
//
// namespace Nutmeg
//
//------------------------------------------------------------------------------

namespace Nutmeg {

	//--------------------------------------------------------------------------
	//
	// class Timer
	//
	//--------------------------------------------------------------------------

	long long Timer::frmFrq;

	//--------------------------------------------------------------------------

	Timer::Timer() {
		#ifdef NUTMEG_PLATFORM_BADA
			start();
		#else
			LARGE_INTEGER frq;
			QueryPerformanceFrequency(&frq);
			frmFrq = frq.QuadPart;
			start();
		#endif
	}

	//--------------------------------------------------------------------------

	Timer::~Timer() { }

	//--------------------------------------------------------------------------

	void Timer::start() {
		#ifdef NUTMEG_PLATFORM_BADA
			Osp::System::SystemTime::GetTicks(time);
		#else
			LARGE_INTEGER start;
			QueryPerformanceCounter(&start);
			time = start.QuadPart;
		#endif

	}

	//--------------------------------------------------------------------------

	double Timer::elapsed() {
		#ifdef NUTMEG_PLATFORM_BADA
			long long end;
			Osp::System::SystemTime::GetTicks(end);
			return double(end - time) / 1000.0f;
		#else
			LARGE_INTEGER end;
			QueryPerformanceCounter(&end);
			return (double)(end.QuadPart - time) / (double)frmFrq;
		#endif
	}

	//--------------------------------------------------------------------------
	//
	// class FPSTimer
	//
	//--------------------------------------------------------------------------

	FPSTimer::FPSTimer() {
		time = 0.0;
		dt = 0.0f;
		adt = 0.0f;
		fps = 0.0f;
		afps = 0.0f;
		frames = 0;
	}

	//--------------------------------------------------------------------------

	float FPSTimer::finish() {

		double elaps = elapsed();
		dt = float(elaps);

		if (elaps > 1e-10) {
			fps	= float(1.0 / elaps);
		} else {
			fps = 0.0f;
		}

		time += elaps;
		frames ++;

		if (time > 1.0) {
			afps = float(double(frames) / time);
			time = 0.0;
			frames = 0;
		}
		adt = adt * 0.5f + dt * 0.5f;
		return fps;
	}

	//--------------------------------------------------------------------------
}

//------------------------------------------------------------------------------
