#ifndef __FXAA_FILTER_FX__  
#define __FXAA_FILTER_FX__  

#define FXAA_SPAN_MAX 8.0
#define FXAA_REDUCE_MUL   (1.0/FXAA_SPAN_MAX)
#define FXAA_REDUCE_MIN   (1.0/128.0)
#define FXAA_SUBPIX_SHIFT (1.0/4.0)
#define FXAA_PC 1  
#define FXAA_HLSL_3 1  
#endif

#include "Shader_fxaa.hpp"
texture				g_BaseSampler;

sampler		BaseSampler = sampler_state
{
	texture = g_BaseSampler;

minfilter = linear;
magfilter = linear;

};
float4 PS_FxAAFilter(in float2 UV : TEXCOORD0) : COLOR
{
	float4 disableParm = float4(0.0f, 0.0f, 0.0f, 0.0f);

return FxaaPixelShader(UV,              //�ȼ��� UV��.  
	disableParm,     //������� �ʽ��ϴ�. �ƹ� ���̳� ��������.  
	BaseSampler,       //FXAA�� ������ Texture  
	g_texelSize,     //�ؼ��� ũ��.xy, x���� 1.0f/pixelWidth, y���� 1.0/pixelHeight  
	disableParm);    //������� �ʽ��ϴ�.  
}

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};
PS_OUT		PS_MAIN(PS_IN In)

{
	PS_OUT			Out = (PS_OUT)0;

	vector vColor =PS_FxAAFilter(In.vTexUV);

	vColor.a = dot(vColor.rgb, float3(0.299, 0.587, 0.114));

	Out.vColor = vColor;

	return Out;
}