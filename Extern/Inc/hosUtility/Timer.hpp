#ifndef TIMER_H
#define TIMER_H

// [2020/11/19 노종원]
// 타이머
// 시간측정을 위한 타이머클래스

// LOG
// [2020/12/31] 노종원 : ut네임스페이스 추가

namespace hos::ut
{
	class Timer
	{
	private:
		double mSecondsPerCount;
		double mDeltaTime;

		__int64 mBaseTime;
		__int64 mPausedTime;
		__int64 mStopTime;
		__int64 mPrevTime;
		__int64 mCurrTime;

		bool mStopped;

	public:
		Timer() : mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0),
			mPausedTime(0), mStopTime(0), mPrevTime(0), mCurrTime(0), mStopped(false)
		{
			__int64 countsPerSec;
			QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
			mSecondsPerCount = 1.0 / (double)countsPerSec;
		}

		virtual ~Timer() {}

		float TotalTime()const
		{
			// If we are stopped, do not count the time that has passed since we stopped.
			// Moreover, if we previously already had a pause, the distance 
			// mStopTime - mBaseTime includes paused time, which we do not want to count.
			// To correct this, we can subtract the paused time from mStopTime:  
			//
			//                     |<--paused time-->|
			// ----*---------------*-----------------*------------*------------*------> time
			//  mBaseTime       mStopTime        startTime     mStopTime    mCurrTime

			if (mStopped)
			{
				return (float)(((mStopTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
			}

			// The distance mCurrTime - mBaseTime includes paused time,
			// which we do not want to count.  To correct this, we can subtract 
			// the paused time from mCurrTime:  
			//
			//  (mCurrTime - mPausedTime) - mBaseTime 
			//
			//                     |<--paused time-->|
			// ----*---------------*-----------------*------------*------> time
			//  mBaseTime       mStopTime        startTime     mCurrTime

			else
			{
				return (float)(((mCurrTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
			}
		} // in seconds
		double DeltaTime()const
		{
			return mDeltaTime;
		} // in seconds

		void Reset()
		{
			__int64 currTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

			mBaseTime = currTime;
			mPrevTime = currTime;
			mStopTime = 0;
			mStopped = false;
		} // Call before message loop.
		void Start()
		{
			__int64 startTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&startTime);


			// Accumulate the time elapsed between stop and start pairs.
			//
			//                     |<-------d------->|
			// ----*---------------*-----------------*------------> time
			//  mBaseTime       mStopTime        startTime     

			if (mStopped)
			{
				mPausedTime += (startTime - mStopTime);

				mPrevTime = startTime;
				mStopTime = 0;
				mStopped = false;
			}
		} // Call when unpaused.
		void Stop()
		{
			if (!mStopped)
			{
				__int64 currTime;
				QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

				mStopTime = currTime;
				mStopped = true;
			}
		}  // Call when paused.
		void Tick()
		{
			if (mStopped)
			{
				mDeltaTime = 0.0;
				return;
			}

			__int64 currTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
			mCurrTime = currTime;

			// Time difference between this frame and the previous.
			mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;

			// Prepare for next frame.
			mPrevTime = mCurrTime;

			// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
			// processor goes into a power save mode or we get shuffled to another
			// processor, then mDeltaTime can be negative.
			if (mDeltaTime < 0.0)
			{
				mDeltaTime = 0.0;
			}
		}  // Call every frame.
		bool IsStop() const
		{
			return mStopped;
		}
	};

	class GameTimer : public Timer
	{
	private:
		float MaxFPS;
		int FPS_Counter;
		float FrameInterval;
		float FPS;
		float RenderDeltaTime;
		float RenderStartTime;
		unsigned __int64 FrameCounter;
		bool IsRender;
	public:
		GameTimer() : Timer(), MaxFPS(60), FPS_Counter(0), FPS(0), RenderDeltaTime(0), FrameCounter(0), IsRender(false)
		{
			Reset();
			FrameInterval = 1.f / MaxFPS;
		}
		~GameTimer() {}

		void Reset()
		{
			Timer::Reset();
			FPS_Counter = 0;
			FPS = 0;
			RenderDeltaTime = 0;
			RenderStartTime = 0;
			FrameCounter = 0;
		}
		void Tick()
		{
			Timer::Tick();
			RenderDeltaTime += float(Timer::DeltaTime());

			IsRender = false;
			if (Timer::TotalTime() - RenderStartTime >= FrameInterval)
			{
				FrameCounter++;
				RenderStartTime = Timer::TotalTime();
				IsRender = true;
			}

			FPS_Counter++;
			if (RenderDeltaTime > 1.f)
			{
				FPS = static_cast<F32>(FPS_Counter);
				FPS_Counter = 0;
				RenderDeltaTime = 0;
			}
		}

		void SetFPS(int fps)
		{
			MaxFPS = static_cast<F32>(fps);
			FrameInterval = 1.f / MaxFPS;
		}
		float GetFPS() const
		{
			return FPS;
		}
		float GetMaxFPS() const
		{
			return MaxFPS;
		}
		float GetFPSCounter() const
		{
			return static_cast<F32>(FPS_Counter);
		}
		unsigned __int64 GetFrameCount() const
		{
			return FrameCounter;
		}
		bool GetIsRender() const
		{
			return IsRender;
		}
	};
}


#endif // !TIMER_H