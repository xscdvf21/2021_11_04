//////////////////////////////////////////////////////////////////////////
//// DEFAULT TEXTURE START ///////////////////////////////////////////////

texture		g_BaseTexture;
texture		g_NormalTexture;
texture		g_MaterialTexture;
texture		g_EmissiveTexture;
texture		g_SpecularTexture;

sampler		BaseSampler = sampler_state
{
	texture = g_BaseTexture;

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

//// DEFAULT TEXTURE END /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//// VARIABLE START //////////////////////////////////////////////////////

// int


// float
float		g_AddUV;
float		g_fAmount;
float		g_DeltaTime;

static float VTTextureWidth = 256.f;
static float VTTextureHeight = 256.f;

// vector, float4
vector		g_vLightDir;

vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlDiffuse;
vector		g_vMtrlAmbient;
vector		g_vMtrlSpecular;

vector		g_vCamPos;
vector		g_vColor;


// matrix
matrix		g_matWorld;	 // 상수 테이블
matrix		g_matView;
matrix		g_matProj;
matrix		g_matLightView;
matrix		g_matLightProj;
matrix		g_matProjInv;
matrix		g_matViewInv;
matrix		g_matCustom;

// texture
texture		g_DepthTexture;

sampler		DepthSampler = sampler_state
{
	texture = g_DepthTexture;

minfilter = linear;
magfilter = linear;
};

texture		g_DissolveTexture;
sampler		DissolveSampler = sampler_state
{
	texture = g_DissolveTexture;

minfilter = linear;
magfilter = linear;
};

texture		g_AlphaTexture;
sampler		AlphaSampler = sampler_state
{
	texture = g_AlphaTexture;

minfilter = linear;
magfilter = linear;
};


texture		g_VTFPos;
sampler		VTFPosSampler = sampler_state
{
	texture = g_VTFPos;

MinFilter = Point;
MagFilter = Point;
MipFilter = None;
AddressU = Wrap;
AddressV = Wrap;
};

texture		g_VTFNormal;
sampler		VTFNormalSampler = sampler_state
{
	texture = g_VTFNormal;

MinFilter = Point;
MagFilter = Point;
MipFilter = None;
AddressU = Wrap;
AddressV = Wrap;
};

texture		g_VTFTangent;
sampler		VTFTangentSampler = sampler_state
{
	texture = g_VTFTangent;

MinFilter = Point;
MagFilter = Point;
MipFilter = None;
AddressU = Wrap;
AddressV = Wrap;
};

texture		g_VTFBiNormal;

sampler		VTFBinormalSampler = sampler_state
{
	texture = g_VTFBiNormal;

MinFilter = Point;
MagFilter = Point;
MipFilter = None;
AddressU = Wrap;
AddressV = Wrap;
};

sampler vtf : register(s0);

//// VARIABLE END ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//// FUNCTOR VARIABLE START //////////////////////////////////////////////

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

// Distortion
float2 g_distortion1;
float2 g_distortion2;
float2 g_distortion3;
float g_distortionScale;
float g_distortionBias;

//// FUNCTOR VARIABLE END ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//// FUNCTION START //////////////////////////////////////////////////////
float4x4 GetMatrix(int mi)
{

	float4 uvCol = float4(((float)((mi % 16) * 4) + 0.5f) / 256.0f, ((float)((mi / 16)) + 0.5f) / 256.0f, 0.0f, 0.0f);
	float4x4 mat =
	{
		tex2Dlod(vtf, uvCol),
		tex2Dlod(vtf, uvCol + float4(1.0f / 256.0f, 0, 0, 0)),
		tex2Dlod(vtf, uvCol + float4(2.0f / 256.0f, 0, 0, 0)),
		tex2Dlod(vtf, uvCol + float4(3.0f / 256.0f, 0, 0, 0))
	};

	return mat;
	// only load 3 of the 4 values, and deocde the matrix from them.
	//rval = decodeMatrix(float3x4(mat1, mat2, mat3));
}
float4 SampleVTPOSTexture(float2 instanceIndex)
{
	float instanceOffset = dot(instanceIndex, float2(1.0f, 256.0f));

	float2 texcoord;
	texcoord.x = instanceOffset / VTTextureWidth;
	texcoord.y = floor(texcoord.x) / VTTextureHeight;
	return tex2Dlod(VTFPosSampler, float4(texcoord, 0.0f, 0.0f));
}
float4 SampleVTNormalTexture(float2 instanceIndex)
{
	float instanceOffset = dot(instanceIndex, float2(1.0f, 256.0f));

	float2 texcoord;
	texcoord.x = instanceOffset / VTTextureWidth;
	texcoord.y = floor(texcoord.x) / VTTextureHeight;
	return tex2Dlod(VTFNormalSampler, float4(texcoord, 0.0f, 0.0f));
}
float4 SampleVTTangentTexture(float2 instanceIndex)
{
	float instanceOffset = dot(instanceIndex, float2(1.0f, 256.0f));

	float2 texcoord;
	texcoord.x = instanceOffset / VTTextureWidth;
	texcoord.y = floor(texcoord.x) / VTTextureHeight;
	return tex2Dlod(VTFTangentSampler, float4(texcoord, 0.0f, 0.0f));
}
float4 SampleVTBinormalTexture(float2 instanceIndex)
{
	float instanceOffset = dot(instanceIndex, float2(1.0f, 256.0f));

	float2 texcoord;
	texcoord.x = instanceOffset / VTTextureWidth;
	texcoord.y = floor(texcoord.x) / VTTextureHeight;
	return tex2Dlod(VTFBinormalSampler, float4(texcoord, 0.0f, 0.0f));
}

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

};



