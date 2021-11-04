matrix			g_matWorld;	 // 상수 테이블
matrix			g_matView;
matrix			g_matProj;

texture			g_BaseTexture;

float			g_Fog;

float			g_fRimDist;
int				g_iRimPower;
vector			g_vRimColor;

sampler		BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

texture		g_DepthTexture;

sampler		DepthSampler = sampler_state
{
	texture = g_DepthTexture;

minfilter = linear;
magfilter = linear;
};

texture		g_LightMaskTexture;

sampler		MaskSampler = sampler_state
{
	texture = g_LightMaskTexture;

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

float4 gaussFilterX[7] =
{
	0.0, -9.0, 0.0, 1.0 / 64.0,
	0.0, -6.0, 0.0, 6.0 / 64.0,
	0.0, -3.0, 0.0, 15.0 / 64.0,
	0.0, 0.0, 0.0, 20.0 / 64.0,
	0.0, 3.0, 0.0, 15.0 / 64.0,
	0.0, 6.0, 0.0, 6.0 / 64.0,
	0.0, 9.0, 0.0, 1.0 / 64.0,
};

float4 gaussFilterY[7] =
{
	-9.0, 0.0, 0.0, 1.0 / 64.0,
	-6.0, 0.0, 0.0, 6.0 / 64.0,
	-3.0, 0.0, 0.0, 15.0 / 64.0,
	0.0, 0.0,0.0, 20.0 / 64.0,
	3.0, 0.0,0.0, 15.0 / 64.0,
	6.0, 0.0,0.0, 6.0 / 64.0,
	9.0, 0.0,0.0, 1.0 / 64.0,
};

float texScaler = 1.0 / 128.0;
float texOffset = 0.0;

struct VS_IN
{
	float4			vPosition	: POSITION;
	float4			vNormal		: NORMAL;
	float2			vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	float4			vPosition	: POSITION;
	float4			vNormal		 : NORMAL;
	float2			vTexUV		: TEXCOORD0;
	float4			vProjPos : TEXCOORD1;
	float4			vPos : TEXCOORD2;
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

	Out.vColor  = tex2D(BaseSampler, In.vTexUV);	
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

PS_OUT			PS_MAIN_CONE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float3 viewDir = normalize(g_vCamPos.xyz - In.vPos);
	float rim = smoothstep(1.f - g_fRimDist, 1.f, 1.f - dot(In.vNormal.xyz, viewDir));
	float3 rimLight = (pow(rim, g_iRimPower) + g_vRimColor).xyz;

	vector vMask = tex2D(MaskSampler, In.vTexUV);

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2차원 텍스처로부터 uv좌표에 해당하는 색을 추출하여 반환하는 함수(반환 타입이 vector타입)
	Out.vColor *= float4(0.3f, 0.6f, 0.7f, 0.02f);
	Out.vColor += vector(rimLight.xyz, 0.02f);

	Out.vColor = Out.vColor * vMask;
	return Out;
}

PS_OUT			PS_MAIN_GRASS(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vMask = tex2D(BaseSampler, In.vTexUV);
	vector vColor = tex2D(MaskSampler, In.vTexUV);

	Out.vColor = vColor * vMask;

	return Out;
}


technique Default_Device
{

	pass //0
	{
	fillmode = solid;
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass //1
	{
		fillmode = solid;
	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN();
	}

		pass //2
	{
		fillmode = solid;

	zwriteenable = true;
	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN();
	}

		pass //3
	{
		//Alphatestenable = true;
		Alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

		fillmode = solid;
	cullmode = None;

	zwriteenable = false;
	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_CONE();
	}

	
		
		pass  //4
	{
		//Alphatestenable = true;
		Alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

	cullmode = None;

	zwriteenable = false;
	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_GRASS();
	}



};
