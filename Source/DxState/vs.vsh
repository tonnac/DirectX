cbuffer cb0
{
	matrix g_matWorld : packoffset(c0);
	matrix g_matView  : packoffset(c4);
	matrix g_matProj  : packoffset(c8);
	float g_fTimer    : packoffset(c12.x);
	float y			  : packoffset(c12.y);
	float z			  : packoffset(c12.z);
	float w			  : packoffset(c12.w);
	float4 gColor	  : packoffset(c13);
};

struct VS_IN
{
	float3 p : POSITION;
	float4 c : COLOR;
};

struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR;
};

VS_OUTPUT VS(VS_IN vIn)
{
	VS_OUTPUT vOut;
	float4 vPos = float4(vIn.p, 1.0f);
	float4 vWorld = mul(vPos, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	vOut.p = vProj;
	vOut.c = vIn.c;
	return vOut;
}