//// FUNCTION END ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//// VERTEX SHADER START /////////////////////////////////////////////////
struct VS_IN
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float3	vTangent : TANGENT;
	float3	vBinormal : BINORMAL;
	float2	vTexUV : TEXCOORD0;
};


struct VS_OUT
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
struct VS_IN_VTF
{
	float4	vPosition : POSITION;
	float3	vNormal : NORMAL;
	float3	vTangent : TANGENT;
	float3	vBinormal : BINORMAL;

	float4 boneIndices : BLENDINDICES;
	float4 blendWeights : BLENDWEIGHT;

	float2	vTexUV : TEXCOORD0;
	float4 instanceIndex : TEXCOORD1;

};
struct VS_OUT_VTF
{
	float4	vtPosition : POSITION;

	float4	position : TEXCOORD0;
	float3	normal : TEXCOORD1;
	float3	tangent : TEXCOORD2;
	float3	binormal : TEXCOORD3;

};
struct VS_IN_SHADOW
{
	float4	vPosition : POSITION;
};

struct VS_OUT_SHADOW
{
	float4			vPosition : POSITION;
	float4			vProjPos : TEXCOORD0;
};


VS_OUT_VTF		VS_MAIN_VTF(VS_IN_VTF input)
{
	VS_OUT_VTF		Out = (VS_OUT_VTF)0;

	int4 boneIndices = (int4)input.boneIndices;
	float4	srcPosition = input.vPosition;
	float3	srcNormal = input.vNormal;
	float3	srcTangent = input.vTangent;
	float3	srcbinormal = input.vBinormal;

	input.vPosition.xyz = 0.0f;
	input.vNormal = 0.0f;
	input.vTangent = 0.0f;

	for (int i = 0; i < 4; ++i)
	{
		float4x4 boneMatrix = GetMatrix(boneIndices[i]);
		input.vPosition.xyz += mul(srcPosition, boneMatrix)* input.blendWeights[i];
		input.vNormal += mul(srcNormal, (float4x3)boneMatrix)*input.blendWeights[i];
		input.vTangent += mul(srcTangent, (float4x3)boneMatrix)*input.blendWeights[i];
		input.vBinormal += mul(srcbinormal, (float4x3)boneMatrix)*input.blendWeights[i];
	}

	float instanceOffset = dot(float2(input.instanceIndex.xy), float2(1.0f, 256.0f));

	float2 texcoord;
	texcoord.x = instanceOffset / VTTextureWidth;
	texcoord.y = floor(texcoord.x) / VTTextureHeight;

	float2 outputPos = frac(texcoord);
	outputPos.x = outputPos.x * 2.0f - 1.0f;
	outputPos.y = 1.0f - outputPos.y * 2.0f;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);
	Out.vtPosition = float4(outputPos, 0.0f, 1.0f);
	Out.position = input.vPosition;
	Out.normal = input.vNormal;
	Out.tangent = input.vTangent;
	Out.binormal = input.vBinormal;

	//Out.vtPosition = 0;

	return Out;
};

