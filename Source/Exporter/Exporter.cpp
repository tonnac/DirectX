#include "Exporter.h"



Exporter::Exporter()
{
}


Exporter::~Exporter()
{
}


int Exporter::ExtCount()
{
	return 1;
}

const MCHAR *Exporter::Ext(int /*i*/)
{
	return L"ggg";
}

const MCHAR *Exporter::LongDesc()
{
	return L"Exporter 1.0";
}

const MCHAR *Exporter::ShortDesc()
{
	return L"Exporter";
}

const MCHAR *Exporter::AuthorName()
{
	return L"ddda";
}

const MCHAR *Exporter::CopyrightMessage()
{
	return L"";
}

const MCHAR *Exporter::OtherMessage1()
{
	return L"";
}

const MCHAR *Exporter::OtherMessage2()
{
	return L"";
}

unsigned int Exporter::Version()
{
	return 100;
}

void Exporter::ShowAbout(HWND /*hWnd*/)
{
}

BOOL Exporter::SupportsOptions(int /*ext*/, DWORD /*options*/)
{
	return TRUE;
}


int	Exporter::DoExport(const MCHAR *name, ExpInterface *ei, Interface *i,
	BOOL suppressPrompts, DWORD options)
{
	return FALSE;
}