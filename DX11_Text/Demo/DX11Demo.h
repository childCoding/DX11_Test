#pragma once
#include "dx11base.h"

class DX11Demo :
	public DX11Base
{
	struct Vertex
	{
		XMFLOAT3 pos;
	};
public:

	DX11Demo(void);
	~DX11Demo(void);

	virtual bool LoadContent();
	virtual void UnloadContent();

	virtual void Render() ;
	virtual void Update(float dtime) ;

private: 
	ID3D11VertexShader* vertexShader_;
	ID3D11PixelShader* pixelShader_;
	ID3D11InputLayout* inputLayout_;
	ID3D11Buffer* vertexBuffer_;
};

