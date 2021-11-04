matrix			g_matWorld;	 // 상수 테이블
matrix			g_matView;
matrix			g_matProj;

texture			g_BaseTexture;
texture		g_NormalTexture;
texture		g_MaterialTexture;
texture		g_EmissiveTexture;
texture		g_SpecularTexture;
vector		g_vColor;

bool		g_bRim = false;

float		g_fMaterialIndex = 0;

float VTTextureWidth = 256.f;
float VTTextureHeight = 256.f;
vector		g_vCamPos;
sampler  sampSkin1 : register(s0);     // Refraction map
sampler  sampSkin2 : register(s1);     // Refraction map
sampler  sampSkin3 : register(s2);     // Refraction map
sampler  sampSkin4 : register(s3);     // Refraction map
float g_fAmount;
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
texture		g_DissolveTexture;
sampler		DissolveSampler = sampler_state
{
	texture = g_DissolveTexture;

minfilter = linear;
magfilter = linear;
};

texture		g_BurnTexture;
sampler		BurnSampler = sampler_state
{

	texture = g_BurnTexture;

minfilter = linear;
magfilter = linear;
};


sampler		NormalSampler = sampler_state
{
	texture = g_NormalTexture;

minfilter = linear;
magfilter = linear;
};

sampler		MaterialSampler = sampler_state
{
	texture = g_MaterialTexture;

minfilter = linear;
magfilter = linear;
};

sampler		EmissiveSampler = sampler_state
{
	texture = g_EmissiveTexture;

minfilter = linear;
magfilter = linear;
};

