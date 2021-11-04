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
AddressU = clamp;
AddressV = clamp;
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
float		g_fDistance;
float		g_DeltaTime = 0.f;
float		g_fMaterialIndex = 0.f;

static float VTTextureWidth = 64.f;
static float VTTextureHeight= 64.f;

// vector, float4
vector		g_vLightDir;

vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlDiffuse;
vector		g_vMtrlAmbient;
vector		g_vMtrlSpecular;

vector		g_vCamPos;
vector		g_vColor = vector(0.f, 0.f, 0.f, 0.f);


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

texture		g_ShadowTexture;
sampler		ShadowSampler = sampler_state
{
	texture = g_ShadowTexture;

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

texture		g_BurnTexture;
sampler		BurnSampler = sampler_state
{

	texture = g_BurnTexture;

minfilter = linear;
magfilter = linear;
};

texture		g_DistortionTexture;
sampler		DistortionSampler = sampler_state
{
	texture = g_DistortionTexture;

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

texture		g_Noise1Texture;
sampler		Noise1Sampler = sampler_state
{
	texture = g_Noise1Texture;

minfilter = linear;
magfilter = linear;
};

texture		g_Noise2Texture;
sampler		Noise2Sampler = sampler_state
{
	texture = g_Noise2Texture;

minfilter = linear;
magfilter = linear;
};

texture		g_Noise3Texture;
sampler		Noise3Sampler = sampler_state
{
	texture = g_Noise3Texture;

minfilter = linear;
magfilter = linear;
};

texture		g_Add1Texture;
sampler		Add1Sampler = sampler_state
{
	texture = g_Add1Texture;

minfilter = linear;
magfilter = linear;
};

texture		g_Add2Texture;
sampler		Add2Sampler = sampler_state
{
	texture = g_Add2Texture;

minfilter = linear;
magfilter = linear;
};

texture		g_Add3Texture;
sampler		Add3Sampler = sampler_state
{
	texture = g_Add3Texture;

minfilter = linear;
magfilter = linear;
};
texture		g_VTFTexture;
sampler		VTFSampler = sampler_state
{
	texture = g_VTFTexture;

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

//// VARIABLE END ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//// FUNCTOR VARIABLE START //////////////////////////////////////////////

// Emmisive
vector gaussFilterX[7] =
{
	0.0,-1.3, 0.0,1.0 / 64.0,
	0.0,-0.8, 0.0,6.0 / 64.0,
	0.0,-0.3, 0.0,15.0 / 64.0,
	0.0,0.0,  0.0,20.0 / 64.0,
	0.0,0.3,  0.0,15.0 / 64.0,
	0.0,0.8,  0.0,6.0 / 64.0,
	0.0,1.3,  0.0,1.0 / 64.0
};
vector gaussFilterY[7] =
{
	-1.3, 0.0, 0.0,1.0 / 64.0,
	-0.8, 0.0, 0.0,6.0 / 64.0,
	-0.3, 0.0, 0.0,15.0 / 64.0,
	0.0,  0.0, 0.0,20.0 / 64.0,
	0.3,  0.0, 0.0,15.0 / 64.0,
	0.8,  0.0, 0.0,6.0 / 64.0,
	1.3,  0.0, 0.0,1.0 / 64.0
};
float		texScaler = 1.0 / 128.0;
float		texOffset = 0.f;

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
	float4 uvCol = float4(((float)((mi % 16) * 4) + 0.5f) / 64.0f, ((float)((mi / 16)) + 0.5f) / 64.0f, 0.0f, 0.0f);

	float4x4 mat =
	{
		tex2Dlod(VTFSampler, uvCol),
		tex2Dlod(VTFSampler, uvCol + float4(1.0f / 64.0f, 0, 0, 0)),
		tex2Dlod(VTFSampler, uvCol + float4(2.0f / 64.0f, 0, 0, 0)),
		tex2Dlod(VTFSampler, uvCol + float4(3.0f / 64.0f, 0, 0, 0))
	};

	return mat;
	// only load 3 of the 4 values, and deocde the matrix from them.
	//rval = decodeMatrix(float3x4(mat1, mat2, mat3));
}
float4 SampleVTPOSTexture(float2 instanceIndex)
{
	float instanceOffset = dot(instanceIndex, float2(1.0f, 64.0f));

	float2 texcoord;
	texcoord.x = instanceOffset / VTTextureWidth;
	texcoord.y = floor(texcoord.x)/ VTTextureHeight;
	return tex2Dlod(VTFPosSampler, float4(texcoord, 0.0f, 0.0f));
}
float4 SampleVTNormalTexture(float2 instanceIndex)
{
	float instanceOffset = dot(instanceIndex, float2(1.0f, 64.0f));

	float2 texcoord;
	texcoord.x = instanceOffset / VTTextureWidth;
	texcoord.y = floor(texcoord.x) / VTTextureHeight;
	return tex2Dlod(VTFNormalSampler, float4(texcoord, 0.0f, 0.0f));
}
float4 SampleVTTangentTexture(float2 instanceIndex)
{
	float instanceOffset = dot(instanceIndex, float2(1.0f, 64.0f));

	float2 texcoord;
	texcoord.x = instanceOffset / VTTextureWidth;
	texcoord.y = floor(texcoord.x) / VTTextureHeight;
	return tex2Dlod(VTFTangentSampler, float4(texcoord, 0.0f, 0.0f));
}
float4 SampleVTBinormalTexture(float2 instanceIndex)
{
	float instanceOffset = dot(instanceIndex, float2(1.0f, 64.0f));

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

float4 RimLight(float4 vPos : POSITION, vector vNormal : NORMAL) : COLOR1
{
	float3 viewDir = normalize(g_vCamPos.xyz - vPos.xyz);
	float rim = smoothstep(1.f - g_fRimDist, 1.f, 1.f - dot(vNormal.xyz, viewDir));

	return vector((pow(rim, g_iRimPower) + g_vRimColor).xyz, 1.f);
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

float4 ShadowMapping(sampler Samplertype, float2 vTexUV : TEXCOORD0, vector vColor : COLOR0, vector vDepth : TEXCOORD1, vector vPos : TEXCOORD2) : COLOR0
{
	// 그림자
	float		fViewZ = vDepth.y;

vector		vPosition;
////투영상태
vPosition.x = (vTexUV.x * 2.f - 1.f) * fViewZ;
vPosition.y = (vTexUV.y * -2.f + 1.f) * fViewZ;
vPosition.z = vDepth.x * fViewZ;
vPosition.w = fViewZ;


vPosition = mul(vPos, g_matLightView);
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

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vPos : TEXCOORD4;
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
	float3	normal: TEXCOORD1;
	float3	tangent: TEXCOORD2;
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
};

VS_OUT			VS_MAIN_NORMALMAPPING(VS_IN In)
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

	Out.T = vsNormalMapping(In.vTangent);
	Out.B = vsNormalMapping(In.vBinormal);
	Out.N = vsNormalMapping(In.vNormal.xyz);
	

	return Out;
};

VS_OUT			VS_MAIN_NORMALMAPPING_INSTANCING(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWorld = float4x4(In.vRight, In.vUp, In.vLook, In.vPos);
	matrix		matWV, matWVP;
	matWV = mul(matWorld, g_matView);
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
VS_OUT		VS_MAIN_VTF(VS_IN_VTF input)
{
	VS_OUT		Out = (VS_OUT)0;

	int4 boneIndices = (int4)input.boneIndices;
	float4	srcPosition = input.vPosition;
	float3	srcNormal = input.vNormal;
	float3	srcTangent = input.vTangent;
	float3	srcbinormal = input.vBinormal;

	float	lastWeight = 0.0f;

	input.vPosition.xyz = 0.0f;
	input.vNormal = 0.0f;
	input.vTangent= 0.0f;
	
	for (int i = 0; i < 4; ++i)
	{
		lastWeight += input.blendWeights[i];

		float4x4 boneMatrix = GetMatrix(input.boneIndices[i]);
		input.vPosition.xyz += mul(srcPosition, boneMatrix)* input.blendWeights[i];
		input.vNormal += mul(srcNormal, (float4x3)boneMatrix)*input.blendWeights[i];
		input.vTangent += mul(srcTangent, (float4x3)boneMatrix)*input.blendWeights[i];
		input.vBinormal += mul(srcbinormal, (float4x3)boneMatrix)*input.blendWeights[i];
	}



	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(input.vPosition.xyz, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(input.vNormal.xyz, 0.f), g_matWorld));
	Out.vTexUV = input.vTexUV;
	Out.vProjPos = Out.vPosition;
	Out.vPos = mul(vector(input.vPosition.xyz, 1.f), g_matWorld);
	Out.T = input.vTangent;
	Out.B = input.vBinormal;
	Out.N = input.vNormal;

	//Out.vtPosition = 0;

	return Out;
};
VS_OUT_SHADOW			VS_MAIN_SHADOW(VS_IN_SHADOW In) // SHADOW
{
	VS_OUT_SHADOW		Out = (VS_OUT_SHADOW)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(float4(In.vPosition.xyz, 1.f), matWVP);
	Out.vProjPos = Out.vPosition;

	return Out;
};

VS_OUT_SHADOW			VS_MAIN_STATICSHADOW(VS_IN_SHADOW In) // SHADOW
{
	VS_OUT_SHADOW		Out = (VS_OUT_SHADOW)0;



	Out.vPosition = mul(float4(In.vPosition.xyz, 1.f), g_matCustom);
	Out.vProjPos = Out.vPosition;

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

PS_OUT			PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	vector Blur = Emissive(BaseSampler, In.vTexUV);
	vector Rim = RimLight(In.vPos, In.vNormal);

	Blur += Rim * 2;
	Out.vColor += Rim;
	Out.vColor += Blur;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);

	return Out;
}

PS_OUT			PS_MAIN_VACUUMGOO(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	In.vTexUV += g_DeltaTime * 0.3f;

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vSpecular = tex2D(SpecularSampler, In.vTexUV);
	vector vMaterial = tex2D(MaterialSampler, In.vTexUV);
	vector vEmissive = Emissive(EmissiveSampler, In.vTexUV) * 3;
	if (vEmissive.x == 0)
		vEmissive.a = 0.f;

	Out.vNormal = psNormalMapping(In.vTexUV, In.T, In.B, In.N);

	Out.vColor = vColor + vEmissive;
	Out.vColor.a = 1.f;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, vSpecular.r, vMaterial.g);
	Out.vEmissive = vEmissive;

	return Out;
}

PS_OUT			PS_MAIN_LAVAGOO(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 UV = In.vTexUV;
	UV.x -= g_DeltaTime * 0.5f;
	UV.y -= g_DeltaTime * 0.2f;
	float2 UV2 = In.vTexUV;
	UV2.x += g_DeltaTime * 0.1f;
	UV2.y += g_DeltaTime * 0.5f;

	vector vColor = tex2D(BaseSampler, UV * 2.f) * vector(1.f, 1.f, 0.66f, 1.f);
	vector vMaterial = tex2D(MaterialSampler, UV * 2.f) * vector(1.f, 1.f, 1.f, 1.f);
	vector vMask = tex2D(Add2Sampler, UV * 2.f) * vector(3.f, 3.f, 3.f, 1.f);
	vector vMask2 = tex2D(Add2Sampler, UV2 * 2.f) * vector(3.f, 3.f, 3.f, 1.f);

	//vColor.a = vMaterial;

	Out.vColor = (vColor + vMask * vMask2) * 0.8f;

	Out.vNormal = psNormalMapping(UV * 2.f, In.T, In.B, In.N);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);

	return Out;
}

