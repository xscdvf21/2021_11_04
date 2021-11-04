matrix			g_matWorld;	 // 상수 테이블
matrix			g_matView;
matrix			g_matProj;

float			g_Alpha;
float			g_Amount;
float			g_TimeDelta;

vector			g_vAniInfo;
vector			g_vColor;
texture			g_BaseTexture;



sampler		BaseSampler = sampler_state
{
	texture = g_BaseTexture;

minfilter = linear;
magfilter = linear;

};

texture			g_AddTexture;
sampler		AddSampler = sampler_state
{
	texture = g_AddTexture;

minfilter = linear;
magfilter = linear;
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

// 정점 쉐이더

VS_OUT			VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vNormal = In.vNormal;
	return Out;
}

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;
	float4			vNormal : NORMAL;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
	vector		vNormal : COLOR1;
};

PS_OUT			PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector temp = tex2D(BaseSampler, In.vTexUV);
	Out.vColor = vector(temp.xyz, temp.w + g_Alpha);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	return Out;
}

PS_OUT			PS_MAIN_PICTURE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector temp = tex2D(BaseSampler, In.vTexUV);
	vector mask = tex2D(AddSampler, In.vTexUV);

	Out.vColor = vector(temp.xyz, temp.w + g_Alpha);
	Out.vColor.a = mask;

	return Out;
}

PS_OUT			PS_MAIN_UI(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector Color = tex2D(BaseSampler, In.vTexUV);

	Out.vColor = Color * (g_vColor / 255);

	return Out;
}

PS_OUT			PS_MAIN_UIANIMATION(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 vUV = In.vTexUV;
	vUV.x = (vUV.x + g_vAniInfo.z) / g_vAniInfo.x;
	vUV.y = (vUV.y + g_vAniInfo.w) / g_vAniInfo.y;
	vector Color = tex2D(BaseSampler, vUV);
	Out.vColor = Color * (g_vColor / 255);

	return Out;
}

PS_OUT			PS_MAIN_HP(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector Color = tex2D(BaseSampler, In.vTexUV);
	vector rColor = vector(Color.rrrr);
	vector Mask = tex2D(AddSampler, In.vTexUV);

	if (Mask.r == 0 && Mask.g == 0 && Mask.b == 0)
		Out.vColor.a = 0.f;
	else
	{
		Out.vColor = rColor * Mask.a * vector(0.8f, 0.3f, 0.2f, 0.7f);
		Out.vColor.a = rColor;
		clip(g_Amount - Mask.r);
	}

	return Out;
}

PS_OUT			PS_MAIN_HPBG(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector Color = tex2D(BaseSampler, In.vTexUV);
	vector Mask = vector(Color.ggg, 1.f);

	if (Mask.r == 0 && Mask.g == 0 && Mask.b == 0)
		Out.vColor.a = 0.f;
	else
	{
		Out.vColor = vector(0.1f, 0.1f, 0.1f, 0.7f);
		Out.vColor.a = Mask * 0.7f;
	}

	return Out;
}

PS_OUT			PS_MAIN_COOLTIME(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector Color = tex2D(BaseSampler, In.vTexUV);
	vector rColor = vector(Color.rrr, Color.a);
	vector Mask = tex2D(AddSampler, In.vTexUV);

	Out.vColor = rColor * Mask.a * (g_vColor / 255.f);
	clip(g_Amount - Mask.r);

	return Out;
}

PS_OUT			PS_MAIN_SKILLBOX(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector Color = tex2D(BaseSampler, In.vTexUV);
	vector gColor = vector(Color.ggg, Color.a);

	Out.vColor = gColor * (g_vColor / 255.f) * 0.5f;

	return Out;
}

PS_OUT			PS_MAIN_CHARGEFRAME(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector Color = tex2D(BaseSampler, In.vTexUV);
	vector gColor = vector(Color.rrrr);

	Out.vColor = gColor;
	Out.vColor.a = gColor;
	return Out;
}

PS_OUT			PS_MAIN_MAYCHARGE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 UV = In.vTexUV;
	UV.x -= g_TimeDelta * 0.2f;

	vector Color = tex2D(BaseSampler, In.vTexUV);
	vector gColor = vector(Color.gggg);
	float gAlpha = gColor;

	vector Mask = tex2D(AddSampler, In.vTexUV);

	vector MoveColor = tex2D(BaseSampler, UV);
	vector bColor = vector(MoveColor.bbbb);
	//bColor.a = bColor;

	vector customColor = vector(0.7f, 0.3f, 0.2f, 1.f);

	bColor *= customColor;
	gColor *= customColor;

	Out.vColor = (gColor + bColor) * Mask.a;
	Out.vColor.a = gAlpha;

	clip(g_Amount - Mask.r);

	return Out;
}

