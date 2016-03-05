#pragma once
#include "DX11Base.h"

class DX11FontDemo :
	public DX11Base
{
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};
public:
	DX11FontDemo(void);
	~DX11FontDemo(void);

	virtual bool LoadContent();
	virtual void UnloadContent();

	virtual void Render() ;
	virtual void Update(float dtime) ;

	bool DrawString(char* str,float x,float y);
private: 
	ID3D11VertexShader* vertexShader_;
	ID3D11PixelShader* pixelShader_;
	ID3D11InputLayout* inputLayout_;
	ID3D11Buffer* vertexBuffer_;

	ID3D11ShaderResourceView* colorMap_;
	ID3D11SamplerState* colorMapSampler_;

	ID3D11BlendState* blendState_;
};

