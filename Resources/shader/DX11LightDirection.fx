Texture2D colorMap_ : register( t0 );
SamplerState colorSampler_ : register( s0 );

cbuffer cbChangesEveryFrame : register( b0 )
{
	matrix worldMatrix;
};
cbuffer cbNeverChanges : register( b1 )
{
	matrix viewMatrix;
};
cbuffer cbChangeOnResize : register( b2 )
{
	matrix projMatrix;
};

cbuffer DirLightConstants : register( b3 )
{
    float3 DirToLight : packoffset( c0 );
    float3 DirLightColor : packoffset( c1 );
}

struct VS_Input
{
	float4 pos : POSITION;
	float2 tex0 : TEXCOORD0;
};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
};

float3 CalcDirectional(float3 position, Material material)
{
    // Phong diffuse
    float NDotL = dot(DirToLight, material.normal);
    float3 finalColor = DirLightColor.rgb * saturate(NDotL);
    // Blinn specular
    float3 ToEye = EyePosition.xyz - position;
    ToEye = normalize(ToEye);
    float3 HalfWay = normalize(ToEye + DirToLight);
    float NDotH = saturate(dot(HalfWay, material.normal));
    finalColor += DirLightColor.rgb * pow(NDotH, material.specExp) *
    material.specIntensity;
    return finalColor * material.diffuseColor.rgb;
}

PS_Input VS_Main( VS_Input vertex )
{
	PS_Input vsOut = ( PS_Input )0;
	vsOut.pos = mul( vertex.pos,worldMatrix);
	vsOut.pos = mul( vsOut.pos,viewMatrix);
	vsOut.pos = mul( vsOut.pos,projMatrix);
	vsOut.tex0 = vertex.tex0;
	return vsOut;
}
float4 PS_Main( PS_Input frag ) : SV_TARGET
{
	float4 col = colorMap_.Sample( colorSampler_, frag.tex0 );
	float4 col2 = secondMap_.Sample( colorSampler_, frag.tex0 );
	return col * col2;
	/*
	float4 col = colorMap_.Sample( colorSampler_, frag.tex0 );
	float4 finalCol;
	finalCol.x = col.y;
	finalCol.y = col.z;
	finalCol.z = col.x;
	finalCol.w = 1.0f;
	return finalCol;
	*/
}

technique11 ColorShift
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0,VS_Main()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0,PS_Main()));
	}
}