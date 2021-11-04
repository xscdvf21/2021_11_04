#ifndef __FXAA_FILTER_FX__  
#define __FXAA_FILTER_FX__  

#define FXAA_QUALITY__EDGE_THRESHOLD_MIN (1.0/12)
#define FXAA_QUALITY__SUBPIX (4.0/4.0)
#define FXAA_EARLY_EXIT 1
#define FXAA_PC 1  
#define FXAA_HLSL_3 1  

#define FXAA_SPAN_MAX 8.0
#define FXAA_REDUCE_MUL   (1.0/FXAA_SPAN_MAX)
#define FXAA_REDUCE_MIN   (1.0/128.0)
#define FXAA_SUBPIX_SHIFT (1.0/4.0)
#endif
#include "Shader_fxaa.hpp"

matrix			g_matWorld;	 // 상수 테이블
matrix			g_matView;
matrix			g_matProj;
vector			g_vSize;
bool			g_vGreyScale;

texture				g_AlbedoTexture;
float2			g_texelSize = { 1.0 / 1920 ,1.0 / 1080 };
// Shadow
matrix		g_matViewInv;
matrix		g_matProjInv;
matrix		g_matLightView;
matrix		g_matLightProj;


sampler		AlbedoSampler = sampler_state
{
	texture = g_AlbedoTexture;

	minfilter = linear;
	magfilter = linear;

};

texture				g_ShadeTexture;

sampler		ShadeSampler = sampler_state
{
	texture = g_ShadeTexture;

	minfilter = linear;
	magfilter = linear;

};

texture				g_SpecularTexture;
sampler		SpecularSampler = sampler_state
{
	texture = g_SpecularTexture;

minfilter = linear;
magfilter = linear;

};


texture				g_EmissiveTexture;
sampler		EmissiveSampler = sampler_state
{
	texture = g_EmissiveTexture;

minfilter = linear;
magfilter = linear;

};


texture			g_CustomSpecularTexture;
sampler		CustomSpecularSampler = sampler_state
{
	texture = g_CustomSpecularTexture;

minfilter = linear;
magfilter = linear;
};


texture				g_DepthTexture;
sampler		DepthSampler = sampler_state
{
	texture = g_DepthTexture;

minfilter = linear;
magfilter = linear;

};



texture		g_ShadowTexture;
sampler		ShadowSampler = sampler_state
{
	texture = g_ShadowTexture;

minfilter = linear;
magfilter = linear;
mipfilter = none;
AddressU = clamp;
AddressV = clamp;
};

texture		g_StencilTexture;
sampler		StencilSampler = sampler_state
{
	texture = g_StencilTexture;

minfilter = linear;
magfilter = linear;
mipfilter = none;
AddressU = clamp;
AddressV = clamp;
};


// Emmisive
static float4		gaussFilterX[15] =
{
	0.0f, -2.8f, 0.0f, 1.f / 64.0f,
	0.0f, -2.1f, 0.0f, 2.0f / 64.0f,
	0.0f, -1.5f, 0.0f, 4.0f / 64.0f,
	0.0f, -1.f, 0.0f, 7.0f / 64.0f,
	0.0f, -0.6f, 0.0f, 11.0f / 64.0f,
	0.0f, -0.3f, 0.0f, 16.0f / 64.0f,
	0.0f, -0.1f, 0.0f, 24.0f / 64.0f,
	0.0f, 0.0, 0.0f, 31.0f / 64.0f,
	0.0f, 0.1f, 0.0f, 24.0f / 64.0f,
	0.0f, 0.3f, 0.0f, 16.0f / 64.0f,
	0.0f, 0.6f, 0.0f, 11.0f / 64.0f,
	0.0f, 1.f, 0.0f, 7.0f / 64.0f,
	0.0f, 1.5f, 0.0f, 4.0f / 64.0f,
	0.0f, 2.1f, 0.0f, 2.0f / 64.0f,
	0.0f, 2.8f, 0.0f, 1.f / 64.0f,
};
static float4		gaussFilterY[15] =
{
	-2.8f, 0.0f, 0.0f, 1.f / 64.0f,
	-2.1f, 0.0f, 0.0f, 2.0f / 64.0f,
	-1.5f, 0.0f, 0.0f, 4.0f / 64.0f,
	-1.f, 0.0f, 0.0f, 7.0f / 64.0f,
	-0.6f, 0.0f, 0.0f, 11.0f / 64.0f,
	-0.3f, 0.0f, 0.0f, 16.0f / 64.0f,
	-0.1f, 0.0f, 0.0f, 24.0f / 64.0f,
	0.0f, 0.0f,0.0f, 31.0f / 64.0f,
	0.1f, 0.0f,0.0f, 24.0f / 64.0f,
	0.3f, 0.0f,0.0f, 16.0f / 64.0f,
	0.6f, 0.0f,0.0f, 11.0f / 64.0f,
	1.f, 0.0f,0.0f, 7.0f / 64.0f,
	1.5f, 0.0f,0.0f, 4.0f / 64.0f,
	2.1f, 0.0f,0.0f, 2.0f / 64.0f,
	2.8f, 0.0f,0.0f, 1.f / 64.0f,
};

