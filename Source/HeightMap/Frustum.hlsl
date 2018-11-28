#include "shaderheader.hlsl"


VS_OUTPUT VS(in VS_IN vIn)
{
	VS_OUTPUT vOut = (VS_OUTPUT)0.0f;

	float4 vPos = float4(vIn.p, 1.0f);
	float4 vWorld = mul(vPos, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	vOut.p = vProj;
	vOut.c = vIn.c;
	vOut.n = vIn.n;
	vOut.t = vIn.t;

	return vOut;
}

float4 PS(in VS_OUTPUT vIn) : SV_Target
{
	float4 Color = vIn.c;
	Color.w = 0.5f;
	return Color;
}