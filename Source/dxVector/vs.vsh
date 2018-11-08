cbuffer cb0
{
	float g_fTimer  : packoffset(c0.x);
	float y			: packoffset(c0.y);
	float z			: packoffset(c0.z);
	float w			: packoffset(c0.w);
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
	vOut.p = float4(vIn.p, 1.0f);
	vOut.c = vIn.c * g_fTimer;
	return vOut;
}