matrix			g_matWorld;	 // 상수 테이블
matrix			g_matView;
matrix			g_matProj;

texture			g_BaseTexture;
vector			g_vecAlpha;
float			g_TimeDelta;

float			g_fRimDist;
int				g_iRimPower;
vector			g_vRimColor;


sampler		BaseSampler = sampler_state
{
	texture = g_BaseTexture;

minfilter = linear;
magfilter = linear;
};

texture			g_ElectricTexture;
sampler		ElectricSampler = sampler_state
{
	texture = g_ElectricTexture;

minfilter = linear;
magfilter = linear;
};

texture			g_DepthTexture;
sampler		DepthSampler = sampler_state
{
	texture = g_DepthTexture;

minfilter = linear;
magfilter = linear;
};

vector		g_vLightDir;

vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlDiffuse;
vector		g_vMtrlAmbient;
vector		g_vMtrlSpecular;

float		g_fPower;

vector		g_vCamPos;

sampler2D Texture0;

float4 gaussFilterX[7] =
{
	0.0, -6.0, 0.0, 1.0 / 64.0,
	0.0, -4.0, 0.0, 6.0 / 64.0,
	0.0, -2.0, 0.0, 15.0 / 64.0,
	0.0, 0.0, 0.0, 20.0 / 64.0,
	0.0, 2.0, 0.0, 15.0 / 64.0,
	0.0, 4.0, 0.0, 6.0 / 64.0,
	0.0, 6.0, 0.0, 1.0 / 64.0,
};

float4 gaussFilterY[7] =
{
	-6.0, 0.0, 0.0, 1.0 / 64.0,
	-4.0, 0.0, 0.0, 6.0 / 64.0,
	-2.0, 0.0, 0.0, 15.0 / 64.0,
	0.0, 0.0,0.0, 20.0 / 64.0,
	2.0, 0.0,0.0, 15.0 / 64.0,
	4.0, 0.0,0.0, 6.0 / 64.0,
	6.0, 0.0,0.0, 1.0 / 64.0,
};

float texScaler = 1.0 / 128.0;
float texOffset = 0.0;


struct VS_IN
{
	float4			vPosition : POSITION;
	float4			vNormal : NORMAL;
	float2			vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4			vPosition : POSITION;
	float4			vNormal : NORMAL;
	float2			vTexUV : TEXCOORD0;
	float4			vProjPos : TEXCOORD1;
	float4			vPos : TEXCOORD2;
	float2			TexCoord : TEXCOORD3;
};

// 정점 쉐이더

VS_OUT			VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	Out.vPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);

	return Out;
}

struct PS_IN
{
	vector			vNormal : NORMAL;
	float2			vTexUV : TEXCOORD0;
	float4			vProjPos : TEXCOORD1;
	float4			vPos : TEXCOORD2;
	float4			vEmissive : TEXCOORD3;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
	vector		vNormal : COLOR1;
	vector		vDepth : COLOR2;
};


PS_OUT			PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;


	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor.a = 1.f;

	// -1~1인 월드 상태의 좌표를 0~1인 텍스쳐 uv좌표로 변환

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w,	// R의 위치에 z나누기가 끝난 투영의 z를 보관
		In.vProjPos.w * 0.001f,			// G의 위치에 뷰스페이스 영역의 z를 보관(1000(FAR)으로 나누고 있는 중 : 텍스쳐UV는 0~1이니깐)
		0.f,
		0.f);

	return Out;
}

PS_OUT			PS_MAIN_ALPHATEST(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2차원 텍스처로부터 uv좌표에 해당하는 색을 추출하여 반환하는 함수(반환 타입이 vector타입)

	return Out;
}

