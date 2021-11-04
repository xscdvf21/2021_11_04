matrix			g_matWorld;	 // ��� ���̺�
matrix			g_matView;
matrix			g_matProj;

texture			g_BaseTexture;

sampler		BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

texture			g_WandTexture;

sampler		WandSampler = sampler_state
{
	texture = g_WandTexture;

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

float		g_fPower;

vector		g_vCamPos;


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

// ���� ���̴�

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

	Out.vColor  = tex2D(BaseSampler, In.vTexUV);	
	Out.vColor.a = 1.f;

	// -1~1�� ���� ������ ��ǥ�� 0~1�� �ؽ��� uv��ǥ�� ��ȯ

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	
	

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w,	// R�� ��ġ�� z�����Ⱑ ���� ������ z�� ����
						In.vProjPos.w * 0.001f,			// G�� ��ġ�� �佺���̽� ������ z�� ����(1000(FAR)���� ������ �ִ� �� : �ؽ���UV�� 0~1�̴ϱ�)
						0.f,
						0.f);
	
	return Out;
}

PS_OUT			PS_MAIN_ALPHATEST(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2���� �ؽ�ó�κ��� uv��ǥ�� �ش��ϴ� ���� �����Ͽ� ��ȯ�ϴ� �Լ�(��ȯ Ÿ���� vectorŸ��)

	return Out;
}

PS_OUT			PS_MAIN_WAND(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor.a = 1.f;

	// -1~1�� ���� ������ ��ǥ�� 0~1�� �ؽ��� uv��ǥ�� ��ȯ

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);



	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w,	// R�� ��ġ�� z�����Ⱑ ���� ������ z�� ����
		In.vProjPos.w * 0.001f,			// G�� ��ġ�� �佺���̽� ������ z�� ����(1000(FAR)���� ������ �ִ� �� : �ؽ���UV�� 0~1�̴ϱ�)
		0.f,
		0.f);

	return Out;
}

technique Default_Device
{
	pass
	{
	cullmode = None;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass	
	{
		alphatestenable = true;
		alpharef = 0xc0;
		alphafunc = greater;
		cullmode = None;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_ALPHATEST();
	}

		pass
	{
		zwriteenable = false;
	cullmode = None;


		VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN();
	}

		pass WAND
	{
	cullmode = None;


	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_WAND();
	}
};
