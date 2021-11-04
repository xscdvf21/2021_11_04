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


matrix			g_matWorld;	 // 상수 테이블
matrix			g_matView;
matrix			g_matProj;

vector		g_vLightDir;

vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlDiffuse;
vector		g_vMtrlAmbient;
vector		g_vMtrlSpecular;

vector		g_vRandom;
vector		g_vAniInfo;
vector		g_vColor;
float		g_TimeDelta;
float		g_Amount;
float		g_fDissolve;

// RimLight
float		g_fRimDist;
int			g_iRimPower;
vector		g_vRimColor;
vector		g_vCamPos;

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
static float		texScaler = 1.0 / 128.0;
static float		texOffset = 0.0;


//// FUNCTOR VARIABLE END ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//// FUNCTION START //////////////////////////////////////////////////////
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

return vector((pow(rim, g_iRimPower) + g_vRimColor).xyz, 0.1f);
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
	clip(tex2D(DissolveSampler, vTexUV).rgb - g_fDissolve);
vector vBurn;
float fDissolve = tex2D(DissolveSampler, vTexUV).rgb - g_fDissolve;
float fBurnSize = 0.3f;

if (fDissolve < fBurnSize && g_fDissolve > 0.0f && g_fDissolve < 1.0f)
{
	vBurn = tex2D(BurnSampler, float2(1.0f - (fDissolve * (1 / fBurnSize)), 0));
	return vBurn;
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
	float4			vProjPos : TEXCOORD1;
	float4			vPos : TEXCOORD2;
};

struct VS_IN2
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float3	vTangent : TANGENT;
	float3	vBinormal : BINORMAL;
	float2	vTexUV : TEXCOORD0;
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

// 정점 쉐이더

VS_OUT			VS_MAIN_BILLBOARD(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP, matBill;

	matBill._11 = g_matView._11;
	matBill._12 = g_matView._12;
	matBill._13 = g_matView._13;
	matBill._14 = 0.f;
	matBill._21 = g_matView._21;
	matBill._22 = g_matView._22;
	matBill._23 = g_matView._23;
	matBill._24 = 0.f;
	matBill._31 = g_matView._31;
	matBill._32 = g_matView._32;
	matBill._33 = g_matView._33;
	matBill._34 = 0.f;
	matBill._41 = 0.f;
	matBill._42 = 0.f;
	matBill._43 = 0.f;
	matBill._44 = 1.f;

	matWV = mul(g_matWorld, matBill);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	Out.vPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);

	return Out;
}

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

VS_OUT2			VS_MAIN_MESH(VS_IN2 In)
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

struct PS_IN
{
	vector			vNormal : NORMAL;
	float2			vTexUV : TEXCOORD0;
	vector			vProjPos : TEXCOORD1;
	float4			vPos : TEXCOORD2;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};

struct PS_OUT2
{
	vector		vColor : COLOR0;
	vector		vNormal : COLOR1;
	vector		vDepth : COLOR2;
};

struct PS_IN_MESH
{
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
	float4		vPos : TEXCOORD2;
	float3		T : TEXCOORD3;
	float3		B : TEXCOORD4;
	float3		N : TEXCOORD5;
};

struct PS_OUT_MESH
{
	vector		vColor : COLOR0;
	vector		vNormal : COLOR1;
	vector		vDepth : COLOR2;
	vector		vEmissive : COLOR3;
};

PS_OUT_MESH			PS_MAIN_MESH(PS_IN_MESH In)
{
	PS_OUT_MESH		Out = (PS_OUT_MESH)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vSpecular = tex2D(SpecularSampler, In.vTexUV);
	vector vEmissive = Emissive(EmissiveSampler, In.vTexUV) * 5;
	if (vEmissive.x == 0)
		vEmissive.a = 0.f;

	Out.vNormal = psNormalMapping(In.vTexUV, In.T, In.B, In.N);

	Out.vColor = (vColor + vEmissive) * (g_vColor / 255);
	//Out.vColor.a = 1.f;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, vSpecular.r, 0.f);
	Out.vEmissive = vEmissive;

	return Out;
}