sampler		SpecularSampler = sampler_state
{
	texture = g_SpecularTexture;

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
sampler		BaseSampler = sampler_state
{
	texture = g_BaseTexture;

minfilter = linear;
magfilter = linear;
};
float4 Emissive(sampler Samplertype, float2 vTexUV : TEXCOORD0) : COLOR0
{
	vector ColorX = 0;
vector ColorY = 0;
for (int i = 0; i < 7; ++i)
{
	ColorX += tex2D(Samplertype, float2(vTexUV.x + gaussFilterX[i].x * texScaler + texOffset,
		vTexUV.y + gaussFilterX[i].y * texScaler + texOffset)) * gaussFilterX[i].w;

	ColorY += tex2D(Samplertype, float2(vTexUV.x + gaussFilterY[i].x * texScaler + texOffset,
		vTexUV.y + gaussFilterY[i].y * texScaler + texOffset)) * gaussFilterY[i].w;
}

return (ColorX + ColorY) / 2;
};

float4 RimLight(float4 vPos : TEXCOORD0, vector vNormal : NORMAL) : COLOR1
{
	float3 viewDir = normalize(g_vCamPos.xyz - vPos.xyz);
float rim = smoothstep(1.f - g_fRimDist, 1.f, 1.f - dot(vNormal.xyz, viewDir));

return vector((pow(rim, g_iRimPower) + g_vRimColor).xyz, 0.01f);
};

float4 HDR(float2 vTexUV : TEXCOORD0, sampler sample) : COLOR2
{
	vector Color = pow(tex2D(sample, vTexUV), 2.2f);
vector x = max(0.f, Color - 0.004);
Color = (x * (6.2f * x + 0.5f)) / (x * (6.2f * x + 1.7f) + 0.06f);

return Color;
};

float3 vsNormalMapping(float3 vNorTanBin : TEXCOORD0) : COLOR3
{
	float3 world = mul(vNorTanBin, (float3x3)g_matWorld);

return normalize(world);

/*
Out.N = vsNormalMapping(In.vNormal);
Out.T = vsNormalMapping(In.vTangent);
Out.B = vsNormalMapping(In.vBinormal);
*/
};

float4 psNormalMapping(float2 vTexUV : TEXCOORD0, float3 T : TEXCOORD1, float3 B : TEXCOORD2, float3 N : TEXCOORD3) : COLOR4
{
	float3 tangentNormal = tex2D(NormalSampler, vTexUV).xyz;
tangentNormal = normalize(tangentNormal * 2 - 1);

float3x3 TBN = float3x3(normalize(T), normalize(B), normalize(N));
TBN = transpose(TBN);

float3 worldNormal = mul(TBN, tangentNormal);

return float4(worldNormal.xyz * 0.5f + 0.5f, 1.f);
};

float4 Dissolve(float2 vTexUV : TEXCOORD0, vector vColor : COLOR0) : COLOR5
{
	clip(tex2D(DissolveSampler, vTexUV).rgb - g_fAmount);
vector vBurn;
float fDissolve = tex2D(DissolveSampler, vTexUV).rgb - g_fAmount;
float fBurnSize = 0.3f;

if (fDissolve < fBurnSize && g_fAmount > 0.0f && g_fAmount < 1.0f)
{
	vBurn = tex2D(BurnSampler, float2(1.0f - (fDissolve * (1 / fBurnSize)), 0));
	return vBurn;
}

return vColor;
};
float4x4 GetMatrix(int idx)
{

	float4 uvCol =
		float4
		(
			((float)((idx % 64) * 4) + 0.5f) / VTTextureWidth, ((float)((idx / 64)) + 0.5f) / VTTextureHeight
			, 0.0f, 0.0f
			);

	float4x4 mat =
	{
		tex2Dlod(sampSkin1, uvCol),
		tex2Dlod(sampSkin1, uvCol + float4(1.0f / VTTextureWidth, 0, 0, 0)),
		tex2Dlod(sampSkin1, uvCol + float4(2.0f / VTTextureWidth, 0, 0, 0)),
		tex2Dlod(sampSkin1, uvCol + float4(3.0f / VTTextureWidth, 0, 0, 0))
	};

	return mat;
}
float4 SampleVTPOSTexture(float2 instanceIndex)
{
	float instanceOffset = dot(instanceIndex, float2(1.0f, 256.0f));

	float2 texcoord;
	texcoord.x = instanceOffset / VTTextureWidth;
	texcoord.y = floor(texcoord.x) / VTTextureHeight;
	return tex2Dlod(sampSkin1, float4(texcoord, 0.0f, 0.0f));
}
float4 SampleVTNormalTexture(float2 instanceIndex)
{
	float instanceOffset = dot(instanceIndex, float2(1.0f, 256.0f));

	float2 texcoord;
	texcoord.x = instanceOffset / VTTextureWidth;
	texcoord.y = floor(texcoord.x) / VTTextureHeight;
	return tex2Dlod(sampSkin2, float4(texcoord, 0.0f, 0.0f));
}
float4 SampleVTTangentTexture(float2 instanceIndex)
{
	float instanceOffset = dot(instanceIndex, float2(1.0f, 256.0f));

	float2 texcoord;
	texcoord.x = instanceOffset / VTTextureWidth;
	texcoord.y = floor(texcoord.x) / VTTextureHeight;
	return tex2Dlod(sampSkin3, float4(texcoord, 0.0f, 0.0f));
}
float4 SampleVTBinormalTexture(float2 instanceIndex)
{
	float instanceOffset = dot(instanceIndex, float2(1.0f, 256.0f));

	float2 texcoord;
	texcoord.x = instanceOffset / VTTextureWidth;
	texcoord.y = floor(texcoord.x) / VTTextureHeight;
	return tex2Dlod(sampSkin4, float4(texcoord, 0.0f, 0.0f));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct VS_IN
{
	float4	vPosition : POSITION;
	float3	vNormal : NORMAL;
	float3	vTangent : TANGENT;
	float3	vBinormal : BINORMAL;

	float4	boneIndices : BLENDINDICES0;
	float4 blendWeights : BLENDWEIGHT0;

	float2	vTexUV : TEXCOORD0;
	float4 instanceIndex : TEXCOORD1;
};
struct VS_IN2
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float3	vTangent : TANGENT;
	float3	vBinormal : BINORMAL;
	float2	vTexUV : TEXCOORD0;
};
struct VS_OUT
{
	float4	vtPosition : POSITION;

	float4	position : TEXCOORD0;
	float3	normal : TEXCOORD1;
	float3	tangent : TEXCOORD2;
	float3	binormal : TEXCOORD3;
};

struct VS_OUT2
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vProjPos : TEXCOORD1;
	float4	vPos : TEXCOORD2;

	float3	T : TEXCOORD3;
	float3	B : TEXCOORD4;
	float3	N : TEXCOORD5;
};
struct VS_OUT_SHADOW
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vProjPos : TEXCOORD1;
	float4	vPos : TEXCOORD2;

	float3	T : TEXCOORD3;
	float3	B : TEXCOORD4;
	float3	N : TEXCOORD5;
	//float4			vPosition : POSITION;
	//float4			vProjPos : TEXCOORD0;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 정점 쉐이더

