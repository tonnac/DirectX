#pragma once
#include "Define.h"
#include "DirectWrite.h"
#include <chrono>

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
public:
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

	std::tstring		m_Buffer;


	std::chrono::system_clock m_sysClock;
	std::chrono::system_clock::time_point m_CurTick;
	std::chrono::system_clock::time_point m_BefTick;
	std::chrono::system_clock::time_point m_OneSecTick;
};