static float4		gaussFilterZ[15] =
{
	2.8f, -2.8f, 0.0f, 1.f / 64.0f,
	2.1f, -2.1f, 0.0f, 2.0f / 64.0f,
	1.5f, -1.5f, 0.0f, 4.0f / 64.0f,
	1.f, -1.f, 0.0f, 7.0f / 64.0f,
	0.6f, -0.6f, 0.0f, 11.0f / 64.0f,
	0.3f, -0.3f, 0.0f, 16.0f / 64.0f,
	0.1f, -0.1f, 0.0f, 24.0f / 64.0f,
	0.0f, 0.0, 0.0f, 31.0f / 64.0f,
	-0.1f, 0.1f, 0.0f, 24.0f / 64.0f,
	-0.3f, 0.3f, 0.0f, 16.0f / 64.0f,
	-0.6f, 0.6f, 0.0f, 11.0f / 64.0f,
	-1.f, 1.f, 0.0f, 7.0f / 64.0f,
	-1.5f, 1.5f, 0.0f, 4.0f / 64.0f,
	-2.1f, 2.1f, 0.0f, 2.0f / 64.0f,
	-2.8f, 2.8f, 0.0f, 1.f / 64.0f,
};
static float4		gaussFilterW[15] =
{
	-2.8f,	-2.8f,	0.0f, 1.f / 64.0f,
	-2.1f,	-2.1f,	0.0f, 2.0f / 64.0f,
	-1.5f,	-1.5f,	0.0f, 4.0f / 64.0f,
	-1.f,	-1.f,	0.0f, 7.0f / 64.0f,
	-0.6f,	-0.6f,	0.0f, 11.0f / 64.0f,
	-0.3f,	-0.3f,	0.0f, 16.0f / 64.0f,
	-0.1f,	-0.1f,	0.0f, 24.0f / 64.0f,
	0.0f,	0.0f,	0.0f, 31.0f / 64.0f,
	0.1f,	0.1f,	0.0f, 24.0f / 64.0f,
	0.3f,	0.3f,	0.0f, 16.0f / 64.0f,
	0.6f,	0.6f,	0.0f, 11.0f / 64.0f,
	1.f,	1.f,	0.0f, 7.0f / 64.0f,
	1.5f,	1.5f,	0.0f, 4.0f / 64.0f,
	2.1f,	2.1f,	0.0f, 2.0f / 64.0f,
	2.8f,	2.8f,	0.0f, 1.f / 64.0f,
};
float		texScaler = 1.0f / 128.0f;
float		texOffset = 0.0f;

float4 PS_FxAAFilter(sampler SamplerType, in float2 UV : TEXCOORD0) : COLOR
{
	float4 disableParm = float4(0.0f, 0.0f, 0.0f, 0.0f);
//return disableParm;
return FxaaPixelShader(UV,              //픽셀의 UV값.  
	disableParm,     //사용하지 않습니다. 아무 값이나 넣으세요.  
	SamplerType,       //FXAA를 적용할 Texture  
	g_texelSize,     //텍셀의 크기.xy, x에는 1.0f/pixelWidth, y에는 1.0/pixelHeight  
	disableParm);    //사용하지 않습니다.  
}

float4 Emissive(sampler Samplertype, float2 vTexUV : TEXCOORD0) : COLOR0
{
	vector ColorX = 0;
vector ColorY = 0;
vector ColorZ = 0;
vector ColorW = 0;
for (int i = 0; i < 15; ++i)
{
	ColorX += tex2D(Samplertype, float2(vTexUV.x + gaussFilterX[i].x * texScaler + texOffset,
		vTexUV.y + gaussFilterX[i].y * texScaler + texOffset)) * gaussFilterX[i].w;

	ColorY += tex2D(Samplertype, float2(vTexUV.x + gaussFilterY[i].x * texScaler + texOffset,
		vTexUV.y + gaussFilterY[i].y * texScaler + texOffset)) * gaussFilterY[i].w;

	ColorZ += tex2D(Samplertype, float2(vTexUV.x + gaussFilterZ[i].x * texScaler + texOffset,
		vTexUV.y + gaussFilterZ[i].y * texScaler + texOffset)) * gaussFilterZ[i].w;

	ColorW += tex2D(Samplertype, float2(vTexUV.x + gaussFilterW[i].x * texScaler + texOffset,
		vTexUV.y + gaussFilterW[i].y * texScaler + texOffset)) * gaussFilterW[i].w;
}

return (ColorX + ColorY + ColorZ + ColorW) / 4;
};


