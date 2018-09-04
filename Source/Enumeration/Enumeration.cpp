#include "Enumeration.h"

bool Enumeration::CreateDeviceInfo(IDXGIFactory* pDXGIFactory)
{
	AdapterList AdapterList_ = LoadAdapterList(pDXGIFactory);
	for (size_t iAdapter = 0; iAdapter < AdapterList_.size(); ++iAdapter)
	{
		OutputList OutputList_ = LoadOutputList(AdapterList_.at(iAdapter)->pAdapter);
		AdapterList_.at(iAdapter)->m_OutputList = OutputList_;
	}
	DeviceInfoSave(AdapterList_);
	return true;
}
OutputList Enumeration::LoadOutputList(IDXGIAdapter* pAdapter)
{
	HRESULT hr;
	UINT iOutput = 0;
	UINT NumModes = 512;
	DXGI_MODE_DESC de[512];
	OutputList OutputList_;
	while (true)
	{
		IDXGIOutput* pDXGIOutput = nullptr;
		Output* pOutput = new Output;
		if (FAILED(hr = pAdapter->EnumOutputs(iOutput, &pDXGIOutput))) break;
		pDXGIOutput->GetDesc(&pOutput->desc);
		pDXGIOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_SCALING,
			&NumModes, de);
		pOutput->_pDXGIOutput = pDXGIOutput;
		for (UINT iMode = 0; iMode < NumModes; ++iMode)
		{
			if (de[iMode].Width >= 800 && de[iMode].Width <= 1920 && de[iMode].Height >= 600 && de[iMode].Height <= 1080)
			{
				pOutput->m_ModList.push_back(de[iMode]);
			}
		}
		OutputList_.push_back(pOutput);
		++iOutput;
	}
	return OutputList_;
}
AdapterList Enumeration::LoadAdapterList(IDXGIFactory* pDXGIFactory)
{
	HRESULT hr;
	UINT iAdapter = 0;
	AdapterList AdapterList_;
	while (true)
	{
		IDXGIAdapter* pDXGIAdapter = nullptr;
		Adapter* pAdapter = new Adapter;
		if (FAILED(hr = pDXGIFactory->EnumAdapters(iAdapter ,&pDXGIAdapter)))
		{
			break;
		}
		pDXGIAdapter->GetDesc(&pAdapter->desc);
		pAdapter->pAdapter = pDXGIAdapter;
		AdapterList_.push_back(pAdapter);
		++iAdapter;
	}
	return AdapterList_;
}
void Enumeration::DeviceInfoSave(AdapterList AdapterList_)
{
	std::ofstream fp("DeviceInfo.txt");
	if (!fp.is_open())
	{
		MessageBox(nullptr, L"File Open Error", L"Error", MB_OK);
		return;
	}

	for (UINT i = 0; i < AdapterList_.size(); ++i)
	{

	}

	for (auto iter : AdapterList_)
	{
		for (auto it : iter->m_OutputList)
		{
			it->_pDXGIOutput->Release();
			delete it;
		}
		iter->m_OutputList.clear();
		iter->pAdapter->Release();
		delete iter;
	}
	AdapterList_.clear();
}