PS_OUT			PS_MAIN_ELECTRIC(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	// UV 좌표 이동
	In.vTexUV.x -= g_TimeDelta * 0.2f;
	In.vTexUV.y -= g_TimeDelta * 0.3f;

	// 림라이트

	float3 viewDir = normalize(g_vCamPos.xyz - In.vPos);
	float rim = smoothstep(1.f - g_fRimDist, 1.f, 1.f - dot(In.vNormal.xyz, viewDir));
	float3 rimLight = (pow(rim, g_iRimPower) + g_vRimColor).xyz;


	vector ColorX = 0;
	vector ColorY = 0;
	for (int i = 0; i < 7; ++i)
	{
		ColorX += tex2D(BaseSampler, float2(In.vTexUV.x + gaussFilterX[i].x * texScaler + texOffset,
			In.vTexUV.y + gaussFilterX[i].y * texScaler + texOffset)) * gaussFilterX[i].w;

		ColorY += tex2D(BaseSampler, float2(In.vTexUV.x + gaussFilterY[i].x * texScaler + texOffset,
			In.vTexUV.y + gaussFilterY[i].y * texScaler + texOffset)) * gaussFilterY[i].w;
	}

	vector Blur = (ColorX + ColorY) / 2;

	// 이미시브

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2차원 텍스처로부터 uv좌표에 해당하는 색을 추출하여 반환하는 함수(반환 타입이 vector타입)

	if (Out.vColor.r == 0 && Out.vColor.g != 0 && Out.vColor.b != 0)
	{
		Out.vColor.rgb = float3(0.1f, 0.1f, 0.1f);
		Out.vColor.a = g_vecAlpha.w;
		Out.vColor += vector(rimLight.xyz, 0.1f);

		Blur.rgb = float3(0.1f, 0.1f, 0.1f);
		Blur.a = g_vecAlpha.w;
		Blur += vector(rimLight.xyz, 0.1f);
	}
	else if (Out.vColor.r != 0 && Out.vColor.g == 0 && Out.vColor.b != 0)
	{
		Out.vColor.rgb = float3(0.1f, 0.1f, 0.1f);
		Out.vColor.a = g_vecAlpha.w;
		Out.vColor += vector(rimLight.xyz, 0.1f);

		Blur.rgb = float3(0.1f, 0.1f, 0.1f);
		Blur.a = g_vecAlpha.w;
		Blur += vector(rimLight.xyz, 0.1f);
	}
	else
	{
		Out.vColor.a = 0.f;
		Out.vColor += vector(rimLight.xyz, 0.1f);

		Blur.a = 0.f;
		Blur += vector(rimLight.xyz, 0.1f);
	}

	Out.vColor += Blur;


	//Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	//Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w,	// R의 위치에 z나누기가 끝난 투영의 z를 보관
	//	In.vProjPos.w * 0.001f,			// G의 위치에 뷰스페이스 영역의 z를 보관(1000(FAR)으로 나누고 있는 중 : 텍스쳐UV는 0~1이니깐)
	//	0.f,
	//	0.f);


	return Out;
}

PS_OUT			PS_MAIN_RING(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	// 림라이트

	float3 viewDir = normalize(g_vCamPos.xyz - In.vPos);
	float rim = smoothstep(1.f - g_fRimDist, 1.f, 1.f - dot(In.vNormal.xyz, viewDir));
	float3 rimLight = (pow(rim, g_iRimPower) + g_vRimColor).xyz;


	vector ColorX = 0;
	vector ColorY = 0;
	for (int i = 0; i < 7; ++i)
	{
		ColorX += tex2D(BaseSampler, float2(In.vTexUV.x + gaussFilterX[i].x * texScaler + texOffset,
			In.vTexUV.y + gaussFilterX[i].y * texScaler + texOffset)) * gaussFilterX[i].w;

		ColorY += tex2D(BaseSampler, float2(In.vTexUV.x + gaussFilterY[i].x * texScaler + texOffset,
			In.vTexUV.y + gaussFilterY[i].y * texScaler + texOffset)) * gaussFilterY[i].w;
	}

	vector Blur = (ColorX + ColorY) / 2;


	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vElectric = tex2D(ElectricSampler, In.vTexUV );
	//vElectric.a = 0.1f;
	//Blur.a = 0.1f;
	Out.vColor = vColor * vElectric;
	Out.vColor += vector(rimLight.xyz, 0.1f);

	//Blur += vector(rimLight.xyz, 0.01f);
	//Out.vColor *= Blur ;


	return Out;
}