//////////////////////////////////////////////////////////////////////////
//// FUNCTION START //////////////////////////////////////////////////////
float4 ShadowMapping(sampler Samplertype, float2 vTexUV : TEXCOORD0, vector vColor : COLOR0, vector vDepth : TEXCOORD1) : COLOR0
{
	// 그림자
	float		fViewZ = vDepth.y * 1000.0f;

vector		vPosition;
////투영상태
vPosition.x = (vTexUV.x * 2.f - 1.f) * fViewZ;
vPosition.y = (vTexUV.y * -2.f + 1.f) * fViewZ;
vPosition.z = vDepth.x * fViewZ;
vPosition.w = fViewZ;

// 뷰 상
vPosition = mul(vPosition, g_matProjInv);

// 월드 상
vPosition = mul(vPosition, g_matViewInv);

vPosition = mul(vPosition, g_matLightView);
vPosition = mul(vPosition, g_matLightProj);

vector		vUVPos = vPosition;
float2		vNewUV;

vNewUV.x = (vUVPos.x / vUVPos.w) * 0.5f + 0.5f;
vNewUV.y = (vUVPos.y / vUVPos.w) * -0.5f + 0.5f;


vector		vShadowDepthInfo = tex2D(Samplertype, vNewUV);
vector		vShadow;

if (vPosition.w - 0.1f > vShadowDepthInfo.r * 1000.0f)
{
	vShadow = vector(0.6f, 0.6f, 0.6f, 0.7f);
	vColor *= vShadow;
}


return vColor;

};




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
};

VS_OUT			VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));
	Out.vTexUV = In.vTexUV;

	return Out;
}


struct PS_IN
{
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};





PS_OUT		PS_MAIN(PS_IN In, uniform bool ZFog, uniform bool YFog)

{
	PS_OUT			Out = (PS_OUT)0;

	float2 UV = In.vTexUV;
	UV.x = UV.x * ((g_vSize.z - g_vSize.x) / 1920) - 0.5f * ((g_vSize.z - g_vSize.x) / 1920) + 0.5f;
	UV.y = UV.y * ((g_vSize.w - g_vSize.y) / 1080) - 0.5f * ((g_vSize.w - g_vSize.y) / 1080) + 0.5f;


	vector	vStencilDepthInfo = PS_FxAAFilter(StencilSampler, UV);
	vector	vAlbedo = PS_FxAAFilter(AlbedoSampler,UV) * vStencilDepthInfo;
	vector	vShade = PS_FxAAFilter(ShadeSampler, UV);
	vector	vSpecular = PS_FxAAFilter(SpecularSampler, UV);
	vector	vDepth = PS_FxAAFilter(DepthSampler, UV);
	vector	vEmissive = Emissive(EmissiveSampler, UV);
	vector	vSpecularTexture = PS_FxAAFilter(CustomSpecularSampler, UV);
	vSpecularTexture.a = 1.f;

	float Zfar = 150;
	float Znear = 0;

	// HDR
	//float3 Color = { 1.f,1.f,1.f };
	float3 Color = pow(abs(PS_FxAAFilter(AlbedoSampler,UV) * vStencilDepthInfo), 2.2f);
	float3 x = max(0.f, Color - 0.004);
	Color = (x * (6.2f * x + 0.5f)) / (x * (6.2f * x + 1.7f) + 0.06f);
	vector vHDR = vector(Color * 0.4f, 1.f);

	Out.vColor = (vHDR + vAlbedo) * vShade + (vSpecularTexture * 0.3f) + vEmissive + vSpecular;
	if (g_vGreyScale)
	{
		float BnW = (Out.vColor.r + Out.vColor.g + Out.vColor.b) / 3.f;
		Out.vColor = vector(BnW, BnW, BnW, Out.vColor.a);
	}

	if (ZFog)
	{

		// FOG
		float fFog = (Zfar - vDepth.y * 1000 * vDepth.x) / (Zfar - Znear);
		//vector FogColor = vector(0.2f, 0.6f, 0.8f, 1.f);
		vector FogColor = vector(0.6f, 0.6f, 0.65f, 1.f);
		Out.vColor = fFog * Out.vColor + (1 - fFog) * FogColor;
	}

	//if (YFog)
	//{

	//}


	// 그림자
	//Out.vColor = ShadowMapping(ShadowSampler, UV, Out.vColor, vDepth);

		return Out;
}

technique Default_Device
{
	pass
	{
		zenable = false;
		zwriteenable = false;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN(false,false);
	}

	pass
	{
		zenable = false;
	zwriteenable = false;
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	
	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN(true,false);
	}

		pass
	{
		zenable = false;
	zwriteenable = false;

	alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN(false,true);
	}

	pass
	{
		zenable = false;
	zwriteenable = false;

	alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN(true,true);
	}
};