#pragma once
#include "../dx113dbase.h"
class DX11CubeEffectDemo :
	public DX113DBase
{
public:
	DX11CubeEffectDemo(void);
	~DX11CubeEffectDemo(void);

public:

	virtual bool LoadContent();
	virtual void UnloadContent();

	virtual void Render() ;
	virtual void Update(float dtime) ;

private: 
	ID3DX11Effect* effect_;

	ID3D11InputLayout* inputLayout_;

	ID3D11Buffer* vertexBuffer_;
	ID3D11Buffer* indexBuffer_;

	ID3D11ShaderResourceView* colorMap_;
	ID3D11ShaderResourceView* secondMap_;
	ID3D11SamplerState* colorMapSampler_;

};

