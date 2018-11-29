#include "shaderheader.hlsl"

VS_OUTPUT VS(VS_IN vIn)
{
	VS_OUTPUT vOut;
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

PC_OUT VS_PC(PC_IN vIn)
{
	PC_OUT vOut;
	float4 vPos = float4(vIn.p, 1.0f);
	float4 vWorld = mul(vPos, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	vOut.p = vProj;
	vOut.c = vIn.c;
	return vOut;
}

float4 PS(VS_OUTPUT vOut) : SV_Target
{
	float4 TexColor = g_txDiffuse.Sample(g_SamState, vOut.t);
	return TexColor;
}

float4 PSLine(PC_OUT vOut) : SV_Target
{
	float4 Color = vOut.c;
	return Color;
}