PS_OUT			PS_MAIN_CHARACTER(PS_IN In)
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

	vector vConstant = g_vColor / 255;
	Out.vColor = (vDissolve + vEmissive) / vConstant;
	//Out.vColor.a = 1.f;

	//Out.vColor /= vConstant;
	// Out.vColor *= vDissolve;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, vSpecular.r, vMaterial.g);

	Out.vEmissive = vEmissive;

	return Out;
}

PS_OUT			PS_MAIN_FIRESWORD(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 UV = In.vTexUV;
	UV.x += g_DeltaTime;
	//	float fAlpha = 0;
	//	fAlpha += g_vColor.w;
	//	vector vAlpha = vector(1.f, 1.f, 1.f, fAlpha / 255);

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vEmissive = tex2D(EmissiveSampler, In.vTexUV);
	vector vMask = tex2D(Add2Sampler, In.vTexUV);	
	vector vFire = tex2D(Add1Sampler, UV);
	vFire.a = vMask;


	//vEmissive.a = vMask;

	Out.vNormal = psNormalMapping(In.vTexUV, In.T, In.B, In.N);

	Out.vColor = vColor * vFire * vector(1.5f, 1.5f, 1.5f, 1.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f
		/*Out.vSpecular.BBB*/, Out.vNormal.r, g_fMaterialIndex);

	Out.vEmissive = vEmissive;
	return Out;
}

