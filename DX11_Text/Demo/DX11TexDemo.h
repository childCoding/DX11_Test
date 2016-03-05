#pragma once
#include "dx11base.h"


class DX11TexDemo :
	public DX11Base
{
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};
public:
	DX11TexDemo(void);
	~DX11TexDemo(void);


	virtual bool LoadContent();
	virtual void UnloadContent();

	virtual void Render() ;
	virtual void Update(float dtime) ;


private: 
	ID3D11VertexShader* vertexShader_;
	ID3D11PixelShader* pixelShader_;
	ID3D11InputLayout* inputLayout_;
	ID3D11Buffer* vertexBuffer_;

	ID3D11ShaderResourceView* colorMap_;
	ID3D11SamplerState* colorMapSampler_;
};