VS_OUT2			VS_MAIN(VS_IN input)
{
	VS_OUT2		Out = (VS_OUT2)0;

	int4 boneIndices = (int4)input.boneIndices;
	float4	srcPosition = input.vPosition;
	float3	srcNormal = input.vNormal;
	float3	srcTangent = input.vTangent;
	float3	srcbinormal = input.vBinormal;
	int n = 3;

	float	lastWeight = 0.0f;

	input.vPosition.xyz = 0.0f;
	input.vNormal = 0.0f;
	input.vTangent = 0.0f;

	for (int i = 0; i < n; ++i)
	{
		lastWeight += input.blendWeights[i];

		float4x4 boneMatrix = GetMatrix(boneIndices[i]);
		input.vPosition.xyz += mul(srcPosition, boneMatrix)* input.blendWeights[i];
		input.vNormal += mul(srcNormal, (float3x3)boneMatrix)*input.blendWeights[i];
		input.vTangent += mul(srcTangent, (float3x3)boneMatrix)*input.blendWeights[i];
		input.vBinormal += mul(srcbinormal, (float3x3)boneMatrix)*input.blendWeights[i];
	}

	lastWeight = 1.0f - lastWeight;

	float4x4 boneMatrix = GetMatrix(boneIndices[n]);
	input.vPosition.xyz += mul(srcPosition, boneMatrix)* lastWeight;
	input.vNormal += mul(srcNormal, (float3x3)boneMatrix)*lastWeight;
	input.vTangent += mul(srcTangent, (float3x3)boneMatrix)*lastWeight;
	input.vBinormal += mul(srcbinormal, (float3x3)boneMatrix)*lastWeight;

	input.vPosition.w = 1.f;


	//float instanceOffset = dot(float2(input.instanceIndex.xy), float2(1.f, VTTextureWidth));

	//float2 texcoord;
	//texcoord.x = instanceOffset / VTTextureWidth;
	//texcoord.y = floor(texcoord.x) / VTTextureHeight;

	//float2 outputPos = frac(texcoord);
	//outputPos.x = outputPos.x * 2.0f - 1.0f;
	//outputPos.y = 1.0f - outputPos.y * 2.0f;

	//Out.vtPosition = float4(outputPos, 0.0f, 1.0f);
	//Out.position = input.vPosition;
	//Out.normal = input.vNormal;
	//Out.tangent = input.vTangent;
	//Out.binormal = input.vBinormal;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(input.vPosition, matWVP);
	Out.vNormal = normalize(mul(vector(input.vNormal.xyz, 0.f), g_matWorld));
	Out.vTexUV = input.vTexUV;
	Out.vProjPos = Out.vPosition;
	Out.vPos = mul(vector(input.vPosition.xyz, 1.f), g_matWorld);
	Out.T = input.vTangent;
	Out.B = input.vBinormal;
	Out.N = input.vNormal;

	return Out;
}
VS_OUT2			VS_MAIN_NORMALMAPPING(VS_IN2 In)
{
	VS_OUT2		Out = (VS_OUT2)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	Out.vPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);

	Out.T = vsNormalMapping(In.vTangent);
	Out.B = vsNormalMapping(In.vBinormal);
	Out.N = vsNormalMapping(In.vNormal.xyz);


	return Out;
};

