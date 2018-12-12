#pragma once
#include "DxObj.h"

struct PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR4 c;
};

struct PNCT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR4 c;
	D3DXVECTOR2 t;
	PNCT_VERTEX()
	{}
	PNCT_VERTEX(D3DXVECTOR3 vp, D3DXVECTOR3 np, D3DXVECTOR4 vc, D3DXVECTOR2 vt)
	{
		p = vp;
		n = np;
		c = vc;
		t = vt;
	}
	PNCT_VERTEX(const PNCT_VERTEX& rhs)
	{
		p = rhs.p;
		n = rhs.n;
		c = rhs.c;
		t = rhs.t;
	}
	bool operator == (const PNCT_VERTEX& rhs) const
	{
		if (p == rhs.p && n == rhs.n && c == rhs.c && t == rhs.t)
		{
			return true;
		}
		return false;
	}
};

struct PT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR2 t;
};

struct CB_DATA
{
	D3DXMATRIX matWorld;
	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	float fTime;
	float fActiveVP;
	float z, w;
	D3DXVECTOR4 vColor;
	D3DXMATRIX matInvWorld;
};