PS_OUT			PS_MAIN_FIREEFFECT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 vUV = In.vTexUV;
	vUV.y += g_DeltaTime;

	vector vColor = tex2D(Add3Sampler, vUV);
	vector vEmissive = Emissive(Add3Sampler, vUV) * 2.2f;

	vector vAlpha = tex2D(Add1Sampler, In.vTexUV);
	vector vAlpha2 = tex2D(Add2Sampler, In.vTexUV);

	vColor += vEmissive;
	vColor.a = vAlpha;

	Out.vColor = vColor * vAlpha2;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);

	return Out;
}

PS_OUT			PS_MAIN_FIRESWIRL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vDistort = tex2D(Add2Sampler, In.vTexUV);
	//float2 RGBuv = float2(vDistort.g, vDistort.r);

	float2 vUV = float2(vDistort.r, vDistort.g);
	vUV.x -= g_DeltaTime;
	vUV.y -= g_DeltaTime;
	vector vColor = tex2D(BaseSampler, In.vTexUV + vUV);


	//vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vEmissive = Emissive(BaseSampler, In.vTexUV + vUV) * 0.5f;
	vector vAlpha = tex2D(Add1Sampler, In.vTexUV);

	vColor += vEmissive;
	vColor.a = vAlpha;

	Out.vColor = vColor;
	//Out.vColor.a = vAlpha;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w,
		In.vProjPos.w * 0.001f,
		0.f,
		0.f);

	return Out;
}

