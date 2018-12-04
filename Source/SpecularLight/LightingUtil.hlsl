
#define MaxLights 16

struct Light
{
	float4x4 invWorld;
	float3 LightDir;
	float LightIntensity;
	float3 LightPos;
	float LightRadius;
	float4 LightStrength;
	float3 EyeDir;
	float FalloffEnd;
	float3 EyePos;
	float FalloffStart;
};

float CalcAttenuation(float d, float falloffStart, float falloffEnd)
{
	return saturate((falloffEnd - d) / (falloffEnd - falloffStart));
}

float3 SpecularLight(float3 normal, float3 halfVec)
{
	float fPower = ((256 + 8) * pow(max(dot(normal, halfVec), 0.0f), 256.0f)) / 8;
	return float3(fPower, fPower, fPower);
}

float3 ComputeDirectionalLight(Light L, float4 color ,float3 normal)
{
	float3 lightVec = -L.LightDir;

	float3 HalfVec = normalize(lightVec + L.EyeDir);

	float fDot = max(dot(lightVec, normal), 0.0f);
	float3 lightStrength = L.LightStrength.xyz * fDot;

	float3 specular = SpecularLight(normal, HalfVec);
	float3 diffuse = color.xyz;
	return (diffuse + specular) * lightStrength;
}

float3 ComputePointLight(Light L, float4 color, float3 normal, float4 pos)
{
	float3 lightVec = L.LightPos - pos.xyz;

	float d = length(lightVec);

	if (d > L.FalloffEnd)
		return 0.0f;

	lightVec /= d;

	float3 HalfVec = normalize(lightVec + L.EyeDir);

	float fDot = max(dot(lightVec, normal), 0.0f);
	float att = CalcAttenuation(d, L.FalloffStart, L.FalloffEnd);

	float3 lightStrength = L.LightStrength.xyz * fDot * att;

	float3 specular = SpecularLight(normal, HalfVec);
	float3 diffuse = color.xyz;

	return (diffuse + specular) * lightStrength;
}

float4 ComputeLighting(Light gLights[MaxLights], float4 color, float3 normal, float4 pos)
{
	float3 result = 0.0f;

	int i = 0;

#if (DIR_LIGHTS > 0)
	for (i = 0; i < DIR_LIGHTS; ++i)
	{
//		result += ComputeDirectionalLight(gLights[i], color, normal);
	}
#endif

#if (POINT_LIGHTS > 0)
	for (i = DIR_LIGHTS; i < DIR_LIGHTS + POINT_LIGHTS; ++i)
	{
		result += ComputePointLight(gLights[i], color, normal, pos);
	}
#endif

	return float4(result, 0.0f);
}