PS_OUT			PS_MAIN_JELLY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	// UV 좌표 이동
	In.vTexUV.x += g_TimeDelta * 3.f;
	In.vTexUV.y += g_TimeDelta * 3.f;

	// 림라이트

	float3 viewDir = normalize(g_vCamPos.xyz - In.vPos);
	float rim = smoothstep(1.f - g_fRimDist, 1.f, 1.f - dot(In.vNormal.xyz, viewDir));
	float3 rimLight = (pow(rim, g_iRimPower) + g_vRimColor).xyz;


	vector ColorX = 0;
	vector ColorY = 0;
	for (int i = 0; i < 7; ++i)
	{
		ColorX += tex2D(BaseSampler, float2(In.vTexUV.x + gaussFilterX[i].x * texScaler + texOffset,
			In.vTexUV.y + gaussFilterX[i].y * texScaler + texOffset)) * gaussFilterX[i].w;

		ColorY += tex2D(BaseSampler, float2(In.vTexUV.x + gaussFilterY[i].x * texScaler + texOffset,
			In.vTexUV.y + gaussFilterY[i].y * texScaler + texOffset)) * gaussFilterY[i].w;
	}

	vector Blur = (ColorX + ColorY) / 2;


	vector vColor = tex2D(BaseSampler, In.vTexUV * 0.7f);
	vector vElectric = tex2D(ElectricSampler, In.vTexUV * 0.7f);


	Out.vColor = vColor * vElectric;
	Out.vColor.a = vColor.r * vElectric.a;
	Out.vColor += vector(rimLight.xyz, 0.01f);

	Blur.a *= vColor.r;
	Blur += vector(rimLight.xyz, 0.1f);

	Out.vColor += Blur;


	return Out;
}

PS_OUT			PS_MAIN_RING2(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	// UV 좌표 이동
	//In.vTexUV.x += g_TimeDelta * 3.f;
	In.vTexUV.y += g_TimeDelta * 1.5f;

	// 림라이트

	float3 viewDir = normalize(g_vCamPos.xyz - In.vPos);
	float rim = smoothstep(1.f - g_fRimDist, 1.f, 1.f - dot(In.vNormal.xyz, viewDir));
	float3 rimLight = (pow(rim, g_iRimPower) + g_vRimColor).xyz;


	vector ColorX = 0;
	vector ColorY = 0;
	for (int i = 0; i < 7; ++i)
	{
		ColorX += tex2D(BaseSampler, float2(In.vTexUV.x + gaussFilterX[i].x * texScaler + texOffset,
			In.vTexUV.y + gaussFilterX[i].y * texScaler + texOffset)) * gaussFilterX[i].w;

		ColorY += tex2D(BaseSampler, float2(In.vTexUV.x + gaussFilterY[i].x * texScaler + texOffset,
			In.vTexUV.y + gaussFilterY[i].y * texScaler + texOffset)) * gaussFilterY[i].w;
	}

	vector Blur = (ColorX + ColorY) / 2;


	vector vColor = tex2D(BaseSampler, In.vTexUV * 10);
	vector vElectric = tex2D(ElectricSampler, In.vTexUV * 10);
	//vElectric.a = 0.1f;
	//Blur.a = 0.1f;
	Out.vColor = vColor * vElectric;
	Out.vColor += vector(rimLight.xyz, 0.1f);

	//Blur += vector(rimLight.xyz, 0.01f);
	//Out.vColor *= Blur ;


	return Out;
}

technique Default_Device
{
	pass // 0
{
	//alphablendenable = true;
//srcblend = srcalpha;
//destblend = invsrcalpha;
//cullmode = None;

	VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN();
}

pass // 1
{
	alphatestenable = true;
alpharef = 0xc0;
alphafunc = greater;
cullmode = None;

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN_ALPHATEST();
}

pass Electric // 2
{
	//zwriteenable = false;
alphablendenable = true;
srcblend = srcalpha;
destblend = invsrcalpha;
cullmode = None;
//zenable = false;


VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN_ELECTRIC();
}

	pass Ring // 3
{
	//zwriteenable = false;
	alphablendenable = true;
srcblend = srcalpha;
destblend = invsrcalpha;
cullmode = None;
//zenable = false;


VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN_RING();
}

pass Jelly // 4
{
	//zwriteenable = false;
	alphablendenable = true;
srcblend = srcalpha;
destblend = invsrcalpha;
cullmode = None;
//zenable = false;


VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN_JELLY();
}

pass Ring2 // 5
{
	//zwriteenable = false;
	alphablendenable = true;
srcblend = srcalpha;
destblend = invsrcalpha;
cullmode = None;
//zenable = false;


VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN_RING2();
}
};