VS_OUT_SHADOW			VS_SHADOW(VS_IN input)
{
	VS_OUT_SHADOW		Out = (VS_OUT_SHADOW)0;

	int4 boneIndices = (int4)input.boneIndices;
	float4	srcPosition = input.vPosition;
	int n = 3;

	float	lastWeight = 0.0f;

	input.vPosition.xyz = 0.0f;

	for (int i = 0; i < n; ++i)
	{
		lastWeight += input.blendWeights[i];

		float4x4 boneMatrix = GetMatrix(boneIndices[i]);
		input.vPosition.xyz += mul(srcPosition, boneMatrix)* input.blendWeights[i];
	}

	lastWeight = 1.0f - lastWeight;

	float4x4 boneMatrix = GetMatrix(boneIndices[n]);
	input.vPosition.xyz += mul(srcPosition, boneMatrix)* lastWeight;
	input.vPosition.w = 1.f;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(input.vPosition, matWVP);
	Out.vProjPos = Out.vPosition;
	Out.vPos = mul(vector(input.vPosition.xyz, 1.f), g_matWorld);

	return Out;
}
VS_OUT2			VS_USING_VTF(VS_IN input)
{
	VS_OUT2		Out = (VS_OUT2)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	float4 vPos = SampleVTPOSTexture(input.instanceIndex.xy*256.f+0.5f);
	float3 vNormal = SampleVTNormalTexture(input.instanceIndex.xy*256.f + 0.5f).xyz;
	float3 vTangent = SampleVTTangentTexture(input.instanceIndex.xy*256.f + 0.5f).xyz;
	float3 vBinormal = SampleVTBinormalTexture(input.instanceIndex.xy*256.f + 0.5f).xyz;


	Out.vPosition = mul(vPos,matWVP);
	Out.vProjPos = Out.vPosition;
	Out.vPos = mul(vPos,g_matWorld);
	Out.vTexUV = input.vTexUV;
	Out.vNormal = normalize(mul(vector(vNormal, 0.f), g_matWorld));
	Out.N = vNormal;
	Out.T = vTangent;
	Out.B = vBinormal;

	return Out;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PS_IN
{
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
	float4		vPos : TEXCOORD2;
	float3		T : TEXCOORD3;
	float3		B : TEXCOORD4;
	float3		N : TEXCOORD5;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
	vector		vNormal : COLOR1;
	vector		vDepth : COLOR2;
	vector		vEmissive : COLOR3;
};

struct PS_IN_VTF
{
	float3	position : TEXCOORD0;
	float3	normal : TEXCOORD1;
	float3	tangent : TEXCOORD2;
	float3	binormal : TEXCOORD3;

};
struct PS_IN_SHADOW
{
	float4			vPosition : POSITION;
	float4			vProjPos : TEXCOORD0;
	float2			vTexUV : TEXCOORD1;
};

struct PS_OUT_SHADOW
{
	vector			vLightDepth : COLOR0;
	vector				vDepth : COLOR1;
};
struct PS_OUT_VTF
{
	vector		position : COLOR0;
	vector		normal : COLOR1;
	vector		tangent : COLOR2;
	vector		binormal : COLOR3;
};

PS_OUT			PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vSpecular = tex2D(SpecularSampler, In.vTexUV);
	vector vDissolve = Dissolve(In.vTexUV, vColor);
	vector vMaterial = tex2D(MaterialSampler, In.vTexUV);
	vector vEmissive = tex2D(EmissiveSampler, In.vTexUV);
	if (vEmissive.x == 0)
		vEmissive.a = 0.f;


	Out.vNormal = psNormalMapping(In.vTexUV, In.T, In.B, In.N);

	vector vConstant = g_vColor / 255;
	Out.vColor = vDissolve / vConstant;
	//Out.vColor.a = 1.f;
	// Out.vColor *= vDissolve;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, vSpecular.r, g_fMaterialIndex);

	Out.vEmissive = vEmissive;

	return Out;
}

