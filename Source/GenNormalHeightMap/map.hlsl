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
	vOut.t = vIn.t *40;
	return vOut;
}

float4 PS(VS_OUTPUT vOut) : SV_Target
{
	float3 Nor = vOut.n;
	float3 LightPos;
	LightPos.x = sin(0.25f * 3.14) * cos(g_fTimer * 0.5f);
	LightPos.y = cos(0.25f * 3.14);
	LightPos.z = sin(0.25f * 3.14) * sin(g_fTimer * 0.5f);

	LightPos = normalize(LightPos);

	float3 LightVec = float3(0.35f, 0.35f, 0.35f);

	float Dot = dot(Nor, LightPos);

	//float4 TexColor = g_txDiffuse.Sample(g_SamState, vOut.t);
	//TexColor *= Dot;
	//TexColor.a = 1.0f;
	//return TexColor;

	float4 Color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	Color = float4(Color.xyz * LightVec * Dot , 1.0f);

	return Color;
}