PS_OUT			PS_MAIN_CODYCHARGE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 UV = In.vTexUV;
	UV.x += g_TimeDelta * 0.2f;

	vector Color = tex2D(BaseSampler, In.vTexUV);
	vector gColor = vector(Color.gggg);
	float gAlpha = gColor;


	vector Mask = tex2D(AddSampler, In.vTexUV);

	vector MoveColor = tex2D(BaseSampler, UV);
	vector bColor = vector(MoveColor.bbbb);

	vector customColor = vector(0.1f, 0.5f, 0.7f, 1.f);

	bColor *= customColor;
	gColor *= customColor;

	Out.vColor = (gColor + bColor) * Mask.a;
	Out.vColor.a = gAlpha;

	clip(g_Amount - Mask.r);


	return Out;
}

PS_OUT			PS_MAIN_DEAD(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector Color = tex2D(BaseSampler, In.vTexUV);
	vector rColor = vector(Color.rrr, 1.f);
	vector gColor = vector(Color.ggg, 1.f);
	vector Mask = tex2D(AddSampler, In.vTexUV);

	vector customColor = vector(0.9f, 0.3f, 0.2f, 1.f);
	rColor = rColor * Mask.a * customColor;

	Out.vColor = rColor * gColor;
	Out.vColor.a = gColor;


	clip(g_Amount - Mask.r);

	return Out;
}

PS_OUT			PS_MAIN_DEAD2(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector Color = tex2D(BaseSampler, In.vTexUV);
	vector rColor = vector(Color.rrrr);
	vector gColor = vector(Color.gggg);

	vector customColor = vector(0.1f, 0.1f, 0.1f, 0.1f);

	Out.vColor = gColor * rColor * customColor;
	Out.vColor.a = gColor * 0.5f;

	return Out;
}

PS_OUT			PS_MAIN_DEAD3(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector Color = tex2D(BaseSampler, In.vTexUV);
	vector bColor = vector(Color.bbb, Color.a);
	vector Mask = tex2D(AddSampler, In.vTexUV);


	Out.vColor = bColor * (g_vColor / 255);
	Out.vColor.a = Color.a;

	return Out;
}

PS_OUT			PS_MAIN_PLAYERHP(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector Color = tex2D(BaseSampler, In.vTexUV);
	vector Mask = tex2D(AddSampler, In.vTexUV);

	Out.vColor = Color * Mask.a * (g_vColor / 255.f);
	clip(g_Amount - Mask.r);

	return Out;
}


PS_OUT			PS_MAIN_BG(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector temp = tex2D(BaseSampler, In.vTexUV);
	Out.vColor = vector(temp.xyz, temp.w + g_Alpha) * (g_vColor / 255);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	return Out;
}
technique Default_Device
{
	pass	Alphablend	//0 menu
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = invsrcalpha;
zwriteenable = false;

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN();
}


pass	// 1 menu
{
	alphablendenable = true;								
srcblend = srcalpha;
destblend = invsrcalpha;

	VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN_PICTURE();
}

pass	// 2 Normal
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = invsrcalpha;
zwriteenable = false;

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN_UI();
}

pass	// 3 animation
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = invsrcalpha;
zwriteenable = false;

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN_UIANIMATION();
}

pass	// 4 HP
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = invsrcalpha;
zwriteenable = false;

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN_HP();
}

pass	// 5 HP
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = invsrcalpha;
zwriteenable = false;

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN_HPBG();
}

pass	// 6 CoolTime
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = invsrcalpha;
zwriteenable = false;

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN_COOLTIME();
}

pass	// 7 SKILLBOX
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = invsrcalpha;
zwriteenable = false;

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN_SKILLBOX();
}

pass	// 8 CHARGEFRAME
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = invsrcalpha;
zwriteenable = false;

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN_CHARGEFRAME();
}

pass	// 9 CHARGEMAY
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = invsrcalpha;
zwriteenable = false;

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN_MAYCHARGE();
}

pass	// 10 CHARGECODY
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = invsrcalpha;
zwriteenable = false;

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN_CODYCHARGE();
}

pass	// 11 DEAD
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = invsrcalpha;
zwriteenable = false;

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN_DEAD();
}

pass	// 12 DEAD
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = invsrcalpha;
zwriteenable = false;

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN_DEAD2();
}

pass	// 13 DEAD
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = invsrcalpha;
zwriteenable = false;

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN_DEAD3();
}

pass	// 14 DEAD
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = invsrcalpha;
zwriteenable = false;

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN_PLAYERHP();
}

pass	// 15 DEAD
{

zwriteenable = false;

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN_BG();
}

};
