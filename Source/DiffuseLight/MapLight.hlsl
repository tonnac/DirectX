
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
	float2 t : TEXCOORD0;
	float4 l : TEXCOORD1;
};

cbuffer cb0 : register(b0)
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

cbuffer cb1 : register(b1)
{
	float4x4 g_matInvWorld;
	float4 g_vLightDir;
	float4 g_vLightPos;
	float4 g_vEyeDir;
	float4 g_vEyePos;
};

Texture2D g_txDiffuse	: register (t0);
SamplerState g_SamState	: register (s0);

VS_OUTPUT VS(VS_IN vIn)
{
	VS_OUTPUT vOut;
	float4 vPos = float4(vIn.p, 1.0f);
	float4 vWorld = mul(vPos, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	vOut.p = vProj;
	vOut.c = vIn.c;
	vOut.n = mul(vIn.n, (float3x3)g_matInvWorld);

	float fDot = dot(vOut.n, g_vLightDir);
	vOut.l = float4(fDot, fDot, fDot, 1.0f);

	vOut.t = vIn.t * 40;
	return vOut;
}

float4 PS(VS_OUTPUT vOut) : SV_Target
{
	return vOut.l;
	//float3 Nor = vOut.n;
	//float3 LightPos;
	//LightPos.x = sin(0.25f * 3.14) * cos(g_fTimer * 0.5f);
	//LightPos.y = cos(0.25f * 3.14);
	//LightPos.z = sin(0.25f * 3.14) * sin(g_fTimer * 0.5f);

	//LightPos = normalize(LightPos);

	//float3 LightVec = float3(0.35f, 0.35f, 0.35f);

	//float Dot = dot(Nor, LightPos);

	////float4 TexColor = g_txDiffuse.Sample(g_SamState, vOut.t);
	////TexColor *= Dot;
	////TexColor.a = 1.0f;
	////return TexColor;

	//float4 Color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	//Color = float4(Color.xyz * LightVec * Dot , 1.0f);

	//return Color;
}
