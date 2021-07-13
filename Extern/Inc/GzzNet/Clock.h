#pragma once
#include <chrono>
#include <string>	
#include <string_view>

// [2020/10/26 김효곤]
// 게임 타이머 등에 쓰일 타이머 입니다. 
// Nano의 정밀도를 갖습니다.

namespace gzz
{
	class Clock
	{
		std::chrono::steady_clock::time_point		StartTime;
		std::chrono::steady_clock::time_point		EndTime;
		std::chrono::steady_clock::time_point		PreTime;

		std::chrono::duration<double> Delta;

	public:
		Clock();
		~Clock();

	public:
		// 시계의 시작지점을 설정합니다.
		void Start();
		void Stop();

		// Pause Time (멈춰있던 시간) 리턴
		double Restart();

		void Tick();

		// get delta
		const double DeltaTime();			// base delta
		const int DeltaTime_Min();			// to min
		const int DeltaTime_Hour();			// to hour
		const __int64 DeltaTime_Nano();		// to nano
		const __int64 DeltaTime_Micro();	// to micro
		const __int64 DeltaTime_Milli();	// to milli
		const __int64 DeltaTime_Second();	// to sec	

		// 현재 타임 카운트, 정밀도 별로있음.
		const double Now();				// base time
		const int Now_Min();			// to min
		const int Now_Hour();			// to hour
		const __int64 Now_Nano();		// to nano
		const __int64 Now_Micro();		// to micro
		const __int64 Now_Milli();	// to milli
		const __int64 Now_Seccond();	// to sec	

	private:
	};
}