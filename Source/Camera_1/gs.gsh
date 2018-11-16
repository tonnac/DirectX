#include "shaderheader.hlsl"

struct GS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};

[maxvertexcount(9)]
void GS( triangle VS_OUTPUT input[3], 
		 inout TriangleStream<GS_OUTPUT> triStream )
{
	float3 cPos = (input[0].p.xyz +
		input[1].p.xyz +
		input[2].p.xyz) / 3.0f;

	float4 cNor = (input[0].n +
		input[1].n +
		input[2].n) / 3.0f;

	float4 cColor = (input[0].c +
		input[1].c +
		input[2].c) / 3.0f;

	float4 cTex = (input[0].t +
		input[1].t +
		input[2].t) / 3.0f;

	GS_OUTPUT newVertex;
	newVertex.vp = active;

	for (int i = 0; i < 3; ++i)
	{
		newVertex.p = input[i].p;
		newVertex.n = input[i].n;
		newVertex.c = input[0].c;
		newVertex.t = input[i].t;
		triStream.Append(newVertex);

		int iNext = (i + 1) % 3;
		newVertex.p = input[iNext].p;
		newVertex.n = input[iNext].n;
		newVertex.c = input[iNext].c;
		triStream.Append(newVertex);

		newVertex.p = float4(cPos, 1.0f);
		newVertex.c = cColor;
		triStream.Append(newVertex);

		triStream.RestartStrip();
	}

	triStream.RestartStrip();
}