#include "Sample.h"

struct Output
{
	DXGI_OUTPUT_DESC desc;
	std::vector<DXGI_MODE_DESC> m_ModList;
	std::vector<IDXGIOutput*> m_OutputList;
};

struct Adapter
{
	IDXGIAdapter* pAdapter;
	DXGI_ADAPTER_DESC desc;
	std::vector<Output> m_OutputList;
};

Sample::Sample()
{}
Sample::~Sample()
{}
bool Sample::PreInit()
{
	HRESULT hr;
	Device::CreateGIFactory();
	IDXGIFactory * pFactory = getGIFactory();
	std::vector<Adapter> m_AdapterList;
	for (int iAdapter = 0; ; ++iAdapter)		// 그래픽카드 찾는다
	{
		IDXGIAdapter* pAdapter;
		Adapter Adapter;
		hr = pFactory->EnumAdapters(iAdapter, &pAdapter);
		if (FAILED(hr)) break;
		pAdapter->GetDesc(&Adapter.desc);
		Adapter.pAdapter = pAdapter;
		m_AdapterList.push_back(Adapter);
	}

	for (int iAdapter = 0; iAdapter < m_AdapterList.size(); ++iAdapter)
	{
		UINT NumModes = 512;
		DXGI_MODE_DESC de[512];
		for (int iOutput = 0; ; ++iOutput) // 그래픽카드 별로 연결된 모니터 찾는다
		{
			Output Output;
			IDXGIOutput* pOutput = nullptr;
			hr = m_AdapterList.at(iAdapter).pAdapter->EnumOutputs(iOutput, &pOutput);
			if (pOutput != nullptr)
			{
				pOutput->GetDesc(&Output.desc);
				hr = pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
					DXGI_ENUM_MODES_SCALING,
					&NumModes, de);
			}
			Output.m_OutputList.push_back(pOutput);
			if (FAILED(hr)) break;
			m_AdapterList.at(iAdapter).m_OutputList.push_back(Output);
			for (int iMode = 0; iMode < NumModes; ++iMode)
			{
				if (de[iMode].Width >= 800 && de[iMode].Width <= 1920 && de[iMode].Height >= 600 && de[iMode].Height <= 1080)
				{
					m_AdapterList.at(iAdapter).m_OutputList[iOutput].m_ModList.push_back(de[iMode]);
				}
			}
		}
	}
	return true;
}
bool Sample::Init()
{
	m_Timer.Init();
//	PreInit();
	return true;
}
bool Sample::Frame()
{
	m_Timer.Frame();
	return true;
}
bool Sample::Render()
{
	m_Timer.Render();
	return true;
}
bool Sample::Release()
{
	m_Timer.Release();
	return true;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleDXGI");
	wd.Run();
	return 0;
}