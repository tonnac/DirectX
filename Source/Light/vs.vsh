
#include "lightutil.hlsl"

cbuffer cb0
{
	matrix g_matWorld : packoffset(c0);
	matrix g_matView  : packoffset(c4);
	matrix g_matProj  : packoffset(c8);
	float g_fTimer    : packoffset(c12.x);
	float y			  : packoffset(c12.y);
	float z			  : packoffset(c12.z);
	float w			  : packoffset(c12.w);
	float3 gEyePosW : packoffset(c13);
	float pad1 : packoffset(c13.w);
	float4 gAmbientLight : packoffset(c14);
	float4 gDiffuseAlbedo : packoffset(c15);
	float3 gFresnelR0 : packoffset(c16);
	float gRoughness : packoffset(c16.w);

	Light Lights : packoffset(c17);
};

struct VS_IN
{
	float3 p : POSITION;
	float3 n : NORMAL;
};

struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 pW : POSITION;
	float3 NormalW : NORMAL;
};

VS_OUTPUT VS(VS_IN vIn)
{
	VS_OUTPUT vout;

	float4 posW = mul(float4(vIn.p, 1.0f), g_matWorld);
	vout.pW = posW.xyz;

	vout.NormalW = mul(vIn.n, (float3x3)g_matWorld);

	float4 vView = mul(posW, g_matView);
	float4 vProj = mul(vView, g_matProj);
	vout.p = vProj;

	return vout;
}