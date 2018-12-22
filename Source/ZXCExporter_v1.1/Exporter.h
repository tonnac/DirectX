#pragma once

#include "ZXCExporter.h"

#define Export_Class_ID Class_ID(0x20777bb2, 0x4fba606d)

class Exporter : public SceneExport
{
public:
	Exporter();
	virtual ~Exporter();

	virtual int				ExtCount()override;
	virtual const MCHAR *	Ext(int n)override;
	virtual const MCHAR *	LongDesc()override;
	virtual const MCHAR *	ShortDesc()override;
	virtual const MCHAR *	AuthorName()override;
	virtual const MCHAR *	CopyrightMessage()override;
	virtual const MCHAR *	OtherMessage1()override;
	virtual const MCHAR *	OtherMessage2()override;
	virtual unsigned int	Version()override;
	virtual void			ShowAbout(HWND hWnd)override;

	virtual BOOL SupportsOptions(int ext, DWORD options)override;
	virtual int  DoExport(const MCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts = FALSE, DWORD options = 0)override;
};


class ClassDescA : public ClassDesc2
{
public:
	virtual int IsPublic() { return TRUE; }
	virtual void* Create(BOOL ) { return new Exporter(); }
	virtual const MCHAR *	ClassName() { return _T("exporter1"); }
	virtual SClass_ID SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
	virtual Class_ID ClassID() { return Export_Class_ID; }
	virtual const MCHAR* Category() { return _T("exporter1"); }

	virtual const MCHAR* InternalName() { return _T("ClassDescA"); }
	virtual HINSTANCE HInstance() { return hInstance; }
};

ClassDesc2* GetExportDesc()
{
	static ClassDescA theDesc;
	return &theDesc;
}