VS_OUT			VS_MAIN_USING_VTF(VS_IN_VTF In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(SampleVTPOSTexture(In.instanceIndex.xy), matWVP);
	Out.vNormal = normalize(mul(SampleVTNormalTexture(In.instanceIndex.xy), g_matWorld));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	Out.vPos = mul(SampleVTPOSTexture(In.instanceIndex.xy), g_matWorld);

	Out.T = vsNormalMapping(SampleVTTangentTexture(In.instanceIndex.xy));
	Out.B = vsNormalMapping(SampleVTBinormalTexture(In.instanceIndex.xy));
	Out.N = vsNormalMapping(SampleVTNormalTexture(In.instanceIndex.xy));


	//float4	vPosition : POSITION;
	//float4	vNormal : NORMAL;
	//float2	vTexUV : TEXCOORD0;
	//float4	vProjPos : TEXCOORD1;
	//float4	vPos : TEXCOORD2;
	//float3	T : TEXCOORD3;
	//float3	B : TEXCOORD4;
	//float3	N : TEXCOORD5;
	return Out;
};

//// VERTEX SHADER END ///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//// PIXEL SHADER START //////////////////////////////////////////////////
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

struct PS_OUT_VTF
{
	vector		position : COLOR0;
	vector		normal : COLOR1;
	vector		tangent : COLOR2;
	vector		binormal : COLOR3;
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
float4 Dissolve(float2 vTexUV : TEXCOORD0, vector vColor : COLOR0) : COLOR5
{
	clip(tex2D(DissolveSampler, vTexUV).rgb - g_fAmount);
vector vBurn;
float fDissolve = tex2D(DissolveSampler, vTexUV).rgb - g_fAmount;
return vColor;
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
PS_OUT			PS_MAIN_DYNAMICMESH(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vSpecular = tex2D(SpecularSampler, In.vTexUV);
	vector vDissolve = Dissolve(In.vTexUV, vColor);
	vector vMaterial = tex2D(MaterialSampler, In.vTexUV);
	vector vEmissive = Emissive(EmissiveSampler, In.vTexUV) * 5;
	if (vEmissive.x == 0)
		vEmissive.a = 0.f;


	Out.vNormal = psNormalMapping(In.vTexUV, In.T, In.B, In.N);

	Out.vColor = vDissolve + vEmissive;
	Out.vColor.a = 1.f;
	// Out.vColor *= vDissolve;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, vSpecular.r, vMaterial.g);

	Out.vEmissive = vEmissive;

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

//// PIXEL SHADER END ////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//// PASS START //////////////////////////////////////////////////////////
technique Default_Device
{
pass // 0 VTF 사용
{
	VertexShader = compile vs_3_0 VS_MAIN_USING_VTF();
	PixelShader = compile ps_3_0 PS_MAIN_DYNAMICMESH();
}



pass // 1 Dynamic VTF
{
	zwriteenable = false;

VertexShader = compile vs_3_0 VS_MAIN_VTF();
PixelShader = compile ps_3_0 PS_MAIN_VertexTransformationPSMain();
}
};
//// PASS END ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////