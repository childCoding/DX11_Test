Texture2D colorMap_ : register( t0 );
Texture2D secondMap_ : register( t1 );
Texture2D clipMap_ : register( t2 );
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

	//多纹理的像素重合
	float4 col = colorMap_.Sample( colorSampler_, frag.tex0 );
	float4 col1 = secondMap_.Sample( colorSampler_, frag.tex0 );
	float4 col2 = clipMap_.Sample( colorSampler_, frag.tex0 );

	col1.a = col2.a;
	clip(col2.a - 0.5f);  
	//if (col2.a > 0.0f)
		return col2;
	
	//return col;

}