#include "Timer.h"

FLOAT g_fGameTimer = 0.0f;
FLOAT g_fSecPerFrame = 0.0f;

Timer::Timer() : m_bStarted(false), m_iFramePerSecond(0), m_iFPSElapse(0), m_fSecPerFrame(0.0f), m_fEventTime(0.0f)
{
	QueryPerformanceFrequency((LARGE_INTEGER *)&m_Frequency);
}
Timer::~Timer()
{}

bool Timer::Init()
{
	QueryPerformanceCounter(&m_Frame);

	if (m_Frame.QuadPart == 0)
	{
		return false;
	}

	m_FPS				= m_Frame;
	m_iFramePerSecond	= 0;
	m_iFPSElapse		= 0;
	m_fSecPerFrame		= 0.0f;

	Start();
	return true;
}
bool Timer::Frame()
{
	QueryPerformanceCounter(&m_Current);
	m_fSecPerFrame = g_fSecPerFrame = static_cast<FLOAT>(m_Current.QuadPart - m_Frame.QuadPart) / static_cast<FLOAT>(m_Frequency.QuadPart);

	g_fGameTimer += m_fSecPerFrame;

	if (((m_Current.LowPart - m_FPS.LowPart) / m_Frequency.LowPart) >= 1)
	{
		std::tsstream is;
		
		is << std::fixed << std::setprecision(3) <<  L"FPS : " << m_iFPSElapse << " Timer : " << g_fGameTimer << " SPF : " << std::setprecision(5) << m_fSecPerFrame;
		m_Buffer = is.str();
		m_iFramePerSecond = m_iFPSElapse;
		m_iFPSElapse = 0;
		m_FPS = m_Current;
	}
	m_iFPSElapse++;

	m_Frame = m_Current;

	return true;
}
bool Timer::Render()
{
	static FLOAT Color1 = 0.0f;
	static FLOAT Color2 = 0.0f;
	static FLOAT Color3 = 0.0f;

	Color1 = cos(g_fGameTimer) * 0.5f + 0.5f;
	Color2 = sin(g_fGameTimer) * 0.5f + 0.5f;
	Color3 = sin(-g_fGameTimer) * 0.5f + 0.5f;

	D2D1_RECT_F rt = D2D1::RectF(0.0f, 0.0f, (float)g_rtClient.right, (float)g_rtClient.bottom);
	S_DirectWrite.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
	S_DirectWrite.DrawText(rt, m_Buffer.c_str(), D2D1::ColorF(Color1, Color2, Color3, 1.0f));
	return true;
}
bool Timer::Release()
{
	return true;
}
void Timer::Reset()
{
	m_bStarted = FALSE;
	m_fEventTime = 0.0f;
	ZeroMemory(&m_Start, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_Elapse, sizeof(LARGE_INTEGER));
}
void Timer::Start()
{
	m_bStarted = TRUE;
	m_fEventTime = 0.0f;
	QueryPerformanceCounter((LARGE_INTEGER *)&m_Start);
}
void Timer::Stop()
{
	getElapstedTime();
	m_bStarted = FALSE;
}
bool Timer::isStarted() const
{
	return m_bStarted;
}
FLOAT Timer::getElapstedTime() const
{
	return m_fEventTime;
}
INT Timer::getFPS() const
{
	return m_iFPSElapse;
}
FLOAT Timer::getSPF() const
{
	return m_fSecPerFrame;
}