PS_OUT			PS_MAIN_TORCH(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vEmissive = Emissive(Add1Sampler, In.vTexUV);
	if (vEmissive.x == 0)
		vEmissive.a = 0.f;

	Out.vNormal = psNormalMapping(In.vTexUV, In.T, In.B, In.N);

	vector Rim = RimLight(In.vPos, Out.vNormal);

	if (vEmissive.x == 0)
		vEmissive.a = 0.f;
	else
	{
		Rim.a = 0.1f;
		vEmissive += Rim;
	}

	vColor += vEmissive;

	Out.vColor = vColor;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w,
		In.vProjPos.w * 0.001f,
		0.f,
		0.f);

	return Out;
}

PS_OUT			PS_MAIN_DYNAMICMESH(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vSpecular = tex2D(SpecularSampler, In.vTexUV);
	vector vDissolve = Dissolve(In.vTexUV, vColor);
	vector vMaterial = tex2D(MaterialSampler, In.vTexUV);
	vector vEmissive = tex2D(EmissiveSampler, In.vTexUV) * 5;
	if (vEmissive.x == 0)
		vEmissive.a = 0.f;


	Out.vNormal = psNormalMapping(In.vTexUV, In.T, In.B, In.N);

	Out.vColor = vDissolve;
	Out.vColor.a = 1.f;
	// Out.vColor *= vDissolve;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, vSpecular.r, g_fMaterialIndex);

	Out.vEmissive = vEmissive;

	return Out;
}

PS_OUT			PS_MAIN_STATICMESH(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vSpecular = tex2D(SpecularSampler, In.vTexUV);
	vector vMaterial = tex2D(MaterialSampler, In.vTexUV);
	vector vEmissive = tex2D(EmissiveSampler, In.vTexUV);
	if (vEmissive.x == 0)
		vEmissive.a = 0.f;

	Out.vNormal = psNormalMapping(In.vTexUV, In.T, In.B, In.N);

	Out.vColor = vColor /** (1 - vMaterial.g)*/;
	Out.vColor.a = 1.f;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, vSpecular.r, g_fMaterialIndex);
	Out.vEmissive = vEmissive;

	Out.vColor = ShadowMapping(Add1Sampler, In.vTexUV, Out.vColor, Out.vDepth, In.vPos);

	return Out;
}

