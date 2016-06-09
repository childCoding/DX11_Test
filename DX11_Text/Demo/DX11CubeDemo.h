#pragma once
#include "dx113Dbase.h"
class DX11CubeDemo :
	public DX113DBase
{
public:
	DX11CubeDemo(void);
	~DX11CubeDemo(void);

	virtual bool LoadContent();
	virtual void UnloadContent();

	virtual void Render() ;
	virtual void Update(float dtime) ;

private: 
	ID3D11VertexShader* vertexShader_;
	ID3D11PixelShader* pixelShader_;

	ID3D11InputLayout* inputLayout_;
	ID3D11Buffer* vertexBuffer_;
	ID3D11Buffer* indexBuffer_;

	ID3D11ShaderResourceView* colorMap_;
	ID3D11ShaderResourceView* secondMap_;
	ID3D11ShaderResourceView* clipMap_;
	ID3D11SamplerState* colorMapSampler_;

	ID3D11Buffer* viewCB_;
	ID3D11Buffer* projCB_;
	ID3D11Buffer* worldCB_;

	
};

