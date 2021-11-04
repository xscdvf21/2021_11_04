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


struct VS_IN
{
	float4			vPosition : POSITION;
	float2			vTexUV : TEXCOORD0;
	float			size : PSIZE;
	float4			vColor : COLOR;
};

struct VS_OUT
{
	float4			vPosition : POSITION;
	float2			vTexUV : TEXCOORD0;
	float			size : PSIZE;
	float4			vColor : COLOR;

};

// 정점 쉐이더

VS_OUT			VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	Matrix matViewWorld, matViewWorldProj;

	matViewWorldProj = mul(g_matView, g_matProj);
	Out.vPosition = mul(In.vPosition, matViewWorldProj);
	Out.size = In.size*50;
	Out.vTexUV = In.vTexUV;
	Out.vColor = In.vColor;
	return Out;
}

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;
	float4			vColor : COLOR;
	//float2			size : TEXCOORD1;

};

struct PS_OUT
{
	vector		vColor : COLOR0;
};


PS_OUT			PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor *=In.vColor;
	Out.vColor.a = 1;

	return Out;
}

technique Default_Device
{

	pass // 0
	{
	ZwriteEnable = false;
	AlphaBlendEnable = true;
AlphaTestEnable = false;
SrcBlend = ONE;
DestBlend = ONE;
POINTSPRITEENABLE = true;
//POINTSIZE = 50.f;
//POINTSIZE_MIN = 20.f;
//POINTSIZE_MAX = 1000.0f;
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

};
