#pragma once
#include "Define.h"

class Timer
{
public:
	Timer();
	~Timer();
public:
	bool				Init();
	bool				Frame();
	bool				Render();
	bool				Release();
	void				Reset();
	void				Start();
	void				Stop();
	bool				isStarted() const;
	FLOAT				getElapstedTime() const;
	INT					getFPS() const;
	FLOAT				getSPF() const;
private:
	LARGE_INTEGER		m_Frequency;
	LARGE_INTEGER		m_Current;
	LARGE_INTEGER		m_Frame;
	FLOAT				m_fSecPerFrame;

	LARGE_INTEGER		m_FPS;
	INT					m_iFramePerSecond;
	INT					m_iFPSElapse;

	LARGE_INTEGER		m_Start;
	LARGE_INTEGER		m_Elapse;
	BOOL				m_bStarted;
	FLOAT				m_fEventTime;
};