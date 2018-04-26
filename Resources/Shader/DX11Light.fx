#include "Light.fx"

cbuffer Frame
{
	float4x4 g_world;
	float4x4 g_view;
	float4x4 g_project;
	float4x4 g_worldViewProj;

	float3	g_lightDirection;
	float3  g_lightDirectionColor;
	Material g_material;
}


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
	float2	tex			: TEXCOORD0;		//Texture coordiation
	float4	shadowTex	: TEXCOORD1;		//Shadow map coordiation
};

void ComputeDirLight(Material mat,			//Material
					DirLight dirLight,		//directional light
					float3 normal,			//vertex normal
					float3 toEye,			//Vector from vertex to eye
					out float4 ambient,		//Outputs: ambient, diffuse, specular
					out float4 diffuse,	
					out float4 specular)
{
	ambient  = float4(0.0f,0.0f,0.f,0.f);
	diffuse  = float4(0.f,0.f,0.f,0.f);
	specular = float4(0.f,0.f,0.f,0.f);

	//Ambient result
	ambient = mat.ambient * dirLight.ambient;

	float diffFactor = -dot(normal,dirLight.dir);

	[flatten]
	if(diffFactor > 0)
	{
		//Diffuse result
		diffuse = mat.diffuse * dirLight.diffuse * diffFactor;

		float3 refLight = reflect(dirLight.dir,normal);
		float specFactor = pow(max(dot(refLight,toEye),0.f),mat.specular.w);
		specular = mat.specular * dirLight.specular * specFactor;
	}
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
	return ComputeDirLight()
}
float4 PS_Point(VertexOut pin): SV_TARGET
{

}
float4 PS_Spot(VertexOut pin): SV_TARGET
{

}

technique11 LightDirection
{
	pass P0
	{
		SetVertexShader( CompileShader(vs_5_0, VS()) );
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