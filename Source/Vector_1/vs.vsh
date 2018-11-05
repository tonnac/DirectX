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
	vOut.c = vIn.c;
	return vOut;
}