PS_OUT			PS_MAIN_STATICMESHALPHA(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vSpecular = tex2D(SpecularSampler, In.vTexUV);
	vector vMaterial = tex2D(MaterialSampler, In.vTexUV);
	vector vEmissive = tex2D(EmissiveSampler, In.vTexUV);
	if (vEmissive.x == 0)
		vEmissive.a = 0.f;

	Out.vNormal = psNormalMapping(In.vTexUV, In.T, In.B, In.N);

	Out.vColor = vColor;
	//Out.vColor.a = 1.f;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, vSpecular.r, vMaterial.g);
	Out.vEmissive = vEmissive;

	return Out;
}

PS_OUT			PS_MAIN_MAGEATTACK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(Add1Sampler, In.vTexUV);
	vColor.a = 0.3f;

	vector vRim = RimLight(In.vPos, In.vNormal) * vector(0.6f * 3.5f, 0.4f* 3.5f, 0.8f* 3.5f, 1.f);

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vColor = vRim;
	//Out.vColor.a = 1.f;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);

	return Out;
}

PS_OUT			PS_MAIN_CBATTACK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(Add1Sampler, In.vTexUV);
	vColor.a = 0.3f;

	vector vRim = RimLight(In.vPos, In.vNormal) * vector(1.f * 5.5f, 0.2f* 5.5f, 0.f* 5.5f, 1.f);

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vColor = vRim;
	//Out.vColor.a = 1.f;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);

	return Out;
}

PS_OUT			PS_MAIN_CBBATTACK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 UV = In.vTexUV;

	UV += g_DeltaTime * 0.5f;

	vector vBase = tex2D(BaseSampler, UV);

	vector vColor = tex2D(Add1Sampler, UV) * vector(1.f * 4.5f, 0.2f* 4.5f, 0.1f* 4.5f, 1.f);
	vColor.a = vBase * 0.15f;
	vector vColor2 = tex2D(Add1Sampler, UV);
	vColor2.a = 0.2f;
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vColor = vColor;
	//Out.vColor.a = 1.f;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);

	Out.vEmissive = vColor;

	return Out;
}

PS_OUT			PS_MAIN_STATICMESHSKYBOX(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vSpecular = tex2D(SpecularSampler, In.vTexUV);
	vector vMaterial = tex2D(MaterialSampler, In.vTexUV);
	vector vEmissive = tex2D(BaseSampler, In.vTexUV);
	if (vEmissive.x == 0)
		vEmissive.a = 0.f;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vColor = vColor;
	//Out.vColor.a = 1.f;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);
	Out.vEmissive = vEmissive * 1.5f;

	return Out;
}

PS_OUT			PS_MAIN_EARTH(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vSpecular = tex2D(SpecularSampler, In.vTexUV);
	vector vMaterial = tex2D(MaterialSampler, In.vTexUV);
	vector vEmissive = tex2D(EmissiveSampler, In.vTexUV);
	if (vEmissive.x == 0)
		vEmissive.a = 0.f;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vColor = vColor + vMaterial;
	//Out.vColor.a = 1.f;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);
	Out.vEmissive = vEmissive;

	return Out;
}

PS_OUT			PS_MAIN_STATICMESHGRAVITY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 UV = In.vTexUV;
	UV.x /= 4.f;

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vWhite = tex2D(NormalSampler, In.vTexUV) * vector(1.f * 4.5f, 1.f * 4.5f, 0.5f * 4.5f, 1.f);
	vector vMask = tex2D(MaterialSampler, UV);

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vColor =  vColor * vWhite * (1.f - vMask * 0.5f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);
	//Out.vEmissive = vWhite * 0.01f;


	return Out;
}

PS_OUT			PS_MAIN_STATICMESHPANEL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 UV = In.vTexUV;
	UV.x /= 4.f;

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vWhite = tex2D(NormalSampler, In.vTexUV) */* (g_vColor / 255);*/ vector(0.5f * 3.5f, 1.f * 3.5f, 0.5f * 3.5f, 1.f);
	vector vMask = tex2D(MaterialSampler, UV);

	Out.vColor = vColor * vWhite * (1.f - vMask * 0.5f);
	return Out;
}

