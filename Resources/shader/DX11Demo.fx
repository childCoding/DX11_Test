
float4 VS_Main(float4 pos:POSITION):SV_POSITION
{
	//float4 re = float4(pos.x*0.5,pos.y*0.5,pos.z); 
	return pos;
}

float4 PS_Main(float4 pos:SV_POSITION):SV_TARGET
{
	float4 color = float4(pos.x,pos.y,pos.z,1.0);
	 return color;// float4(0.0,1.0,0.0,1.0);
}