PS_OUT			PS_MAIN_TYPETEXTURE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2차원 텍스처로부터 uv좌표에 해당하는 색을 추출하여 반환하는 함수(반환 타입이 vector타입)

	return Out;
}

PS_OUT			PS_MAIN_PARTICLE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV) * (g_vColor / 255);	// 2차원 텍스처로부터 uv좌표에 해당하는 색을 추출하여 반환하는 함수(반환 타입이 vector타입)

	return Out;
}

PS_OUT			PS_MAIN_POINT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vMask = tex2D(Add1Sampler, In.vTexUV);
	vColor.a = vMask;

	Out.vColor = vColor * (g_vColor / 255);	// 2차원 텍스처로부터 uv좌표에 해당하는 색을 추출하여 반환하는 함수(반환 타입이 vector타입)

	return Out;
}

PS_OUT			PS_MAIN_MOON(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vMask = tex2D(Add1Sampler, In.vTexUV);
	vMask.a = vColor;

	Out.vColor = vMask * (g_vColor / 255);	// 2차원 텍스처로부터 uv좌표에 해당하는 색을 추출하여 반환하는 함수(반환 타입이 vector타입)

	return Out;
}

PS_OUT			PS_MAIN_ELECTRIC1(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 vUV = In.vTexUV;
	vUV.x = (vUV.x + g_vAniInfo.z) / g_vAniInfo.x;
	vUV.y = (vUV.y + g_vAniInfo.w) / g_vAniInfo.y;
	vector Color = tex2D(Add1Sampler, vUV);
	vector vMask = tex2D(BaseSampler, vUV);
	Color.a = vMask;

	Out.vColor = Color * (g_vColor / 255);


	return Out;
}

PS_OUT			PS_MAIN_GRADIENT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);

	vector vMask = tex2D(Add1Sampler, In.vTexUV);
	vector vGradient = tex2D(Add2Sampler, In.vTexUV);
	vector vDissolve = Dissolve(In.vTexUV, vColor);
	vDissolve.a = vMask;

	Out.vColor = vDissolve * vGradient.a * (g_vColor / 255);	// 2차원 텍스처로부터 uv좌표에 해당하는 색을 추출하여 반환하는 함수(반환 타입이 vector타입)
	clip(g_Amount - vGradient.r);

	return Out;
}

PS_OUT			PS_MAIN_PARTICLEANIMATION(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 vUV = In.vTexUV;
	vUV.x = (vUV.x + g_vAniInfo.z) / g_vAniInfo.x;
	vUV.y = (vUV.y + g_vAniInfo.w) / g_vAniInfo.y;
	vector Color = tex2D(BaseSampler, vUV);
	Out.vColor = Color * (g_vColor / 255);

	return Out;
}

PS_OUT			PS_MAIN_ENERMYHIT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 vUV = In.vTexUV;
	vUV.x = (vUV.x + g_vAniInfo.z) / g_vAniInfo.x;
	vUV.y = (vUV.y + g_vAniInfo.w) / g_vAniInfo.y;
	vector Color = tex2D(BaseSampler, vUV);
	vector vMask = vector(Color.rrr, 1.f);
	Color.a = vMask;

	Out.vColor = Color * (g_vColor / 255);

	return Out;
}

PS_OUT			PS_MAIN_DARKFIRE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 vUV = In.vTexUV;
	vUV.x = (vUV.x + g_vAniInfo.z) / g_vAniInfo.x;
	vUV.y = (vUV.y + g_vAniInfo.w) / g_vAniInfo.y;
	vector Color = tex2D(BaseSampler, vUV);

	vector Rim = RimLight(In.vPos, In.vNormal);
	Rim.a = Color.a;
	Out.vColor = (Color * (g_vColor / 255)) + Rim;

	return Out;
}

