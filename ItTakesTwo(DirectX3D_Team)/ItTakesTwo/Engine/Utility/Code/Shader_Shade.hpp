texture			g_NormalTexture;

sampler		NormalSampler = sampler_state
{
	texture = g_NormalTexture;
};

texture			g_DepthTexture;

sampler		DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

texture			g_LightDepthTexture;
sampler		LightDepthSampler = sampler_state
{
	texture = g_LightDepthTexture;
};

texture			g_RoughnessTexture;
sampler		RoughnessSampler = sampler_state
{
	texture = g_RoughnessTexture;
};


vector		g_vLightDir;
vector		g_vLightDiffuse;
vector		g_vLightAmbient;

vector		g_vMtrlDiffuse = (vector)1.f;
vector		g_vMtrlAmbient = (vector)1.f;

vector		g_vCamPos;
float		g_fPower;

matrix     g_matViewInv;
matrix     g_matProjInv;

vector		g_vLightPos;
float		g_fRange;

float		g_fCosOuter;
float		g_fCosInner;

matrix			g_matLightView;
matrix			g_matLightProj;



struct PS_IN
{
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
	vector		vSpecularTexture : COLOR2;
};



PS_OUT			PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector	vNormal = tex2D(NormalSampler, In.vTexUV);

	vNormal = vector(vNormal.xyz * 2.f - 1.f, 0.f);
	vector shade = saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);

	vector		vReflect = normalize(reflect(normalize(vector(g_vLightDir.xyz, 0.f)), vNormal));

	// 깊이 타겟으로부터 얻어온 값

	// R의 위치에 z나누기가 끝난 투영의 z를 보관 : vDepth.x
	// G의 위치에 뷰스페이스 영역의 z를 보관(1000(FAR)으로 나누고 있는 중 : 텍스쳐UV는 0~1이니깐) : vDepth.y
	vector		vDepth = tex2D(DepthSampler, In.vTexUV);

	float		fViewZ = vDepth.y * 1000.f;

	vector		vPosition;

	// 0 ~ 1 -> -1 ~ 1 (텍스처 uv좌표에서 투영 좌표로 변환)
	// z나누기 이전의 투영상태의 z값
	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.x * fViewZ;
	vPosition.w = fViewZ;

	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);

	vector		vLook = normalize(g_vCamPos - vPosition);


	// Through brute force iteration I found this approximation. Time to test it out.
	half LdotN = dot(g_vLightDir, vNormal);
	half VdotN = dot(vLook, vNormal);
	half result = saturate(LdotN);
	half soft_rim = saturate(1.f - VdotN / 2.f); //soft view dependant rim
	half fakey = pow(1.f - result*soft_rim, 2.f);//modulate lambertian by rim lighting
	half fakey_magic = 0.62f;
	//(1-fakey)*fakey_magic to invert and scale down the lighting
	fakey = fakey_magic - fakey * fakey_magic;



	vector  vRoughness = vector(vDepth.w, vDepth.w, vDepth.w, 1.f);

	Out.vShade = shade + ((shade * lerp(result, fakey, vRoughness) * 0.5f));
	//Out.vShade.a = 1.f;

	float amb = -g_vLightDir.w;
	float3 L = -g_vLightDir;
	float3 H = normalize(L + vLook);

	vector vBlinPong = max(amb, dot(vNormal, L)) + pow(max(0, dot(vNormal, H)), 10);

	if (vDepth.w == 0)
	{
		Out.vSpecular = 0.f;
	}
	else if (vDepth.w == 1)
	{
		//// 블린퐁
		Out.vSpecular = vBlinPong;
	}
	else if (vDepth.w == 2)
	{
		// 쿡토런스
		float NV = dot(vNormal, vLook);
		float NH = dot(vNormal, H);
		float VH = dot(vLook, H);
		float NL = dot(vNormal, L);
		float LH = dot(L, H);

		const float m = 0.35f;
		float NH2 = NH * NH;
		float D = exp(-(1 - NH2) / (NH2 * m * m)) / (4 * m * m * NH2 * NH2);

		float G = min(1, min(2 * NH * NV / VH, 2 * NH * NL / VH));

		float n = 20.f;
		float g = sqrt(n * n + LH * LH - 1);
		float gpc = g + LH;
		float gnc = g - LH;
		float cgpc = LH * gpc - 1;
		float cgnc = LH * gnc + 1;
		float F = 0.5f * gnc * gnc * (1 + cgpc * cgpc / (cgnc * cgnc)) / (gpc * gpc);
		float F0 = ((n - 1) * (n - 1)) / ((n + 1) * (n + 1));

		float4 light_color = { 0.296f, 0.304f, 1.f, 1.f };
		float4 c0 = { 0.486f * light_color.x, 0.433f * light_color.y, 0.185f * light_color.z, 1.f };
		float4 color = c0 + (light_color - c0) * max(0, F - F0) / (1 - F0);

		//vector ks = { 2.f * 0.486f, 2.f * 0.433f, 2.f * 0.185f, 1.0f };
		Out.vSpecular = vBlinPong * max(0, F * D * G / NV);
	}

	Out.vSpecularTexture = Out.vSpecular * vDepth.z * g_vLightDiffuse;

	return Out;
}