PS_OUT			PS_MAIN_LIGHT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vNormal = psNormalMapping(In.vTexUV, In.T, In.B, In.N);
	vector vEmissive = tex2D(EmissiveSampler, In.vTexUV) * (g_vColor / 255);
	if (vEmissive.x == 0)
		vEmissive.a = 0.f;
	//Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vColor = vColor;
	Out.vColor.a = 1.f;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f
		/*Out.vSpecular.BBB*/, 0.f, g_fMaterialIndex);
	Out.vEmissive = vEmissive;

	return Out;
}

PS_OUT			PS_MAIN_CONE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 UV = In.vTexUV;
	float2 LongUV = In.vTexUV;
	LongUV.x /= 12.f;
	LongUV.x += 0.4f;
	//LongUV.y /= 1.1f;
	//LongUV.y += 0.05f;
	UV.x /= 10.f;
	UV.x -= g_DeltaTime;


	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vMask = tex2D(Add1Sampler, UV);
	vector vMask2 = tex2D(Add2Sampler, LongUV);
	vector vAmount = tex2D(DistortionSampler, LongUV);

	vColor.a = vMask * vMask2;

	Out.vColor = vColor * vAmount.a * (g_vColor / 255);

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f
		/*Out.vSpecular.BBB*/, 0.f, 0.f);

	clip(g_fDistance - vAmount.r);

	return Out;
}

PS_OUT			PS_MAIN_ARCHERCONE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 UV = In.vTexUV;
	float2 LongUV = In.vTexUV;
	LongUV.x /= 12.f;
	LongUV.x += 0.4f;
	//LongUV.y /= 1.1f;
	//LongUV.y += 0.05f;
	UV.x /= 10.f;
	UV.x -= g_DeltaTime;


	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vMask2 = tex2D(Add2Sampler, LongUV);
	vector vAmount = tex2D(DistortionSampler, LongUV);

	vColor.a = vMask2;

	Out.vColor = vColor * vAmount.a * (g_vColor / 255);

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f
		/*Out.vSpecular.BBB*/, 0.f, 0.f);

	clip(g_fDistance - vAmount.r);

	return Out;
}

PS_OUT			PS_MAIN_MAGECONE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 UV = In.vTexUV;
	float2 LongUV = In.vTexUV;
	LongUV.x /= 12.f;
	LongUV.x += 0.4f;
	//LongUV.y /= 1.1f;
	//LongUV.y += 0.05f;
	UV.x /= 10.f;
	UV.x -= g_DeltaTime;


	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vMask = tex2D(Add1Sampler, UV);
	vector vMask2 = tex2D(Add2Sampler, LongUV);
	vector vAmount = tex2D(DistortionSampler, LongUV);

	vColor.a = vMask * vMask2;

	Out.vColor = vColor * vAmount.a * (g_vColor / 255);

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f
		/*Out.vSpecular.BBB*/, 0.f, 0.f);

	clip(g_fDistance - vAmount.r);

	return Out;
}

PS_OUT			PS_MAIN_CONE2(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 UV = In.vTexUV;
	float2 LongUV = In.vTexUV;
	LongUV.x /= 12.f;
	LongUV.x += 0.4f;
	//LongUV.y /= 1.1f;
	//LongUV.y += 0.05f;
	//UV.x /= 10.f;
	UV.x -= g_DeltaTime * 7.f;

	float2 Noise = In.vTexUV;
	Noise.x -= g_DeltaTime * 12.f;
	Noise /= 6.f;


	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vMask = tex2D(Add1Sampler, Noise);
	vector vMask2 = tex2D(Add2Sampler, LongUV);
	vector vMask3 = tex2D(Add3Sampler, UV);
	vector vAmount = tex2D(DistortionSampler, LongUV);

	vColor.a = (1.f - vMask) * vMask2 * vMask3;

	Out.vColor = vColor * vAmount.a * (g_vColor / 255);

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f
		/*Out.vSpecular.BBB*/, 0.f, 0.f);
	clip(g_fDistance - vAmount.r);

	return Out;
}