PS_OUT			PS_MAIN_FIRESKILL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 vUV = In.vTexUV;
	vUV.x = (vUV.x + g_vAniInfo.z) / g_vAniInfo.x;
	vUV.y = (vUV.y + g_vAniInfo.w) / g_vAniInfo.y;
	vector Color = tex2D(BaseSampler, vUV);
	vector vFire = tex2D(Add1Sampler, In.vTexUV) * 1.5f;

	Out.vColor = Color * vFire  * (g_vColor / 255);

	return Out;
}

PS_OUT			PS_MAIN_FIRESKILL2(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 vUV = In.vTexUV;
	float2 vFlow = In.vTexUV;
	vFlow += g_TimeDelta;

	vUV.x = (vUV.x + g_vAniInfo.z) / g_vAniInfo.x;
	vUV.y = (vUV.y + g_vAniInfo.w) / g_vAniInfo.y;
	vector Color = tex2D(BaseSampler, vUV);
	vector vFire = tex2D(Add1Sampler, vFlow);

	vFire.a = Color;

	Out.vColor =  vFire * (g_vColor / 255) * 1.5f;

	return Out;
}

PS_OUT			PS_MAIN_ENERMYDEAD(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 vUV = In.vTexUV;
	float2 vFlow = In.vTexUV;
	vFlow += g_TimeDelta;

	vUV.x = (vUV.x + g_vAniInfo.z) / g_vAniInfo.x;
	vUV.y = (vUV.y + g_vAniInfo.w) / g_vAniInfo.y;
	vector Color = tex2D(BaseSampler, vUV);
	vector vFire = tex2D(Add1Sampler, vFlow);

	vFire.a = Color;

	Out.vColor = vFire * (g_vColor / 255) * 1.5f;

	return Out;
}

PS_OUT			PS_MAIN_RAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 vUV = In.vTexUV;

	vUV.x = (vUV.x + g_vAniInfo.z) / g_vAniInfo.x;
	vUV.y = (vUV.y + g_vAniInfo.w) / g_vAniInfo.y;
	vector Color = tex2D(BaseSampler, In.vTexUV);
	vector vRain = tex2D(Add1Sampler, vUV);

	Color.a = vRain;

	Out.vColor = Color * (g_vColor / 255);

	return Out;
}

PS_OUT			PS_MAIN_FIREULT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 UV = In.vTexUV;
	UV += g_TimeDelta;

	vector Color = Emissive(BaseSampler, In.vTexUV) * 10;
	vector rColor = vector(Color.rgb, Color.a);

	vector Swirl = tex2D(Add1Sampler, In.vTexUV);
	vector Center = tex2D(Add3Sampler, In.vTexUV);
	vector Mask = tex2D(Add2Sampler, In.vTexUV);
	rColor.a = Center * Swirl;
	Out.vColor = rColor * Mask.a * (g_vColor / 255.f);
	clip(g_Amount - Mask.r);

	return Out;
}

PS_OUT			PS_MAIN_ICEDASH(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vMask = tex2D(Add1Sampler, In.vTexUV);
	vector vGradient = tex2D(Add2Sampler, In.vTexUV);
	//vector vDissolve = Dissolve(In.vTexUV, vColor);
	vColor.a = vMask;

	Out.vColor = vColor * vGradient.a * (g_vColor / 255);
	clip(g_Amount - vGradient.r);

	return Out;
}

PS_OUT2			PS_MAIN_TYPEMESH(PS_IN In)
{
	PS_OUT2		Out = (PS_OUT2)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor.a = 1.f;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);

	return Out;
}

PS_OUT2			PS_MAIN_VACUUM(PS_IN In)
{
	PS_OUT2		Out = (PS_OUT2)0;

	float2 UV = In.vTexUV;
	UV.y -= g_TimeDelta * 1.8f;


	vector vColor = tex2D(Add3Sampler, UV);

	vector vGradient = tex2D(Add1Sampler, In.vTexUV);
	vector vNoise = tex2D(Add2Sampler, UV);

	vColor.a = vNoise.r * vGradient;


	Out.vColor = vColor * vector(1.5f, 1.5f, 1.5f, 1.f);

	//Out.vColor *= vGradient;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);

	return Out;
}

