static const int g_iNumTexture = 6;

struct VS_IN
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};

struct PS_INPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
	uint id : SV_PrimitiveID;
};

struct VSCUBE_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD0;
	float3 r : TEXCOORD1;
};

struct PSCUBE_INPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD0;
	float3 r : TEXCOORD1;
	uint id : SV_PrimitiveID;
};

cbuffer cb0
{
	matrix g_matWorld	: packoffset(c0);
	matrix g_matView	: packoffset(c4);
	matrix g_matProj	: packoffset(c8);
	float g_fTimer : packoffset(c12.x);
	float y : packoffset(c12.y);
	float z : packoffset(c12.z);
	float w : packoffset(c12.w);
	float4 gColor		: packoffset(c13);
};

Texture2D g_txDiffuse						: register (t0);
Texture2D g_txDiffuseArray[g_iNumTexture]	: register (t1);
TextureCube g_txCubeDiffuse					: register (t2);

SamplerState g_samPoint	: register (s0);

VS_OUTPUT VS(VS_IN vIn)
{
	VS_OUTPUT vOut;
	float4 vPos = float4(vIn.p, 1.0f);
	float4 vWorld = mul(vPos, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	vOut.p = vProj.xyzz;
	vOut.c = vIn.c;
	vOut.n = vIn.n;
	vOut.t = vIn.t;
	return vOut;
}

VSCUBE_OUTPUT VSCube(VS_IN vIn)
{
	VSCUBE_OUTPUT vOut;
	float4 vPos = float4(vIn.p, 1.0f);
	float4 vWorld = mul(vPos, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	vOut.p = vProj.xyzz;
	vOut.c = vIn.c;
	vOut.n = vIn.n;
	vOut.t = vIn.t;
	vOut.r = normalize(vIn.p);
	return vOut;
}

float4 GetTextureColor(uint index, float2 uv)
{
	float4 fColor = float4(1, 1, 1, 1);
	switch (index)
	{
	case 0: fColor = g_txDiffuseArray[0].Sample(g_samPoint, uv); break;
	case 1: fColor = g_txDiffuseArray[1].Sample(g_samPoint, uv); break;
	case 2: fColor = g_txDiffuseArray[2].Sample(g_samPoint, uv); break;
	case 3: fColor = g_txDiffuseArray[3].Sample(g_samPoint, uv); break;
	case 4: fColor = g_txDiffuseArray[4].Sample(g_samPoint, uv); break;
	case 5: fColor = g_txDiffuseArray[5].Sample(g_samPoint, uv); break;
	}
	return fColor;
}

float4 PS(PS_INPUT pIn) : SV_Target
{
	float4 TexColor = g_txDiffuse.Sample(g_samPoint, pIn.t);
	return TexColor;
}

float4 PSArray(PS_INPUT pIn) : SV_Target
{
	uint index = pIn.id / 2;
	float4 fColor = float4(1, 1, 1, 1);
	fColor = GetTextureColor(index, pIn.t);
	return fColor;
}

float4 PSCube(PSCUBE_INPUT pIn) : SV_Target
{
	float4 vTexColor = g_txCubeDiffuse.Sample(g_samPoint, pIn.r);
	return vTexColor;
}