PS_OUT			PS_POINT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector	vNormal = tex2D(NormalSampler, In.vTexUV);
	vNormal = vector(vNormal.xyz * 2.f - 1.f, 0.f);


	vector		vDepth = tex2D(DepthSampler, In.vTexUV);

	float		fViewZ = vDepth.y * 1000.f;

	vector		vPosition;

	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.x * fViewZ;
	vPosition.w = fViewZ;

	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);

	vector		vLightDir = vPosition - g_vLightPos;
	float		fDistance = length(vLightDir);
	vector		vLook = normalize(g_vCamPos - vPosition);

	float		fAtt = saturate((g_fRange - fDistance) / g_fRange);

	vector shade = (saturate(dot(normalize(vLightDir) * -1.f, vNormal)) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;

	// Through brute force iteration I found this approximation. Time to test it out.
	half LdotN = dot(g_vLightDir, vNormal);
	half VdotN = dot(vLook, vNormal);
	half result = saturate(LdotN);
	half soft_rim = saturate(1.f - VdotN / 2.f); //soft view dependant rim
	half fakey = pow(1.f - result*soft_rim, 2.f);//modulate lambertian by rim lighting
	half fakey_magic = 0.62f;
	//(1-fakey)*fakey_magic to invert and scale down the lighting
	fakey = fakey_magic - fakey * fakey_magic;



	vector  vRoughness = vector(vDepth.w, vDepth.w, vDepth.w, 1.f);

	Out.vShade = shade + ((shade * lerp(result, fakey, vRoughness) * 0.5f));
	//Out.vShade.a = 1.f;



	vector		vReflect = normalize(reflect(normalize(vector(vLightDir.xyz, 0.f)), vNormal));

	float amb = -g_vLightDir.w;
	float3 L = -g_vLightDir.xyz;
	float3 N = normalize(vNormal.xyz);
	float3 V = normalize(g_vCamPos - vPosition);
	float3 H = normalize(L + V);

	vector vBlinPong = max(amb, dot(vNormal, L)) + pow(max(0, dot(vNormal, H)), 10);


	vector vColor = max(amb, dot(vNormal, -vLightDir));

	if (vDepth.w == 0)
	{
		Out.vSpecular = 0.f;
	}
	else if (vDepth.w == 1)
	{
		// 블린퐁
		Out.vSpecular = vBlinPong * fAtt;
	}
	else if (vDepth.w == 2)
	{
		// 쿡토런스
		float NV = dot(N, V);
		float NH = dot(N, H);
		float VH = dot(V, H);
		float NL = dot(N, L);
		float LH = dot(L, H);

		const float m = 0.35f;
		float NH2 = NH * NH;
		float D = exp(-(1 - NH2) / (NH2 * m * m)) / (4 * m * m * NH2 * NH2);

		float G = min(1, min(2 * NH * NV / VH, 2 * NH * NL / VH));

		float n = 3.f;
		float g = sqrt(n * n + LH * LH - 1);
		float gpc = g + LH;
		float gnc = g - LH;
		float cgpc = LH * gpc - 1;
		float cgnc = LH * gnc + 1;
		float F = 0.5f * gnc * gnc * (1 + cgpc * cgpc / (cgnc * cgnc)) / (gpc * gpc);
		//float F0 = ((n - 1) * (n - 1)) / ((n + 1) * (n + 1));

		float4 ks = {2.f * 0.486f, 2.f * 0.433f, 2.f * 0.185f, 1.f};
		//float4 light_color = { 0.296f, 0.304f, 1.f, 1.f };
		//float4 c0 = { 0.486f * light_color.x, 0.433f * light_color.y, 0.185f * light_color.z, 1.f };
		//float4 color = c0 + (light_color - c0) * max(0, F - F0) / (1 - F0);

		Out.vSpecular = ks * max(0, F * D * G / NV) * fAtt;
	}
	Out.vSpecularTexture = Out.vSpecular * vDepth.z * g_vLightDiffuse * fAtt;


	return Out;
}


