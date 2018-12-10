
#define MaxLights 16

struct Light
{
	float3 LightDir;
	float LightIntensity;
	float3 LightPos;
	float LightRadius;
	float3 EyeDir;
	float FalloffEnd;
	float3 EyePos;
	float FalloffStart;
	float4 g_vSpotInfo;
	float4 LightStrength;
};

float CalcAttenuation(float d, float falloffStart, float falloffEnd)
{
	return saturate((falloffEnd - d) / (falloffEnd - falloffStart));
//	return smoothstep(falloffStart, d, falloffEnd);
}

float3 SpecularLight(float3 normal, float3 halfVec)
{
	float cosIncidentAngle = saturate(dot(normal, halfVec));
	float f0 = 1.0f - cosIncidentAngle;

	float fPower = ((128 + 8) * pow(max(dot(normal, halfVec), 0.0f), 128.0f)) / 8;
	float reflectPercent = 0.1f + (1.0f - 0.1f) * (f0*f0*f0*f0*f0);
	return float3(fPower, fPower, fPower)*reflectPercent;
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

float3 ComputeSpotLight(Light L, float4 color, float3 normal, float4 pos)
{
	float3 lightVec = L.LightPos - pos.xyz;

	float d = length(lightVec);

	if (d > L.FalloffEnd)
		return 0.0f;

	lightVec /= d;

	float3 HalfVec = normalize(lightVec + L.EyeDir);

	float fDot = max(dot(lightVec, normal), 0.0f);
	float att = CalcAttenuation(d, L.FalloffStart, L.FalloffEnd);
	float spotFactor = pow(max(dot(-lightVec, L.LightDir), 0.0f), L.g_vSpotInfo.x);

	float3 lightStrength = L.LightStrength.xyz * fDot * att * spotFactor;

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
//		result += ComputePointLight(gLights[i], color, normal, pos);
	}
#endif

#if (SPOT_LIGHTS > 0)
	for (i = DIR_LIGHTS + POINT_LIGHTS; i < DIR_LIGHTS + POINT_LIGHTS + SPOT_LIGHTS; ++i)
	{
		result += ComputeSpotLight(gLights[i], color, normal, pos);
	}
#endif

	return float4(result, 0.0f);
}