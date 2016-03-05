#pragma once
#include "dx11base.h"
class Sprite
{
public:
	Sprite();
	~Sprite();
	XMMATRIX GetWorldMatrix();

	void SetPosition(XMFLOAT2& pos);
	void SetRotation(float rot);
	void SetScale(XMFLOAT2& scale);
private:
	XMFLOAT2 m_pos;
	float m_rotation;
	XMFLOAT2 m_scale;
};


class DX11SpriteDemo :
	public DX11Base
{
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};
public:
	DX11SpriteDemo(void);
	~DX11SpriteDemo(void);

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

	Sprite sprites_[2];
	ID3D11BlendState* blendState_;
	ID3D11Buffer* mvpBuffer_;
	XMMATRIX vpMatrix_;
};

