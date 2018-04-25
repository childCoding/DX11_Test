#pragma once
#include "../DX11Base.h"
//#include "DX11FreeTypeFontDemo.h"

class DX11FontDemo :
	public DX11Base
{
	//friend class DX11FreeTypeFontDemo;
protected: 
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

	virtual bool DrawString(char* str,float x,float y);

protected: 

	ID3D11VertexShader* vertexShader_;
	ID3D11PixelShader* pixelShader_;
	ID3D11InputLayout* inputLayout_;
	ID3D11Buffer* vertexBuffer_;

	ID3D11ShaderResourceView* colorMap_;
	ID3D11SamplerState* colorMapSampler_;

	ID3D11BlendState* blendState_;
};

