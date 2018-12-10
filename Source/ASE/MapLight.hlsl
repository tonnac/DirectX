
#ifndef DIR_LIGHTS
	#define DIR_LIGHTS 0
#endif

#ifndef POINT_LIGHTS
	#define POINT_LIGHTS 0
#endif

#ifndef SPOT_LIGHTS
	#define SPOT_LIGHTS 1
#endif


#include "LightingUtil.hlsl"

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
	float4 w : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD0;
	float4 l : TEXCOORD1;
};

cbuffer cb0 : register(b0)
{
	matrix g_matWorld		: packoffset(c0);
	matrix g_matView		: packoffset(c4);
	matrix g_matProj		: packoffset(c8);
	float g_fTimer			: packoffset(c12.x);
	float y					: packoffset(c12.y);
	float z					: packoffset(c12.z);
	float w					: packoffset(c12.w);
	float4 gColor			: packoffset(c13);
	float4x4 g_matInvWorld	: packoffset(c14);
};

cbuffer cb1 : register(b1)
{
	Light gLights[MaxLights];
};

static const float4 gAmbient = float4(0.3f, 0.3f, 0.3f, 1.0f);

Texture2D g_txDiffuse	: register (t0);
SamplerState g_SamState	: register (s0);

VS_OUTPUT VS(VS_IN vIn)
{
	VS_OUTPUT vOut = (VS_OUTPUT)0.0f;
	float4 PosW = mul(float4(vIn.p, 1.0f), g_matWorld);
	vOut.w = PosW;
	PosW = mul(PosW, g_matView);
	PosW = mul(PosW, g_matProj);

	vOut.p = PosW;
	vOut.n = normalize(mul(vIn.n, (float3x3)g_matWorld));
	vOut.c = vIn.c;
	vOut.t = vIn.t;

	return vOut;
}

float4 PS(VS_OUTPUT vOut) : SV_Target
{
	float4 Diffuse = g_txDiffuse.Sample(g_SamState, vOut.t);

	float4 Color = float4(0.7f, 0.7f, 0.7f, 1.0f);

	float4 ambient = Diffuse * gAmbient;

	float4 directLight = ComputeLighting(gLights, Diffuse, vOut.n, vOut.w);

	float4 retColor = ambient + directLight;

	retColor.a = 1.0f;

	return retColor;
}