PS_OUT			PS_MAIN_RIM(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vSpecular = tex2D(SpecularSampler, In.vTexUV);
	vector vDissolve = Dissolve(In.vTexUV, vColor);
	vector vMaterial = tex2D(MaterialSampler, In.vTexUV);
	vector vEmissive = tex2D(EmissiveSampler, In.vTexUV);
	if (vEmissive.x == 0)
		vEmissive.a = 0.f;


	Out.vNormal = psNormalMapping(In.vTexUV, In.T, In.B, In.N);
	vector vRim;
	if (g_bRim)
		vRim = RimLight(In.vPos, In.vNormal) * 0.5f;
	else
		vRim = vector(0.f, 0.f, 0.f, 0.f);

	vector vConstant = g_vColor / 255;

	Out.vColor = (vDissolve / vConstant) + vRim;
	//Out.vColor.a = 1.f;
	// Out.vColor *= vDissolve;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, vSpecular.r, g_fMaterialIndex);

	Out.vEmissive = vEmissive;

	return Out;
}

PS_OUT_SHADOW			PS_MAIN_SHADOW(PS_IN_SHADOW In)
{
	PS_OUT_SHADOW		Out = (PS_OUT_SHADOW)0;

	Out.vLightDepth.r = In.vProjPos.w / 1000.f;

	Out.vLightDepth.a = 1.f;

	return Out;
}
PS_OUT_VTF			PS_MAIN_VertexTransformationPSMain(PS_IN_VTF In)
{
	PS_OUT_VTF		Out = (PS_OUT_VTF)0;
	//
	//Out.position = vector(1.f,1.f,1.f, 1.0f);
	//Out.normal = vector(1.f, 1.f, 1.f, 0.0f);
	//Out.tangent = vector(1.f, 1.f, 1.f, 0.0f);
	//Out.binormal = vector(1.f, 1.f, 1.f, 0.0f);

	Out.position = vector(In.position, 0.0f);
	Out.normal = vector(In.normal, 0.0f);
	Out.tangent = vector(In.tangent, 0.0f);
	Out.binormal = vector(In.binormal, 0.0f);

	return Out;
}

PS_OUT			PS_MAIN_STENCIL(PS_IN_SHADOW In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = g_vColor / 255;

	Out.vColor = vector(1.f, 1.f, 1.f, 1.f) * vColor;

	return Out;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
technique Default_Device
{
	pass //0
{
	//zfunc = always;
//	stencilenable = true;
//stencilref = 1;
//stencilmask = 255;
//stencilwritemask = 255;
//stencilfunc = Always;
//stencilpass = keep;
//stencilfail = keep;
//stencilzfail = 4;

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN();
}
pass //1
{
VertexShader = compile vs_3_0 VS_USING_VTF();
PixelShader = compile ps_3_0 PS_MAIN();
}

pass //SHADOW
{
	zwriteenable = false;
	VertexShader = compile vs_3_0 VS_SHADOW();
PixelShader = compile ps_3_0 PS_MAIN_SHADOW();
}

pass //Stencil
{

	VertexShader = compile vs_3_0 VS_SHADOW();
	PixelShader = compile ps_3_0 PS_MAIN_STENCIL();
}
pass //4
{
	VertexShader = compile vs_3_0 VS_MAIN_NORMALMAPPING();
	PixelShader = compile ps_3_0 PS_MAIN();
}

pass //5
{
	VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN_RIM();
}

};
