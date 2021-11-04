// int


// float
float		g_AddUV;


// vector, float4
vector		g_vLightDir;

vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlDiffuse;
vector		g_vMtrlAmbient;
vector		g_vMtrlSpecular;

vector		g_vCamPos;


// matrix
matrix		g_matWorld;	 // 상수 테이블
matrix		g_matView;
matrix		g_matProj;


// Texture
texture		g_BaseTexture;
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
};


// Functor

// Emmisive
float4		gaussFilterX[7] =
{
	0.0, -9.0, 0.0, 1.0 / 64.0,
	0.0, -6.0, 0.0, 6.0 / 64.0,
	0.0, -3.0, 0.0, 15.0 / 64.0,
	0.0, 0.0, 0.0, 20.0 / 64.0,
	0.0, 3.0, 0.0, 15.0 / 64.0,
	0.0, 6.0, 0.0, 6.0 / 64.0,
	0.0, 9.0, 0.0, 1.0 / 64.0,
};

float4		gaussFilterY[7] =
{
	-9.0, 0.0, 0.0, 1.0 / 64.0,
	-6.0, 0.0, 0.0, 6.0 / 64.0,
	-3.0, 0.0, 0.0, 15.0 / 64.0,
	0.0, 0.0,0.0, 20.0 / 64.0,
	3.0, 0.0,0.0, 15.0 / 64.0,
	6.0, 0.0,0.0, 6.0 / 64.0,
	9.0, 0.0,0.0, 1.0 / 64.0,
};

float		texScaler = 1.0 / 128.0;
float		texOffset = 0.0;

// RimLight
float		g_fRimDist;
int			g_iRimPower;
vector		g_vRimColor;


// function
vector Emissive(vector vTexUV)
{
	vector ColorX = 0;
	vector ColorY = 0;
	for (int i = 0; i < 7; ++i)
	{
		ColorX += tex2D(BaseSampler, float2(vTexUV.x + gaussFilterX[i].x * texScaler + texOffset,
			vTexUV.y + gaussFilterX[i].y * texScaler + texOffset)) * gaussFilterX[i].w;

		ColorY += tex2D(BaseSampler, float2(vTexUV.x + gaussFilterY[i].x * texScaler + texOffset,
			vTexUV.y + gaussFilterY[i].y * texScaler + texOffset)) * gaussFilterY[i].w;
	}

	return (ColorX + ColorY) / 2;
};


vector RimLight(float4 vPos, vector vNormal)
{
	float3 viewDir = normalize(g_vCamPos.xyz - vPos);
	float rim = smoothstep(1.f - g_fRimDist, 1.f, 1.f - dot(vNormal.xyz, viewDir));

	return vector((pow(rim, g_iRimPower) + g_vRimColor).xyz, 1.f);
};


struct VS_IN
{
	float4	vPosition	: POSITION;
	float4	vNormal		: NORMAL;
	float2	vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition	: POSITION;
	float4	vNormal		: NORMAL;
	float2	vTexUV		: TEXCOORD0;
	float4	vProjPos	: TEXCOORD1;
};

// 정점 쉐이더

VS_OUT			VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV	= mul(g_matWorld, g_matView);
	matWVP	= mul(matWV, g_matProj);

	Out.vPosition	= mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vNormal		= normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));
	Out.vTexUV		= In.vTexUV;
	Out.vProjPos	= Out.vPosition;

	return Out;
}

struct PS_IN
{
	vector		vNormal	: NORMAL;
	float2		vTexUV	: TEXCOORD0;
	float4		vProjPos: TEXCOORD1;
};

struct PS_OUT
{
	vector		vColor	: COLOR0;
	vector		vNormal : COLOR1;
	vector		vDepth	: COLOR2;
};


PS_OUT			PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w,	// R의 위치에 z나누기가 끝난 투영의 z를 보관
						In.vProjPos.w * 0.001f,			// G의 위치에 뷰스페이스 영역의 z를 보관(1000(FAR)으로 나누고 있는 중 : 텍스쳐UV는 0~1이니깐)
						0.f,
						0.f);
	
	return Out;
}


technique Default_Device
{
	pass // 0
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};
