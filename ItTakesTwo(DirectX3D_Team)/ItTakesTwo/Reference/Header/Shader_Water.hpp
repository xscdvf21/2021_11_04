matrix			g_matWorld;	 // 상수 테이블
matrix			g_matView;
matrix			g_matProj;
float			g_AddUV;

texture			g_BaseTexture;

sampler		BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

texture			g_WaterColor;
sampler		ColorSampler = sampler_state
{
	texture = g_WaterColor;

minfilter = linear;
magfilter = linear;
};

texture			g_WaterMask;
sampler		MaskSampler = sampler_state
{
	texture = g_WaterMask;

minfilter = linear;
magfilter = linear;
};

texture			g_Ice;
sampler		IceSampler = sampler_state
{
	texture = g_Ice;

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

	return Out;
}

struct PS_IN
{
	vector			vNormal : NORMAL;
	float2			vTexUV : TEXCOORD0;
	float4			vProjPos : TEXCOORD1;
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

	In.vTexUV += g_AddUV;
	vector vMask  = tex2D(BaseSampler, In.vTexUV);	
	vector vWaterColor = tex2D(ColorSampler, In.vTexUV);

	Out.vColor = vWaterColor + 1 * vMask;
	Out.vColor *= vector(0.8f, 0.8f, 0.8f, 0.7f);

	//Out.vColor.a = 1.f;

	// -1~1인 월드 상태의 좌표를 0~1인 텍스쳐 uv좌표로 변환


	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	
	

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w,	// R의 위치에 z나누기가 끝난 투영의 z를 보관
						In.vProjPos.w * 0.001f,			// G의 위치에 뷰스페이스 영역의 z를 보관(1000(FAR)으로 나누고 있는 중 : 텍스쳐UV는 0~1이니깐)
						0.f,
						0.f);

	
	return Out;
}

PS_OUT			PS_MAIN_2(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(IceSampler, In.vTexUV);
	Out.vColor.a = 1.f;
	// -1~1인 월드 상태의 좌표를 0~1인 텍스쳐 uv좌표로 변환


	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);



	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w,	// R의 위치에 z나누기가 끝난 투영의 z를 보관
		In.vProjPos.w * 0.001f,			// G의 위치에 뷰스페이스 영역의 z를 보관(1000(FAR)으로 나누고 있는 중 : 텍스쳐UV는 0~1이니깐)
		0.f,
		0.f);

	return Out;
}

PS_OUT			PS_MAIN_CURTAIN(PS_IN In)
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

PS_OUT			PS_MAIN_SHOWER(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	In.vTexUV.y -= g_AddUV * 20.f;

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

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	//Out.vColor *= 0.5f;


	Out.vColor = Blur + Out.vColor;
	// -1~1인 월드 상태의 좌표를 0~1인 텍스쳐 uv좌표로 변환



	return Out;
}

technique Default_Device
{

	pass // 0
	{
	alphablendenable = true;
	fillmode = solid;
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass // 1
	{
		alphablendenable = true;
	fillmode = solid;
	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_2();
	}

		pass // 2
	{
		fillmode = solid;
	cullmode = None;
	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_CURTAIN();
	}

		pass // 3
	{
		fillmode = solid;
	alphablendenable = true;
	cullmode = None;
	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_SHOWER();
	}

	//pass	
	//{
	//	alphatestenable = true;
	//	alpharef = 0xc0;
	//	alphafunc = greater;
	//	cullmode = none;

	//	//alphablendenable = true;
	//	//fillmode = solid;

	//	VertexShader = compile vs_3_0 VS_MAIN();
	//	PixelShader = compile ps_3_0 PS_MAIN_ALPHATEST();
	//}
};