PS_OUT			PS_SPOT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector	vNormal = tex2D(NormalSampler, In.vTexUV);

	// texture uv 좌표를 월드 상태의 -1~1사이로 변환
	vNormal = vector(vNormal.xyz * 2.f - 1.f, 0.f);

	// R의 위치에 z나누기가 끝난 투영의 z를 보관 : vDepth.x
	// G의 위치에 뷰스페이스 영역의 z를 보관(1000(FAR)으로 나누고 있는 중 : 텍스쳐UV는 0~1이니깐) : vDepth.y
	vector		vDepth = tex2D(DepthSampler, In.vTexUV);

	float		fViewZ = vDepth.y * 1000.f;

	vector		vPosition;

	// 0 ~ 1 -> -1 ~ 1 (텍스처 uv좌표에서 투영 좌표로 변환)
	// z나누기 이전의 투영상태의 z값
	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.x * fViewZ;
	vPosition.w = fViewZ;

	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);

	vector		vLightDir = vPosition - g_vLightPos;
	float		fDistance = length(vLightDir);

	float		fAtt = saturate((g_fRange - fDistance) / g_fRange);

	float fCosCurrent = dot(normalize(g_vLightDir), normalize(vLightDir));
	float fConAtt = saturate((fCosCurrent - g_fCosOuter) / g_fCosInner);

	Out.vShade = (saturate(dot(normalize(vLightDir) * -1.f, vNormal)) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt * fConAtt;
	Out.vShade.a = 1.f;

	vector		vReflect = normalize(reflect(normalize(vector(vLightDir.xyz, 0.f)), vNormal));

	vector		vLook = normalize(g_vLightPos - vPosition);

	Out.vSpecular = pow(saturate(dot(vReflect, vLook)), g_fPower) * fAtt * fConAtt;


	return Out;
}


technique Default_Device
{
	pass	directional	// 기능의 캡슐화
	{
		zwriteenable = false;
alphablendenable = true;
srcblend = one;
destblend = one;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass	PointLight
	{
		zwriteenable = false;
		alphablendenable = true;
		srcblend = one;
		destblend = one;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_POINT();
	}

		pass	SpotLight
	{
		zwriteenable = false;
	alphablendenable = true;
	srcblend = one;
	destblend = one;

	VertexShader = NULL;
	PixelShader = compile ps_3_0 PS_SPOT();
	}
};
