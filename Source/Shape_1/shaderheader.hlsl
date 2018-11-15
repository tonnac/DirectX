

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

struct PC_IN
{
	float3 p : POSITION;
	float4 c : COLOR;
};

struct PC_OUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR;
};

cbuffer cb0
{
	matrix g_matWorld	: packoffset(c0);
	matrix g_matView	: packoffset(c4);
	matrix g_matProj	: packoffset(c8);
	float g_fTimer		: packoffset(c12.x);
	float y				: packoffset(c12.y);
	float z				: packoffset(c12.z);
	float w				: packoffset(c12.w);
	float4 gColor		: packoffset(c13);
};

Texture2D g_txDiffuse	: register (t0);
SamplerState g_SamState	: register (s0);