PS_OUT2			PS_MAIN_VCCIRCLE(PS_IN In)
{
	PS_OUT2		Out = (PS_OUT2)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV) * 1.5f;
	vector vMask = tex2D(Add1Sampler, In.vTexUV);

	Out.vColor = vColor * (g_vColor / 255);
	Out.vColor.a = vMask;
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);

	return Out;
}

PS_OUT2			PS_MAIN_FIREEFFECT(PS_IN In)
{
	PS_OUT2		Out = (PS_OUT2)0;

	float2 vUV = In.vTexUV;
	vUV.y += g_TimeDelta;

	vector vColor = tex2D(BaseSampler, vUV) ;
	vector vEmissive = Emissive(BaseSampler, vUV) * 0.7f;

	vector vAlpha = tex2D(Add1Sampler, In.vTexUV);
	vector vAlpha2 = tex2D(Add2Sampler, In.vTexUV);

	vColor += vEmissive;
	vColor.a = vAlpha + 0.3f;

	Out.vColor = vColor * vAlpha2;

	//Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	//Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);

	return Out;
}

PS_OUT2			PS_MAIN_GROUND(PS_IN In)
{
	PS_OUT2		Out = (PS_OUT2)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vMask = tex2D(Add1Sampler, In.vTexUV);

	vColor.a = vMask;

	Out.vColor = vColor * (g_vColor / 255);

	return Out;
}

PS_OUT2			PS_MAIN_DEBRIS(PS_IN In)
{
	PS_OUT2		Out = (PS_OUT2)0;

	float2 vUV = In.vTexUV;
	vUV.x = vUV.x / 2.f + g_fDissolve;
	vUV.y = vUV.y / 2.f + g_Amount;
	vector Color = tex2D(BaseSampler, vUV);

	Out.vColor = Color * (g_vColor / 255);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);

	return Out;
}

PS_OUT2			PS_MAIN_WARP(PS_IN In)
{
	PS_OUT2		Out = (PS_OUT2)0;

	float2 vUV = In.vTexUV;
	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector rColor = vector(vColor.rrr, 1.f) * vector(0.5f, 0.5f, 1.f, 1.f);
	vector bColor = vector(vColor.bbb, 1.f) * vector(1.f, 0.5f, 0.5f, 1.f);

	vector vMask = tex2D(Add1Sampler, In.vTexUV);

	rColor.a = vMask;
	bColor.a = vMask;

	Out.vColor = (rColor + bColor)/* * (g_vColor / 255)*/;

	return Out;
}

PS_OUT			PS_MAIN_LASERHIT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 vUV = In.vTexUV;
	vUV.x = (vUV.x + g_vAniInfo.z) / g_vAniInfo.x;
	vUV.y = (vUV.y + g_vAniInfo.w) / g_vAniInfo.y;
	vector vColor = tex2D(BaseSampler, vUV);

	//vector vDissolve = Dissolve(In.vTexUV, vColor);

	Out.vColor = vColor * (g_vColor / 255);

	return Out;
}

PS_OUT			PS_MAIN_ENERMYDAMAGEFONT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 vUV = In.vTexUV;
	vUV.x = (vUV.x / 16.f) + (g_Amount / 16.f);
	vector vColor = tex2D(BaseSampler, vUV);

	Out.vColor = vColor * (g_vColor / 255);

	return Out;
}

PS_OUT			PS_MAIN_ENERMYHP(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector Color = vector(vColor.ggg, 1.f);
	Color.a = Color;

	vector Mask = tex2D(Add1Sampler, In.vTexUV);

	Out.vColor = Color * Mask.a * (g_vColor / 255);

	clip(g_Amount - Mask.r);


	return Out;
}


