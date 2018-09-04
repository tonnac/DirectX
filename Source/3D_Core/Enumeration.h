#pragma once
#include "Define.h"

struct Output
{
	Output() : _pDXGIOutput(nullptr)
	{
		m_ModList.clear();
	}
	IDXGIOutput* _pDXGIOutput;
	DXGI_OUTPUT_DESC desc;
	std::vector<DXGI_MODE_DESC> m_ModList;
};

struct Adapter
{
	Adapter() : pAdapter(nullptr)
	{
		m_OutputList.clear();
	}
	IDXGIAdapter* pAdapter;
	DXGI_ADAPTER_DESC desc;
	std::vector<Output*> m_OutputList;
};

using OutputList = std::vector<Output*>;
using AdapterList = std::vector<Adapter*>;

class Enumeration
{
public:
	static bool CreateDeviceInfo(IDXGIFactory* pDXGIFactory);
private:
	static OutputList LoadOutputList(IDXGIAdapter* pAdapter);
	static AdapterList LoadAdapterList(IDXGIFactory* pDXGIFactory);
	static void DeviceInfoSave(AdapterList AdapterList_);
};