PS_OUT			PS_MAIN_LASER(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 UV = In.vTexUV;
	float2 LongUV = In.vTexUV;
	//LongUV.x /= 2.f;
	//LongUV.x += 0.4f;
	//LongUV.y /= 1.1f;
	//LongUV.y += 0.05f;
	UV.x /= 10.f;
	UV.x -= g_DeltaTime;


	vector vMask = tex2D(BaseSampler, LongUV);
	vector vColor = tex2D(Add1Sampler, UV) * (g_vColor / 255);
	vector vAmount = tex2D(DistortionSampler, LongUV);

	vColor.a = vMask;

	Out.vColor = vColor * vAmount.a * (g_vColor / 255);

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f
		/*Out.vSpecular.BBB*/, 0.f, 0.f);

	clip(g_fDistance - vAmount.r);

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

PS_OUT_SHADOW			PS_MAIN_SHADOW(PS_IN_SHADOW In)
{
	PS_OUT_SHADOW		Out = (PS_OUT_SHADOW)0;

	Out.vLightDepth.r = In.vProjPos.w / 1000.f;

	Out.vLightDepth.a = 1.f;

	return Out;
}

PS_OUT			PS_MAIN_STENCIL(PS_IN_SHADOW In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = g_vColor / 255;

	Out.vColor = vector(1.f, 1.f, 1.f, 1.f) * vColor;

	return Out;
}
//// PIXEL SHADER END ////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//// PASS START //////////////////////////////////////////////////////////
technique Default_Device
{
	pass // 0 Main_Character
	{
		VertexShader = compile vs_3_0 VS_MAIN_NORMALMAPPING();
		PixelShader = compile ps_3_0 PS_MAIN_CHARACTER();
	}

	pass // 1 IceOrb
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass // 2 FireSword
	{
		VertexShader = compile vs_3_0 VS_MAIN_NORMALMAPPING();
		PixelShader = compile ps_3_0 PS_MAIN_FIRESWORD();
		//PixelShader = compile ps_3_0 PS_MAIN_DYNAMICMESH();
	}

	pass // 3 FireEffect
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		cullmode = None;
		zwriteenable = false;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_FIREEFFECT();
	}

	pass // 4 FireSwirl
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		cullmode = None;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_FIRESWIRL();
	}

	pass // 5 VTF 사용
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_FIRESWIRL();
	}

	pass // 6 Default DynamicMesh
	{


	VertexShader = compile vs_3_0 VS_MAIN_NORMALMAPPING();
		PixelShader = compile ps_3_0 PS_MAIN_DYNAMICMESH();
	}

	pass // 7 Shadow
	{
		zwriteenable = false;
		VertexShader = compile vs_3_0 VS_MAIN_SHADOW();
		PixelShader = compile ps_3_0 PS_MAIN_SHADOW();
	}

	pass // 8 Map, StaticObject
	{
	////	zfunc = less;
	//	stencilenable = true;
	//stencilref = 1;
	//stencilmask = 255;
	//stencilwritemask = 255;
	//stencilfunc = Always;
	//stencilpass = Replace;
	//stencilfail = keep;
	//stencilzfail = keep;


			VertexShader = compile vs_3_0 VS_MAIN_NORMALMAPPING();
			PixelShader = compile ps_3_0 PS_MAIN_STATICMESH();
		}

		pass // 9 COLOR StaticObject
		{

			VertexShader = compile vs_3_0 VS_MAIN_NORMALMAPPING();
			PixelShader = compile ps_3_0 PS_MAIN_LIGHT();
		}
		pass // 10 CONE
		{
			alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = false;
		cullmode = None;

			VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_CONE();
		}

			pass // 11 CONE2
		{
			alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = false;
		cullmode = None;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_CONE2();
		}

			pass // 12 Static Shadow
		{
			zwriteenable = false;
		VertexShader = compile vs_3_0 VS_MAIN_STATICSHADOW();
		PixelShader = compile ps_3_0 PS_MAIN_SHADOW();
		}

			pass // 13 stencil
		{
		//	stencilenable = true;
		//stencilref = 1;
		//stencilmask = 255;
		//stencilwritemask = 255;
		//stencilfunc = Always;
		//stencilpass = Replace;
		//stencilfail = keep;
		//stencilzfail = keep;

		VertexShader = compile vs_3_0 VS_MAIN_SHADOW();
		PixelShader = compile ps_3_0 PS_MAIN_STENCIL();
		}

			pass // 14 StaticObject ALPHA
		{
			stencilenable = true;
		stencilref = 1;
		stencilmask = 255;
		stencilwritemask = 255;
		stencilfunc = Always;
		stencilpass = Replace;
		stencilfail = keep;
		stencilzfail = keep;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		//zwriteenable = false;
		cullmode = None;

		VertexShader = compile vs_3_0 VS_MAIN_NORMALMAPPING();
		PixelShader = compile ps_3_0 PS_MAIN_STATICMESHALPHA();
		}


			pass // 15 water_vacuumgoo
		{
		VertexShader = compile vs_3_0 VS_MAIN_NORMALMAPPING();
		PixelShader = compile ps_3_0 PS_MAIN_VACUUMGOO();
		}

			pass // 16 Gravity
		{
		VertexShader = compile vs_3_0 VS_MAIN_NORMALMAPPING();
		PixelShader = compile ps_3_0 PS_MAIN_STATICMESHGRAVITY();
		}

			pass // 17 Panel
		{
			VertexShader = compile vs_3_0 VS_MAIN_NORMALMAPPING();
		PixelShader = compile ps_3_0 PS_MAIN_STATICMESHPANEL();

		}

			pass // 18 water_Lavagoo
		{
			alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

			VertexShader = compile vs_3_0 VS_MAIN_NORMALMAPPING();
		PixelShader = compile ps_3_0 PS_MAIN_LAVAGOO();
		}

			pass // 19 ARCHER CONE
		{
			alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		//zwriteenable = false;


		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_ARCHERCONE();
		}

			pass // 20 MAGE CONE
		{
			alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = false;
		cullmode = None;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_MAGECONE();
		}

			pass // 21 LASER
		{
			alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = false;
		cullmode = None;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_LASER();
		}

			pass // 22 Map, StaticObject Instancing
		{


		VertexShader = compile vs_3_0 VS_MAIN_NORMALMAPPING_INSTANCING();
		PixelShader = compile ps_3_0 PS_MAIN_STATICMESH();
		}

			pass // 23 StaticObject ALPHA
		{
			zfunc = lessequal;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = false;
		//zenable = false;
		//cullmode = None;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_STATICMESHSKYBOX();
		}

			pass // 24 StaticObject 
		{
			VertexShader = compile vs_3_0 VS_MAIN_NORMALMAPPING();
			PixelShader = compile ps_3_0 PS_MAIN_STATICMESH();
		}

			pass // 25 earth
		{
			zfunc = lessequal;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		//zwriteenable = false;

			VertexShader = compile vs_3_0 VS_MAIN_NORMALMAPPING();
		PixelShader = compile ps_3_0 PS_MAIN_EARTH();
		}

			pass // 26 StaticObject ALPHA
		{
			stencilenable = true;
		stencilref = 1;
		stencilmask = 255;
		stencilwritemask = 255;
		stencilfunc = Always;
		stencilpass = Replace;
		stencilfail = keep;
		stencilzfail = keep;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = false;
		//cullmode = None;

		VertexShader = compile vs_3_0 VS_MAIN_NORMALMAPPING();
		PixelShader = compile ps_3_0 PS_MAIN_STATICMESHALPHA();
		}

			pass // 27 MAGEATTACK
		{

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		//zwriteenable = false;
		//cullmode = None;

		VertexShader = compile vs_3_0 VS_MAIN_NORMALMAPPING();
		PixelShader = compile ps_3_0 PS_MAIN_MAGEATTACK();
		}

			pass // 28 BABOONATTACK
		{

			alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		//zwriteenable = false;
		//cullmode = None;

		VertexShader = compile vs_3_0 VS_MAIN_NORMALMAPPING();
		PixelShader = compile ps_3_0 PS_MAIN_CBATTACK();
		}

			pass // 29 BABOONATTACK
		{

			alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		//zwriteenable = false;
		//cullmode = None;

		VertexShader = compile vs_3_0 VS_MAIN_NORMALMAPPING();
		PixelShader = compile ps_3_0 PS_MAIN_CBBATTACK();
		}

};
//// PASS END ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////