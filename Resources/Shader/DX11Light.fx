#include "Light.fx"

cbuffer Frame
{
	float4x4 g_world;
	float4x4 g_view;
	float4x4 g_project;
	float4x4 g_worldViewProj;
	float4x4 g_worldInvTranspose;

	float3	g_eyePos;
	float3	g_lightToDirection;
	float3  g_lightToDirectionColor;
	Material g_material;
}

Texture2D colorMap_ ;
//Input vertex
struct VertexIn
{
	float3	pos		: POSITION;			//Local space position
	float3	normal	: NORMAL;			//normal
	float3	tangent : TANGENT;			//tangent
	float2	tex		: TEXCOORD;			//texture coordiation (u,v)
};

//Output vertex of vertex shader
struct VertexOut
{
	float3	posL		: POSITION;			//World space position
	float4	posH		: SV_POSITION;		//Projection space position
	float3	normal		: NORMAL;			//World space normal
	float3	tangent		: TANGENT;			//World space tangent
	//float2	tex			: TEXCOORD0;		//Texture coordiation
	//float4	shadowTex	: TEXCOORD1;		//Shadow map coordiation
};

float3 ComputeDirLight(Material mat,			//Material
					float3 dirToLight,		//directional light
					float3 lightColor,		//directional light
					float3 normal,			//vertex normal
					float3 toEye)
{
	normal = normalize(normal);
	dirToLight = normalize(dirToLight);
	float diffFactor = dot(normal,dirToLight);
	float3 finalColor = lightColor * saturate(diffFactor);
	
	toEye =	normalize(toEye);
	float3 halfway = normalize(toEye + dirToLight);
	float sepcFactor = dot(halfway,normal);
	sepcFactor = saturate(sepcFactor);
	sepcFactor = pow( sepcFactor,mat.specular.w ) * mat.specular.z;
	finalColor +=  lightColor.rgb * saturate(sepcFactor);
	return finalColor;
}


//Vertex shader
VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.posL = mul(float4(vin.pos,1.f), g_world).xyz;
	vout.posH = mul(float4(vin.pos,1.f),g_worldViewProj);
	vout.normal = mul(vin.normal, (float3x3)g_worldInvTranspose);
	vout.tangent = mul(vin.tangent,(float3x3)g_world);
	//vout.tex = mul(float4(vin.tex,0.f,1.f),g_texTrans).xy;
	//vout.shadowTex = mul(float4(vin.pos,1.f),g_shadowTrans);

	return vout;
}
float4 PS_Direction(VertexOut pin): SV_TARGET
{
	float3 toeye = normalize(g_eyePos - pin.posL);
	float3 color = ComputeDirLight(g_material,g_lightToDirection,g_lightToDirectionColor,normalize(pin.normal),toeye);
	return float4(color.r,color.g,color.b,1.f);
	//return float4(0.f,0.f,0.f,0.f);
}
float4 PS_Point(VertexOut pin): SV_TARGET
{
	return float4(0.f,0.f,0.f,1.f);
}
float4 PS_Spot(VertexOut pin): SV_TARGET
{
	return float4(0.f,0.f,0.f,1.f);
}

technique11 LightDirection
{
	pass P0
	{
		SetVertexShader( CompileShader(vs_5_0, VS()) );
		SetGeometryShader(NULL);
		SetPixelShader( CompileShader(ps_5_0, PS_Direction()) );
	}
}

technique11 LightPoint
{
	pass P0
	{
		SetVertexShader( CompileShader(vs_5_0, VS()) );
		SetPixelShader( CompileShader(ps_5_0, PS_Point()) );
	}
}

technique11 LightSpot
{
	pass P0
	{
		SetVertexShader( CompileShader(vs_5_0, VS()) );
		SetPixelShader( CompileShader(ps_5_0, PS_Spot()) );
	}
}