PS_OUT			PS_MAIN_HANGDOOR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 UV = In.vTexUV;
	float2 fUV = In.vTexUV;

	UV.x -= g_TimeDelta * 0.01f;
	UV.y -= g_TimeDelta * 0.3f;
	vector vColor = tex2D(BaseSampler, UV) *  (g_vColor / 255);

	fUV.x += g_TimeDelta * 0.01f;
	fUV.y += g_TimeDelta * 0.1f;
	vector vColor2 = tex2D(BaseSampler, fUV * 2.f) * vector(1.f * 2.5f, 0.1f, 0.1f, 1.f);

	vector Color = tex2D(Add1Sampler, UV);
	vector Color2 = tex2D(Add1Sampler, fUV* 2.f);
	vector Mask = tex2D(Add2Sampler, In.vTexUV);

	vColor.a = Color;
	vColor2.a = Color2 * Mask;

	Out.vColor = vColor + vColor2;

	return Out;
}

PS_OUT			PS_MAIN_PLHIT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);

	vector rColor = vector(vColor.rrr, 1.f);
	vector bColor = vector(vColor.bbb, 1.f);
	vector Mask = tex2D(Add1Sampler, In.vTexUV);

	Mask.a = bColor * rColor;

	Out.vColor = Mask * (g_vColor / 255);

	return Out;
}

technique Default_Device
{
	pass	// 0 TYPE_TEXTURE
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;


		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_TYPEMESH();
	}

	pass	// 1 TYPE_MESH
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_TYPEMESH();
	}

	pass	// 2 TYPE_VACUUM
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	cullmode = None;


	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_VACUUM();
	}

	pass	// 3 Particle
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_PARTICLE();
	}

	pass	// 4 Particle Animation
	{
	//	stencilref = 2;
	//stencilfunc = always;
	//stencilpass = replace;
	//stencilfail = keep;
	//stencilzfail = keep;
	//stencilmask = 255;
	//stencilwritemask = 255;

	alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;
	cullmode = None;


		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_PARTICLEANIMATION();
	}

	pass // 5 vacuum circle
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_VCCIRCLE();
	}

		pass // 6 FireEffect
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_FIREEFFECT();
	}

		pass	// 7 Particle Animation
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_FIRESKILL();
	}

		pass	// 8 Ground
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_GROUND();
	}

		pass	// 9 Particle Animation
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_FIRESKILL2();
	}

		pass	// 10 swirl
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_FIREULT();
	}

		pass	// 11 point
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_POINT();
	}

		pass	// 12 moon
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_MOON();
	}

		pass	// 13 mesh Particle
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

	VertexShader = compile vs_3_0 VS_MAIN_MESH();
	PixelShader = compile ps_3_0 PS_MAIN_MESH();
	}

		pass	// 14 Gradient
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_GRADIENT();
	}

		pass	// 15 ICEDASH
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_ICEDASH();
	}

		pass	// 16 moon
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_POINT();
	}

		pass	// 17 Debris
	{
		alphatestenable = true;
	//	alphablendenable = true;
	//srcblend = srcalpha;
	//destblend = invsrcalpha;
	//zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_DEBRIS();
	}

		pass	// 18 shockwave
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_MOON();
	}

		pass	// 19 Particle Animation
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_DARKFIRE();
	}

		pass	// 20 Electric1
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_ELECTRIC1();
	}

		pass	// 21 RAIn
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_RAIN();
	}

		pass	// 22 RAIn
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_ENERMYDEAD();
	}

		pass	// 23 Debris
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_DEBRIS();
	}

		pass	// 24 Particle
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

	cullmode = None;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_PARTICLE();
	}

		pass	// 25 Hit
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_ENERMYHIT();
	}

		pass	// 26 Particle Animation
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;
	cullmode = None;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_LASERHIT();
	}

		pass	// 27 Particle Animation
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_ENERMYDAMAGEFONT();
	}

		pass	// 28 Particle Animation
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_ENERMYHP();
	}


		pass	// 29 Particle
	{
		alphatestenable = true;
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_PARTICLE();
	}

		pass	// 30 Particle
	{
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	cullmode = none;
	zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_HANGDOOR();
	}

		pass	// 31 Particle
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_PLHIT();
	}

		pass	// 32 Debris
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_DEBRIS();
	}

		pass	// 33 warpgate
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	zwriteenable = false;
	cullmode = None;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_WARP();
	}
};


